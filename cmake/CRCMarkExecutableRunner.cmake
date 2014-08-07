
IF ( NOT EXISTS ${crcchecker} )
    MESSAGE ( FATAL_ERROR "CRCMarkExecutable:: The CRC utility has not been provided" )
    RETURN ()
ENDIF ( NOT EXISTS ${crcchecker} )

IF ( NOT EXISTS ${crcmarker} )
    MESSAGE ( FATAL_ERROR "CRCMarkExecutable:: The CRC marker has not been provided" )
    RETURN ()
ENDIF ( NOT EXISTS ${crcmarker} )

IF ( NOT EXISTS ${REFERENCE_PATH} )
    MESSAGE ( FATAL_ERROR "CRCMarkExecutable:: The CRC reference file(s) could not be found on the given location: ${REFERENCE_PATH}" )
    RETURN ()
ENDIF ( NOT EXISTS ${REFERENCE_PATH} )

IF ( NOT EXISTS ${exeLocation} )
    MESSAGE ( FATAL_ERROR "CRCMarkExecutable:: The executable could not be found on the given location: ${exeLocation}" )
    RETURN ()
ENDIF ( NOT EXISTS ${exeLocation} )

IF ( "${EXE_NAME}" STREQUAL "" )
    MESSAGE ( FATAL_ERROR "CRCMarkExecutable:: No name for the target has been given" )
    RETURN ()
ENDIF ( "${EXE_NAME}" STREQUAL "" )

EXECUTE_PROCESS (
    COMMAND ${crcchecker} -C -f ${REFERENCE_PATH}
    COMMAND ${crcmarker} ${exeLocation}
    RESULT_VARIABLE PROCESS_RESULT
    ERROR_VARIABLE  PROCESS_ERROR
    OUTPUT_VARIABLE PROCESS_OUTPUT
)

IF ( NOT ${PROCESS_RESULT} EQUAL 0 )
    MESSAGE ( STATUS "WARNING: CRCMarkExecutable (${PROCESS_RESULT}) failed for file ${exeLocation}" )
ENDIF ( NOT ${PROCESS_RESULT} EQUAL 0 )
