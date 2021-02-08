Contact:  cgtrails@pacificwest.com

Deadly rocks is a game similiar to the arcade game Asteroids that came out back in 1979.  It  was my favorite arcade game and I wasted a lot of quarters playing it at a nearby Plaid Pantry convenience store back in the early 1980's when I was in middle and high school.  Fast forward to 2017 I started teaching myself C++ and OpenGL at age 50.  Now I present you my version of asteroids that I programmed.

SYSTEM REQUIREMENTS:
A single core 64 bit AMD or Intel processor.  Deadly rocks uses only one physical thread.  Will work on a multi core processor but doesn't utilize the other cores.
Graphics card that supports OpenGL 4.
Tested on Windows 10 only.  Not sure if it works on earlier versions.  If anyone is interested in making it work on Linux, please do.  I have no idea how to convert it to Linux compatiblility.

GETTING STARTED:
To get started just click on the DeadlyRocks.exe icon (file) or the shortcut icon.  No installation needed.  The game starts in demonstration mode displaying credits, ranks, and a menu.

DEFAULT CONTROLL SETTINGS:
Keyboard...
rotate left: "A"
rotate right: "S"
thrust: "K"
fire: "L"
shields: Space bar
bomb: "B"

Gamepad...
rotate left & right: left axis
thrust: right trigger
fire: A button
shields: X button
bomb: B button

FUNCTION KEYS:
"F1" will start the game.  
"F2" to access controller settings.
"F3" to access miscellaneous settings. 
"F4" toggles between windowed and fullscreen.
"F5" toggles between hollow and solid rocks.
"F6" displays framerate.

  When changing settings, use the up and down arrow keys to highlight the setting you want to change.  Left and right arrow keys cycle through the choices for the setting. 
  For changing keyboard assignments type the key you want to use.  Program prevents you from using the same key for more than one action.
  To change gamepad assignments use the "plus sign" shaped button on the gamepad.
  For the first player you can use a keyboard or a gamepad. Additional players need a gamepad.  Have all the controllers plugged in before starting the game.

HIGH SCORES:
  Deadly Rocks keeps track of the 15 highest scores.  If you and other players beat the current scores, then at the end of the game you are asked to enter your initials.  Use the keyboard to type the initial and then use left or right arrow keys to move to next initial.  Hit "Enter" to save.

GAME LEVELS:
  The game levels are set by the "levels.txt" file. If you want to change or add levels just edit this file.  The instructions are in the file itself.  You can add as many levels as you want.  If you're getting an error when the game is trying to load this file and all looks good, chances are, there's a blank line or lines at the end of the file.  The game doesn't put limits on the numbers you use so using crazy numbers can crash the game or cause it to not work correctly.

RESOLUTION:
  In main settings you can choose the resolution to match the common size monitors available;  standard, 2K, 4K, 8K, and 16K. Any of these resolutions will work on any monitor, but, for example choosing 16K on a standard resolution monitor, the game objects will be very small!  The program will compensate the number of rocks to the resolution you choose.

GAME DIFFICULTY:
  Using the in game menu settings and editing the "levels.txt" file, you can make this game too easy or impossible to play.  There's some more settings available in the "settings.ini" file to expirement with.  The setting names are somewhat self explanetory.

PERFORMANCE:
  I programmed this game on a Ryzen 3 1200 processor and Radeon rx580 GPU.  The Ryzen 3 1200 is a four core CPU and this game only uses one core.  Even then, the single core load is quite low so this game should work good on a much slower CPU.
  What can really slow down performance is using a large number of draw commands in OpenGL.  The overhead in issuing a draw command to the GPU is insane!  While this isn't a serious problem in this game using a recent GPU, it could make a big difference on an older laptop with an on board GPU so the programming trick is to use as few draw commands as possible.
  In the settings file there's a setting called "Rock_instancing". If it's set to "0" then a draw command is used for each rock.  So if your game has 500 rocks moving around then there's 500 draw commands for each frame being drawn.  If "Rock_instancing" is set to one "1" then all the rocks are drawn with only one draw command.  When rendering thousands of rocks this will greatly improve performance (I've gotten up to 10x frame rate improvement).  The problem with instancing though is you can only render one shape.  To get around this I use a random number generator in the shader program (GPU program) to alter the rock shape.  It's duplicated on the CPU for narrowphase collision detection.  
  Another setting to consider is "Max_rocks".  This reserves GPU memory for drawing rocks.  For example, if you set a level to render 100 rocks, on standard level set Max_rocks to at least 2500.  At 2K resolution double that (5000), 4K resolution set to 10000, 8K set to 40000, 16K set to 160000.

ISSUES:
You can set player to "NPC" but I don't have that programmed yet so NPC ship will just sit in one spot doing nothing.

Game crashes after entering high score initials:  Most likely caused by antivirus software not allowing high scores to be saved to storage media.

Controller issues;  I've only used the Xbox type gamepads for controllers on this game.  Let me know what other controllers do.  I plan to purchase a joystick later on to try out.

SOURCE CODE:
  If you want to make changes to the game, install MinGW-w64 - GCC compiler for Windows and the IDE Code::Blocks.  I'm sure it will work with other C++ compilers but I haven't tried it out.  
  I would really like someone to convert it to a Linux version.