include(FetchContent)

project(ImGui)

set(FETCHCONTENT_QUIET OFF)

FetchContent_Populate(ImGui)
FetchContent_GetProperties(ImGui SOURCE_DIR ImGui_FETCHED_SOURCE)

FetchContent_Populate(ImGuiTextEdit)
FetchContent_GetProperties(ImGuiTextEdit SOURCE_DIR ImGuiTextEdit_FETCHED_SOURCE)

FetchContent_Populate(IconFontCppHeaders)
FetchContent_GetProperties(IconFontCppHeaders SOURCE_DIR IconFontCppHeaders_FETCHED_SOURCE)

set(ImGui_Sources
    "${ImGui_FETCHED_SOURCE}/imgui.cpp"
    "${ImGui_FETCHED_SOURCE}/imgui_draw.cpp"
    "${ImGui_FETCHED_SOURCE}/imgui_tables.cpp"
    "${ImGui_FETCHED_SOURCE}/imgui_widgets.cpp"
    "${ImGui_FETCHED_SOURCE}/imgui_demo.cpp"
    "${ImGui_FETCHED_SOURCE}/misc/cpp/imgui_stdlib.cpp"
    "${ImGui_FETCHED_SOURCE}/backends/imgui_impl_win32.cpp"
    "${ImGui_FETCHED_SOURCE}/backends/imgui_impl_dx11.cpp"
    "${ImGui_FETCHED_SOURCE}/backends/imgui_impl_opengl3.cpp"
    "${ImGui_FETCHED_SOURCE}/backends/imgui_impl_glfw.cpp"
    "${ImGuiTextEdit_FETCHED_SOURCE}/TextEditor.cpp")

add_library(ImGui ${ImGui_Sources})

# Enabling c++23 support
target_compile_features(ImGui PUBLIC cxx_std_23)

target_link_libraries(ImGui PUBLIC glfw)

target_include_directories(ImGui PUBLIC 
    "${ImGui_FETCHED_SOURCE}"
    "${ImGuiTextEdit_FETCHED_SOURCE}"
    "${IconFontCppHeaders_FETCHED_SOURCE}")
    
if (${CMAKE_CXX_COMPILER_ID} STREQUAL MSVC)
    target_compile_options(ImGui PRIVATE /W0)
elseif (${CMAKE_CXX_COMPILER_ID} MATCHES "Clang")
    target_compile_options(ImGui PRIVATE 
        -xc++
        -Wno-deprecated-enum-enum-conversion
        -Wno-enum-enum-conversion
        -Wno-enum-float-conversion
        -Wno-deprecated-enum-float-conversion
        -Wno-deprecated-anon-enum-enum-conversion
        -Wno-error=enum-enum-conversion
        -Wno-error=enum-float-conversion
        -Wno-error=deprecated-enum-enum-conversion
        -Wno-error=deprecated-enum-float-conversion
        -Wno-error=deprecated-anon-enum-enum-conversion
        -w  # Suppress all warnings for this library
    )
endif ()