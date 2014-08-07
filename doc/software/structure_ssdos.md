Safety Service Data Server (SSDOS) {#sw_ssdos}
----------------------------------

The internal module [SSDOS] enables access to the local object directory via the network. This can be done segmented or non-segmented and with read or write access. Only 1 [SSDO] transfer can be made at one time for each instance.

Because timeout monitoring is not implemented, any current transfers will always be interrupted by a new transfer. There is only one [SCM] for each [SD] and only the [SCM] has [SSDO] client functionality.

If a segmented transfer takes place, then the [SOD] is accessed directly and therefore blocked for the amount of time needed for the transfer.

[SCM]: @ref SNMT "Safety Configuration Manager (SCM)"
[SN]: @ref SN "Safety Node (SN)"
[SDG]: @ref SDG "Safety Domain Gateway (SDG)"
[SSDO]: @ref SSDO "Safety Service Data Object (SSDO)"
[SSDOC]: @ref SSDO "Safety Service Data Object Client (SSDOC)"
[SSDOS]: @ref SSDO "Safety Service Data Object Server (SSDOS)"
[SSC]: @ref SSC "Safety Stack Control (SSC)"

[SOD]: @ref [SOD] "Safety Object Dictionary (SOD)"

