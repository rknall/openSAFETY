/**
 * @addtogroup SSDOS
 * @{
 *
 *
 * @file SSDOSsodAcs.c
 * This file contains functionality to access the SOD and generate
*             response for the SOD access.
 *
 *
 * @copyright Copyright (c) 2009,  Bernecker + Rainer Industrie-Elektronik Ges.m.b.H and IXXAT Automation GmbH
 * @copyright All rights reserved, Bernecker + Rainer Industrie-Elektronik Ges.m.b.H
 * @copyright This source code is free software; you can redistribute it and/or modify it under the terms of the BSD license (according to License.txt).
 *
 * @author K. Fahrion, IXXAT Automation GmbH
 * @author M. Molnar, IXXAT Automation GmbH
 *
 * <h2>History for SSDOSsodAcs.c</h2>
 * <table>
 *     <tr><th>Date</th><th>Author</th><th>Change Description</th></tr>
 *     <tr><td>03.06.2009</td><td>Hans Pill</td><td>Review SL V10</td></tr>
 * </table>
 *
 */

#include "EPLStarget.h"
#include "EPLScfg.h"
#include "EPLStypes.h"
#include "SCFMapi.h"
#include "SNMTSapi.h"
#include "SHNF.h"
#include "SERRapi.h"
#include "SERR.h"
#include "SFS.h"
#include "SODapi.h"
#include "SODerr.h"
#include "SOD.h"
#include "SSDOSerr.h"
#include "SSDOint.h"
#include "SSDOSint.h"


/**
 * This symbol represents the frame ID of an SSDO Service Response.
*/
#define k_ID_SERV_RESP          (UINT8)0x39

/**
 * Structured data type to process the SOD access and generate response for the SOD access.
*/
typedef struct
{
    /** internal toggle bit */
  BOOLEAN o_intToggleBit;
  /** flag to signal the call of the SOD_Lock() */
  BOOLEAN o_sodLockCalled;
  /** flag to signal the call of the SOD_Write() */
  BOOLEAN o_sodSegWriteCalled;

  /** index of SOD entry */
  UINT16 w_idx;
  /** sub index of SOD */
  UINT8 b_subIdx;

  /** handle to refer to an SOD entry */
  SOD_t_ACCESS s_sodEntry;
} t_SOD_ACCESS;

/**
 * Array of structures to access the SOD.
*/
static t_SOD_ACCESS as_SodAcs[EPLS_cfg_MAX_INSTANCES] SAFE_NO_INIT_SEKTOR;

static BOOLEAN SodAcsWriteAllowed(BYTE_B_INSTNUM_ BOOLEAN o_seg,
                                  BOOLEAN o_otherInst);

/**
* @brief This function initializes SOD access structure.
*
* @param        b_instNum        instance number (not checked, checked in SSC_InitAll() or SSC_ProcessSNMTSSDOFrame()), valid range: 0 .. ( EPLS_cfg_MAX_INSTANCES - 1 )
*
*/
void SSDOS_SodAcsInit(BYTE_B_INSTNUM)
{
  t_SOD_ACCESS *ps_sodAcs = &as_SodAcs[B_INSTNUMidx]; /* pointer to the sod
                                                         access structure */

  ps_sodAcs->w_idx = 0x0000U;
  ps_sodAcs->b_subIdx = 0x00U;

  ps_sodAcs->o_intToggleBit = FALSE;
  ps_sodAcs->o_sodLockCalled = FALSE;
  ps_sodAcs->o_sodSegWriteCalled = FALSE;

  ps_sodAcs->s_sodEntry.dw_hdl = 0x00UL;
  ps_sodAcs->s_sodEntry.o_applObj = FALSE;
  ps_sodAcs->s_sodEntry.ps_attr = (SOD_t_ATTR *)NULL;

  SCFM_TACK_PATH();
  return;
}

