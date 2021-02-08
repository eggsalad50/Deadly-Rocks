#ifndef MAIN_H_INCLUDED
#define MAIN_H_INCLUDED
#define _USE_MATH_DEFINES

using namespace std;
#include <Windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <sstream>
#include <time.h>
#include <thread>
#include <chrono>
#include <stdarg.h>
#include <string.h>
#include <ctype.h>
#include <algorithm>
#include <vector>
#include <random>
#include <math.h>
#include <cmath>
#include <array>
#include <locale.h>
#include <cfloat>

#define GLEW_STATIC
#define GLFW_DLL
#include <openAL/al.h>
#include <openAL/alc.h>
#include <GL/glew.h> // include GLEW and new version of GL on Windows
#include <GL/glfw3.h> // GLFW helper library
#include <GL/glfw3native.h>

#include "AudioFile.h"
#include "Constants.h"
#include "OpenGl_math.h"
#include "OpenGl_log.h"
#include "initialize.h"

extern GLFWwindow* window;
extern FILE* Debug;

extern int g_win_width;
extern int g_win_height;
extern int g_fb_width;
extern int g_fb_height;
extern const GLFWvidmode *vmode;
extern bool fullscreen;

extern int64_t counter;
extern bool Application_thread;

extern GLenum error;

struct ascii
{
    int start;
    int num;
    float width;
};
extern ascii ascii_table[256];
extern ascii letter_table[256];
extern vector<ascii> star_words;

struct level_data
{
    int level;
    int num_rocks;     // normal rocks
    int num_fast;      // fast rocks
    int num_brittle;   // brittle rocks
    int num_hard;      // hard rocks
    int num_enemy;     // # of enemies at the same time
    int num_bullets;   // # of bullets fired at once
    int num_clusters;  // # of drone clusters
    vec2i range;
    float dr_speed;    // Base drone speed
    vec2 dr_accel;     // Drone acceleration range
};
extern vector<level_data> levels;

struct player_ship
{
    GLuint program;
    GLuint vao;
    GLuint points_vbo;
    vec2 *points;
    mat4 proj_mat;
    GLuint proj_mat_location;
    GLuint matrix_mat_location;
    GLuint color_location;
    GLuint depth_location;
};
extern player_ship sh_pship;

struct shield_ship
{
    GLuint program;
    GLuint vao;
    GLuint points_vbo;
    vec2 *points;
    mat4 proj_mat;
    GLuint proj_mat_location;
    GLuint matrix_mat_location;
    GLuint color_location;
    GLuint depth_location;
    GLuint bright_location;
};
extern shield_ship sh_shield;

struct exhaust
{
    GLuint program;
    GLuint vao;
    GLuint points_vbo;
    vec2 *points;
    GLuint color_vbo;
    vec3 *color;
    mat4 proj_mat;
    GLuint proj_mat_location;
    GLuint matrix_mat_location;
    GLuint color_location;
    GLuint seed_location;
    GLuint depth_location;
};
extern exhaust sh_exhaust;

struct text2D_shader
{
    GLuint program;
    GLuint vao;
    GLuint points_vbo;
    GLfloat *points;
    mat4 proj_mat;
    GLuint proj_mat_location;
    GLuint matrix_mat_location;
    GLuint color_location;
    GLuint depth_location;
    int character_end;
};
extern text2D_shader sh_2Dtext;

struct bullet_shader
{
    GLuint program;
    GLuint vao;
    GLuint points_vbo;
    vec2 *points;
    GLuint color_vbo;
    vec3 *color;
    mat4 proj_mat;
    GLuint proj_mat_location;
    GLuint matrix_mat_location;
    GLuint depth_location;
};
extern bullet_shader sh_bullets;

struct rock_shader
{
    GLuint program;
    GLuint vao;
    GLuint points_vbo;
    vec2 *points;
    mat4 proj_mat;
    GLuint proj_mat_location;
    GLuint matrix_mat_location;
    GLuint color_location;
    GLuint depth_location;
    GLuint radius_location;
    GLuint pos_location;
    GLuint xratio_location;
    GLuint yratio_location;
    GLuint angle_location;
    int point_count;
};
extern rock_shader sh_rocks;

