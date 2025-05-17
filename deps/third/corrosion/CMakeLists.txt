include(FetchContent)

FetchContent_MakeAvailable(corrosion)

# Clear any generator expressions from output directories before importing
# This ensures Corrosion can properly evaluate the paths
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/Output/bin")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/Output/bin")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/Output/bin")

# Import Rust crate
corrosion_import_crate(MANIFEST_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../../first/patternsleuth_bind/Cargo.toml")

# After importing, set the per-configuration output directories on the target
if(TARGET patternsleuth_bind)
    # Get all build configurations from the parent scope
    if(DEFINED BUILD_CONFIGS)
        foreach(config ${BUILD_CONFIGS})
            string(TOUPPER ${config} config_upper)
            set_target_properties(patternsleuth_bind PROPERTIES
                ARCHIVE_OUTPUT_DIRECTORY_${config_upper} "${CMAKE_BINARY_DIR}/Output/${config}/bin"
                LIBRARY_OUTPUT_DIRECTORY_${config_upper} "${CMAKE_BINARY_DIR}/Output/${config}/bin"
                RUNTIME_OUTPUT_DIRECTORY_${config_upper} "${CMAKE_BINARY_DIR}/Output/${config}/bin"
                PDB_OUTPUT_DIRECTORY_${config_upper} "${CMAKE_BINARY_DIR}/Output/${config}/bin"
            )
        endforeach()
    else()
        # Fallback if BUILD_CONFIGS is not available
        foreach(config_type ${CMAKE_CONFIGURATION_TYPES})
            string(TOUPPER ${config_type} config_upper)
            set_target_properties(patternsleuth_bind PROPERTIES
                ARCHIVE_OUTPUT_DIRECTORY_${config_upper} "${CMAKE_BINARY_DIR}/Output/${config_type}/bin"
                LIBRARY_OUTPUT_DIRECTORY_${config_upper} "${CMAKE_BINARY_DIR}/Output/${config_type}/bin"
                RUNTIME_OUTPUT_DIRECTORY_${config_upper} "${CMAKE_BINARY_DIR}/Output/${config_type}/bin"
                PDB_OUTPUT_DIRECTORY_${config_upper} "${CMAKE_BINARY_DIR}/Output/${config_type}/bin"
            )
        endforeach()
    endif()
endif()