/**
* @brief This function clears the object data in case of an abort of the segmented download and calls the SOD_Unlock() if the SOD was locked.
*
* @param        b_instNum        instance number (not checked, checked in SSC_ProcessSNMTSSDOFrame()), valid range: 0 .. ( EPLS_cfg_MAX_INSTANCES - 1 )
* @param        o_abort
* - TRUE : reset because an abort frame was sent or received
* - FALSE : reset because of end of the transfer (not checked, any value allowed), valid range: TRUE, FALSE
*
* @return
* - TRUE           - reset of SOD access succeeded
* - FALSE          - reset of SOD access failed, FATAL error is already reported
*/
BOOLEAN SSDOS_SodAcsReset(BYTE_B_INSTNUM_ BOOLEAN o_abort)
{
  BOOLEAN o_return = FALSE;
  t_SOD_ACCESS *ps_sodAcs = &as_SodAcs[B_INSTNUMidx]; /* pointer to the sod
                                                         access structure */

  /* if the SOD_Lock() was called */
  if (ps_sodAcs->o_sodLockCalled)
  {
    /* if the SOD_Write() was called to write a segment and
          transfer was aborted */
    if ((ps_sodAcs->o_sodSegWriteCalled) && (o_abort))
    {
      /* if the SOD entry was cleared successfully */
      if(SOD_ActualLenSet(B_INSTNUM_ ps_sodAcs->s_sodEntry.dw_hdl,
                          ps_sodAcs->s_sodEntry.o_applObj, 0UL))
      {
        /* object unlock */
        o_return = SOD_Unlock(B_INSTNUM_ ps_sodAcs->s_sodEntry.dw_hdl,
                              ps_sodAcs->s_sodEntry.o_applObj);
      }
      /* no else : the SOD entry was not cleared successfully */
    }
    else /* the SOD_Write() was not called to write a segment or
            transfer was not aborted */
    {
      /* object unlock */
      o_return = SOD_Unlock(B_INSTNUM_ ps_sodAcs->s_sodEntry.dw_hdl,
                            ps_sodAcs->s_sodEntry.o_applObj);
    }
  }
  else /* else the SOD_Lock() was not called */
  {
    /* if the SOD_Write() was called to write a segment */
    if (ps_sodAcs->o_sodSegWriteCalled)
    {
      SERR_SetError(B_INSTNUM_ SSDOS_k_ERR_LOCK_WR_SEG, SERR_k_NO_ADD_INFO);
    }
    else /* the SOD_Write() was not called to write a segment */
    {
      o_return = TRUE;
    }
  }

  SCFM_TACK_PATH();
  return o_return;
}

/**
* @brief This function checks whether the SOD access request in the SSDOC request is valid.
*
* Toggle bit, SOD index and SOD sub-index are checked.
*
* @param        b_instNum            instance number (not checked, checked in SSC_ProcessSNMTSSDOFrame()), valid range: 0 .. ( EPLS_cfg_MAX_INSTANCES - 1 )
*
* @param        ps_rxPyldData        reference to the payload data in the received openSAFETY frame
*       (pointer not checked, only called with reference to array in DwnldWfReqSegState() or UpldWfReqSegState()),
*       valid range: see EPLS_t_FRM
*
* @param        o_checkObj           objet index subindex is to be checked
*
* @return
* - TRUE               - SSDOC Request is valid
* - FALSE              - SSDOC Request is invalid
*/
BOOLEAN SSDOS_SodAcsReqValid(BYTE_B_INSTNUM_ const UINT8 *ps_rxPyldData, BOOLEAN const o_checkObj)
{
  BOOLEAN o_ret = FALSE; /* return value */
  t_SOD_ACCESS *ps_sodAcs = &as_SodAcs[B_INSTNUMidx]; /* pointer to the sod
                                                         access structure */
  UINT16 w_rxIdx;   /* received index for comparison */
  UINT8 b_rxSubIdx; /* received sub index for comparison */
  UINT8 b_saCmd;    /* received SOD access command to check the toggle bit */

  /* get saCmd, Index and Sub-index from received payload */
  SFS_NET_CPY8(&b_saCmd, &(ps_rxPyldData[k_OFS_SACMD]));
  SFS_NET_CPY16(&w_rxIdx, &(ps_rxPyldData[k_OFS_IDX]));
  SFS_NET_CPY8(&b_rxSubIdx, &(ps_rxPyldData[k_OFS_SIDX]));

  /* if toggle bit is valid or abort frame received */
  if ((EPLS_IS_BIT_SET(b_saCmd, k_TOGGLE_SACMD) == ps_sodAcs->o_intToggleBit) ||
      (b_saCmd == k_ABORT))
  {
    /* if SOD index and sub-index are valid */
    if((!o_checkObj) ||
       ((w_rxIdx == ps_sodAcs->w_idx) && (b_rxSubIdx == ps_sodAcs->b_subIdx)))
    {
      o_ret = TRUE;
    }
    else /* wrong SOD index or sub-index received */
    {
      SERR_SetError(B_INSTNUM_ SSDOS_k_ERR_IDX_SIDX, SERR_k_NO_ADD_INFO);
    }
  }
  else /* wrong toggle bit */
  {
    SERR_SetError(B_INSTNUM_ SSDOS_k_ERR_TOGGLE_BIT, SERR_k_NO_ADD_INFO);
  }

  SCFM_TACK_PATH();
  return o_ret;
}

