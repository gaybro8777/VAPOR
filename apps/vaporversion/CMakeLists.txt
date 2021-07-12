add_executable (vaporversion vaporversion.cpp)

target_link_libraries (vaporversion common)

if (NOT WIN32)
	OpenMPInstall (
		TARGETS vaporversion
		DESTINATION ${INSTALL_BIN_DIR}
		COMPONENT Utilites
		)
endif()
