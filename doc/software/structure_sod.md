Safety Object Dictionary (SOD)               {#structure_sod}
------------------------------

The [SOD] module manages access to the object directory. One usage for the function SOD_AttrGetNext() is to save the object directory in non-volatile memory because this is something that is not performed by the stack.

This is necessary in order to ensure data consistency for segmented objects.

The function SOD_DisableSodWrite() can be called in order to lock the entire object directory from write access (both segmented and non-segmented objects - e.g. in order to perform a CRC calculation, see [SHNF]). The lock can be reversed by using the function SOD_EnableSodWrite().

It is especially important to make sure that write access to mandatory objects in the [SOD] (particularly the SPDO Mapping and Communication Parameter) is not permitted in the state "Operational" or during the transition to "Operational".

The openSAFETY-SW also supports a variable length for domains and strings. Variable length means that the current length can be shorter or equal to the maximum length (for more information about this, see SOD_t_ACT_LEN_PTR_DATA). The current length can be read by using the function SOD_AttrGet().

The [SOD] unit also has a few callback functions (e.g. SAPL_SOD_DefaultValueSetClbk()).

Objects in the [SOD] must be located directly in the memory (via a reference in the structure SOD_t_OBJECT). This means that delayed access attempts are either not possible, or only possible via repeated reading.

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
