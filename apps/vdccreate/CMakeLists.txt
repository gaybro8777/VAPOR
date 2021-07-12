add_executable (vdccreate vdccreate.cpp)

target_link_libraries (vdccreate common vdc)

if(NOT WIN32)
	OpenMPInstall (
		TARGETS vdccreate
		DESTINATION ${INSTALL_BIN_DIR}
		COMPONENT Utilites
		)
endif()