/**
* @brief This function gets the data type of the accessed object.
*
* @param        b_instNum        instance number (not checked, checked in SSC_ProcessSNMTSSDOFrame()), valid range: 0 .. ( EPLS_cfg_MAX_INSTANCES - 1 )
*
* @return       data type of the object see EPLS_t_DATATYPE
*/
EPLS_t_DATATYPE SSDOS_SodAcsDataTypeGet(BYTE_B_INSTNUM)
{
  t_SOD_ACCESS *ps_sodAcs = &as_SodAcs[B_INSTNUMidx]; /* pointer to the sod
                                                         access structure */

  SCFM_TACK_PATH();
  return ps_sodAcs->s_sodEntry.ps_attr->e_dataType;
}

/**
* @brief This function copies SOD index and SOD sub-index from the received SSDOC request.
*
* @param        b_instNum            instance number (not checked, checked in SSC_ProcessSNMTSSDOFrame()),
*       valid range: 0 .. &lt;EPLS_cfg_MAX_INSTANCES&gt; - 1
*
* @param       	ps_rxPyldData        reference to the payload data in the received openSAFETY frame
*       (pointer not checked, only called with reference to array in WfReqInitState() or SSDOS_UpldInitReqProc()
*       or SSDOS_DwnldInitSegReqProc() or SSDOS_DwnldInitExpReqProc()), valid range: see EPLS_t_FRM
*/
void SSDOS_SodAcsIdxCopy(BYTE_B_INSTNUM_ const UINT8 *ps_rxPyldData)
{
  t_SOD_ACCESS *ps_sodAcs = &as_SodAcs[B_INSTNUMidx]; /* pointer to the sod
                                                         access structure */

  /* get SOD index and sub-index from the received openSAFETY frame */
  SFS_NET_CPY16(&(ps_sodAcs->w_idx), &(ps_rxPyldData[k_OFS_IDX]));
  SFS_NET_CPY8(&(ps_sodAcs->b_subIdx), &(ps_rxPyldData[k_OFS_SIDX]));
}

