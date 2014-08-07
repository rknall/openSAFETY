Safety Error (SERR)  {#sw_serr}
--------------------

The [SERR] unit makes it possible to detect errors in the stack and to inform the application via the function SAPL_SERR_SignalErrorClbk(). Additionally, the application can read a detailed error string about an error by using the function SERR_GetErrorStr().

If a "Fail Safe" error occurs (see "error type"), then the application must create a safe state for the device (potentially including all relevant [SOD] objects).

[SERR]: @ref SERR "Safety Error Reporting (SERR)"

[SOD]: @ref [SOD] "Safety Object Dictionary (SOD)"
