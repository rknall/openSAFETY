/**
 * @addtogroup SPDO
 * @{
 *
 * @file SPDOrxConsSm.c
 *
 * This file contains the RX SPDO Consumer State machine.
 *
 * @copyright Copyright (c) 2009,  Bernecker + Rainer Industrie-Elektronik Ges.m.b.H and IXXAT Automation GmbH
 * @copyright All rights reserved, Bernecker + Rainer Industrie-Elektronik Ges.m.b.H
 * @copyright This source code is free software; you can redistribute it and/or modify it under the terms of the BSD license (according to License.txt).
 *
 * @author M. Molnar, IXXAT Automation GmbH
 *
 * <h2>History for SPDOtxProdSm.c</h2>
 * <table>
 *     <tr><th>Date</th><th>Author</th><th>Change Description</th></tr>
 *     <tr><td>03.06.2009</td><td>Hans Pill</td><td>Review SL V10</td></tr>
 *     <tr><td>04.12.2009</td><td>Hans Pill</td><td>changed ProcessData for not taking over data of the first valid telegram after a failed time synchronization</td></tr>
 *     <tr><td>11.01.2010</td><td>Hans Pill</td><td>Review SL V15</td></tr>
 *     <tr><td>05.04.2011</td><td>Hans Pill</td><td>A&P258030 interface for SHNF to get the info of the RxSPDOs</td></tr>
 *     <tr><td>29.08.2011</td><td>Hans Pill</td><td>A&P270145 negative propagation delay leads to a fatal error in any case</td></tr>
 *     <tr><td>05.10.2011</td><td>Hans Pill</td><td>Review SL V22</td></tr>
 *     <tr><td>15.03.2013</td><td>Hans Pill</td><td>added statistic counters</td></tr>
 *     <tr><td>09.08.2013</td><td>Hans Pill</td><td>Review OpenSAFETY 1.4.0.0</td></tr>
 * </table>
 *
 */

#include "EPLScfg.h"

#if (SPDO_cfg_MAX_NO_RX_SPDO != 0)

#include "EPLStarget.h"
#include "EPLStypes.h"

#include "SCFMapi.h"

#include "SERRapi.h"
#include "SERR.h"
#include "SPDOerr.h"
#include "SPDOint.h"

#include "SHNF.h"


/**
 * Macro to decide whether the CT is valid or not.
 *
 * @param actCt actual consecutive time
 * @param lastCt - last valid consecutive time
 *
 * @return
 * - TRUE  - timeout
 * - FALSE - no timeout
*/
#define CT_VALID(actCt,lastCt) (((UINT16)((actCt)-(lastCt))) < 0x8000U)

/**
 * Object structure to control the Rx SPDO Consumer State Machine.
*/
static t_RX_CONS_SM as_ConsSm[EPLS_cfg_MAX_INSTANCES]
                             [SPDO_cfg_MAX_NO_RX_SPDO] SAFE_NO_INIT_SEKTOR;


static void ProcessData(BYTE_B_INSTNUM_ UINT32 dw_ct, UINT16 w_rxSpdoIdx,
                        const EPLS_t_FRM_HDR *ps_rxSpdoHeader,
                        const UINT8 *pb_rxSpdoData);

#if (SPDO_cfg_MAX_NO_RX_SPDO != 0)
/**
 * @brief This function connects a RxSPDO to the TxSPDO in which the synchronization request is sent.
 *
 * @param        b_instNum                  instance number (not checked, checked in SSC_ProcessSNMTSSDOFrame()) valid range: 0..(EPLS_cfg_MAX_INSTANCES-1)
 *
 * @param        w_rxSpdoIdx                index in the internal array of the Rx SPDO structures (not checked,
 *      created in SPDO_ActivateRxSpdoMapping()), valid range: 0..(SPDO_cfg_MAX_NO_RX_SPDO-1)
 *
 * @return
 * - TRUE if the operation was successful
 * - FALSE otherwise
 */
