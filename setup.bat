@echo off
setlocal
set "VCPKG_DIR=external\vcpkg"

rem Clone vcpkg repo if it isn’t there yet
if not exist "%VCPKG_DIR%\.git" (
    echo Cloning vcpkg...
    git clone https://github.com/microsoft/vcpkg.git "%VCPKG_DIR%" || goto :error
)

pushd "%VCPKG_DIR%" || goto :error
call bootstrap-vcpkg.bat -disableMetrics || goto :error
popd
echo vcpkg ready at %VCPKG_DIR%
goto :eof

:error
echo.
echo *** setup_vcpkg.bat failed – see messages above ***
exit /b 1
