/**
 * @file doxygen_safetyintro.h
* @page safety_doc_safety_intro openSAFETY Introduction
*
* POWERLINK Safety uses a variety of safety mechanisms. In addition to saving data content via CRC Codes, timing of
* the data that is being transferred is also monitored independent of the actual (non-secure) transport protocol that
* is being used. This allows POWERLINK Safety to provide secure transfer of data across non-secure networks.
*
* Despite having the name Ethernet POWERLINK Safety, this protocol can also be used with transfer protocols other
* than Ethernet POWERLINK. Different transfer protocols and media such as CAN can also be used with POWERLINK Safety.
* POWERLINK Safety only uses the non-secure transfer layer for exchang-ing the secured POWERLINK Safety Frames, whose
* contents are not interpreted by the transfer layer. Of course, POWERLINK is the preferred transfer layer because of
* the close relationship between the communication mechanisms in POWERLINK and POWERLINK Safety, which enable an ideal
* level of performance.
*
* A POWERLINK Safety network can consist of up to 1,023 POWERLINK Safety domains, whereby each domain can contain up
* to 1,023 devices. The POWER-LINK Safety domain can be spread across several, even inhomogenous net-works. Special
* POWERLINK Safety Domain Gateways enable communication between POWERLINK Safety domains.
*
* POWERLINK Safety uses a variety of safety mechanisms. In addition to saving data content via CRC Codes, timing of
* the data that is being transferred is also monitored independent of the actual (non-secure) transport protocol that
* is being used. This allows POWERLINK Safety to provide secure transfer of data across non-secure networks.
*
* Despite having the name Ethernet POWERLINK Safety, this protocol can also be used with transfer protocols other
* than Ethernet POWERLINK. Different transfer protocols and media such as CAN can also be used with POWERLINK Safety.
* POWERLINK Safety only uses the non-secure transfer layer for exchang-ing the secured POWERLINK Safety Frames,
* whose contents are not interpreted by the transfer layer. Of course, POWERLINK is the preferred transfer layer
* because of the close relationship between the communication mechanisms in POWERLINK and POWERLINK Safety, which
* enable an ideal level of performance.
*
* A POWERLINK Safety network can consist of up to 1,023 POWERLINK Safety domains, whereby each domain can contain up
* to 1,023 devices. The POWER-LINK Safety domain can be spread across several, even inhomogenous net-works. Special
* POWERLINK Safety Domain Gateways enable communication between POWERLINK Safety domains.
*
* \image html safety_domains.png
*
* POWERLINK Safety uses a variety of safety mechanisms. In addition to saving data content via CRC Codes, timing of
* the data that is being transferred is also monitored independent of the actual (non-secure) transport protocol
* that is being used. This allows POWERLINK Safety to provide secure transfer of data across non-secure networks.
*
* Despite having the name Ethernet POWERLINK Safety, this protocol can also be used with transfer protocols other
* than Ethernet POWERLINK. Different transfer protocols and media such as CAN can also be used with POWERLINK Safety.
* POWERLINK Safety only uses the non-secure transfer layer for exchang-ing the secured POWERLINK Safety Frames, whose
* contents are not interpreted by the transfer layer. Of course, POWERLINK is the preferred transfer layer because
* of the close relationship between the communication mechanisms in POWERLINK and POWERLINK Safety, which enable an
* ideal level of performance.
*
* A POWERLINK Safety network can consist of up to 1,023 POWERLINK Safety domains, whereby each domain can contain
* up to 1,023 devices. The POWER-LINK Safety domain can be spread across several, even inhomogenous net-works.
* Special POWERLINK Safety Domain Gateways enable communication between POWERLINK Safety domains.
*/