BOOLEAN SPDO_ConsSmConnect(BYTE_B_INSTNUM_ UINT16 w_rxSpdoIdx)
{
  /* pointer to the Consumer SM structure */
  t_RX_CONS_SM *ps_consSm = &as_ConsSm[B_INSTNUMidx][w_rxSpdoIdx];
  UINT16 w_TxSpdoIdx;
  BOOLEAN o_retVal = TRUE;
  /* index = number - 1 */
  w_TxSpdoIdx = SPDO_GetTxSpdoNo(B_INSTNUM_ w_rxSpdoIdx) - 1;
  /* check if SPDO number is valid */
  if ( SPDO_cfg_MAX_NO_TX_SPDO > w_TxSpdoIdx)
  {
    o_retVal = SPDO_SyncOkConnect(B_INSTNUM_ w_TxSpdoIdx, ps_consSm);
  }

  return ( o_retVal);
}
#endif
/**
 * @brief This function initializes pointers of the t_RX_COMM_PARAM structure to the Rx communication parameters and the
 * variables for the SPDO Consumer State Machine.
 *
 * @param        b_instNum                  instance number (not checked, checked in SSC_InitAll()) valid range: 0..(EPLS_cfg_MAX_INSTANCES-1)
 *
 * @param        w_rxSpdoIdx                index in the internal array of the Rx SPDO structures (not checked, checked in RxMappCommInit()), valid range: 0..(SPDO_cfg_MAX_NO_RX_SPDO-1)
 *
 * @param        pw_sct                     pointer to the SCT in the SOD (not checked, only called with reference to variable in SPDO_SetPtrToRxCommPara()) valid range: <> NULL
 *
 * @param        pw_minSPDOPropDelay        pointer to the minimum prop. delay in the SOD
 * 	(not checked, only called with reference to variable in SPDO_SetPtrToRxCommPara()) valid range: <> NULL
 *
 * @param        pw_maxSPDOPropDelay        pointer to the maximum prop. delay in the SOD
 * 	(not checked, only called with reference to variable in SPDO_SetPtrToRxCommPara()) valid range: <> NULL
 */
void SPDO_ConsSmInit(BYTE_B_INSTNUM_ UINT16 w_rxSpdoIdx, const UINT16 *pw_sct,
                     const UINT16 *pw_minSPDOPropDelay,
                     const UINT16 *pw_maxSPDOPropDelay)
{
  /* pointer to the Consumer SM structure */
  t_RX_CONS_SM *ps_consSm = &as_ConsSm[B_INSTNUMidx][w_rxSpdoIdx];

  ps_consSm->pw_sct = pw_sct;
  ps_consSm->pw_minSPDOPropDelay = pw_minSPDOPropDelay;
  ps_consSm->pw_maxSPDOPropDelay = pw_maxSPDOPropDelay;

  SPDO_ConsSmResetRx(B_INSTNUM_ w_rxSpdoIdx);

  SCFM_TACK_PATH();
}

/**
 * @brief This function resets the SPDO Consumer State Machines.
 *
 * @param        b_instNum          instance number (not checked, checked in SSC_ProcessSNMTSSDOFrame() or SSC_InitAll()), valid range: 0..(EPLS_cfg_MAX_INSTANCES-1)
 *
 * @param        w_rxSpdoIdx        index in the internal array of the Rx SPDO structures
 * 	(not checked, checked in RxMappCommInit() or SPDO_ResetRxSm()), valid range: 0..(SPDO_cfg_MAX_NO_RX_SPDO-1)
 */
void SPDO_ConsSmResetRx(BYTE_B_INSTNUM_ UINT16 w_rxSpdoIdx)
{
  /* pointer to the Consumer SM structure */
  t_RX_CONS_SM *ps_consSm = &as_ConsSm[B_INSTNUMidx][w_rxSpdoIdx];

  ps_consSm->o_safeState = TRUE;

  ps_consSm->o_lastValidRxSpdoCtChk = FALSE;
  ps_consSm->w_lastValidRxSpdoCt    = 0U;

  ps_consSm->o_timeSyncOk = FALSE;
  ps_consSm->w_tRefCons   = 0U;
  ps_consSm->w_tRefProd   = 0U;

  ps_consSm->dw_sct = 0UL;

  /* All references to the SOD entries are initialized once in
     SPDO_ConsSmInit(). These references do not have to be reset */

  /* call the Control Flow Monitoring */
  SCFM_TACK_PATH();
}

