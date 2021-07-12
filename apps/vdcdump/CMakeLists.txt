add_executable (vdcdump vdcdump.cpp)

target_link_libraries (vdcdump common vdc)

if(NOT WIN32)
	OpenMPInstall (
		TARGETS vdcdump
		DESTINATION ${INSTALL_BIN_DIR}
		COMPONENT Utilites
		)
endif()
