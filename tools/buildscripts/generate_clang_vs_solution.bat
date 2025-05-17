@echo off

set BuildMode=%1

IF NOT DEFINED BuildMode (
    set BuildMode=Release
)

echo Generating Clang Visual Studio Solution for %BuildMode% mode...
echo.

call internal_build_tools\generate_clang_vs_solution_internal.bat %BuildMode%

echo.
echo Clang Visual Studio solution generated in VS_Solution_Clang directory
echo Open VS_Solution_Clang\UE4SSMonorepo.sln in Visual Studio
echo.
pause