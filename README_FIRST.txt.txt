Contact:  cgtrails@pacificwest.com

Deadly rocks is an asteroids remake. Asteroids was my favorite arcade game that I wasted a lot of quarters playing at a nearby Plaid Pantry convenience store back in the early 1980's when I was in middle and high school.  Fast forward 40 years later I started teaching myself C++ and OpenGL at age 50.  Now I present you my version of asteroids that I programmed.

SYSTEM REQUIREMENTS:
A single core 64 bit AMD or Intel processor.  Deadly rocks uses only one physical thread.  Will work on a multi core processor but doesn't utilize the other cores.
Graphics card that supports OpenGL 4.
Tested on Windows 10 only.  Not sure if it works on earlier versions.  If anyone is interested in making it work on Linux, please do.  I have no idea how to convert it to Linux compatiblility.

GETTING STARTED:
To get started just click on the DeadlyRocks.exe icon (file) or the shortcut icon.  No installation needed.  The game starts in demonstration mode displaying credits, ranks, and a menu.

"F1" will start the game.  Press F2 to change settings.
Use the up and down arrow keys to highlight the setting you want to change.  Left and right arrow keys cycle through the choices for the setting.  For changing keyboard assignments type the key you want to use.  Program prevents you from using the same key for more than one action.
For the first player you can use a keyboard or a gamepad. Additional players need a gamepad.  Have all the controllers plugged in before starting the game.

Settings not shown are as follows...
"F3" displays framerate.  If the setting "Vsync" in "settings.ini" file is set to zero, the program will run full blast usually resulting in very high frame rates depending on the performance of the CPU and GPU.  If set to one then the framerate will usually match the monitor refresh rate which is normally 60 Hertz.

"F4" toggles between windowed and fullscreen.

"F5" toggles between outlined and solid rocks.  Doesn't seem to be a performance difference.

If the game asks for entering initials for high score, use the keyboard to type the initial and then use left or right arrow keys to move to next initial.  Hit "Enter" to save.

GAME LEVELS:
The "levels.txt" file contains the data for game levels.  There are 31 levels.  If you play beyond 31 levels then you'll just repeat playing the last level. You can easily add more levels or change the existing ones by editing the file with Notepad or some other text editor.  You can add as many levels as you want.  The comments in the file tells you how to do it.  The program doesn't put limits on the numbers you use so using crazy numbers can crash the game or cause it to not work correctly.

SETTINGS:
Other ways to change the game behavoir is to edit the "Settings.ini" file.  I used names for the settings that would be somewhat self explanatory but I'll explain some of them here.  Using extreme numbers will cause the game to malfunction or crash.

"Starting_level"  Game starts at this level.  Use "1" or more.
"Start_ships"  Number of ships to start with.
"Extra_ship_points"  Bonus ship everytime you score this many points.
"Player_hit_player"  If multiplayers, you can shoot other players or get shot by them.

"Monitor_resolution" Default resolution (0-4) 0 is standard, 2K, 4K, 8K, and 16K.  You can use any of these on any monitor.  The program will compensate the number of rocks to the resolution you choose.

"Rock_instancing"  In OpenGL graphics instancing refers to drawing a number of objects with one draw command.  This can greatly improve performance.  The downside is that you can only render one shape.  To get around this I use a random number generator in a shader (GPU program) to alter the shape of the rock.  I duplicate this on the CPU for collision detection.  So if you're going to create a level with thousands of rocks then it would be a good idea to enable this by setting to "1".  On the Radeon rx560 graphics card, it had no problem rendering 60000 rocks at once.

"Max_rocks"  The maximum number of rocks allowed for instancing mode.  This simply reserves memory on the graphics card for this number of rocks.  I have the default at 10000 but modern graphics cards have memory for perhaps millions.  If you're going to play with an insane number of rocks then bump this number up.  There are other settings for reserving GPU memory for bullets and explosion fragments.

"Line_width" AMD and Nvidia renders lines differently.  AMD lines are thinner and missing the last pixel.  Not sure why.  Setting to "2.0" helps out this problem.

ISSUES:
You can set player to "NPC" but I don't have that programmed yet so your player will just sit in one spot doing nothing.

Game crashes after entering high score initials:  Most likely caused by antivirus software not allowing high scores to be saved to storage media.

Controller issues;  I've only used the Xbox type gamepads for controllers on this game.  Let me know what other controllers do.  I plan to purchase a joystick later on to try out.