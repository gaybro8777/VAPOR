add_executable (waspcreate waspcreate.cpp)

target_link_libraries (waspcreate common wasp)

if(NOT WIN32)
	OpenMPInstall (
		TARGETS waspcreate
		DESTINATION ${INSTALL_BIN_DIR}
		COMPONENT Utilites
		)
endif()
