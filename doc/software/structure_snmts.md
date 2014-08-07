Safety Network Management Slave (SNMTS) {#sw_snmts}
---------------------------------------

The following tasks are performed by the [SNMTS] module:
- Processing [SNMT] requests and creating a response, if necessary. When doing this, the module is called internally by the [SSC] module
- Monitoring the guard and refresh time. To monitor the guard and refresh time, the function SNMTS_TimerCheck() must be called cyclically.
- Managing the node state. After initialization, the application can optionally update the object directory (e.g. with values saved in non-volatile memory).

The function SNMTS_PerformTransPreOp() must be called after initialization. When this function is called, the node is changed from the "Initialization" state to the "Pre-Operational" state.

If the callback function SAPL_SNMTS_CalcParamChkSumClbk() is called, then the application must validate the [SOD] checksum(s) and transfer the result to the function SNMTS_PassParamChkSumValid().

After the callback function SAPL_SNMTS_SwitchToOpReqClbk() is called, the application is then able to save parameters in non-volatile memory. Of course, the parameters can also be saved at a later point in time.

The application can call the function SNMTS_GetSnState() to query the node state at any time. If the SN receives the service "SNMT_SN_ACK", then the application will be informed via the callback function SAPL_SNMTS_ErrorAckClbk() that the [SCM] has confirmed an error.

An openSAFETY frame will be rejected if the [SADR] or [SDN] does not match its own. 

The services SNMT_SCM_set_to_STOP and SNMT_SCM_set_to_OP will be processed by the openSAFETY software stack as long as it is an [SCM]. The two requests must be sent using a tool according to [1].

[INTEGRATION_GUIDE]: @ref integration_guide "Integration Guide"

[SCM]: @ref SNMT "Safety Configuration Manager (SCM)"
[SN]: @ref SN "Safety Node (SN)"
[SDG]: @ref SDG "Safety Domain Gateway (SDG)"
[SNMT]: @ref SNMT "Safety Network Management (SNMT)"
[SNMTS]: @ref SNMTS "Safety Network Management Slave (SNMTS)"
[SSDO]: @ref SSDO "Safety Service Data Object (SSDO)"
[SSDOC]: @ref SSDO "Safety Service Data Object Client (SSDOC)"
[SSDOS]: @ref SSDO "Safety Service Data Object Server (SSDOS)"
[SPDO]: @ref SPDO "Safety Process Data Object (SPDO)"
[SERR]: @ref SERR "Safety Error Reporting (SERR)"
[SSC]: @ref SSC "Safety Stack Control (SSC)"
[EPLS]: @ref EPLS "openSAFETY Support Functionality (EPLS)"

[SOD]: @ref SOD "Safety Object Dictionary (SOD)"

[SCFM]: @ref SCFM "Safety Control Flow Monitoring (SCFM)"
[SHNF]: @ref SHNF "Safety Hardware Near Firmware (SHNF)"
