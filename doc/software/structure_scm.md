Safety Configuration Manager (SCM) {#sw_scm}
----------------------------------

The [SCM] module is used for configuring, verifying and monitoring all SNs within an openSAFETY domain. 

The configuration can also be terminated by calling the function SCM_Deactivate() (e.g. to change the configuration). The configuration can then be started from the beginning by calling SCM_Activate().

The [SOD] entries that are relevant to the configuration cannot be changed during the configuration.

The function SCM_Trigger() must be called cyclically to handle the configuration of any [SN].

The function SCM_ResetNodeGuarding() can be called to restart the monitoring (Node Guarding).
The callback function SAPL_ScmUdidMismatchClbk() is called in the event of a "UDID mismatch".

A "UDID mismatch" must be confirmed by calling the function SCM_OperatorAck().

If the activation of an SN fails, then the function SAPL_ScmSnFailClbk() is called.

The callback functions SAPL_ScmNodeStatusChangedClbk() and SAPL_ScmRevisionNumberClbk() inform the application if the state of a node changes or if a revision number is received.

The [SCM] also checks, weather an [SN] is to be processed or not using the function SAPL_ScmProcessSn() which the application must provide. Using this function, the application can exclude certain SNs from being processed.

Problems could otherwise occur such as on the object 0x100C (GuardTime_U32) because it is read just once when the [SCM] is started. Problems could also rise if the [SN] locks the [SOD] to calculate the CRC because the [SCM] also has to access the [SOD] (e.g. to enter the node state of other SNs).

The [SCM] always configures and monitors just one domain in accordance with [1]. This is why the [SCM] is always assigned the fixed instance 0.

[sw_struct]: ../software_structure.png "openSAFETY software structuree"
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
