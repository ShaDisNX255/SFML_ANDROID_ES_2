@echo off
setlocal

set "ROOT=%~dp0.."
pushd "%ROOT%"

echo [ONB_WEB_BOOT] Building SFML graphics browser test...

where em++ >nul 2>nul
if errorlevel 1 (
    echo [ONB_WEB_BOOT] ERROR: em++ is not in PATH. Run this from an Emscripten 3.1.50 environment.
    popd
    exit /b 1
)

if not exist "cmake-build-web\lib\libsfml-graphics-s-d.a" (
    echo [ONB_WEB_BOOT] ERROR: cmake-build-web\lib\libsfml-graphics-s-d.a is missing.
    popd
    exit /b 1
)

if not exist "cmake-build-web\lib\libsfml-window-s-d.a" (
    echo [ONB_WEB_BOOT] ERROR: cmake-build-web\lib\libsfml-window-s-d.a is missing.
    popd
    exit /b 1
)

if not exist "cmake-build-web\lib\libsfml-system-s-d.a" (
    echo [ONB_WEB_BOOT] ERROR: cmake-build-web\lib\libsfml-system-s-d.a is missing.
    popd
    exit /b 1
)

if not exist "web-graphics-poc-build" mkdir "web-graphics-poc-build"

del /q web-graphics-poc-build\sfml-web-graphics.* 2>nul

em++ web-graphics-poc\main.cpp ^
  -std=c++14 ^
  -DSFML_STATIC ^
  -DSFML_OPENGL_ES ^
  -DGL_GLEXT_PROTOTYPES ^
  -Iinclude ^
  -Isrc ^
  cmake-build-web\lib\libsfml-graphics-s-d.a ^
  cmake-build-web\lib\libsfml-window-s-d.a ^
  cmake-build-web\lib\libsfml-system-s-d.a ^
  -lEGL ^
  -lGLESv2 ^
  -sUSE_FREETYPE=1 ^
  -sFULL_ES2=1 ^
  -sASSERTIONS=2 ^
  -sGL_ASSERTIONS=1 ^
  -sALLOW_MEMORY_GROWTH=1 ^
  --preload-file web-graphics-poc/assets/test.png@/assets/test.png ^
  -o web-graphics-poc-build\sfml-web-graphics.html

if errorlevel 1 (
    echo [ONB_WEB_BOOT] ERROR: Browser graphics test build failed.
    popd
    exit /b 1
)

echo.
echo [ONB_WEB_BOOT] Build succeeded.
echo [ONB_WEB_BOOT] Output:
dir /b web-graphics-poc-build\sfml-web-graphics.*

popd
endlocal