struct rock_shader2                  // For OpenGL instancing
{
    GLuint program;
    GLuint vao;
    GLuint points_vbo;
    vec2 *points;
    GLuint position_vbo;
    int *active;
    GLuint active_vbo;
    vec2 *position;
    GLuint direction_vbo;
    vec2 *direction;
    GLuint radius_vbo;
    float *radius;
    GLuint angle_vbo;
    float *angle;
    GLuint color_vbo;
    vec3 *color;
    mat4 proj_mat;
    GLuint proj_mat_location;
    GLuint matrix_location;
    GLuint ratio_location;
    GLuint depth_location;
    GLuint seed1_location;
    GLuint seed2_location;
};
extern rock_shader2 sh_rocks2;

struct drone_shader                  // For OpenGL instancing
{
    GLuint program;
    GLuint vao;
    GLuint points_vbo;
    vec2 *points;
    GLuint position_vbo;
    vec2 *position;
    GLuint angle_vbo;
    float *angle;
    GLuint color_vbo;
    vec3 *color;
    mat4 proj_mat;
    GLuint proj_mat_location;
    GLuint matrix_location;
    GLuint xratio_location;
    GLuint yratio_location;
    GLuint radius_location;
    GLuint depth_location;
};
extern drone_shader sh_drone;

struct moverock_shader
{
    GLuint program;
    GLuint position_vbo;
    vec2 *position;
    GLuint direction_vbo;
    vec2 *direction;
    GLuint angle_vbo;
    float *angle;
    GLuint anglestep_vbo;
    float *anglestep;
    int *active;
};
extern moverock_shader sh_mrock;

struct rockexplode_shader
{
    GLuint program;
    GLuint vao;
    GLuint direction_vbo;
    vec2 *direction;
    mat4 proj_mat;
    GLuint proj_mat_location;
    GLuint matrix_mat_location;
    GLuint color_location;
    GLuint depth_location;
    GLuint center_location;
    GLuint time_locaton;
    GLuint xratio_location;
    GLuint yratio_location;
};
extern rockexplode_shader sh_rexplode;

struct miniexplode_shader
{
    GLuint program;
    GLuint vao;
    GLuint points_vbo;
    vec2 *points;
    GLuint direction_vbo;
    vec2 *direction;
    GLuint angle_vbo;
    float *angle;
    GLuint radius_vbo;
    float *radius;
    mat4 proj_mat;
    GLuint proj_mat_location;
    GLuint matrix_mat_location;
    GLuint color_location;
    GLuint depth_location;
    GLuint center_location;
    GLuint time_locaton;
    GLuint xratio_location;
    GLuint yratio_location;
    double time;
};
extern miniexplode_shader sh_miniexplode;

struct impact_shader
{
    GLuint program;
    GLuint vao;
    GLuint direction_vbo;
    vec2 *direction;
    mat4 proj_mat;
    GLuint proj_mat_location;
    GLuint matrix_mat_location;
    GLuint color_location;
    GLuint depth_location;
    GLuint center_location;
    GLuint time_locaton;
    GLuint xratio_location;
    GLuint yratio_location;
};
extern impact_shader sh_impact;

struct drexplode_shader
{
    GLuint program;
    GLuint vao;
    GLuint direction_vbo;
    vec2 *direction;
    GLuint col_vbo;
    vec3 *col;              // Color
    GLuint psize_vbo;
    float *psize;
    mat4 proj_mat;
    GLuint proj_mat_location;
    GLuint matrix_mat_location;
    GLuint depth_location;
    GLuint center_location;
    GLuint time_locaton;
    GLuint xratio_location;
    GLuint yratio_location;
    GLuint Xmax_location;
    GLuint Ymax_location;
};
extern drexplode_shader sh_drexplode;