/**
* @brief This function gets the SOD attributes for the corresponding SOD index and SOD sub-index.
*
* @param        b_instNum            instance number (not checked, checked in SSC_ProcessSNMTSSDOFrame()), valid range: 0 .. ( EPLS_cfg_MAX_INSTANCES - 1 )
*
* @param        ps_rxPyldData        reference to the payload data in the received openSAFETY frame
*       (pointer not checked, only called with reference to array in SSDOS_UpldInitReqProc() or SSDOS_DwnldInitSegReqProc()
*       or SSDOS_DwnldInitExpReqProc()), valid range: see EPLS_t_FRM
*
* @param        o_readAcs
* - TRUE  : read access is requested
* - FALSE : write access is requested (not checked, any value allowed), valid range: TRUE, FALSE
*
* @param        pdw_objSize          length of the object (pointer not checked, only called with reference to variable in SSDOS_UpldInitReqProc() or SSDOS_DwnldInitSegReqProc() or SSDOS_DwnldInitExpReqProc()), valid range: <> NULL
*
* @return
* - if SOD_ABT_NO_ERROR no abort response has to be sent,
* - otherwise abort response has to be sent with the returned abort code. See SOD_t_ABORT_CODES
*/
UINT32 SSDOS_SodAcsAttrGet(BYTE_B_INSTNUM_ const UINT8 *ps_rxPyldData,
                           BOOLEAN o_readAcs, UINT32 *pdw_objSize)
{
  UINT32 dw_abortCode = (UINT32)SOD_ABT_GENERAL_ERROR; /* return value */
  t_SOD_ACCESS *ps_sodAcs = &as_SodAcs[B_INSTNUMidx]; /* pointer to the sod
                                                         access structure */
  SOD_t_ERROR_RESULT s_errRes; /* SOD access error */

  *pdw_objSize = 0UL;

  SSDOS_SodAcsIdxCopy(B_INSTNUM_ ps_rxPyldData);

  /* get handle to destined SOD entry */
  ps_sodAcs->s_sodEntry.ps_attr = SOD_AttrGet(B_INSTNUM_ ps_sodAcs->w_idx,
                                             ps_sodAcs->b_subIdx,
                                             &(ps_sodAcs->s_sodEntry.dw_hdl),
                                             &(ps_sodAcs->s_sodEntry.o_applObj),
                                             &s_errRes);

  /* if SOD attribute is not available */
  if(ps_sodAcs->s_sodEntry.ps_attr == NULL)
  {
    #if (SOD_cfg_APPLICATION_OBJ == EPLS_k_DISABLE)
      #pragma CTC SKIP
    #endif
    /* if SOD access error */
    if ((s_errRes.e_abortCode != SOD_ABT_OBJ_DOES_NOT_EXIST) &&
        (s_errRes.e_abortCode != SOD_ABT_SUB_IDX_DOES_NOT_EXIST))
    {
      SERR_SetError(B_INSTNUM_ s_errRes.w_errorCode,
                    (UINT32)(s_errRes.e_abortCode));
    }
    /*no else : object was not found */
    #if (SOD_cfg_APPLICATION_OBJ == EPLS_k_DISABLE)
      #pragma CTC ENDSKIP
    #endif

    dw_abortCode = (UINT32)s_errRes.e_abortCode;
  }
  else /* SOD attribute is available */
  {
    /* if read access was requested */
    if (o_readAcs)
    {
      #if (SOD_cfg_APPLICATION_OBJ == EPLS_k_DISABLE)
        #pragma CTC SKIP
      #endif

      /* if the getting of the actual length of the object succeeded */
      if (SOD_ActualLenGet(B_INSTNUM_ ps_sodAcs->s_sodEntry.dw_hdl,
                          ps_sodAcs->s_sodEntry.o_applObj, pdw_objSize))

      {
        dw_abortCode = (UINT32)SOD_ABT_NO_ERROR;
      }
      else /* the getting of the actual length of the object failed */
      {
        dw_abortCode = SERR_GetLastAddInfo();
      }

      #if (SOD_cfg_APPLICATION_OBJ == EPLS_k_DISABLE)
        #pragma CTC ENDSKIP
      #endif
    }
    else /* write access was requested */
    {
      /* set the length of the object */
      *pdw_objSize = ps_sodAcs->s_sodEntry.ps_attr->dw_objLen;
      dw_abortCode = (UINT32)SOD_ABT_NO_ERROR;
    }
  }

  SCFM_TACK_PATH();
  return dw_abortCode;
}

/**
* @brief This function checks the current SN state. Only in state PREOPERATIONAL an SOD write access is allowed.
*
* In state OPERATIONAL only SOD read access is possible. In case of a shared SOD object to be accessed all instances
* are checked for state OPERATIONAL. Write access to a shared object is only possible in case of no instance of SNs
* is in state OPERATIONAL.
*
* @param        b_instNum        instance number (not checked, checked in SSC_ProcessSNMTSSDOFrame()),
*       valid range: 0 .. ( EPLS_cfg_MAX_INSTANCES - 1 )
* @param        o_seg
* - TRUE : segmented download
* - FALSE : expedited download (not checked, any value allowed), valid range: TRUE, FALSE
*
* @return
* - TRUE           - SOD write access is allowed
* - FALSE          - SOD write access is not allowed
*/
BOOLEAN SSDOS_SodAcsWriteAllowed(BYTE_B_INSTNUM_ BOOLEAN o_seg)
{
  BOOLEAN o_return = FALSE;  /* predefined return value */
  t_SOD_ACCESS *ps_sodAcs = &as_SodAcs[B_INSTNUMidx]; /* pointer to the sod
                                                          access structure */

  #if(EPLS_cfg_MAX_INSTANCES > 1)
    UINT8 b_inst = 0x00u;         /* loop counter to check all instances */
    BOOLEAN o_loopAbort = FALSE;  /* flag to signal loop abortion */


    /* if SOD entry to be accessed (download) is a shared object */
    if(EPLS_IS_BIT_SET(ps_sodAcs->s_sodEntry.ps_attr->w_attr,
                       SOD_k_ATTR_SHARED))
    {
      /* check all instances */
      do
      {
        /* if active instance */
        if (B_INSTNUM == b_inst)
        {
          /* if the write access is allowed */
          if (SodAcsWriteAllowed(B_INSTNUM_ o_seg, FALSE))
          {
            b_inst++; /* increment loop counter */
          }
          else /* write access is not allowed */
          {
            o_loopAbort = TRUE;
          }
        }
        else /* other instance */
        {
          /* if the write access is allowed */
          if (SodAcsWriteAllowed(b_inst, o_seg, TRUE))
          {
            b_inst++; /* increment loop counter */
          }
          else /* write access is not allowed */
          {
            o_loopAbort = TRUE;
          }
        }
      }
      while((!o_loopAbort) && (b_inst < (UINT8)EPLS_cfg_MAX_INSTANCES));

      /* if the loop was not broken */
      if (!o_loopAbort)
      {
        o_return = TRUE;
      }
    }
    else /* SOD entry to be accessed (download) is NOT shared */
  #endif
    {
      /* check whether the write access is allowed or not */
      o_return = SodAcsWriteAllowed(B_INSTNUM_ o_seg, FALSE);
    }

  /* if write access is allowed */
  if (o_return)
  {
    /* if the object data type is domian or visible string or octet string */
    if ((ps_sodAcs->s_sodEntry.ps_attr->e_dataType == EPLS_k_DOMAIN) ||
        (ps_sodAcs->s_sodEntry.ps_attr->e_dataType == EPLS_k_VISIBLE_STRING) ||
        (ps_sodAcs->s_sodEntry.ps_attr->e_dataType == EPLS_k_OCTET_STRING))
    {
      /* reset the actual length */
      o_return = SOD_ActualLenSet(B_INSTNUM_ ps_sodAcs->s_sodEntry.dw_hdl,
                                  ps_sodAcs->s_sodEntry.o_applObj, 0UL);
    }
    /* no else : other data type */
  }
  /* no else : write access is not allowed */

  SCFM_TACK_PATH();
  return o_return;
}

