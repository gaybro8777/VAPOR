add_executable (raw2wasp raw2wasp.cpp)

target_link_libraries (raw2wasp common wasp)

if(NOT WIN32)
	OpenMPInstall (
		TARGETS raw2wasp
		DESTINATION ${INSTALL_BIN_DIR}
		COMPONENT Utilites
		)
endif()
