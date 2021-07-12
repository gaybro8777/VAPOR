add_executable (raw2vdc raw2vdc.cpp)

target_link_libraries (raw2vdc common vdc)

if(NOT WIN32)
	OpenMPInstall (
		TARGETS raw2vdc
		DESTINATION ${INSTALL_BIN_DIR}
		COMPONENT Utilites
		)
endif()
