OpenNetBattle / SFML Web Canvas PoC
===================================

This folder is intentionally standalone. Configure it with Emscripten 3.1.50.

From an Emscripten-enabled Command Prompt:

  cd /d C:\Users\Administrator\Documents\GitHub\SFML_ANDROID_ES_2
  rmdir /s /q web-poc-build 2>nul
  emcmake cmake -S web-poc -B web-poc-build -G Ninja -DCMAKE_BUILD_TYPE=Debug
  cmake --build web-poc-build --target sfml-web-canvas --parallel

Expected output files include:

  web-poc-build\sfml-web-canvas.html
  web-poc-build\sfml-web-canvas.js
  web-poc-build\sfml-web-canvas.wasm

Serve the build directory over HTTP:

  cd web-poc-build
  python -m http.server 8000

Then open:

  http://localhost:8000/sfml-web-canvas.html

Expected browser console diagnostics:

  [ONB_WEB_BOOT] Browser canvas test started
  [ONB_WEB_GL] Creating SFML window
  [ONB_WEB_GL] SFML window created: 640x480
  [ONB_WEB_GL] WebGL context activated
  [ONB_WEB_GL] Frame rendered successfully at 640x480
