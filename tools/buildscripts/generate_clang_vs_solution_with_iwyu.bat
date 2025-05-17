@echo off
REM Generate Clang Visual Studio Solution with IWYU support

set BuildMode=%1

IF NOT DEFINED BuildMode (
    set BuildMode=Release
)

echo Generating Clang Visual Studio Solution with IWYU for %BuildMode% mode...
echo.

REM Check if IWYU is available
where include-what-you-use >nul 2>&1
if %ERRORLEVEL% neq 0 (
    echo Warning: include-what-you-use not found in PATH
    echo IWYU analysis will be disabled
    set IWYU_ENABLED=OFF
) else (
    echo Found include-what-you-use, enabling IWYU analysis
    set IWYU_ENABLED=ON
)

REM Create VS_Solution_Clang directory if it doesn't exist
if not exist "../../VS_Solution_Clang" (
    mkdir "../../VS_Solution_Clang"
)

cd ../../VS_Solution_Clang

REM Generate Visual Studio solution with Clang toolset and optional IWYU
cmake -DCMAKE_BUILD_TYPE=%BuildMode% ^
      -DRC_FORCE_ALL_STATIC_LIBS= ^
      -DCMAKE_C_COMPILER=clang ^
      -DCMAKE_CXX_COMPILER=clang++ ^
      -DUE4SS_USE_IWYU=%IWYU_ENABLED% ^
      -T"ClangCL" ^
      -G"Visual Studio 17 2022" ^
      ..

cd tools\buildscripts

echo.
echo Clang Visual Studio solution generated in VS_Solution_Clang directory
echo IWYU enabled: %IWYU_ENABLED%
echo Open VS_Solution_Clang\UE4SSMonorepo.sln in Visual Studio
echo.
pause
