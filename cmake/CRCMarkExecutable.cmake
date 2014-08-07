

MACRO ( CRCMarkExecutable EXE_NAME REFERENCE_PATH )

    GET_TARGET_PROPERTY(exeLocation "${EXE_NAME}" LOCATION)

    IF ( "${exeLocation}" STREQUAL "" )
        MESSAGE ( FATAL_ERROR "The file for the CRC Marker does not exist! ${EXE_NAME}" )
        RETURN()
    ENDIF ( "${exeLocation}" STREQUAL "" )

    IF ( NOT EXISTS ${REFERENCE_PATH} )
        MESSAGE ( FATAL_ERROR "The reference file for the CRC Marker does not exist! ${REFERENCE_PATH}" )
        RETURN()
    ENDIF ( NOT EXISTS ${REFERENCE_PATH} )

    GET_TARGET_PROPERTY(oscrcmarkerLocation oscrcmarker LOCATION)
    GET_TARGET_PROPERTY(oscrccheckLocation oscrccheck LOCATION)

    ADD_CUSTOM_TARGET( "${EXE_NAME}_marker" ALL
        COMMAND cmake -Dcrcchecker=${oscrccheckLocation} -Dcrcmarker=${oscrcmarkerLocation} -DREFERENCE_PATH=${REFERENCE_PATH} -DexeLocation=${exeLocation} -DEXE_NAME=${EXE_NAME} -P ${CMAKE_SOURCE_DIR}/../cmake/CRCMarkExecutableRunner.cmake
        COMMENT "Marking executable ${EXE_NAME}"
        VERBATIM
    )
    ADD_DEPENDENCIES ( "${EXE_NAME}_marker" "${EXE_NAME}" "oscrccheck" "oscrcmarker" )

ENDMACRO ( CRCMarkExecutable EXE_NAME REFERENCE_PATH )
