/**
 * @addtogroup SCM
 * @{
 * @file SCMverParam.c
 *
 * FSM state functions for the process "Verify Parameters".
 *
 * @copyright Copyright (c) 2009,  Bernecker + Rainer Industrie-Elektronik Ges.m.b.H and IXXAT Automation GmbH
 * @copyright All rights reserved, Bernecker + Rainer Industrie-Elektronik Ges.m.b.H
 * @copyright This source code is free software; you can redistribute it and/or modify it under the terms of the BSD license (according to License.txt).
 *
 * @author Joachim Stolberg, IXXAT Automation GmbH
 * @author M. Molnar, IXXAT Automation GmbH
 *
 * The aim of the SCM is to configure, verify and start all SNs of the assigned domain.
 * After the configuration of the SNs the SCM performs the node guarding for all SNs,
 * which have reached the state OPERATIONAL.
 *
 * <h2>History for SCMverParam.c</h2>
 * <table>
 *     <tr><th>Date</th><th>Author</th><th>Change Description</th></tr>
 *     <tr><td>03.06.2009</td><td>Hans Pill</td><td>Review SLV10</td></tr>
 *     <tr><td>07.03.2013</td><td>Hans Pill</td><td>changes for parameter checksum being written to the SN</td></tr>
 *     <tr><td>23.03.2013</td><td>Hans Pill</td><td>changes for handling of the new timestamp and CRC domain</td></tr>
 *     <tr><td>09.08.2013</td><td>Hans Pill</td><td>Review OpenSAFETY 1.4.0.0</td></tr>
 * </table>
 *
 * @addtogroup fsm_verify_param FSM - Verify Parameters
 * @{
 */

#include "SCMint.h"

/**
* @brief This function represents the state SCM_k_WF_TIMESTAMP of the finite state machine ScmFsmProcess().
*
* @param         ps_fsmCb       Pointer to the current slot of the FSM control block (not checked, only called with reference to struct in ScmFsmProcess()).
*
* @param        w_snNum         FSM slot number (=> index to the DVI list)(not checked, checked in SCM_Trigger()). Valid range: < SCM_cfg_MAX_NUM_OF_NODES
*
* @param        dw_ct           Consecutive time (not checked, any value allowed). Valid range: UINT32
*
* @return
* - FALSE - Abort forcing error
*/
BOOLEAN SCM_WfTimestamp(SCM_t_FSM_CB *ps_fsmCb, UINT16 w_snNum, UINT32 dw_ct)
{
  BOOLEAN o_res = FALSE;                 /* function result */
  UINT8 *pb_timeCrc = (UINT8 *)NULL;     /* pointer to the timestamp and crc domain */
  UINT32 dw_timeCrcLen = 0;              /* length of the timestamp and crc domain */

  /* if response received */
  if(ps_fsmCb->w_event == SCM_k_EVT_SSDOC_RESP_RX)
  {
    /* reset the event */
    ps_fsmCb->w_event = SCM_k_EVT_NO_EVT_OCCURRED;
    /* read the timestamp and crc domain from the DVI list */
    pb_timeCrc = (UINT8 *)SCM_SodRead(SCM_k_IDX_PARA_CHKSM(w_snNum));

    if(pb_timeCrc != NULL)
    {
      /* try to read the parameter set length */
      dw_timeCrcLen = SCM_SodGetActLen(SCM_k_IDX_PARA_CHKSM(w_snNum));
      /* if remote timestamp and crc domain ==
       * timestamp and crc domain from DVI list */
      if ((0 != dw_timeCrcLen) &&
    	  (dw_timeCrcLen == ps_fsmCb->dw_SsdocUploadLen) &&
    	  (0 == MEMCOMP(pb_timeCrc,ps_fsmCb->pb_remTimeCrc,dw_timeCrcLen)))
      {
        ps_fsmCb->w_event = SCM_k_EVT_GENERIC_EVENT;
        ps_fsmCb->e_state = SCM_k_SEND_PUT_TO_OP;
        o_res = TRUE;
      }
      else /* wrong received timestamp */
      {
        /* set SN to PRE_OP */
        o_res = SNMTM_ReqSnTrans(dw_ct, w_snNum, ps_fsmCb->w_sadr,
                                 SNMTM_k_OP_TO_PREOP, 0U);

        /* if no error happened */
        if (o_res)
        {
          ps_fsmCb->e_state = SCM_k_WF_PRE_OP_RESP;
          SCM_NumFreeFrmsDec();
        }
        /* no else : error happened */
      }
    }
    /* else: o_res = FALSE */
  }
  /* else if SSDOC timeout occurred */
  else if (ps_fsmCb->w_event == SCM_k_EVT_SSDOC_TIMEOUT)
  {
    /* reset the event */
    ps_fsmCb->w_event = SCM_k_EVT_NO_EVT_OCCURRED;
    /* set node state to MISSING and restart FSM */
    o_res = SCM_SetNodeStatus(ps_fsmCb, w_snNum, SCM_k_NS_MISSING, FALSE);

    /* if no error happened */
    if (o_res)
    {
      ps_fsmCb->e_state = SCM_k_SEND_ASSIGN_SADR_REQ;
      ps_fsmCb->w_event = SCM_k_EVT_GENERIC_EVENT;
    }
  }
  /* else if error occurred */
  else if(ps_fsmCb->w_event == SCM_k_EVT_RESP_ERROR)
  {
    /* reset the event */
    ps_fsmCb->w_event = SCM_k_EVT_NO_EVT_OCCURRED;
    /* set node state to INVALID */
    o_res = SCM_SetNodeStatus(ps_fsmCb, w_snNum, SCM_k_NS_INVALID,
                              ps_fsmCb->o_reportSnSts);

    /* if no error happened */
    if (o_res)
    {
      /* set the timer to the guard frequency */
      ps_fsmCb->dw_timer = dw_ct + SCM_dw_SnGuardTime;
      ps_fsmCb->e_state = SCM_k_IDLE2;
    }
    /* no else : error happened */
  }
  else  /* other events ignored */
  {
    /* error is generated in ScmFsmProcess() */
    o_res = TRUE;
  }
  /* call the Control Flow Monitoring */
  SCFM_TACK_PATH();
  return o_res;
}

/* @} */
/* @} */
