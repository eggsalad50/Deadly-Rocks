#ifndef CONSTANTS_H_INCLUDED
#define CONSTANTS_H_INCLUDED

/// OpenGL RGB colors
#define WHITE vec3(1.0, 1.0, 1.0)
#define HALF_WHITE vec3(0.5, 0.5, 0.5)
#define GOLD vec3(1.0, 215.0/255.0, 0.0)
#define DEEP_SKY_BLUE vec3(0.0, 191.0/255.0, 1.0)
#define LIGHT_SKY_BLUE vec3(135.0/255.0, 206.0/255.0, 1.0)
#define DODGER_BLUE vec3(30.0/255.0, 144.0/255.0, 1.0)
#define TURQUOISE vec3(64.0/255.0, 224.0/255.0, 208.0/255.0)
#define CADET_BLUE vec3(95.0/255.0, 158.0/255.0, 160.0/255.0)
#define LIME vec3(0.0, 1.0, 0.0)
#define YELLOW vec3(1.0, 1.0, 0.0)
#define VIOLET vec3(238.0/255.0, 130.0/255.0, 238.0/255.0)
#define ORANGE vec3(1.0, 140.0/255.0, 0.0)
#define TOMATO vec3(1.0, 99.0/255.0, 71.0/255.0)
#define MAGENTA vec3(1.0, 0.0, 1.0)
#define LAVENDER vec3(230.0/255.0, 230.0/255.0, 250.0/255.0)
#define CHOCOLATE vec3(210.0/255.0, 105.0/255.0, 30.0/255.0)
#define RED vec3(1.0, 0.0, 0.0)
#define WHEAT vec3(245.0/255.0, 222.0/255.0, 179.0/255.0)
#define SALMON vec3(250.0/255.0, 128.0/255.0, 114.0/255.0)
#define TAN vec3(210.0/255.0, 180.0/255.0, 140.0/255.0)
#define PINK vec3(1.0, 192.0/255.0, 203.0/255.0)
#define THISTLE vec3(216.0/255.0, 191.0/255.0, 216.0/255.0)
#define AQUA_MARINE vec3(127.0/255.0, 1.0, 212.0/255.0)
#define KHAKI vec3(240.0/255.0, 230.0/255.0, 140.0/25.0)
#define MOCCASIN vec3(1.0, 228.0/255.0, 181.0/255.0)

// Other defines
#define UP_VECTOR vec3d(0.0, 1.0, 0.0)
#define RIGHT_VECTOR vec3d(1.0, 0.0, 0.0)

/// ******** Universal and conversion constants ********
const double TO_DEGRESS = 180.0 / M_PI;
const double TO_RADIANS = M_PI / 180.0;
const double CIRCLE = M_PI * 2.0;                   //radians in a circle
const double SQ_ROOT_3 = 1.7320508075688772935274463415059;
const double SQ_ROOT_2 = 1.4142135623730950488016887242097;

// Game constants
const int BOMB_BULLETS = 360;
const int MAX_BBULLETS = BOMB_BULLETS*17;
const float X_MAX = 1920;
const float Y_MAX = 1080;
const int GRID_SIZE = 6;
const int GRID_SIZE_MINUS = 5;
const float FGRID_SIZE = 5.0;
const float X_GRID = (X_MAX+100) / FGRID_SIZE;      // Collision grid
const float Y_GRID = (Y_MAX+100) / FGRID_SIZE;
const float EXPLODE_TIME = 0.75;
const int MAX_EXDRONE = 100;
const int DRONE_PARTICLES = 1000;

const int BIG_ROCK = 1;     // Rock sizes
const int MEDIUM_ROCK = 2;
const int SMALL_ROCK = 3;

const int COMMON_ROCK = 1;  // Rock types
const int FAST_ROCK = 2;
const int HARD_ROCK = 3;
const int BRITTLE_ROCK = 4;
const int COMET_ROCK = 5;

const int NO_SHIPS = 0;
const int SHIP_ACTIVE = 1;
const int SHIP_EXPLODE = 2;
const int SHIP_SPAWN = 3;

// Object ID
const int ROCK = 1;
const int SHIP = 2;
const int ENEMY = 3;
const int DRONE = 4;

// Bullet source
const int SHIP_BULLET = 1;
const int BOMB_BULLET = 2;
const int ENEMY_BULLET = 3;

