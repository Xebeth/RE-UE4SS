@echo off
echo Generating Ninja build with system clang...
echo.

REM Check if clang is in PATH
where clang >nul 2>&1
if %ERRORLEVEL% neq 0 (
    echo Error: clang not found in PATH
    echo Please install clang and add it to your PATH
    pause
    exit /b 1
)

REM Check if ninja is in PATH  
where ninja >nul 2>&1
if %ERRORLEVEL% neq 0 (
    echo Error: ninja not found in PATH
    echo Please install ninja and add it to your PATH
    pause
    exit /b 1
)

REM Get clang paths
for /f "tokens=*" %%i in ('where clang') do set CLANG_PATH=%%i
for /f "tokens=*" %%i in ('where clang++') do set CLANGXX_PATH=%%i

echo Found clang at: %CLANG_PATH%
echo Found clang++ at: %CLANGXX_PATH%
echo.

REM Create SystemClangNinja directory
if not exist "../../SystemClangNinja" (
    mkdir "../../SystemClangNinja"
)

cd ../../SystemClangNinja

REM Clean previous build
del CMakeCache.txt 2>nul
rmdir /s /q CMakeFiles 2>nul

REM Generate with system clang and Ninja
cmake -DCMAKE_BUILD_TYPE=Debug ^
      -DRC_FORCE_ALL_STATIC_LIBS= ^
      -DCMAKE_C_COMPILER="%CLANG_PATH%" ^
      -DCMAKE_CXX_COMPILER="%CLANGXX_PATH%" ^
      -G"Ninja" ^
      ..

cd tools\buildscripts

echo.
echo System clang Ninja build generated in SystemClangNinja directory
echo To build: cd SystemClangNinja && ninja
echo.
pause