struct shipexplode_shader
{
    GLuint program;
    GLuint vao;
    GLuint points_vbo;
    vec2 *points;
    GLuint direction_vbo;
    vec2 *direction;
    GLuint spin_vbo;
    vec2 *spin;
    GLuint radius_vbo;
    float *radius;
    mat4 proj_mat;
    GLuint proj_mat_location;
    GLuint matrix_mat_location;
    GLuint color_location;
    GLuint depth_location;
    GLuint center_location;
    GLuint time_locaton;
    GLuint xratio_location;
    GLuint yratio_location;
    GLuint Xmax_location;
    GLuint Ymax_location;
    double time;
};
extern shipexplode_shader sh_shipexplode;

struct shipexplode2_shader
{
    GLuint program;
    GLuint vao;
    GLuint points_vbo;
    vec2 *points;
    GLuint direction_vbo;
    vec2 *direction;
    GLuint spin_vbo;
    vec2 *spin;
    GLuint radius_vbo;
    float *radius;
    mat4 proj_mat;
    GLuint proj_mat_location;
    GLuint matrix_mat_location;
    GLuint color_location;
    GLuint depth_location;
    GLuint center_location;
    GLuint time_locaton;
    GLuint xratio_location;
    GLuint yratio_location;
    GLuint Xmax_location;
    GLuint Ymax_location;
    double time;
};
extern shipexplode2_shader sh_shipexplode2;

struct ship_data
{
    int status;
    float xpos;   // position
    float ypos;
    float l_xpos;  // last position
    float l_ypos;
    float angle;  // Orientation
    float angle_step;
    float dirx;   // direction moving
    float diry;
    float accel;  //acceleration
    float mass;
    int ships;    // Number of ships you have
    int add_score;
    int score;
    int hits;
    int fired;
    int spawn_tries;
    int shape_pntr;           // Starting data in OpenGL buffer
    vector<vec2>raw_shape;
    vector<vec2>shape;
    vector<vec2>lshape;
    vec3 color;
    bool thrust_flag;
    double bomb_timer;
    bool fire_pressed;
    float thrust_pitch;
    float fire_pitch;

    int controller;   // keyboard, joystick or gamepad
    int jstick;       // joystick or gamepad number
    int rotate_left;  // Input codes for ship
    int rotate_right;
    int thrust;
    int fire;
    int shields;
    int bomb;
    string srank;

};
extern ship_data pship[17];
bool extern pship_status;         // Master status flag
extern int max_players;
extern int selected_player;
extern double spawn_shield_timer[17];

struct drone_ship
{
    int cluster;    // -1 = Individual (not in cluster)
    float x_offset;
    float y_offset;
    float xpos;     // position
    float ypos;
    float angle;    // Orientation
    float speed;    // current speed
    float tspeed;   // top speed
    float accel;    // acceleration factor
    int target;     // Player ship target
    vec3 color;
    bool status;
};
extern vector<drone_ship>drones;

struct drone_cluster
{
    bool status;
    float xpos;
    float ypos;
    float xdir;
    float ydir;
};
extern vector<drone_cluster>cluster;

struct ship_bullets
{
    bool status;
    bool edge;
    int ship;
    float xpos;
    float ypos;
    float l_xpos;    // last X position
    float l_ypos;    // last Y position
    float xdir;      // Direction and magnitude
    float ydir;
    float xstart;    // launch point (ship or overlap)
    float ystart;
    float sx_offset;  // Speed of ship compensate
    float sy_offset;
    float x_norm;     // Normal direction (length of one)
    float y_norm;
    vec3 color;
    double life;     // Life in seconds
};
extern vector<ship_bullets> bullets;
extern ship_bullets bbullets[MAX_BBULLETS];
extern bool bomb_flag;
extern int last_bbullet;
extern float bullet_life;

