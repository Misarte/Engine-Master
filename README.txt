 “README.md” containing 
info about the engine, 
author, 
a link to the github repository 
and ​license ​ . 
Any special instructions for execution should be included in this file. 
==================================================================
Artemis Engine v1

3D Game Engine

Author: Artemis Georgakopoulou

Artemis Engine is licensed under the MIT License, see LICENSE for more information.
====================================================================================
Instructions:

-Load 3D Models in .fbx format.

-Load 2D Textures in .png format.

-Visualize!
====================================================================================
Controls:

Key Inputs
-> Window Controls
-->Quit Application - Hit Escape Keyboard Key, "X" on Window Left Mouse Click
-->Resize Window Drag & Drop Sides of Application Window.
-->Set Fullsize "FULLSIZE" on Window Left Mouse Click.

-Camera Controls 
-->Move Camera Front/Back/Left/Right/Up/Down with W/S/A/D/Q/E Keyboard Keys correspondingly, ONLY when Right Mouse Click.
-->Rotate Camera 
---Around Camera's Axis (X, Y) Move Mouse (Left/Right, Up/Down) accordingly , ONLY when Right Mouse Click.
---Around Model's Axis (X, Y) Move Mouse (Left/Right, Up/Down) accordingly, ONLY when Left Mouse Click & Left ALT Keyboard Key.
--> Focus Camera on Loaded Model Press F Keyboard Key 
--> Zoom Camera In/Out Mouse Scroll Button Up/Down

-Model Controls
-->Drop fbx Model Drag & Drop Model within Application Window.
-->Drop png Texture Drag & Drop Texture within Application Window.

IMGUI Inputs
->All windows contains "Close Me" Button option to close.
 
->Main Menu
---Visit Github Page of Application - "Visit Github page" Option.
---Quit Application - Hit Quit Option.


->Tools
---Properties 
----- Texture - Select texture for uploaded model, Checkers.png texture available.

---Configuration Window
-----Modules - Window - Set Fullsize - Hit Checkbox.
-----Modules - Camera - Render Grid Checkbox. 
-----Modules - Camera - Render Axis on Loaded Model Checkbox.
-----Modules - Camera - Adjust Position/Up-Front Vectors Sliders.
-----Modules - Camera - Adjust FOV Slider.

IMGUI Info
->Main Menu
---Demo Window - Shows Demo Window of IMGUI library.
---About Window - Show About info regarding the Application Game Engine.
---About Window - Show Config/Build Info Checkbox, displays System/Software Configuration settings and Frame Rate.
->Tools
---Console Window - Shows Application Logs
---Properties - Model Transformation Info
---Properties - Model Geometry Info
---Properties - Model current Texture Info

---Configuration Window - Window/Camera/Input/Renderer Application information.
---Configuration Window - Libraries Information.
