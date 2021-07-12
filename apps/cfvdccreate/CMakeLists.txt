add_executable (cfvdccreate cfvdccreate.cpp)

target_link_libraries (cfvdccreate common vdc)

if(NOT WIN32)
	OpenMPInstall (
		TARGETS cfvdccreate
		DESTINATION ${INSTALL_BIN_DIR}
		COMPONENT Utilites
		)
endif()