struct enemy_bullets
{
    bool status;
    bool edge;
    bool aim;
    float xpos;
    float ypos;
    float l_xpos;    // last X position
    float l_ypos;    // last Y position
    float xdir;
    float ydir;
    float xstart;    // launch point (ship or overlap)
    float ystart;
    float x_norm;
    float y_norm;
    vec3 color;
    double life;     // Life in seconds
};

extern vector<enemy_bullets> ebullets;
//extern bool ebullet_status;
//extern int last_ebullet;
extern float ebullet_life;
extern float pre_ebullet_life;

struct eship_data
{
    int status;
    int type;        // Big or small ship
    int code;        // Shape index
    vec2 xysize;
    float xpos;
    float ypos;
    float dirx;
    float diry;
    float speed;
    double bullet_delay;
    int left_right;
    double change_time;
    double time;
    int shape_pntr;
    vec3 color;
    vector<vec2>raw_shape;
    vector<vec2>shape;
};
extern vector<eship_data> enemy;
extern vector<vec2>enemy_shapes[10];
extern int enemy_shapes_pntr[10];
extern vec3 enemy_colors[10];

struct rock_data
{
    bool status;
    int8_t stage;
    int8_t type;
    vec3 color;
    float xpos;
    float ypos;
    float xdir;
    float ydir;
    float radius;
    float mass;
    float angle;
    float angle_step;
    int hits;
    int score;
    vec2i pntr;       // Holds pointer to outline rock shape
    vec2i s_pntr;     // Holds pointer to solid rock shape
};
extern vector<rock_data> rocks;
extern vector<int8_t> rstarts;
extern vector<vec2i> rock_table;
extern vector<vec2i> s_rock_table;
extern vector<vec2>rck_data;      // Rock shape data
extern int Custom_rock_shapes_pntr;

struct rock_explode_struct
{
    bool status;
    float time;
    float xpos;
    float ypos;
    float xdir;
    float ydir;
    vec3 color;
};
extern vector<rock_explode_struct> rock_explode;
extern int explosion_cnt;

struct mini_explode_struct
{
    bool status;
    float time;
    float xpos;
    float ypos;
    float xdir;
    float ydir;
    vec3 color;
};
extern vector<mini_explode_struct> mini_explode;

struct impact_struct
{
    bool status;
    float time;
    float xpos;
    float ypos;
    float xrock;
    float yrock;
    float xdir;
    float ydir;
    int rock_pntr;
    vec3 color;
};
extern vector<impact_struct> impact;
extern int impact_cnt;

struct drexplode_struct
{
    bool status;
    float time;
    float xdrone;
    float ydrone;
    float xdir;
    float ydir;
    int rock_pntr;
    vec3 color;
};
extern vector<drexplode_struct> drexplode;

extern vector<vector<vector<vec2i>>>rgrid;

struct circle_collide_struct
{
    float dist;
    float b1_xpos;
    float b1_ypos;
    float b1_xdir;
    float b1_ydir;
    float b1_radius;
    float b1_mass;
    float b2_xpos;
    float b2_ypos;
    float b2_xdir;
    float b2_ydir;
    float b2_radius;
    float b2_mass;
};

struct ship_explode_struct
{
    bool status;
    vec2i object;
    float time;
    float xpos;
    float ypos;
    float xdir;
    float ydir;
    vec2i code;
    vec3 color;
};
extern vector<ship_explode_struct> ship_explode;

struct player_scores
{
    string name;
    int points;
};
extern vector<player_scores> p_scores;

extern FILE* Debug;
extern FILE* Debug2;

extern void clear_errors();
extern int log_errors();
extern bool Get_all_shaders();

string readFile(const char *filePath);
bool Initialize();
void Create_camera(float heading, int width, int height);


void GetFrustrumPlanes (mat4d view, mat4d project);
void Application(GLFWwindow* window);
void _update_fps_counter (GLFWwindow* window);
void check_keyboard (GLFWwindow* window, double elapsed_seconds);
void glfw_keyboard(GLFWwindow* window, int key, int scancode, int action, int mods);
void glfw_mousebuttonfun (GLFWwindow* window, int button, int action, int mods);
void glfw_cursorposfun (GLFWwindow* window, double xpos, double ypos);
void glfw_scrollfun (GLFWwindow* window, double xoffset, double yoffset);