/**
 * @brief This function signals that the time synchronization for the SPDO Consumer State Machines succeeded.
 *
 * @param        b_instNum          instance number (not checked, checked in SPDO_ProcessRxSpdo()) valid range: 0..(EPLS_cfg_MAX_INSTANCES-1)
 *
 * @param        w_rxSpdoIdx        index in the internal array of the Rx SPDO structures (not checked, checked in SPDO_GetSpdoIdxForSAdr() and ProcessTRes()), valid range: 0..(SPDO_cfg_MAX_NO_RX_SPDO-1)
 *
 * @param        w_tRefCons         time reference of the consumer (not checked, any value allowed), valid range: (UINT16)
 *
 * @param        w_tRefProd         time reference of the producer (not checked, any value allowed), valid range: (UINT16)
 *
 * @param        w_tPropDelay       propagation delay of the synchronization (not checked, any value allowed), valid range: (UINT16)
 *
 */
void SPDO_ConsSmTimeSyncSucceeded(BYTE_B_INSTNUM_ UINT16 w_rxSpdoIdx,
                                  UINT16 w_tRefCons, UINT16 w_tRefProd,
                                  UINT16 w_tPropDelay)
{
  /* pointer to the Consumer SM structure */
  t_RX_CONS_SM *ps_consSm = &as_ConsSm[B_INSTNUMidx][w_rxSpdoIdx];

  /* TRefCons and TRefProd are stored */
  ps_consSm->w_tRefCons = w_tRefCons;
  ps_consSm->w_tRefProd = w_tRefProd;
  ps_consSm->w_tPropDel = w_tPropDelay;

  /* Synchronization OK */
  ps_consSm->o_timeSyncOk = TRUE;

  SCFM_TACK_PATH();
}

/**
 * @brief Realization of the "SPDO Consumer" state machine.
 *
 * This state machine processes the payload data in the received SPDOs, checks the safety control timer and manages
 * the time synchronization failure.
 *
 * @param        b_instNum                instance number (not checked, checked in SPDO_CheckRxTimeout() or SPDO_BuildTxSpdo() or SPDO_ProcessRxSpdo()), valid range: 0..(EPLS_cfg_MAX_INSTANCES-1)
 *
 * @param        dw_ct                    consecutive time, internal timer value (not checked, any value allowed) valid range: (UINT32)
 *
 * @param        w_rxSpdoIdx              Rx SPDO index (not checked, checked in SPDO_CheckRxTimeout() or SPDO_BuildTxSpdo() or ProcessTReq() or ProcessTRes() or SPDO_ProcessRxSpdo() and SPDO_GetSpdoIdxForSAdr()), valid range: 0..(SPDO_cfg_MAX_NO_RX_SPDO-1)
 *
 * @param        ps_rxSpdoHeader          reference to RxSPDO header info (pointer checked) valid range: <> NULL, == NULL
 *
 * @param        pb_rxSpdoData            reference to the data of the RxSPDO (pointer not checked, only called with NULL in SPDO_CheckRxTimeout() or SPDO_TimeSyncConsSm() or StateWaitForTres(), checked in SPDO_ProcessRxSpdo()) valid range: <> NULL, == NULL
 *
 * @param        o_timeSyncFailure        TRUE : time synchronization failure FALSE : no time synchronization failure (checked) valid range: TRUE,FALSE
 */
