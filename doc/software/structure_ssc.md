Safety Stack Control (SSC) {#sw_ssc}
--------------------------

The [SSC] module is the central interface for the application. The openSAFETY software stack must be initialized using the function SSC_InitAll() before any other API function is called. 

If a new [SNMT] or [SSDO] frame is received, then it must be transferred to the function SSC_ProcessSNMTSSDOFrame().

[SNMT]: @ref SNMT "Safety Network Management (SNMT)"
[SSDO]: @ref SSDO "Safety Service Data Object (SSDO)"
[SSC]: @ref SSC "Safety Stack Control (SSC)"
