include(FetchContent)

set(FETCHCONTENT_QUIET OFF)

# ------------------------------------------------------------------------------
# Third-Party Dependencies
# ------------------------------------------------------------------------------

# glaze JSON library
FetchContent_Declare(
    glaze
    GIT_REPOSITORY https://github.com/stephenberry/glaze.git
    GIT_TAG v2.9.5
    GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(glaze)

# GLFW
add_subdirectory("GLFW")

# ImGui and related libraries
FetchContent_Declare(
    ImGui
    GIT_REPOSITORY https://github.com/ocornut/imgui.git
    GIT_TAG v1.91.9b
    GIT_SHALLOW TRUE
    GIT_PROGRESS ON
    CONFIGURE_COMMAND ""
)
    
FetchContent_Declare(
    ImGuiTextEdit
    GIT_REPOSITORY https://github.com/UE4SS-RE/ImGuiColorTextEdit.git
    GIT_TAG master
    GIT_SHALLOW TRUE
    GIT_PROGRESS ON
    CONFIGURE_COMMAND ""
)
    
FetchContent_Declare(
    IconFontCppHeaders
    GIT_REPOSITORY https://github.com/juliettef/IconFontCppHeaders.git
    GIT_TAG main
    GIT_SHALLOW TRUE
    GIT_PROGRESS ON
    CONFIGURE_COMMAND ""
)

add_subdirectory("imgui")

# Zydis
FetchContent_Declare(
    zydis
    GIT_REPOSITORY https://github.com/zyantific/zydis.git
    GIT_TAG v4.1.1
    GIT_SHALLOW TRUE
    GIT_PROGRESS ON
)
add_subdirectory("zydis")

# PolyHook
FetchContent_Declare(
    PolyHook2
    GIT_REPOSITORY https://github.com/stevemk14ebr/PolyHook_2_0.git
    GIT_TAG 4c8872e207e76ea43fd23f802e3bf5acb43fee8a
    GIT_PROGRESS ON
)
add_subdirectory("PolyHook_2_0")

# raw_pdb
FetchContent_Declare(
    raw_pdb
    GIT_REPOSITORY https://github.com/MolecularMatters/raw_pdb.git
    GIT_TAG 8c6a7146393c83d27fa101e8bc8017f2a7f151df
    GIT_PROGRESS ON
)
add_subdirectory("raw_pdb")

# Corrosion (Rust integration)
FetchContent_Declare(
    Corrosion
    GIT_REPOSITORY https://github.com/corrosion-rs/corrosion.git
    GIT_TAG v0.5.2
)
add_subdirectory("corrosion")

# fmt
FetchContent_Declare(
    fmt
    GIT_REPOSITORY https://github.com/fmtlib/fmt.git
    GIT_TAG 10.2.1
    GIT_SHALLOW TRUE
    GIT_PROGRESS ON
)
set(FMT_HEADER_ONLY OFF CACHE BOOL "Build fmt as a compiled library, not header-only")
FetchContent_MakeAvailable(fmt)

# Tracy profiler
FetchContent_Declare(
    tracy
    GIT_REPOSITORY https://github.com/wolfpld/tracy.git
    GIT_TAG 37aff70dfa50cf6307b3fee6074d627dc2929143 # v0.10
    GIT_SHALLOW TRUE
    GIT_PROGRESS ON
)
set(TRACY_ENABLE ON CACHE BOOL "Enable Tracy profiler")
set(TRACY_ON_DEMAND OFF CACHE BOOL "On-demand profiling")
set(BUILD_SHARED_LIBS OFF CACHE BOOL "Build shared library")
FetchContent_MakeAvailable(tracy)
