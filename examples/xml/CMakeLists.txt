#XML file name
SET(XML_FILE_LIST
    com.Hiworld.Gdbus.example.xml
 )

#generated file name
SET(XML_GEN_FILE_LIST
    Example-generated
)

LIST(LENGTH XML_FILE_LIST XML_FILE_LIST_LEN)
LIST(LENGTH XML_GEN_FILE_LIST NAME_LIST_LEN)
MATH(EXPR XML_FILE_LIST_LEN "${XML_FILE_LIST_LEN}-1")
MATH(EXPR NAME_LIST_LEN "${NAME_LIST_LEN}-1")
IF(${XML_FILE_LIST_LEN} EQUAL ${NAME_LIST_LEN})
	#generate files from xml
	FOREACH(INDEX RANGE 0 ${XML_FILE_LIST_LEN})
		LIST(GET XML_FILE_LIST ${INDEX} XML)
		LIST(GET XML_GEN_FILE_LIST ${INDEX} NAME_TMP)
		EXECUTE_PROCESS(COMMAND gdbus-codegen --interface com.Hiworld. --c-namespace CHiworld --generate-c-code ${PROJECT_BINARY_DIR}/generated/${NAME_TMP} ${PROJECT_SOURCE_DIR}/xml/${XML})
	ENDFOREACH(INDEX)
ELSE (${XML_FILE_LIST_LEN} EQUAL ${NAME_LIST_LEN})
    MESSAGE(FATAL_ERROR "The length (${XML_FILE_LIST_LEN}) of xml files list does not equal to the length(${NAME_LIST_LEN}) of the generated files name list")
ENDIF(${XML_FILE_LIST_LEN} EQUAL ${NAME_LIST_LEN})

