add_subdirectory(Utils)
add_subdirectory(Packets)
add_subdirectory(Communication)

get_all_targets(TARGETS ./)

set_target_properties( ${TARGETS}
    PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/lib/cpp"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/lib/cpp"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/lib/cpp"
)