add_executable (wrf2vdc wrf2vdc.cpp)

target_link_libraries (wrf2vdc common vdc)

if(NOT WIN32)
	OpenMPInstall (
		TARGETS wrf2vdc
		DESTINATION ${INSTALL_BIN_DIR}
		COMPONENT Utilites
		)
endif()
