Safety Service Data Client (SSDOC) {#sw_ssdoc}
-----------------------------------

The module [SSDOC] enables access to the local object directory of a remote [SSDO] server. This can be done segmented or non-segmented and with read or write access.

In addition to the segmented and non-segmented transfer a slim [SSDO] service was added for fast downloading the initialization data to an [SN] (write access). This service may not be used by the application. The slim [SSDO] service uses a different CRC16 (polynomial 0x755B) for which the calculation function has to be provided by the [SHNF].

The function SSDOC_SendReadReq() can be called to read the data of an SN from the SOD. The function SSDOC_SendWriteReq() can be called to write data.

Multiple simultaneous accesses are possible.

The [SCM] also performs [SSDO] access. The request must be repeated if a response is not received. The function SSDOC_BuildRequest()  must be called cyclically to re-send requests if necessary.

[sw_struct]: ../software_structure.png "openSAFETY software structuree"
[INTEGRATION_GUIDE]: @ref integration_guide "Integration Guide"

[SCM]: @ref SNMT "Safety Configuration Manager (SCM)"
[SN]: @ref SN "Safety Node (SN)"
[SSDO]: @ref SSDO "Safety Service Data Object (SSDO)"
[SSDOC]: @ref SSDO "Safety Service Data Object Client (SSDOC)"
[SSDOS]: @ref SSDO "Safety Service Data Object Server (SSDOS)"

[SOD]: @ref [SOD] "Safety Object Dictionary (SOD)"
