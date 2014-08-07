/**
 * \file tools.h
 * \addtogroup Tools Tools & Libraries
 * \{
 * The openSAFETY stack distribution provides various tools and libraries, which can be used to implement new openSAFETY targets.
 *
 * The following tools are provided:
 *
 * - \ref oschecksum Library for calculcating CRC8, CRC16 and CRC32 cecksums
 * - \ref oscrccheck Build process helper tool for ensuring the CRC checksum on each stack file
 *
 * \ref CROSSWIN32
 *
 * \}
 *
 * \defgroup datalogger DataLogger library
 * \ingroup Tools
 * \{
 * A small library, which provides functionality to write debug logging information to files and stdout
 * \}
 *
 * \defgroup oschecksum openSAFETY CRC Checksum library
 * \ingroup Tools
 * \{
 * This library defines the CRC tables used by the openSAFETY stack, as well as methods for calculating CRC checksums
 * \}
 *
 * \defgroup oscrccheck openSAFETY CRC check utility
 * \ingroup Tools
 * \{
 * Utility which is being used by the build process, to check for predefined CRC values for all stack files
 * \}
 *
 */