void SPDO_ConsSm(BYTE_B_INSTNUM_ UINT32 dw_ct, UINT16 w_rxSpdoIdx,
                 const EPLS_t_FRM_HDR *ps_rxSpdoHeader,
                 const UINT8 *pb_rxSpdoData, BOOLEAN o_timeSyncFailure)
{
  /* pointer to the Consumer SM structure */
  t_RX_CONS_SM *ps_consSm = &as_ConsSm[B_INSTNUMidx][w_rxSpdoIdx];

#if (SPDO_cfg_CONNECTION_VALID_BIT_FIELD == EPLS_k_ENABLE)
  /* the "connection valid" bit field is optional and not to be created on IO Modules */
  if ((o_timeSyncFailure) ||
    (ps_consSm->o_safeState))
  {
    #if (SPDO_cfg_CONNECTION_VALID_STATISTIC == EPLS_k_ENABLE)
    /* increase the statistic counter in case a connection turns from valid to invalid */
    if ( SHNF_aaulConnValidBit[B_INSTNUMidx][w_rxSpdoIdx / 32] & (0x00000001UL << (w_rxSpdoIdx % 32)))
    {
      SHNF_aaulConnValidStatistic[B_INSTNUMidx][w_rxSpdoIdx]++;
    }
    #endif
    /* reset the connection valid bit */
    SHNF_aaulConnValidBit[B_INSTNUMidx][w_rxSpdoIdx / 32] =
    SHNF_aaulConnValidBit[B_INSTNUMidx][w_rxSpdoIdx / 32] & (~(0x00000001UL << (w_rxSpdoIdx % 32)));
  }
  else if ( NULL != ps_rxSpdoHeader)
  {
    if ( 0 == (ps_rxSpdoHeader->b_id & k_FRAME_BIT_CONN_VALID))
    {
      #if (SPDO_cfg_CONNECTION_VALID_STATISTIC == EPLS_k_ENABLE)
      /* increase the statistic counter in case a connection turns from valid to invalid */
      if ( SHNF_aaulConnValidBit[B_INSTNUMidx][w_rxSpdoIdx / 32] & (0x00000001UL << (w_rxSpdoIdx % 32)))
      {
        SHNF_aaulConnValidStatistic[B_INSTNUMidx][w_rxSpdoIdx]++;
      }
      #endif
      /* reset the connection valid bit */
      SHNF_aaulConnValidBit[B_INSTNUMidx][w_rxSpdoIdx / 32] =
      SHNF_aaulConnValidBit[B_INSTNUMidx][w_rxSpdoIdx / 32] & (~(0x00000001UL << (w_rxSpdoIdx % 32)));
    }
    else
    {
      /* set the connection valid bit */
      SHNF_aaulConnValidBit[B_INSTNUMidx][w_rxSpdoIdx / 32] =
      SHNF_aaulConnValidBit[B_INSTNUMidx][w_rxSpdoIdx / 32] | (0x00000001UL << (w_rxSpdoIdx % 32));
    }
  }
#endif

  /* if the state machine is in the safe state */
  if  (ps_consSm->o_safeState)
  {
    /* all data related to the producer is set to default */
    SPDO_RxSpdoToSafeState(B_INSTNUM_ w_rxSpdoIdx);

    /* if the timeout has not to be checked */
    if (ps_rxSpdoHeader != NULL)
    {
      /* Rx SPDO data is processed */
      ProcessData(B_INSTNUM_ dw_ct, w_rxSpdoIdx, ps_rxSpdoHeader,
                  pb_rxSpdoData);
    }
    /* no else : only timeout checking (no timeout) */
  }
  /* else the state machine is not in the safe state (waiting for Rx SPDO) */
  else
  {
    /* if time synchronization failure */
    if (o_timeSyncFailure)
    {
      /* all data related to the producer is set to default */
      SPDO_RxSpdoToSafeState(B_INSTNUM_ w_rxSpdoIdx);
      /* Rx SPDO ct checking is disabled */
      ps_consSm->o_lastValidRxSpdoCtChk = FALSE;
      /* Time Synchronization is not completed */
      ps_consSm->o_timeSyncOk = FALSE;
      /* change state */
      ps_consSm->o_safeState = TRUE;
    }
    else /* no time synchronization failure */
    {
      /* if data was received (called by SPDO_ProcessRxSpdo()) */
      if (ps_rxSpdoHeader != NULL)
      {
        /* Rx SPDO data is processed */
        ProcessData(B_INSTNUM_ dw_ct, w_rxSpdoIdx, ps_rxSpdoHeader,
                    pb_rxSpdoData);
      }
      else /* else no data received, timeout is checked
              (called by SPDO_CheckRxTimeout()) */
      {
        /* if SCT timer elapsed (timeout) */
        if (EPLS_TIMEOUT(dw_ct, ps_consSm->dw_sct))
        {
          /* count the elapsed SCT events */
          SERR_CountSpdoEvt(B_INSTNUM_ w_rxSpdoIdx,SERR_k_SPDO_SCT_TOUT);
          /* SCT timer expired */
          SERR_SetError(B_INSTNUM_ SPDO_k_ERR_SCT_TIMER, (UINT32)w_rxSpdoIdx);
          /* all data related to the producer is set to default */
          SPDO_RxSpdoToSafeState(B_INSTNUM_ w_rxSpdoIdx);
          /* Rx SPDO ct checking is disabled */
          ps_consSm->o_lastValidRxSpdoCtChk = FALSE;
          /* Time Synchronization is not completed */
          ps_consSm->o_timeSyncOk = FALSE;
          /* change state */
          ps_consSm->o_safeState = TRUE;
        }
        /* no else : no SCT timeout */
      }
    }
  }

  /* call the Control Flow Monitoring */
  SCFM_TACK_PATH();
}

