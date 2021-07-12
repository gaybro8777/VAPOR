add_executable (vapor_check_udunits vapor_check_udunits.cpp)

target_link_libraries (vapor_check_udunits common vdc)

if(NOT WIN32)
	OpenMPInstall (
		TARGETS vapor_check_udunits
		DESTINATION ${INSTALL_BIN_DIR}
		COMPONENT Utilites
		)
endif()
