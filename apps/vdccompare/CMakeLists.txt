add_executable (vdccompare vdccompare.cpp)

target_link_libraries (vdccompare common vdc)

if(NOT WIN32)
	OpenMPInstall (
		TARGETS vdccompare
		DESTINATION ${INSTALL_BIN_DIR}
		COMPONENT Utilites
		)
endif()