/**
 * @brief This function checks whether the CT in the received SPDO is valid or not.
 *
 * @param        b_instNum          instance number (not checked, checked in SPDO_ProcessRxSpdo()), valid range: 0..(EPLS_cfg_MAX_INSTANCES-1)
 *
 * @param        w_rxSpdoIdx        Rx SPDO index (not checked, checked in SPDO_ProcessRxSpdo() or ProcessTReq() or ProcessTRes() and SPDO_GetSpdoIdxForSAdr()), valid range: 0..(SPDO_cfg_MAX_NO_RX_SPDO-1)
 *
 * @param        w_rxSpdoCt         received consecutive time (not checked, any value allowed),  valid range: UINT16
 *
 * @return
 * - TRUE             - CT is valid (changed and increased)
 * - FALSE            - CT is invalid
 */
BOOLEAN SPDO_CtValid(BYTE_B_INSTNUM_ UINT16 w_rxSpdoIdx, UINT16 w_rxSpdoCt)
{
  BOOLEAN o_ret = FALSE; /* return value */
  /* pointer to the Consumer SM structure */
  t_RX_CONS_SM *ps_consSm = &as_ConsSm[B_INSTNUMidx][w_rxSpdoIdx];

  /* if CT has not to be checked or CT has changed and increased */
  if (!ps_consSm->o_lastValidRxSpdoCtChk ||
      ((w_rxSpdoCt != ps_consSm->w_lastValidRxSpdoCt) &&
      CT_VALID(w_rxSpdoCt, ps_consSm->w_lastValidRxSpdoCt)))
  {
    o_ret = TRUE;
  }
  /* no else : CT is invalid */

  SCFM_TACK_PATH();
  return o_ret;
}

#pragma CTC SKIP
/**
* @brief Returns a pointer to the internal object. This function is only called by the unit test.
*
* @note This function is used for unit tests.
*
* @param        b_instNum          instance number (not checked) valid range: 0..(EPLS_cfg_MAX_INSTANCES-1)
* @param        w_rxSpdoIdx        Rx SPDO index (not checked) valid range: 0..(SPDO_cfg_MAX_NO_RX_SPDO-1)
*
* @return
* - == NULL - memory allocation failed
* - <> NULL - memory allocation and the mapping processing succeeded, reference to the openSAFETY frame
*/
const t_RX_CONS_SM* SPDO_GetRxConsSmObj(BYTE_B_INSTNUM_ UINT16 w_rxSpdoIdx)
{
  const t_RX_CONS_SM* ps_obj = (t_RX_CONS_SM *)NULL;
  #if (EPLS_cfg_MAX_INSTANCES > 1)
    /* check range of instance number */
    if(B_INSTNUMidx < (UINT8)EPLS_cfg_MAX_INSTANCES)
    {
      /* get pointer to object */
      ps_obj = &as_ConsSm[B_INSTNUMidx][w_rxSpdoIdx];
    }
    /* else - do nothing */
  #else
    ps_obj = &as_ConsSm[B_INSTNUMidx][w_rxSpdoIdx];
  #endif

  /* call the Control Flow Monitoring */
  SCFM_TACK_PATH();

  return ps_obj;
}
#pragma CTC ENDSKIP