extern string Add_commas_float(string &snum, int fraction);

/// Random generator
extern mt19937_64 rndrock;

/// Text globals
extern vector<float>text_data;

/// Globals
extern double elapsed_time;
extern float x_ratio;
extern float y_ratio;
extern vec2 rock_seed;                  // for GPU randomly generated rocks
extern int total_rocks;
extern float row_size;
extern float text_scale;
extern float col_size;
extern int game_score;
extern int game_level;
extern bool new_level_flag;
extern double new_level_delay;
extern bool shield_flag[17];
extern double shield_time[17];
extern bool spawn_shield_flag[17];
extern int enemy_ships;
extern double enemy_time;
extern bool demo;
extern bool game_over;
extern bool controls;
extern bool miscell;
extern float bullet_speed;
extern bool thrust_flag;
extern int num_enemy;
extern int potential_rocks;
extern int potential_max;
extern bool cpu_load_flag;
extern double bomb_timer;
extern bool siren_flag;
extern bool siren2_flag;
extern int new_high_score;
extern int num_players;
extern bool score_flag;
extern double cpu_load;
extern bool Rock_buffers_changed;
extern vector<vec2i>player_highscore_list;
extern bool joysticks;
extern int custom_rock_count;
extern vector<vec2i>phrase_pntr;
extern int cluster_index;
extern double Rock_spawn_timer;

extern int k_rotate_left;
extern int k_rotate_right;
extern int k_thrust;
extern int k_fire;
extern int k_bomb;
extern int k_shield;
extern int k_num_players;
extern int settings_row;
extern int misc_row;
extern int resolution;
extern float reso_factor;
extern float pre_reso_factor;
extern string reso_text;
extern float time_sum;

extern float Xmax;
extern float Ymax;
extern float Xgrid;
extern float Ygrid;
extern int Xgrid_size;
extern int Ygrid_size;
extern float pre_Xmax;
extern float pre_Ymax;

extern int k_first_initial;
extern int k_middle_initial;
extern int k_last_initial;
extern int initial_pos;

// From settings file
extern int Starting_level;
extern int Start_ships;
extern int Extra_ship_points;
extern float Ship_size, Shield_size;
extern int Ship_explode_debris;
extern int Rock_explode_debris;
extern int Mini_explode_debris;
extern int Impact_particles;
extern float Enemy_ship_big;
extern float Enemy_ship_small;
extern float Rock_speed_factor;
extern float Ship_speed_factor;
extern float Max_ship_speed;
extern float Bullet_speed_factor;
extern float Bomb_speed_factor;
extern float Ebullet_speed_factor;
extern float Min_enemy_speed;
extern float Max_enemy_speed;
extern float Enemy_speed_factor;
extern float Ebullet_delay;
extern double Bomb_delay;
extern double Shield_life;
extern float Shield_charge_factor;
extern float Ship_acceleration_factor;
extern float Ship_rotate_speed;
extern double Ship_decay_factor;
extern bool Solid_rock;
extern float Sound_volume;
extern int Demo_ships;
extern float Demo_bullet_delay;
extern float Demo_bulletspeed_factor;
extern int Player_hit_player;
extern float Line_width;
extern bool Rock_instancing;
extern float Rock_rotation_factor;
extern int Vsync;
extern int Monitor_resolution;
extern int Max_rocks;
extern int Big_rock_shapes;
extern int Medium_rock_shapes;
extern int Small_rock_shapes;
extern bool Custom_rocks_flag;
extern float Bullet_length;
extern float Drone_size;
extern int Max_rock_explode;
extern int Max_mini_explode;
extern int Max_ship_explode;
extern int Max_impact;
extern int Max_bullets;
extern int Max_ebullets;
extern double Rock_spawn_delay;

