Safety Control Flow Monitoring (SCFM) {#sw_scfm}
-------------------------------------

The [SCFM] module offers functionality for monitoring correct program flow.

To monitor correct program flow, the user must read and compare the number of function calls on both safety controllers (see figure [openSAFETY Software Structure](@ref swstructure) ) using the function SCFM_GetResetPath(). If the values are different, then the application must react accordingly to this error.

To monitor correct program flow, the openSAFETY software calls the macro SCFM_TACK_PATH(). This can also be used by the application.

[SCFM]: @ref SCFM "Safety Control Flow Monitoring (SCFM)"

