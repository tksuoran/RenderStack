#
# export file: copy it to the build tree on every build invocation and add rule for installation
#
function (cm_export_file FILE DEST TARGETNAME)
    if (NOT TARGET export-files-${TARGETNAME})
        add_custom_target(export-files-${TARGETNAME} ALL COMMENT "Exporting ${TARGETNAME} files into build tree")
    endif (NOT TARGET export-files-${TARGETNAME})

    get_filename_component (FILENAME "${FILE}" NAME)
    add_custom_command (TARGET export-files-${TARGETNAME} COMMAND ${CMAKE_COMMAND} -E copy_if_different "${FILE}" "${DEST}")
    install (FILES "${FILE}" DESTINATION "${DEST}")
endfunction (cm_export_file)
