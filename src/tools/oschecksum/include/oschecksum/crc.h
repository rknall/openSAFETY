/**
 * \file crc.h
 * \addtogroup Tools
 * \{
 * \addtogroup oschecksum
 * \{
 * \details
 * \{
 * Copyright (c) 2011,  Bernecker + Rainer Industrie-Elektronik Ges.m.b.H
 * All rights reserved, Bernecker + Rainer Industrie-Elektronik Ges.m.b.H
 *
 * This source code is free software; you can redistribute it and/or modify it
 * under the terms of the BSD license (according to License.txt).
 * \}
 * \author Roland Knall, Bernecker + Rainer Industrie-Elektronik Ges.m.b.H
 * \version 1.0
 */

#ifndef _LIBOSCHECKSUM_CRC_H_
#define _LIBOSCHECKSUM_CRC_H_

/*polynomial defines*/
/** \brief CRC-8 Generator polynomial */
#define CRC8_POLY 0x2F
/** \brief CRC-16 Generator polynomial, in use since V1.5 */
#define CRC16_POLY 0xBAAD
/** \brief Old CRC-16 generator polynomial */
#define CRC16_POLY_AC9A 0xAC9A
/** \brief CRC-32 Generator polynomial */
#define CRC32_POLY 0x1EDC6F41
/** \brief CRC32 PNG generator polynomial, see links for information */
/*http://www.dattalo.com/technical/software/pic/crc_32.asm*/
/*http://wiki.osdev.org/CRC32*/
/*algorithms: http://people.sc.fsu.edu/~jburkardt/cpp_src/crc/crc.cpp*/
/*the zlib library implements the crc_png algorithm as well*/
#define CRC32PNG_POLY 0xEDB88320


#include "crc8.h"
#include "crc16.h"
#include "crc16_AC9A.h"
#include "crc32.h"

#if defined (_WIN32)
  #if defined(oschecksum_EXPORTS)
/** \brief Defines the export definition for the shared library */
    #define  OSCHECKSUM_EXPORT extern __declspec(dllexport)
  #else
/** \brief Defines the export definition for the shared library */
    #define  OSCHECKSUM_EXPORT extern __declspec(dllimport)
  #endif /* oschecksum_EXPORTS */
#else /* defined (_WIN32) */
/** \brief Defines the export definition for the shared library */
 #define OSCHECKSUM_EXPORT extern
#endif

#pragma pack(push, 4)

