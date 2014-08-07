Safety Process Data (SPDO) {#sw_spdo}
--------------------------

The [SPDO] module is used to cyclically exchange process data and to handle time synchronization and time validation.

The function SPDO_TxDataChanged() can be called if the data for an [SPDO] has changed and the [SPDO] should be sent even though the time has not yet expired.

The openSAFETY software supports dynamic mapping (i.e. the mapping parameters can be changed during operation). Bit mapping different than [1] is not supported (i.e. one byte must always be used for a bit) . 

The mapping parameters can only be changed to whatever sequence needed when in the preoperational state, because mapping is not activated until the state changes to "Operational". The RxSPDO and TxSPDO mapping parameters (object 0x1800 and 0xC000 plus following objects) can also be written with 0.

When working with mapping parameters, the user must make sure that there is a valid object for the same amount of respective entries (sub-index 00h).

For example, if the sub-index 00h (NumberOfEntries) equals 3, then sub-index 01h to 03h must contain valid mapping entries. The sub-indices starting with 04h are not taken into consideration, but should be 0.

When mapping, the application must take into account that the maximum length is used for mapped objects with variable length (e.g. domains) in the [SPDO].

Due to the speed, the range of objects of an [SPDO] is not checked and callback functions are not called.
The application must ensure that no range violations or incompatibilities occur with mapped objects.

Using the function SPDO_GetRxSpdoStatus() the application can additionally validate the timing of the received data itself.

The RxSPDOs also do provide a connection valid bit if enabled. The variable needs to be provided by the application but the related bits are set and reset by the stack. Each RxSPDO owns one bit in the bit field which is set if the connection to the producer is synchronized and valid. 

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

[SOD]: @ref [SOD] "Safety Object Dictionary (SOD)"

[SCFM]: @ref SCFM "Safety Control Flow Monitoring (SCFM)"
[SHNF]: @ref SHNF "Safety Hardware Near Firmware (SHNF)"