/**
* @brief This function converts the data from network format to host format and writes into the SOD.
*
* @param        b_instNum          instance number (not checked, checked in
*                                  SSC_ProcessSNMTSSDOFrame()), valid range: 0 .. ( EPLS_cfg_MAX_INSTANCES - 1 )
* @param        pb_data            pointer to the data to be written, (pointer not checked, only called
*       with reference to array in SSDOS_DwnldMidSegReqProc() or SSDOS_DwnldEndSegReqProc or SSDOS_DwnldInitExpRespSend()
*       or SSDOS_DwnldInitSegRespSend()), valid range: <> NULL
*
* @param        dw_offset          start offset in bytes of the segment within the data block (not checked,
*       checked in SOD_Write()) valid range: UINT32
*
* @param        dw_size            size in bytes of the segment (not checked, checked in SOD_Write()) valid range: (UINT32)
*
* @param        o_actLenSet
* - TRUE  : set actual length
* - FALSE : do not set actual length (not checked, any value allowed), valid range: TRUE, FALSE
*
* @return
* - if SOD_ABT_NO_ERROR no abort response has to be sent,
* - otherwise abort response has to be sent with the returned abort code.
*/
UINT32 SSDOS_SodAcsWrite(BYTE_B_INSTNUM_ const UINT8 *pb_data, UINT32 dw_offset,
                         UINT32 dw_size, BOOLEAN o_actLenSet)
{
  UINT32 dw_abortCode = (UINT32)SOD_ABT_GENERAL_ERROR; /* return value */
  UINT32 adw_tempData[(EPLS_cfg_MAX_PYLD_LEN_FAST_SSDO/4)+1]; /* dword array to adapt
                                        network byte order to host byte order */
  t_SOD_ACCESS *ps_sodAcs = &as_SodAcs[B_INSTNUMidx]; /* pointer to the sod
                                                          access structure */

  /* if the actual length of the SOD object has to be set */
  if (o_actLenSet)
  {
    /* if the setting of the actual length succeeded */
    if (SOD_ActualLenSet(B_INSTNUM_ ps_sodAcs->s_sodEntry.dw_hdl,
                          ps_sodAcs->s_sodEntry.o_applObj, dw_offset+dw_size))
    {
      dw_abortCode = (UINT32)SOD_ABT_NO_ERROR;
    }
    else /* the setting of the actual length failed */
    {
      dw_abortCode = SERR_GetLastAddInfo();
    }
  }
  else /* the actual length of the SOD object has not to be set */
  {
    dw_abortCode = (UINT32)SOD_ABT_NO_ERROR;
  }

  if (dw_abortCode == (UINT32)SOD_ABT_NO_ERROR)
  {
    /* if expedited access */
    if ((dw_offset == SOD_k_NO_OFFSET) && (dw_size == SOD_k_LEN_NOT_NEEDED))
    {
      /* if actual length was not set */
      if (!o_actLenSet)
      {
        /* if the byte order conversion succeeded */
        if(SFS_NetworkCopyGen(adw_tempData, pb_data,
                              ps_sodAcs->s_sodEntry.ps_attr->dw_objLen,
                              ps_sodAcs->s_sodEntry.ps_attr->e_dataType))
        {
          /* if write access succeeded */
          if(SOD_Write(B_INSTNUM_ ps_sodAcs->s_sodEntry.dw_hdl,
                      ps_sodAcs->s_sodEntry.o_applObj, adw_tempData,
                      SOD_k_NO_OVERWRITE, SOD_k_NO_OFFSET,
                      SOD_k_LEN_NOT_NEEDED))
          {
            dw_abortCode = (UINT32)SOD_ABT_NO_ERROR;
          }
          else /* write access failed */
          {
            dw_abortCode = SERR_GetLastAddInfo();
          }
        }
        else /* the byte order conversion failed */
        {
          dw_abortCode = (UINT32)SOD_ABT_GENERAL_ERROR;
        }
      }
      /* no else : no data has to be copied, actual length was set to 0 */
    }
    else /* segmented access */
    {
      /* byte order conversion */
      SFS_NET_CPY_DOMSTR(adw_tempData, pb_data, dw_size);

      /* if write access succeeded */
      if(SOD_Write(B_INSTNUM_ ps_sodAcs->s_sodEntry.dw_hdl,
                   ps_sodAcs->s_sodEntry.o_applObj, adw_tempData,
                   SOD_k_NO_OVERWRITE, dw_offset, dw_size))
      {
        /* if no expedited access or last segment was written and
              the o_sodSegWriteCalled flag was already set */
        if (!o_actLenSet)
        {
          ps_sodAcs->o_sodSegWriteCalled = TRUE;
        }
        dw_abortCode = (UINT32)SOD_ABT_NO_ERROR;
      }
      else /* write access failed */
      {
        dw_abortCode = SERR_GetLastAddInfo();
      }
    }
  }
  /* no else : abort response will be sent */

  SCFM_TACK_PATH();
  return dw_abortCode;
}