const int LEFT = 0;
const int RIGHT = 1;

const int BIG_ENEMY = 1;
const int SMALL_ENEMY = 2;

const int MAX_BR_EXPLODE = 5;
const int MAX_MR_EXPLODE = 5;
const int MAX_SMR_EXPLODE = 20;
const int MAX_SE_EXPLODE = 10;
const int MAX_FIRE_SND = 17;
const int MAX_DR_EXPLODE = 20;

const double NO_SHOW_STUFF = 60.0;
const double HI_SCORE_ON = 10.0;
const double HI_SCORE_OFF = 0.0;
const double CREDITS_ON = 30.0;
const double CREDITS_OFF = 20.0;
const double RANK_ON = 50.0;
const double RANK_OFF = 40.0;

const int KEYBOARD = 1;
const int JOYSTICK = 2;
const int GAMEPAD = 3;
const int NPC = 4;

// Text constants
const int TITLE = 0;
const int F1_TO_START = TITLE+1;
const int F2_SETTINGS = F1_TO_START+1;
const int F3_MISC_SETTINGS = F2_SETTINGS+1;
const int PROGRAMMER = F3_MISC_SETTINGS+1;
const int PROGRAM_TOOLS = PROGRAMMER+1;
const int COMPILER = PROGRAM_TOOLS+1;
const int OPENGL = COMPILER+1;
const int GLFW32 = OPENGL+1;
const int OPENAL11 = GLFW32+1;
const int CODE_BLOCKS = OPENAL11+1;
const int AUDIO_FILE = CODE_BLOCKS+1;

const int RANK_TEXT = AUDIO_FILE+1;
const int SR_SHORT = RANK_TEXT+1;
const int SR_TEXT = SR_SHORT+1;
const int SB_SHORT = SR_TEXT+1;
const int SB_TEXT = SB_SHORT+1;
const int SMN_SHORT = SB_TEXT+1;
const int SMN_TEXT = SMN_SHORT+1;
const int S1C_SHORT = SMN_TEXT+1;
const int S1C_TEXT = S1C_SHORT+1;
const int SSRS_SHORT = S1C_TEXT+1;
const int SSRS_TEXT = SSRS_SHORT+1;
const int SSGT_SHORT = SSRS_TEXT+1;
const int SSGT_TEXT = SSGT_SHORT+1;
const int TSGT_SHORT = SSGT_TEXT+1;
const int TSGT_TEXT = TSGT_SHORT+1;
const int MSGT_SHORT = TSGT_TEXT+1;
const int MSGT_TEXT = MSGT_SHORT+1;
const int SMSGT_SHORT = MSGT_TEXT+1;
const int SMSGT_TEXT = SMSGT_SHORT+1;
const int CMSGT_SHORT = SMSGT_TEXT+1;
const int CMSGT_TEXT = CMSGT_SHORT+1;
const int LT2D_SHORT = CMSGT_TEXT+1;
const int LT2D_TEXT = LT2D_SHORT+1;
const int LT1D_SHORT = LT2D_TEXT+1;
const int LT1D_TEXT = LT1D_SHORT+1;
const int CAPT_SHORT = LT1D_TEXT+1;
const int CAPT_TEXT = CAPT_SHORT+1;
const int MAJ_SHORT = CAPT_TEXT+1;
const int MAJ_TEXT = MAJ_SHORT+1;
const int LTCOL_SHORT = MAJ_TEXT+1;
const int LTCOL_TEXT = LTCOL_SHORT+1;
const int COL_SHORT = LTCOL_TEXT+1;
const int COL_TEXT = COL_SHORT+1;
const int BRGEN_SHORT = COL_TEXT+1;
const int BRGEN_TEXT = BRGEN_SHORT+1;
const int MJGEN_SHORT = BRGEN_TEXT+1;
const int MJGEN_TEXT = MJGEN_SHORT+1;
const int LTGEN_SHORT = MJGEN_TEXT+1;
const int LTGEN_TEXT = LTGEN_SHORT+1;
const int GEN_SHORT = LTGEN_TEXT+1;
const int GEN_TEXT = GEN_SHORT+1;
const int GAMEOVER = GEN_TEXT+1;
const int PLAY_AGAIN = GAMEOVER+1;
#endif // CONSTANTS_H_INCLUDED
