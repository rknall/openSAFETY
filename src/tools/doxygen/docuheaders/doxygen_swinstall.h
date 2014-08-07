/**
 * @file doxygen_swinstall.h
* @page safety_doc_safety_swinstall Installation of the openSAFETY-SW
*
* To install the openSAFETY-SW, all directories and files must be copied from the delivered ZIP file into a target
* directory of your choice on your PC.
*
* The structure of the openSAFETY-SW is defined as follows:
* <ul>
*     <li>@b build - This directory will contain the compiled code</li>
*     <li>@b cmake - Various helper scripts for the cmake meta compiler</li>
*     <li>@b doc - Containts this documentation in printed form</li>
*     <li>@b src
*         <ul>
*             <li>@b contrib - Routines needed for compiling the test applications</li>
*             <li>@b eplssrc - The main openSAFETY-SW source</li>
*             <li>@b targets - This directory will contain the sources for your target</li>
*             <li>@b tools
*                 <ul>
*                     <li>@b doxygenfilter - Helper program for the documentation generator</li>
*                     <li>@b liboschecksum - Library which provides methods for CRC checksum</li>
*                     <li>@b oscrccheck - CRC checksum application</li>
*                     <li>@b pclint - Various config files for the PC Lint static code checker</li>
*                 </ul>
*             </li>
*             <li>@b unittest - Unittests for the openSAFETY-SW source code</li>
*         </ul>
*     </li>
* </ul>
*
* The openSAFETY-SW source code uses a meta compiler, to enable easy deployment on various build targets. The meta-compiler
* in this case is called <a href="http://www.cmake.org">@b CMAKE </a>, and needs to be installed prior to development.
*
* For the following steps it is assumed, that the sourcecode package has been downloaded and extracted to the following path.
*
* <b>For Windows</b>:
* @verbatim
   c:\src\openSAFETY
@endverbatim
*
* <b>For Linux</b>:
* @verbatim
   /home/<user>/src/openSAFETY
@endverbatim
*
* If a different directory has been used, the following steps should be adapted accordingly.
*
* @section safety_doc_safety_swinstall_prep Preparing the build environment
*
* If not done already, the meta-compiler @b CMAKE should be downloaded and installed on the system. For Windows, an
* installation is provided on the website.
* @par Please Note
* During the installation of the Windows setup program for @b CMAKE the installation dialog asks, if the program path
* should be made known system-wide. It is recommended, that this option is selected.
*
* For a Linux based development, most major distributions provide appropriate packages for @b CMAKE using their
* distribution repositories. It is recommended, that this version is being used.
*
* The openSAFETY-SW distribution can also create a HTML based source-code documentation. To be able to generate this
* on the development system, two program have to be installed.
*
* @arg @b DoxyGen - This is the program used for generating the documentation, and it can be downloaded
* from <a href="http://www.doxygen.org">www.doxygen.org</a> free of charge. Again most Linux distributions provide
* packages in their repositories.
* @arg @b GraphViz - To generate dependancy graphics, doxygen uses GraphViz, which can be downloaded from
* <a href="http://www.graphviz.org/">www.graphviz.org</a> also free of charge.Again most Linux distributions provide
* packages in their repositories
*
* @subsection safety_doc_safety_swinstall_prepcmake Preparing the meta-compiler
*
* After all necessary programs have been installed, the software stack may be compiled. @b CMAKE allows for various
* targets to be compiled. To view the list of possible targets on the system, the following steps should be executed
*
* <b>For Windows</b>:
* @arg Press @b CTRL+R
* @arg Enter @c cmd and hit @b Enter
* @verbatim
   cd c:\src\openSAFETY
   cd build
   cmake --help
@endverbatim
*
* <b>For Linux</b>:
* @arg Open a terminal
* @verbatim
   cd /home/<user>/src/openSAFETY
   cd build
   cmake --help
@endverbatim
*
* On a Linux based development system the list may contain the following entries:
* @verbatim
  Unix Makefiles                 = Generates standard UNIX makefiles.
  CodeBlocks - Unix Makefiles    = Generates CodeBlocks project files.
  Eclipse CDT4 - Unix Makefiles  = Generates Eclipse CDT 4.0 project files.
  KDevelop3                      = Generates KDevelop 3 project files.
  KDevelop3 - Unix Makefiles     = Generates KDevelop 3 project files.
@endverbatim
*
* On a Windows based targets additional generators will show up, regarding the Visual Studio IDE in it's various versions.
*
* If @b CMAKE is called without a specific generator, it will allways use the first generator from this list.
*
* To generate a build environment for an Eclipse based target (e.g. <i>Code Warrior</i> from <i>Texas Instruments</i>
* as well as <i>Code Composer Studio</i> from <i>Freescale</i> are Eclipse based IDE), the following command will be
* used:
* @verbatim
    cmake -G 'Eclipse CDT4 - Unix Makefiles' ../src
@endverbatim
*
* If different compilers as the one listed in this command need to be used, the compilers can be provided on the
* command line:
* @verbatim
    CC=<path_to_your_c_compiler> CPP=<path_to_your_cpp_compiler> cmake -G 'Eclipse CDT4 - Unix Makefiles' ../src
@endverbatim
*
* This will create an Eclipse project file in the same directory (@b build ) which can be imported in the Eclipse
* development environment.
*
* Now the build environment is prepared, and ready for the next step: @subpage safety_doc_safety_swinstall_target
*
*/
