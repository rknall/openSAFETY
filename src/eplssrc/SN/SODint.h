/**
 * @addtogroup SOD
 * @{
 *
 * @file SODint.h
 *
 * This file is the internal header-file of the unit.
 *
 * @copyright Copyright (c) 2009,  Bernecker + Rainer Industrie-Elektronik Ges.m.b.H and IXXAT Automation GmbH
 * @copyright All rights reserved, Bernecker + Rainer Industrie-Elektronik Ges.m.b.H
 *
 * @copyright This source code is free software; you can redistribute it and/or modify it under the terms of the BSD license (according to License.txt).
 *
 * @author M. Molnar, IXXAT Automation  GmbH
 *
 */

#ifndef SODINT_H
#define SODINT_H


/**
 * This define is used to access the low limit of the object in the range array.
*/
#define k_RANGE_LOW_LIMIT  0

/**
 * This define is used to access the high limit of the object in the range array.
*/
#define k_RANGE_HIGH_LIMIT 1

/**
* @brief This function initializes the flags used to lock and disable the SOD.
*
* @param        b_instNum         instance number (not checked, checked in SSC_InitAll()) valid range: 0..(EPLS_cfg_MAX_INSTANCES-1)
*/
void SOD_InitFlags(BYTE_B_INSTNUM);

/**
* @brief This function initializes the addresses and number of entries.
*
* Start-, end- and actual address are set and number of SOD entries are calculated.
*
* @param        b_instNum         instance number (not checked, checked in SSC_InitAll()) valid range: 0..(EPLS_cfg_MAX_INSTANCES-1)
*
* @return
* - TRUE            - success
* - FALSE           - failure
*/
BOOLEAN SOD_InitAddr(BYTE_B_INSTNUM);

/**
* @brief This function checks the handle of the SOD entry.
*
* @param        b_instNum            instance number (not checked, checked in SOD_ActualLenSet() or SOD_Write() or SOD_Read() or SOD_ActualLenGet()) valid range: 0..(EPLS_cfg_MAX_INSTANCES-1)
*
* @param        dw_hdl               object handle (checked) valid range: UINT32
*
* @retval       ps_errRes
* - == NULL error is generate in the function
* - <> NULL error result, only relevant if the return value == FALSE, (pointer not checked, any value allowed), valid range : <> or == NULL
*
* @return
* - TRUE                 - SOD entry handle is valid
* - FALSE              - SOD entry handle is not valid, see ps_errRes
*/
BOOLEAN SOD_HdlValid(BYTE_B_INSTNUM_ UINT32 dw_hdl,
                     SOD_t_ERROR_RESULT *ps_errRes);

/**
* @brief This function gets a pointer to the first SOD object.
*
* @param      b_instNum      instance number (not checked, checked in SSC_InitAll()) valid range: 0..(EPLS_cfg_MAX_INSTANCES-1)
*
* @return     Pointer to the first SOD object.
*/
const SOD_t_OBJECT *SOD_GetSodStartAddr(BYTE_B_INSTNUM);

/**
* @brief This function checks the openSAFETY Object Dictionary (SOD), calculates and sets the length of the SOD.
*
* @param        b_instNum        instance number (not checked, checked in SSC_InitAll()), valid range: 0..(EPLS_cfg_MAX_INSTANCES-1)
*
* @return
* - TRUE            - success
* - FALSE           - failure
*/
BOOLEAN SOD_Check(BYTE_B_INSTNUM);

#endif

/** @} */
