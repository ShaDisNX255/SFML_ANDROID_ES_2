OpenNetBattle / SFML Web Graphics PoC - corrected shader test
================================================================

Why this replacement exists
---------------------------
The first graphics test loaded the PNG correctly, but normal sprite drawing
crashed because this SFML GLES2 fork expects RenderTarget::setDefaultShader()
to be called before drawing.

RenderTarget stores a raw m_defaultShader pointer. The standalone test had not
installed one, so the first sprite draw treated an uninitialized pointer as a
Shader and WebGL received a bogus program ID.

This corrected test:
  1. Creates sf::RenderWindow.
  2. Compiles a minimal GLES2 vertex + fragment shader.
  3. Installs it with RenderWindow::setDefaultShader().
  4. Marks texture0 as sf::Shader::CurrentTexture.
  5. Loads /assets/test.png with sf::Texture::loadFromFile().
  6. Draws the sf::Sprite through SFML's normal RenderTarget path.

Files
-----
  web-graphics-poc\main.cpp
  web-graphics-poc\assets\test.png
  web-graphics-poc\build.cmd
  web-graphics-poc\serve.cmd

Build
-----
From the SFML_ANDROID_ES_2 repository root:

  web-graphics-poc\build.cmd

Run
---
  web-graphics-poc\serve.cmd

Open
----
  http://localhost:8000/sfml-web-graphics.html

Expected console
----------------
  [ONB_WEB_BOOT] SFML graphics browser test started
  [ONB_WEB_GL] Creating sf::RenderWindow
  ...
  [ONB_WEB_GL] sf::RenderWindow created: 640x480
  [ONB_WEB_GL] Creating GLES2 default SFML shader
  [ONB_WEB_GL] Default shader installed, program ID: <small nonzero ID>
  [ONB_WEB_FS] Loading /assets/test.png through sf::Texture
  [ONB_WEB_FS] Texture loaded successfully: 64x64
  [ONB_WEB_GL] Sprite prepared and centered
  [ONB_WEB_BOOT] Entering browser render loop
  [ONB_WEB_GL] SFML sprite frame rendered successfully

Expected visual
---------------
A dark 640x480 canvas with a centered 192x192 colored pixel-art square.
