openSAFETY Software Concept {#swconcept}
===========================

Introduction
------------

The main task of openSAFETY on an [SN] is to create and process openSAFETY Frames. openSAFETY Frames can be [SSDO], [SPDO] or [SNMT] commands. 

Furthermore, the stack also provides a [SCM] for configuration purposes and a [SOD], which contains all Safety Objects (SO) and is an important interface to the application. Finally the openSAFETY software stack also provides the functionality needed to implement a Safety Domain Gateway (SDG). 

In principle, the stack can also be used on SIL1 and SIL2 systems, however the certification authority must be consulted first and the program flow monitor might have to be adjusted because the openSAFETY software stack contains a program flow monitor (see [SCFM]) that requires a homogeneous redundant structure (see picture).

The stack has a pure function interface (without internal scheduler) and can be used with or without an operating system because the stack is independent of any one operating system. Reentrant functions are marked explicitly with and errors detected by the stack are reported to the application (see section [SERR]).

[SCM]: @ref SNMT "Safety Configuration Manager (SCM)"
[SN]: @ref SN "Safety Node (SN)"

[SNMT]: @ref SNMT "Safety Network Management (SNMT)"
[SSDO]: @ref SSDO "Safety Service Data Object (SSDO)"
[SPDO]: @ref SPDO "Safety Process Data Object (SPDO)"
[SERR]: @ref SERR "Safety Error Reporting (SERR)"

[SOD]: @ref SOD "Safety Object Dictionary (SOD)"

[SCFM]: @ref SCFM "Safety Control Flow Monitoring (SCFM)"
[SHNF]: @ref SHNF "Safety Hardware Near Firmware (SHNF)"

[nw_struct]: ./node_comm_structure.png "System structure openSAFETY node"

[INTEGRATION_GUIDE]: @ref integration_guide "Integration Guide"
