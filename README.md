openSAFETY Distribution Version 1.4 
===================================

[TOC]

General Information
--------------------

openSAFETY is an IEC 61508:2010 compliant safety transport protocol suitable for implementing safety-oriented process data transport between individual stations in a safety network. openSAFETY also controls the safety-oriented parameters and configuration of the safety network stations. 

The openSAFETY protocol stack was developed by B&R as a reference implementation, in accordance with IEC 61508:2010 of the openSAFETY protocol specification from the EPSG (http://www.ethernet-powerlink.org). Provided as an openSAFETY distribution within the framework of the BSD license together with the documentation for the source code and a CRC calculation tool, it can be downloaded from SourceForge free of charge.

Included as part of the BSD license, an example implementation for openSAFETY 1.4.0 of the safety transport protocol has been made available for demonstration purposes. Without further measures and documentation, this implementation is not suitable for developing products in accordance with IEC 61508:2010. The concrete measures and required documents can be obtained from openSAFETY conformity centers and service providers. A list of certified service providers and test centers for openSAFETY is available from the EPSG.


Rights of use 
-------------

    It is hereby explicitly stated that the licensee must ensure and provide    proof of error-free operation of his product using suitable test methods when using the openSAFETY protocol stack in safety-related products in accordance with IEC 61508:2010.
<!-- -->
    
    Any errors found in the openSAFETY distribution by the licensee are to be published on SourceForge in the bug tracker for the openSAFETY project. The licensee is also obligated to continually check the error list without being explicitly prompted to do so and to evaluate the errors in the list in relation to the usage of the openSAFETY stack in his product.
<!-- -->
    
     When downloading the openSAFETY distribution, the user agrees to the rights of use and conditions of the BSD license as specified.


Installation
---------------------

This is a CMake based distribution, whcih allows for an integration of  the CRC check as well as other checking steps within the build process.

The following utilites are necessary to perform the checks for this distribution, and must be installed in an accessible system path in order to correctly perform all necessary checks and steps:

* CMake 2.8.12 or higher - http://www.cmake.org
* Python 3.4.1 or higher - http://www.python.org

To generate the api documentation, please install doxygen:

* Doxygen 1.8.7 or higher - http://www.doxygen.org



Supported compiler version: 

* All GCC-based version starting with 4.1.0 
* Cygwin and MingwSys ports of GCC

**To Note:** The MSVC compiler is not able to generate unittests correctly and therefore is not supported with this distribution

Please note *License.txt* for a copy of the BSD license applying for this product
Please note *ChangeLog.txt* for changes regarding this version
