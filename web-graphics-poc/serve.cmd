@echo off
setlocal

set "ROOT=%~dp0.."
pushd "%ROOT%\web-graphics-poc-build"

echo [ONB_WEB_BOOT] Serving SFML graphics browser test on http://localhost:8000/
echo [ONB_WEB_BOOT] Open http://localhost:8000/sfml-web-graphics.html
echo.

python -m http.server 8000

popd
endlocal
