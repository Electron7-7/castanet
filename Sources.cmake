if(NOT SRC)
	set(SRC ${CMAKE_CURRENT_SOURCE_DIR}/src)
endif()

set(CASTANET_SOURCES
	${SRC}/main/dedicated_main.cpp
	${SRC}/main/output_file.cpp
	${SRC}/main/regex_operations.cpp
)

set(GETARGS_SOURCES
	${SRC}/thirdparty/getargs/argument.cpp
	${SRC}/thirdparty/getargs/argument_parser.cpp
)