/**
* @brief Realization of the "Process Data" state machine.
*
* @param        b_instNum              instance number (not checked, checked in SPDO_CheckRxTimeout() or SPDO_BuildTxSpdo() and SPDO_ProcessRxSpdo()), valid range: 0..(EPLS_cfg_MAX_INSTANCES-1)
*
* @param        dw_ct                  consecutive time, internal timer value (not checked, any value allowed) valid range: (UINT32)
*
* @param        w_rxSpdoIdx            Rx SPDO index (not checked, checked in SPDO_CheckRxTimeout() or SPDO_BuildTxSpdo() or ProcessTReq() or ProcessTRes() or SPDO_ProcessRxSpdo() and SPDO_GetSpdoIdxForSAdr()), valid range: 0..(SPDO_cfg_MAX_NO_RX_SPDO-1)
*
* @param        ps_rxSpdoHeader        reference to RxSPDO header info (pointer checked) valid range: <> NULL, == NULL
*
* @param        pb_rxSpdoData          reference to the data of the RxSPDO (pointer not checked, checked in SPDO_ProcessRxSpdo()) valid range: <> NULL
*/
static void ProcessData(BYTE_B_INSTNUM_ UINT32 dw_ct, UINT16 w_rxSpdoIdx,
                        const EPLS_t_FRM_HDR *ps_rxSpdoHeader,
                        const UINT8 *pb_rxSpdoData)
{
  /* pointer to the Consumer SM structure */
  t_RX_CONS_SM *ps_consSm = &as_ConsSm[B_INSTNUMidx][w_rxSpdoIdx];
  UINT16 w_propDelay;  /* temporary variable for the propagation delay */
  UINT16 w_deltaTCons; /* temporary variable for delta T consumer */
  UINT16 w_deltaTProd; /* temporary variable for delta T producer */

  /* if time synchronization is OK */
  if (ps_consSm->o_timeSyncOk)
  {
    /* T SPDO consumer - T Ref consumer */
    w_deltaTCons = (UINT16)((UINT16)(dw_ct) - ps_consSm->w_tRefCons);
    /* T SPDO producer - T Ref producer */
    w_deltaTProd = (UINT16)(ps_rxSpdoHeader->w_ct - ps_consSm->w_tRefProd);
    /* propagation delay */
    w_propDelay = (UINT16)(w_deltaTCons - w_deltaTProd);

    /* if propagation delay is too short */
    if ((w_propDelay < *ps_consSm->pw_minSPDOPropDelay) ||
    	(w_deltaTProd > w_deltaTCons))
    {
      /* count the propagation delay too short events */
      SERR_CountSpdoEvt(B_INSTNUM_ w_rxSpdoIdx,SERR_k_SPDO_PROP_SHORT);

      SERR_SetError(B_INSTNUM_ SPDO_k_ERR_DELAY_SHORT,
                    (((UINT32)(w_propDelay))<<16) + w_rxSpdoIdx);

      /* all data related to the producer is set to zero */
      SPDO_RxSpdoToSafeState(B_INSTNUM_ w_rxSpdoIdx);
      /* Rx SPDO ct checking is disabled */
      ps_consSm->o_lastValidRxSpdoCtChk = FALSE;
      /* Time Synchronization is not completed */
      ps_consSm->o_timeSyncOk = FALSE;
      /* change state */
      ps_consSm->o_safeState = TRUE;
    }
    /* else if propagation delay is OK */
    else if ((w_propDelay <= *ps_consSm->pw_maxSPDOPropDelay) &&
             (w_propDelay >= *ps_consSm->pw_minSPDOPropDelay))
    {
      /* do not take over the data of a telegram if the CT was not checked */
      if (!ps_consSm->o_lastValidRxSpdoCtChk)
      {
          /** SPDO data is not valid **/
          /* all data related to the producer is set to zero */
          SPDO_RxSpdoToSafeState(B_INSTNUM_ w_rxSpdoIdx);
          /* the ct in the rx SPDO is checked */
          ps_consSm->o_lastValidRxSpdoCtChk = TRUE;
          /* the valid ct is stored */
          ps_consSm->w_lastValidRxSpdoCt = ps_rxSpdoHeader->w_ct;
          /* change state */
          ps_consSm->o_safeState = TRUE;
      }
      /* if payload size is right (SPDO data to SOD) */
      else if (SPDO_RxMappingProcess(B_INSTNUM_ w_rxSpdoIdx,
                                ps_rxSpdoHeader->b_le,pb_rxSpdoData) == TRUE)
      {
        /** SPDO data is valid **/

        /* the ct in the rx SPDO is checked */
        ps_consSm->o_lastValidRxSpdoCtChk = TRUE;
        /* calculate the producer delta */
        ps_consSm->w_dCtProd = ps_rxSpdoHeader->w_ct -
        		ps_consSm->w_lastValidRxSpdoCt;
        /* the valid ct is stored */
        ps_consSm->w_lastValidRxSpdoCt = ps_rxSpdoHeader->w_ct;
        /* SCT timer is (re)started */
        ps_consSm->dw_sct = (UINT32)(dw_ct + *ps_consSm->pw_sct);
        /* change state */
        ps_consSm->o_safeState = FALSE;
      }
      /* no else : wrong payload size */
    }
    /* else propagation delay is too long */
    else
    {
      /* count the propagation delay too long events */
      SERR_CountSpdoEvt(B_INSTNUM_ w_rxSpdoIdx,SERR_k_SPDO_PROP_LONG);
      SERR_SetError(B_INSTNUM_ SPDO_k_ERR_DELAY_LONG,
                    (((UINT32)(w_propDelay))<<16) + w_rxSpdoIdx);
    }
  }
  /* no else : time synchronization is not finished, the SPDO payload data
                is ignored without generating an error */

  /* call the Control Flow Monitoring */
  SCFM_TACK_PATH();
}

