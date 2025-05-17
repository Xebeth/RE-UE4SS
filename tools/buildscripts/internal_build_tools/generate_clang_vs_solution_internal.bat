set BuildMode=%1

IF NOT DEFINED BuildMode (
    set BuildMode=Game__Debug__Win64
)

REM Navigate to the VS_Solution_Clang directory (from internal_build_tools)
cd ../../../VS_Solution_Clang

REM Generate Visual Studio solution with Clang toolset
cmake -DCMAKE_BUILD_TYPE=Game__Debug__Win64 ^
      -G "Visual Studio 17 2022" -T ClangCL ^
      -DRC_FORCE_ALL_STATIC_LIBS= ^
      -DCMAKE_C_COMPILER="C:\Program Files\LLVM\bin\clang-cl.exe" ^
      -DCMAKE_CXX_COMPILER="C:\Program Files\LLVM\bin\clang-cl.exe" ^
      ..


pause