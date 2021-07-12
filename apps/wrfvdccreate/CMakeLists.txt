add_executable (wrfvdccreate wrfvdccreate.cpp)

target_link_libraries (wrfvdccreate common vdc)

if(NOT WIN32)
	OpenMPInstall (
		TARGETS wrfvdccreate
		DESTINATION ${INSTALL_BIN_DIR}
		COMPONENT Utilites
		)
endif()
