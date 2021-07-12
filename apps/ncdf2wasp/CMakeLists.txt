add_executable (ncdf2wasp ncdf2wasp.cpp)

target_link_libraries (ncdf2wasp common wasp)

if(NOT WIN32)
	OpenMPInstall (
		TARGETS ncdf2wasp
		DESTINATION ${INSTALL_BIN_DIR}
		COMPONENT Utilites
		)
endif()