extern vec3 Common_color;
extern vec3 Fast_color;
extern vec3 Brittle_color;
extern vec3 Hard_color;

extern int Points_standard_big;
extern int Points_standard_medium;
extern int Points_standard_small;
extern int Points_fast_big;
extern int Points_fast_medium;
extern int Points_fast_small;
extern int Points_brittle_big;
extern int Points_brittle_small;
extern int Points_hard_big;
extern int Points_hard_medium;
extern int Points_hard_small;
extern int Points_drone;
extern int Points_enemyship;

extern int Hits_standard_big;
extern int Hits_standard_medium;
extern int Hits_standard_small;
extern int Hits_fast_big;
extern int Hits_fast_medium;
extern int Hits_fast_small;
extern int Hits_brittle_big;
extern int Hits_brittle_small;
extern int Hits_hard_big;
extern int Hits_hard_medium;
extern int Hits_hard_small;

// Joysticks, gamepads
extern vec2i joystick[16];
extern string joystick_name[16];
extern int  player_menu;
extern GLFWgamepadstate Gstate;

/// Sound globals
extern ALuint bomb_sourceid;
extern ALuint rk_explode_sourceid[MAX_BR_EXPLODE];
extern ALuint rk_explode2_sourceid[MAX_MR_EXPLODE];
extern ALuint rk_explode3_sourceid[MAX_SMR_EXPLODE];
extern ALuint ship_explode_sourceid[MAX_SE_EXPLODE];
extern ALuint drexplode_sourceid[MAX_DR_EXPLODE];
extern ALuint enemy_explode_sourceid;
extern ALuint ship_bullet_sourceid[MAX_FIRE_SND];
extern ALuint enemy_bullet_sourceid;
extern ALuint rock_impact_sourceid;
extern ALuint ship_bounce_sourceid;
extern ALuint enemy_siren_sourceid;
extern ALuint enemy_siren2_sourceid;
extern ALuint thrust_sourceid[17];

extern int rock1_pntr;
extern int rock2_pntr;
extern int rock3_pntr;
extern bool sound_thrust_flag[17];
extern double hiscore_timer;
extern int ship_snd_pntr;
extern int ship_fire_pntr;
extern int drexplode_pntr;

/// Functions
extern bool Get_settings();
extern void Create_rocks(int amount);
void Start_level(int level);
extern void Save_scores();
extern void Draw_score();
extern void Set_textsize(float rows, float cols);
extern float Draw_textrow(char text_line[], float column, float y, float z, vec4 color);
extern void Demo_text();
extern void Controls_text(float row);
extern void Draw_credits(float row);
extern void Draw_scores_text(float row);
extern void Draw_ranks(float row);
extern void Game_over_text();
extern void Enter_player_name(float row);
extern void Check_for_highscores();
extern void Sort_high_score(vector<player_scores> &scores);
extern void Fire_bullet(int sh);
extern void Start_new_game();
extern bool No_same_keys(int key);
extern bool Valid_key(int key);
extern void Get_high_score(int key);
extern void Control_selection();
extern void Computer_controled(int sh);
extern void Joystick_settings();
extern void Performance_text(float frames);
extern void Draw_everything();
extern void Update_everything();
extern void Check_for_collisions();
extern void Fire_enemy_bullet(int index);
extern void Start_ship_explosion(int ship, int ship_index, int object, int object_index, int xpos, int ypos);
extern void Bullet_hit_rock(int b_index, int r_index, circle_line *cl);
extern void Start_impact(int object, int r_index, circle_line *cl);
extern void Bullet_hit_enemy(int b_index, int e_index);
extern void Ebullet_hit_rock(int b_index, int r_index, circle_line *cl);
extern void Bbullet_hit_rock(int b_index, int r_index, circle_line *cl);
extern void Bullet_hit_enemy(int b_index, int e_index);
extern void Bbullet_hit_enemy(int b_index, int e_index);
extern void Add_cluster();
extern void Start_drexplode(int index);

#endif // MAIN_H_INCLUDED