#ifdef __cplusplus
extern "C" {
#endif


#include "crcint.h"

/**
 * \brief Calculates a CRC32 checksum for the given buffer using the precompiled table
 *
 * \param len the length of the given buffer
 * \param pBuffer a pointer to a buffer of the given length
 * \param initCRC Initial value for the CRC calculation. In case of openSAFETY, this is always 0
 *
 * \return the CRC32 checksum for the buffer
 */
OSCHECKSUM_EXPORT uint32_t crc32Checksum(uint32_t len, const uint8_t * pBuffer, uint32_t initCRC);

/**
 * \brief Calculates a CRC32 checksum with the PNG crc checksum for the given buffer using the precompiled table
 *
 * \param len the length of the given buffer
 * \param pBuffer a pointer to a buffer of the given length
 * \param initCRC Initial value for the CRC calculation. In case of openSAFETY, this is always 0
 *
 * \return the CRC32 checksum for the buffer
 */
OSCHECKSUM_EXPORT uint32_t crc32ChecksumPNG(uint32_t len, const uint8_t * pBuffer, uint32_t initCRC);

/**
 * \brief Calculates a CRC32 checksum with the PNG crc checksum for the given buffer using the precompiled table
 *
 * \param len the length of the given buffer
 * \param pBuffer a pointer to a buffer of the given length
 * \param initCRC Initial value for the CRC calculation. In case of openSAFETY, this is always 0
 *
 * \return the CRC32 checksum for the buffer
 */
OSCHECKSUM_EXPORT uint32_t crc32ChecksumPNG_Calculator(uint32_t len, const uint8_t * pBuffer, uint32_t initCRC);

/**
 * \brief Calculates a CRC32 checksum for the given buffer
 *
 * \param len the length of the given buffer
 * \param pBuffer a pointer to a buffer of the given length
 * \param initCRC Initial value for the CRC calculation. In case of openSAFETY, this is always 0
 *
 * \return the CRC32 checksum for the buffer
 */
OSCHECKSUM_EXPORT uint32_t crc32ChecksumCalculator(uint32_t len, const uint8_t * pBuffer, uint32_t initCRC);

/**
 * \brief Calculates a CRC16 checksum for the given buffer using the precompiled table
 *
 * \param len the length of the given buffer
 * \param pBuffer a pointer to a buffer of the given length
 * \param initCRC Initial value for the CRC calculation. In case of openSAFETY, this is always 0
 *
 * \return the CRC16 checksum for the buffer
 */
OSCHECKSUM_EXPORT uint16_t crc16Checksum(uint32_t len, const uint8_t * pBuffer, uint16_t initCRC);

/**
 * \brief Calculates a CRC16 checksum for the given buffer using the precompiled table
 *
 * \param len the length of the given buffer
 * \param pBuffer a pointer to a buffer of the given length
 * \param initCRC Initial value for the CRC calculation. In case of openSAFETY, this is always 0
 *
 * \return the CRC16 checksum for the buffer
 */
OSCHECKSUM_EXPORT uint16_t crc16Checksum_AC9A(uint32_t len, const uint8_t * pBuffer, uint16_t initCRC);

/**
 * \brief Calculates a CRC16 checksum for the given buffer
 *
 * \param len the length of the given buffer
 * \param pBuffer a pointer to a buffer of the given length
 * \param initCRC Initial value for the CRC calculation. In case of openSAFETY, this is always 0
 *
 * \return the CRC16 checksum for the buffer
 */
OSCHECKSUM_EXPORT uint16_t crc16ChecksumCalculator(uint32_t len, const uint8_t * pBuffer, uint16_t initCRC);

/**
 * \brief Calculates a CRC16 checksum for the given buffer
 *
 * \param len the length of the given buffer
 * \param pBuffer a pointer to a buffer of the given length
 * \param initCRC Initial value for the CRC calculation. In case of openSAFETY, this is always 0
 *
 * \return the CRC16 checksum for the buffer
 */
OSCHECKSUM_EXPORT uint16_t crc16ChecksumCalculator_AC9A(uint32_t len, const uint8_t * pBuffer, uint16_t initCRC);

/**
 * \brief CRC32 Algorithm sanity check
 *
 * \return although being an uint8, the return value is 0 for incorrect functionality, 1 for correct one
 */
OSCHECKSUM_EXPORT uint16_t crc16SanityCheck(void);

/**
 * \brief Calculates a CRC8 checksum for the given buffer using the precompiled table
 *
 * \param len the length of the given buffer
 * \param pBuffer a pointer to a buffer of the given length
 * \param initCRC Initial value for the CRC calculation. In case of openSAFETY, this is always 0
 *
 * \return the CRC8 checksum for the buffer
 */
OSCHECKSUM_EXPORT uint8_t crc8Checksum(uint32_t len, const uint8_t * pBuffer, uint8_t initCRC);

/**
 * \brief Calculates a CRC8 checksum for the given buffer
 *
 * \param len the length of the given buffer
 * \param pBuffer a pointer to a buffer of the given length
 * \param initCRC Initial value for the CRC calculation. In case of openSAFETY, this is always 0
 *
 * \return the CRC8 checksum for the buffer
 */
OSCHECKSUM_EXPORT uint8_t crc8ChecksumCalculator(uint32_t len, const uint8_t * pBuffer, uint8_t initCRC);

/**
 * \brief CRC8 Algorithm sanity check
 *
 * \return although being an uint8, the return value is 0 for incorrect functionality, 1 for correct one
 */
OSCHECKSUM_EXPORT uint8_t crc8SanityCheck(void);
#ifdef __cplusplus
}
#endif

#pragma pack(pop)

#endif /* _LIBOSCHECKSUM_CRC_H_ */

/**
 * \}
 * \}
 */
