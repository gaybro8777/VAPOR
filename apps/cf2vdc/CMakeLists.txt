add_executable (cf2vdc cf2vdc.cpp)

target_link_libraries (cf2vdc common vdc)

if(NOT WIN32)
	OpenMPInstall (
		TARGETS cf2vdc
		DESTINATION ${INSTALL_BIN_DIR}
		COMPONENT Utilites
		)
endif()
