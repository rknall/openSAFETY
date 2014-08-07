Safety Network Management (SNMTM)   {#sw_snmtm}
----------------------------------

The [SNMTM] module provides services for controlling the state of SNs as well as for monitoring and assigning an [SN] to SADRs. The module is used by the [SCM] mostly for sending [SNMTM] requests. Responses are forwarded to the [SNMTM] via the [SSC] unit (see section 2.2.1). The request must be sent again if a response is not received.

The function SNMTM_BuildRequest() must be called cyclically to resend a service request if necessary.

[SCM]: @ref SCM "Safety Configuration Manager (SCM)"
[SNMT]: @ref SNMT "Safety Network Management (SNMT)"
[SNMTS]: @ref SNMTS "Safety Network Management Slave (SNMTS)"
[SNMTM]: @ref SNMTM "Safety Network Management Master (SNMTM)"
[SSC]: @ref SSC "Safety Stack Control (SSC)"
