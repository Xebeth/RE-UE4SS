include(FetchContent)

set(FETCHCONTENT_QUIET OFF)

if (UE4SS_zydis_BUILD_SHARED)
    set(ZYDIS_BUILD_SHARED_LIB ON CACHE BOOL "Build zydis as a shared library" FORCE)
    set(ZYCORE_BUILD_SHARED_LIB ON CACHE BOOL "Build zycore as shared libary" FORCE)
else ()
    set(ZYDIS_BUILD_SHARED_LIB OFF CACHE BOOL "Build zydis as a shared library" FORCE)
    set(ZYCORE_BUILD_SHARED_LIB OFF CACHE BOOL "Build zycore as shared libary" FORCE)
endif ()

set(ZYDIS_BUILD_TOOLS OFF CACHE BOOL "")
set(ZYDIS_BUILD_EXAMPLES OFF CACHE BOOL "")

set(ASMTK_NO_CUSTOM_FLAGS ON CACHE BOOL "" FORCE)
set(ASMJIT_NO_CUSTOM_FLAGS ON CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(zydis)

# Apply compiler fixes after target creation
if (${CMAKE_CXX_COMPILER_ID} MATCHES "Clang")
    # Apply fixes to zydis-related targets if they exist
    if(TARGET Zydis)
        target_compile_options(Zydis PRIVATE -w)
    endif()
    if(TARGET Zycore)
        target_compile_options(Zycore PRIVATE -w)
    endif()
endif ()