/**
* @brief This function reads the data the SOD.
*
* @param        b_instNum             instance number (not checked, checked in SSC_ProcessSNMTSSDOFrame()), valid range: 0 .. ( EPLS_cfg_MAX_INSTANCES - 1 )
* @param        dw_offset             start offset in bytes of the segment within the data block
*       (not checked, checked in SOD_Read()), valid range: (UINT32)
* @param        dw_size               size in bytes of the segment (not checked, checked in
*       SOD_Read()), valid range: (UINT32)
* @retval          pdw_abortCode         abort code, if SOD_ABT_NO_ERROR no abort response has to be sent,
*       otherwise abort response has to be sent with the this abort code. (pointer not checked, only called
*       with reference to variable in SSDOS_UpldInitReqProc() or SSDOS_UpldSegReqProc()) valid range: <> NULL, see SOD_t_ABORT_CODES
*
* @return
* - <> NULL             - pointer to the data, if pdw_abortCode is SOD_ABT_NO_ERROR
* - == NULL             - read access failed, see pdw_abortCode
*/
UINT8 *SSDOS_SodAcsRead(BYTE_B_INSTNUM_ UINT32 dw_offset, UINT32 dw_size,
                        UINT32 *pdw_abortCode)
{
  UINT8 *pb_return = (UINT8 *)NULL; /* return value */
  t_SOD_ACCESS *ps_sodAcs = &as_SodAcs[B_INSTNUMidx]; /* pointer to the sod
                                                         access structure */
  SOD_t_ERROR_RESULT s_errRes; /* SOD access error */

  /* if the object is readable or readable constant */
  if ((EPLS_IS_BIT_SET(ps_sodAcs->s_sodEntry.ps_attr->w_attr, SOD_k_ATTR_RO)) ||
      (EPLS_IS_BIT_SET(ps_sodAcs->s_sodEntry.ps_attr->w_attr,
                       SOD_k_ATTR_RO_CONST)))
  {
    /* get reference to the data to be read from the SOD */
    pb_return = (UINT8 *)SOD_Read(B_INSTNUM_ ps_sodAcs->s_sodEntry.dw_hdl,
                                  ps_sodAcs->s_sodEntry.o_applObj, dw_offset,
                                  dw_size, &s_errRes);

    /* if the read access failed */
    if (pb_return == NULL)
    {
      /* if the callback abort happened or 
            SAPL read abort happened */
      if ((s_errRes.w_errorCode == SOD_k_ERR_CLBK_ABORT_BR) ||
          (s_errRes.w_errorCode == SOD_k_ERR_SAPL_READ))
      {
        *pdw_abortCode = (UINT32)(s_errRes.e_abortCode);
      }
      else /* other error happened */
      {
        SERR_SetError(B_INSTNUM_ s_errRes.w_errorCode,
                      (UINT32)(s_errRes.e_abortCode));
      }
    }
    else /* read access succeeded */
    {
      *pdw_abortCode = (UINT32)SOD_ABT_NO_ERROR;
    }
  }
  else /* object is not readable */
  {
    *pdw_abortCode = (UINT32)SOD_ABT_OBJ_NOT_READABLE;
  }

  SCFM_TACK_PATH();
  return pb_return;
}

