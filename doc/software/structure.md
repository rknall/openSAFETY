openSAFETY Software Structure {#swstructure}
=============================

[TOC]

Introduction
------------

The openSAFETY software stack is split up in separate modules. The interface to the fieldbus network is called [SHNF] and is not part of the stack software. This allows for an easy adjustment of the openSAFETY stack to different target systems. The following picture shows the openSAFETY software stack structure.

![sw_struct]

Relationships between individual modules are indicated by arrow symbols. The modules [SERR], [SCFM] and [EPLS] are auxiliary and may be used by any other module. Modules which have an indication in the form of a black marker, provide a function interface for the target application. This interface is exported via a header file that is named after the module in question followed by api.h. For instance, the module [SSC] provides functions that are defined in the header file SSCapi.h. These functions are described in greater detail in their respective module section and in the [integration_guide].

[sw_struct]: ./software_structure.png "openSAFETY software structuree"
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