/**
* @brief This function reads the status of the SPDO.
*
* @param       b_instNum           instance number, valid range: 0..(EPLS_cfg_MAX_INSTANCES-1)
*
* @param       w_rxSpdoIdx         Rx SPDO index, valid range: 0..(SPDO_cfg_MAX_NO_RX_SPDO-1)
*
* @param       dw_ct                actual consecutive time, any value allowed, valid range: UINT16
*
* @retval         pw_dCt              time between the last two valid RxSPDOs:
*
* @retval         pdw_ageSpdo         age of the current data
*
* @retval         pw_propDel          propagation delay of the last synchronization
*
* @return
* - TRUE             - data of RxSPDO is valid
* - FALSE            - data of RxSPDO is not valid
*/
BOOLEAN SPDO_GetRxSpdoStatus(BYTE_B_INSTNUM_ UINT16 const w_rxSpdoIdx, UINT32 const dw_ct,
		UINT16 * const pw_dCt, UINT32 * const pdw_ageSpdo, UINT16 * const pw_propDel)
{
	t_RX_CONS_SM *ps_consSm = NULL;
	BOOLEAN	bRetVal = FALSE;

#if (EPLS_cfg_MAX_INSTANCES > 1)
	/* if b_instNum is wrong */
	if(B_INSTNUMidx >= (UINT8)EPLS_cfg_MAX_INSTANCES)
	{
		SERR_SetError(B_INSTNUM_ SPDO_k_ERR_INST_INV,
					(UINT32)B_INSTNUMidx);
	}
	/* not main instance and SPDO index too high */
	else if ((0 != B_INSTNUMidx) &&
			 (SPDO_cfg_MAX_NO_RX_SPDO_SDG <= w_rxSpdoIdx))
	{
		SERR_SetError(B_INSTNUM_ SPDO_k_ERR_RX_SPDO_IDX,
					(((UINT32)(B_INSTNUMidx))<<16) + w_rxSpdoIdx);
	}
	else
#endif
	/* main instance and SPDO index too high */
	if ((0 == B_INSTNUMidx) &&
		(SPDO_cfg_MAX_NO_RX_SPDO <= w_rxSpdoIdx))
	{
		SERR_SetError(B_INSTNUM_ SPDO_k_ERR_RX_SPDO_IDX,
					(((UINT32)(B_INSTNUMidx))<<16) + w_rxSpdoIdx);
	}
	/* check input data ranges and return pointers */
	else if ((NULL == pw_dCt) ||
			 (NULL == pdw_ageSpdo) ||
			 (NULL == pw_propDel))
	{
		SERR_SetError(B_INSTNUM_ SPDO_k_ERR_PTR_INV,0UL);
	}
	else
	{
		/* set the ptr to the SPDO instance */
		ps_consSm = &as_ConsSm[B_INSTNUMidx][w_rxSpdoIdx];
		/* check if process data is valid */
		if ((ps_consSm->o_timeSyncOk) &&
			(!ps_consSm->o_safeState))
		{
			/* set return data valid */
			bRetVal	= TRUE;
			/* set return data once */
			*pw_dCt	= ps_consSm->w_dCtProd;
			*pdw_ageSpdo = dw_ct - (ps_consSm->dw_sct - (UINT32)*ps_consSm->pw_sct);
			*pw_propDel = ps_consSm->w_tPropDel;
		}
	}
	return bRetVal;
}

#else
  /* This file is compiled with different configuration (EPLScfg.h).
     If SPDO_cfg_MAX_NO_RX_SPDO is 0 then a compiler warning can be generated.
     To avoid this warning, remove this file from the project. */
#endif
/** @} */