/**
* @brief This function locks the SOD.
*
* @param        b_instNum        instance number (not checked, checked in  SSC_ProcessSNMTSSDOFrame()), valid range:  0 .. ( EPLS_cfg_MAX_INSTANCES - 1 )
*
* @return
* - if SOD_ABT_NO_ERROR no abort response has to be sent,
* - otherwise abort response has  to be sent with the returned abort code.
*/
UINT32 SSDOS_SodAcsLock(BYTE_B_INSTNUM)
{
  UINT32 dw_abortCode = (UINT32)SOD_ABT_GENERAL_ERROR; /* return value */
  t_SOD_ACCESS *ps_sodAcs = &as_SodAcs[B_INSTNUMidx]; /* pointer to the sod
                                                          access structure */

  /* if locking of the SOD succeeded */
  if(SOD_Lock(B_INSTNUM_ ps_sodAcs->s_sodEntry.dw_hdl,
              ps_sodAcs->s_sodEntry.o_applObj))
  {
    ps_sodAcs->o_sodLockCalled = TRUE;
    dw_abortCode = (UINT32)SOD_ABT_NO_ERROR;
  }
  else /* locking of the SOD failed */
  {
    dw_abortCode = SERR_GetLastAddInfo();
  }

  SCFM_TACK_PATH();
  return dw_abortCode;
}

/**
* @brief This function assembles a SSDO Service Response for the SOD access except the raw data.
*
* @param b_instNum - instance number (not checked, checked in SSC_ProcessSNMTSSDOFrame()), valid range: 0 .. ( EPLS_cfg_MAX_INSTANCES - 1 )
*
* @param        ps_rxBuf         reference to received openSAFETY frame  to be distributed (pointer not
*       checked, only called with reference to struct in processStateReqProc()), valid range: <> NULL, see EPLS_t_FRM
*
* @param        ps_txBuf         reference to openSAFETY frame  to be transmitted (pointer not checked,
*       only called with reference to struct in processStateReqProc()), valid range: <> NULL, see EPLS_t_FRM
*
* @param        b_saCmd          SOD access command for the response (not checked, any value allowed), valid range: (UINT8)
*
* @param        b_respLen        length of the response (not checked, any value allowed), valid range: (UINT8)
*
* @param        o_insObjInfo     index and subindex information is needed in the response
*/
void SSDOS_SodAcsResp(BYTE_B_INSTNUM_ const EPLS_t_FRM *ps_rxBuf,
                      EPLS_t_FRM *ps_txBuf, UINT8 b_saCmd, UINT8 b_respLen,
                      BOOLEAN o_insObjInfo)
{
  t_SOD_ACCESS *ps_sodAcs = &as_SodAcs[B_INSTNUMidx]; /* pointer to the sod
                                                         access structure */

  /* store response frame header info into SSC FSM */
  ps_txBuf->s_frmHdr.b_id = k_ID_SERV_RESP;
  ps_txBuf->s_frmHdr.w_adr = ps_rxBuf->s_frmHdr.w_tadr;
  ps_txBuf->s_frmHdr.w_tadr = ps_rxBuf->s_frmHdr.w_adr;
  ps_txBuf->s_frmHdr.w_sdn = ps_rxBuf->s_frmHdr.w_sdn;
  ps_txBuf->s_frmHdr.b_le = b_respLen;
  ps_txBuf->s_frmHdr.w_ct = ps_rxBuf->s_frmHdr.w_ct;
  ps_txBuf->s_frmHdr.b_tr = EPLS_k_TR_NOT_USED;

  /* store response data into SSC FSM */
  SFS_NET_CPY8(&(ps_txBuf->ab_frmData[k_OFS_SACMD]), &b_saCmd);
  /* only insert the index subindex information if needed */
  if ( o_insObjInfo)
  {
    SFS_NET_CPY16(&(ps_txBuf->ab_frmData[k_OFS_IDX]), &(ps_sodAcs->w_idx));
    SFS_NET_CPY8(&(ps_txBuf->ab_frmData[k_OFS_SIDX]), &(ps_sodAcs->b_subIdx));
  }

  /* switch internal toggle bit */
  ps_sodAcs->o_intToggleBit = (BOOLEAN)(!ps_sodAcs->o_intToggleBit);

  SCFM_TACK_PATH();
  return;
}

/***
*    static functions
***/
/**
* @brief This function checks the given SN state, whether the SOD write access is allowed or not.
*
* @param        b_instNum          instance number (not checked, checked in SSC_ProcessSNMTSSDOFrame()), valid range: 0 .. &lt;EPLS_cfg_MAX_INSTANCES&gt; - 1
*
* @param        o_seg              valid range: TRUE, FALSE
* - TRUE : segmented download
* - FALSE : expedited download (not checked, any value allowed),
*
* @param        o_otherInst        valid range: TRUE, FALSE
* - TRUE : the write access is checked at the other instance (other instance means that the passed instance number is not
*   accessed via SSDO write)
* - FALSE : the write access is checked at the active instance (active instance means that the passed instance number is
*   accessed via SSDO write) (not checked, any value allowed),
*
* @return
* - TRUE             - SOD write access is allowed
* - FALSE            - SOD write access is not allowed
*/
static BOOLEAN SodAcsWriteAllowed(BYTE_B_INSTNUM_ BOOLEAN o_seg,
                                  BOOLEAN o_otherInst)
{
  BOOLEAN o_return = FALSE;  /* predefined return value */
  SNMTS_t_SN_STATE_MAIN e_actSnState; /* variable to get the actual state */

  /* if the actual state is available */
  if (SNMTS_GetSnState(B_INSTNUM_ &e_actSnState))
  {
    /* if SOD write access is enabled */
    if (SOD_WriteAccess(B_INSTNUM))
    {
      /* if SOD is not locked or not segmented download */
      if (!SOD_IsLocked(B_INSTNUM) || !o_seg)
      {
      #if (EPLS_cfg_MAX_INSTANCES > 1)
        /* if other instance is checked */
        if (o_otherInst)
        {
          /* if the actual state is not OPERATIONAL */
          if (e_actSnState != SNMTS_k_ST_OPERATIONAL)
          {
            o_return = TRUE;
          }
          else /* the actual state is OPERATIONAL */
          {
            SERR_SetError(B_INSTNUM_ SSDOS_k_ERR_WR_ACS4,
                    (UINT32)(SOD_ABT_PRESENT_DEVICE_STATE));
          }
        }
        /* RSM_IGNORE_QUALITY_BEGIN Notice #22 - 'else' is not bound with scope
                                         braces {} */
        else /* active instance is checked */
      #else
        o_otherInst = o_otherInst;
      #endif
        {
        /* RSM_IGNORE_QUALITY_END */

          /* if the actual state is PRE_OPERATIONAL */
          if (e_actSnState == SNMTS_k_ST_PRE_OPERATIONAL)
          {
            o_return = TRUE;
          }
          else /* the actual state is not PRE_OPERATIONAL */
          {
            SERR_SetError(B_INSTNUM_ SSDOS_k_ERR_WR_ACS3,
                    (UINT32)(SOD_ABT_PRESENT_DEVICE_STATE));
          }
        }
      }
      else /* SOD is locked */
      {
        SERR_SetError(B_INSTNUM_ SSDOS_k_ERR_WR_ACS2,
                    (UINT32)(SOD_ABT_PRESENT_DEVICE_STATE));
      }
    }
    else  /* write access is disabled */
    {
      SERR_SetError(B_INSTNUM_ SSDOS_k_ERR_WR_ACS1,
                    (UINT32)(SOD_ABT_PRESENT_DEVICE_STATE));
    }
  }
  /* no else : the actual state is not available */

  SCFM_TACK_PATH();
  return o_return;
}


/** @} */
