#include "initialize.h"

GLenum error;
mt19937_64 rndrock;

ascii ascii_table[256];
ascii letter_table[256];
vector<float>text_data;
vector<vec2i>phrase_pntr;

vector<player_scores> p_scores;
ship_data pship[17];
bool pship_status;         // Master status flag
vector<ship_bullets> bullets;
bool bullet_status;
int last_bullet;
float bullet_life;
int max_players;
int selected_player;

vector<enemy_bullets> ebullets;
bool ebullet_status;
int last_ebullet;
float ebullet_life;
float pre_ebullet_life;

ship_bullets bbullets[MAX_BBULLETS];
bool bomb_flag;
int last_bbullet;
vec2i joystick[16];
string joystick_name[16];
int player_menu;
GLFWgamepadstate Gstate;
bool missing_flag;
bool error_flag;

vector<drone_ship>drones;
vector<drone_cluster>cluster;

int Custom_rock_shapes_pntr;
vector<rock_data> rocks;
vector<int8_t> rstarts;
vector<vec2i> rock_table;                     // Pointers to outlined rock shapes: v[0] is pointer, v[1] vertex count
vector<vec2i> s_rock_table;                   // Same but for rendering solid rocks
vector<vec2>rck_data;                         // Rock shape data (used for collision detection)
vector<vector<vector<vec2i>>>rgrid;
vector<rock_explode_struct> rock_explode;
vector<ship_explode_struct> ship_explode;
vector<mini_explode_struct> mini_explode;
vector<eship_data> enemy;
vector<impact_struct>impact;
vector<drexplode_struct> drexplode;
vector<level_data> levels;
vector<vec2>enemy_shapes[10];
int enemy_shapes_pntr[10];
vec3 enemy_colors[10];

int g_win_width;
int g_win_height;
int g_fb_width;
int g_fb_height;
float x_ratio;
float y_ratio;
vec2 rock_seed;
int total_rocks;
int explosion_cnt;
int game_score;
int game_level;
bool shield_flag[17];
double shield_time[17];
bool spawn_shield_flag[17];
int enemy_ships;
double enemy_time;
bool demo;
bool controls;
bool miscell;
bool game_over;
float bullet_speed;
int impact_cnt;
bool thrust_flag;
int num_enemy;
int potential_rocks;
int potential_max;
double bomb_timer;
bool siren_flag;
bool siren2_flag;
int new_high_score;
int num_players;
bool score_flag;
float time_sum;
bool Rock_buffers_changed;
int cluster_index;
double Rock_spawn_timer;

int k_rotate_left;
int k_rotate_right;
int k_thrust;
int k_fire;
int k_bomb;
int k_shield;
int k_num_players;
int settings_row;
int misc_row;
int resolution;
float reso_factor;
float pre_reso_factor;
string reso_text;
bool cpu_load_flag;
double hiscore_timer;
bool joysticks;
int custom_rock_count;

float Xmax;
float Ymax;
float Xgrid;
float Ygrid;
int Xgrid_size;
int Ygrid_size;
float pre_Xmax;
float pre_Ymax;

// Global settings
int Starting_level;
int Start_ships;
int Extra_ship_points;
float Ship_size, Shield_size;
int Ship_explode_debris;
int Rock_explode_debris;
int Mini_explode_debris;
int Impact_particles;
float Enemy_ship_big;
float Enemy_ship_small;
float Rock_speed_factor;
float Ship_speed_factor;
float Max_ship_speed;
float Bullet_speed_factor;
float Bomb_speed_factor;
float Ebullet_speed_factor;
float Min_enemy_speed;
float Max_enemy_speed;
float Enemy_speed_factor;
float Ebullet_delay;
double Bomb_delay;
double Shield_life;
float Shield_charge_factor;
float Ship_acceleration_factor;
float Ship_rotate_speed;
double Ship_decay_factor;
bool Solid_rock;
float Sound_volume;
int Demo_ships;
float Demo_bulletspeed_factor;
float Demo_bullet_delay;
int Player_hit_player;
float Line_width;
bool Rock_instancing;
float Rock_rotation_factor;
int Vsync;
int Monitor_resolution;
int Max_rocks;
int Big_rock_shapes;
int Medium_rock_shapes;
int Small_rock_shapes;
bool Custom_rocks_flag;
float Bullet_length;
float Drone_size;
int Max_rock_explode;
int Max_ship_explode;
int Max_mini_explode;
int Max_impact;
int Max_bullets;
int Max_ebullets;
double Rock_spawn_delay;

vec3 Common_color;
vec3 Fast_color;
vec3 Brittle_color;
vec3 Hard_color;

int Points_standard_big;
int Points_standard_medium;
int Points_standard_small;
int Points_fast_big;
int Points_fast_medium;
int Points_fast_small;
int Points_brittle_big;
int Points_brittle_small;
int Points_hard_big;
int Points_hard_medium;
int Points_hard_small;
int Points_drone;
int Points_enemyship;

int Hits_standard_big;
int Hits_standard_medium;
int Hits_standard_small;
int Hits_fast_big;
int Hits_fast_medium;
int Hits_fast_small;
int Hits_brittle_big;
int Hits_brittle_small;
int Hits_hard_big;
int Hits_hard_medium;
int Hits_hard_small;

/// Sound declarations
ALuint bomb_sourceid;
ALuint rk_explode_sourceid[MAX_BR_EXPLODE];
ALuint rk_explode2_sourceid[MAX_MR_EXPLODE];
ALuint rk_explode3_sourceid[MAX_SMR_EXPLODE];
ALuint ship_explode_sourceid[MAX_SE_EXPLODE];
ALuint drexplode_sourceid[MAX_DR_EXPLODE];
ALuint enemy_explode_sourceid;
ALuint ship_bullet_sourceid[MAX_FIRE_SND];
ALuint enemy_bullet_sourceid;
ALuint rock_impact_sourceid;
ALuint ship_bounce_sourceid;
ALuint enemy_siren_sourceid;
ALuint enemy_siren2_sourceid;
ALuint thrust_sourceid[17];

int rock1_pntr;
int rock2_pntr;
int rock3_pntr;
bool sound_thrust_flag[17];
int ship_snd_pntr;
int ship_fire_pntr;
int drexplode_pntr;



bool Initialize()
{
    demo = true;
    controls = false;
    miscell = false;
    explosion_cnt = 0;
    impact_cnt = 0;
    game_score = 0;
    game_level = 1;
    new_level_flag = false;
    enemy_ships = 0;
    ebullet_status = false;
    last_ebullet = 0;
    bullet_status = false;
    last_bullet = 0;
    last_bbullet = 0;
    bomb_flag = false;
    game_over = false;
    k_rotate_left = GLFW_KEY_A;
    k_rotate_right = GLFW_KEY_S;
    k_thrust = GLFW_KEY_K;
    k_fire = GLFW_KEY_L;
    k_shield = GLFW_KEY_SPACE;
    k_bomb = GLFW_KEY_B;
    settings_row = 0;

    cpu_load_flag = false;
    bomb_timer = 0;
    siren_flag = false;
    siren2_flag = false;
    rock1_pntr = 0;
    rock2_pntr = 0;
    rock3_pntr = 0;
    ship_snd_pntr = 0;
    ship_fire_pntr = 0;
    new_high_score = 0;
    hiscore_timer = HI_SCORE_ON;              // Ten seconds
    num_players = 1;
    k_num_players = 1;
    pship_status = false;
    player_menu = 0;
    score_flag = false;
    Rock_buffers_changed = false;

    Set_starting_monitor();
    Check_for_joysticks();

    Debug=fopen(DEBUG_FILE, "a");            // For debugging purposes
    if (!Get_characters()) return false;
    if (!Get_scores()) return false;
    Set_random_generator();
    if (!Create_playership()) return false;
    if (!Get_level_data()) return false;
    Initialize_audio();
    Create_rock_shapes();

    rock_explode.clear();
    mini_explode.clear();

    glUseProgram(sh_rexplode.program);
    glBindVertexArray(sh_rexplode.vao);
    sh_rexplode.direction = new vec2[Max_rock_explode * Rock_explode_debris];
    glBindBuffer(GL_ARRAY_BUFFER, sh_rexplode.direction_vbo);
    glBufferData(GL_ARRAY_BUFFER,  Max_rock_explode*Rock_explode_debris * sizeof(vec2), sh_rexplode.direction, GL_DYNAMIC_DRAW);
    glBindVertexArray(0);

    fprintf(stderr,"\nInitialized successfully\n");
    fprintf(Debug,"\nInitialized successfully\n");
    fclose(Debug);

    Start_level(game_level);
    return true;
}

void Set_starting_monitor()
{
    if (Monitor_resolution == 0)
    {
        resolution = 0;
        reso_factor = 1.0;
        pre_reso_factor = reso_factor;
        reso_text = "1080p";
        Xmax = 1920;
        Ymax = 1080;
    }
    else if (Monitor_resolution == 1)
    {
        resolution = 1;
        reso_factor = 2.0;
        pre_reso_factor = reso_factor;
        reso_text = "1440p";
        Xmax = 2560;
        Ymax = 1440;
    }
    else if (Monitor_resolution == 2)
    {
        resolution = 2;
        reso_factor = 8.3/2.1;
        pre_reso_factor = reso_factor;
        reso_text = "2160p";
        Xmax = 3840;
        Ymax = 2160;
    }
    else if (Monitor_resolution == 3)
    {
        resolution = 3;
        reso_factor = 33.2/2.1;
        pre_reso_factor = reso_factor;
        reso_text = "4320p";
        Xmax = 7680;
        Ymax = 4320;
    }
    else
    {
        resolution = 4;
        reso_factor = 132.7 /2.1;
        pre_reso_factor = reso_factor;
        reso_text = "8640p";
        Xmax = 15360;
        Ymax = 8640;
    }

    Xgrid = (Xmax+100) / FGRID_SIZE;      // Collision grid
    Ygrid = (Ymax+100) / FGRID_SIZE;
    pre_Xmax = Xmax;
    pre_Ymax = Ymax;
    bullet_life = pre_Xmax*0.8;
    ebullet_life = pre_Xmax*1.0;
    pre_ebullet_life = ebullet_life;
    x_ratio = (float) g_win_width / Xmax;
    y_ratio = (float) g_win_height / Ymax;
}

bool Initialize_audio()
{
    ALCenum error;
    ALCdevice *device;
    device = alcOpenDevice(NULL);
    if (!device)
    {
        fprintf(stderr, "\nUnable to start audio device\n");
        fprintf(Debug, "\nUnable to start audio device\n");
        return false;
    }
    else
    {
        fprintf(stderr, "\nOpenAL started\n");
        fprintf(Debug, "\nOpenAL started\n");
    }

    ALboolean enumeration;
    enumeration = alcIsExtensionPresent(NULL, "ALC_ENUMERATION_EXT");
    if (enumeration == AL_FALSE)
    {
        fprintf(stderr,"Enumeration not supported\n");
        fprintf(Debug,"Enumeration not supported\n");
    }
    else
    {
        fprintf(stderr,"Enumeration supported\n");
        fprintf(Debug,"Enumeration supported\n");
    }

    list_audio_devices(alcGetString(NULL, ALC_DEVICE_SPECIFIER));

    ALCcontext *context;
    context = alcCreateContext(device, NULL);
    if (!alcMakeContextCurrent(context))
    {
        fprintf(stderr,"Failed to make audio context current\n");
        fprintf(Debug,"Failed to make audio context current\n");
        return false;
    }
    else
    {
        fprintf(stderr,"audio context current\n");
        fprintf(Debug,"audio context current\n");
    }

    int dsize;
    short int *data;
    AudioFile<float> rifle;
    ALuint format;

    if (rifle.load("Sound/Sniper_Rifle.wav"))
    {
        data = Convert_to_sound_data(rifle, format, dsize);
        ALuint buffer;
        alGetError();  //Clear error
        alGenBuffers (1, &buffer);
        alBufferData(buffer, format, data, sizeof(short)*dsize, rifle.getSampleRate());
        alGenSources(1, &bomb_sourceid);
        alSourcei(bomb_sourceid, AL_BUFFER, buffer);
    }
    else
    {
        fprintf(Debug," Error! Can't load \"Sound/Sniper_Rifle.wav\"\n");
    }

    AudioFile<float> explo;
    if (explo.load("Sound/Bigrock_explosion.wav"))
    {
        data = Convert_to_sound_data(explo, format, dsize);
        ALuint buffer2;
        alGetError();  //Clear error
        alGenBuffers (1, &buffer2);
        alBufferData(buffer2, format, data, sizeof(short)*dsize, explo.getSampleRate());
        alGenSources(MAX_BR_EXPLODE, &rk_explode_sourceid[0]);
        for (int lp=0; lp<MAX_BR_EXPLODE; lp++)
        {
            alSourcei(rk_explode_sourceid[lp], AL_BUFFER, buffer2);
        }
    }
    else
    {
        fprintf(Debug," Error! Can't load \"Sound/Bigrock_explosion.wav\"\n");
    }

    AudioFile<float> explo2;
    if (explo2.load("Sound/Medrock_explosion.wav"))
    {
        data = Convert_to_sound_data(explo2, format, dsize);
        ALuint buffer4;
        alGetError();  //Clear error
        alGenBuffers (1, &buffer4);
        alBufferData(buffer4, format, data, sizeof(short)*dsize, explo2.getSampleRate());
        alGenSources(MAX_MR_EXPLODE, &rk_explode2_sourceid[0]);
        for (int lp=0; lp<MAX_MR_EXPLODE; lp++)
        {
            alSourcei(rk_explode2_sourceid[lp], AL_BUFFER, buffer4);
        }
    }
    else
    {
        fprintf(Debug," Error! Can't load \"Sound/Medrock_explosion.wav\"\n");
    }

    AudioFile<float> explo3;
    if (explo3.load("Sound/Smallrock_explosion.wav"))
    {
        data = Convert_to_sound_data(explo3, format, dsize);
        ALuint buffer5;
        alGetError();  //Clear error
        alGenBuffers (1, &buffer5);
        alBufferData(buffer5, format, data, sizeof(short)*dsize, explo3.getSampleRate());
        alGenSources(MAX_SMR_EXPLODE, &rk_explode3_sourceid[0]);
        for (int lp=0; lp<MAX_SMR_EXPLODE; lp++)
        {
            alSourcei(rk_explode3_sourceid[lp], AL_BUFFER, buffer5);
        }
    }
    else
    {
        fprintf(Debug," Error! Can't load \"Sound/Smallrock_explosion.wav\"\n");
    }

    AudioFile<float> ship_explo;
    if (ship_explo.load("Sound/Ship_explode.wav"))
    {
        data = Convert_to_sound_data(ship_explo, format, dsize);
        ALuint buffer3;
        alGetError();  //Clear error
        alGenBuffers (1, &buffer3);
        alBufferData(buffer3, format, data, sizeof(short)*dsize, ship_explo.getSampleRate());
        alGenSources(10, &ship_explode_sourceid[0]);
        for (int lp=0; lp<MAX_SE_EXPLODE; lp++)
        {
            alSourcei(ship_explode_sourceid[lp], AL_BUFFER, buffer3);
        }
    }
    else
    {
        fprintf(Debug," Error! Can't load \"Sound/Ship_explode.wav\"\n");
    }

    AudioFile<float> E_explo;
    if (E_explo.load("Sound/Enemy_explode.wav"))
    {
        data = Convert_to_sound_data(E_explo, format, dsize);
        ALuint buffer10;
        alGetError();  //Clear error
        alGenBuffers (1, &buffer10);
        alBufferData(buffer10, format, data, sizeof(short)*dsize, E_explo.getSampleRate());
        alGenSources(1, &enemy_explode_sourceid);
        alSourcei(enemy_explode_sourceid, AL_BUFFER, buffer10);
    }
    else
    {
        fprintf(Debug," Error! Can't load \"Sound/Enemy_explode.wav\"\n");
    }

    AudioFile<float> ship_shot;
    if (ship_shot.load("Sound/Ship_bullet.wav"))
    {
        data = Convert_to_sound_data(ship_shot, format, dsize);
        ALuint buffer6;
        alGetError();  //Clear error
        alGenBuffers (1, &buffer6);
        alBufferData(buffer6, format, data, sizeof(short)*dsize, ship_shot.getSampleRate());
        alGenSources(MAX_FIRE_SND, &ship_bullet_sourceid[0]);
        for (int lp=0; lp<MAX_FIRE_SND; lp++)
        {
            alSourcei(ship_bullet_sourceid[lp], AL_BUFFER, buffer6);
        }
    }
    else
    {
        fprintf(Debug," Error! Can't load \"Sound/Ship_bullet.wav\"\n");
    }

    AudioFile<float> E_shot;
    if (E_shot.load("Sound/Enemy_bullet.wav"))
    {
        data = Convert_to_sound_data(E_shot, format, dsize);
        ALuint buffer11;
        alGetError();  //Clear error
        alGenBuffers (1, &buffer11);
        alBufferData(buffer11, format, data, sizeof(short)*dsize, E_shot.getSampleRate());
        alGenSources(1, &enemy_bullet_sourceid);
        alSourcei(enemy_bullet_sourceid, AL_BUFFER, buffer11);
    }
    else
    {
        fprintf(Debug," Error! Can't load \"Sound/Enemy_bullet.wav\"\n");
    }

    AudioFile<float> impact;
    if (impact.load("Sound/Rockimpact.wav"))
    {
        data = Convert_to_sound_data(impact, format, dsize);
        ALuint buffer7;
        alGetError();  //Clear error
        alGenBuffers (1, &buffer7);
        alBufferData(buffer7, format, data, sizeof(short)*dsize, impact.getSampleRate());
        alGenSources(1, &rock_impact_sourceid);
        alSourcei(rock_impact_sourceid, AL_BUFFER, buffer7);
    }
    else
    {
        fprintf(Debug," Error! Can't load \"Sound/Rockimpact.wav\"\n");
    }

    AudioFile<float> sh_bounce;
    if (sh_bounce.load("Sound/Shipbounce.wav"))
    {
        data = Convert_to_sound_data(sh_bounce, format, dsize);
        ALuint buffer8;
        alGetError();  //Clear error
        alGenBuffers (1, &buffer8);
        alBufferData(buffer8, format, data, sizeof(short)*dsize, sh_bounce.getSampleRate());
        alGenSources(1, &ship_bounce_sourceid);
        alSourcei(ship_bounce_sourceid, AL_BUFFER, buffer8);
    }
    else
    {
         fprintf(Debug," Error! Can't load \"Sound/Shipbounce.wav\"\n");
    }

    AudioFile<float> siren;
    if (siren.load("Sound/Enemy_siren.wav"))
    {
        data = Convert_to_sound_data(siren, format, dsize);
        ALuint buffer9;
        alGetError();  //Clear error
        alGenBuffers (1, &buffer9);
        alBufferData(buffer9, format, data, sizeof(short)*dsize, siren.getSampleRate());
        alGenSources(1, &enemy_siren_sourceid);
        alSourcei(enemy_siren_sourceid, AL_BUFFER, buffer9);
    }
    else
    {
        fprintf(Debug," Error! Can't load \"Sound\Enemy_siren.wav\"\n");
    }

    AudioFile<float> siren2;
    if (siren2.load("Sound/Enemy_siren2.wav"))
    {
        data = Convert_to_sound_data(siren2, format, dsize);
        ALuint buffer12;
        alGetError();  //Clear error
        alGenBuffers (1, &buffer12);
        alBufferData(buffer12, format, data, sizeof(short)*dsize, siren2.getSampleRate());
        alGenSources(1, &enemy_siren2_sourceid);
        alSourcei(enemy_siren2_sourceid, AL_BUFFER, buffer12);
    }
    else
    {
        fprintf(Debug," Error! Can't load \"Sound\Enemy_siren2.wav\"\n");
    }

    AudioFile<float> thrust;
    if (thrust.load("Sound/Thrust.wav"))
    {
        data = Convert_to_sound_data(thrust, format, dsize);
        ALuint buffer13;
        alGetError();  //Clear error
        alGenBuffers (1, &buffer13);
        alBufferData(buffer13, format, data, sizeof(short)*dsize, thrust.getSampleRate());
        alGenSources(17, &thrust_sourceid[0]);
        for (int lp=0; lp<17; lp++)
        {
            sound_thrust_flag[lp] = false;
            alSourcei(thrust_sourceid[lp], AL_BUFFER, buffer13);
        }
    }
    else
    {
        fprintf(Debug," Error! Can't load \"Sound\Thrust.wav\"\n");
    }

    drexplode_pntr = 0;
    AudioFile<float> dr_explode;
    if (dr_explode.load("Sound/Drone_explode.wav"))
    {
        data = Convert_to_sound_data(dr_explode, format, dsize);
        ALuint buffer14;
        alGetError();  //Clear error
        alGenBuffers (1, &buffer14);
        alBufferData(buffer14, format, data, sizeof(short)*dsize, dr_explode.getSampleRate());
        alGenSources(MAX_DR_EXPLODE, &drexplode_sourceid[0]);
        for (int lp=0; lp<MAX_DR_EXPLODE; lp++)
        {
            alSourcei(drexplode_sourceid[lp], AL_BUFFER, buffer14);
        }
    }
    else
    {
        fprintf(Debug," Error!  Can't load \"Sound\Drone_explode.wav\"\n");
    }

    return true;
}

void Show_sound_error(ALCenum error)
{
    if (error == AL_INVALID_NAME) fprintf(stdout, " Invalid Name\n");
    if (error == AL_INVALID_ENUM) fprintf(stdout, " Invalid ENUM\n");
    if (error == AL_INVALID_VALUE) fprintf(stdout, " Invalid Value\n");
    if (error == AL_INVALID_OPERATION) fprintf(stdout, " Invalid operation\n");
    if (error == AL_OUT_OF_MEMORY) fprintf(stdout, "Out of memory");
}

static void list_audio_devices(const ALCchar *devices)
{
        const ALCchar *device = devices, *next = devices + 1;
        size_t len = 0;

        fprintf(stdout, "Devices list:\n");
        fprintf(Debug,"Devices list:\n");
        fprintf(stdout, "----------\n");
        fprintf(Debug, "----------\n");
        while (device && *device != '\0' && next && *next != '\0') {
                fprintf(stdout, "%s\n", device);
                fprintf(Debug, "%s\n", device);
                len = strlen(device);
                device += (len + 1);
                next += (len + 2);
        }
        fprintf(stdout, "----------\n");
        fprintf(Debug, "----------\n");
}

short int *Convert_to_sound_data(AudioFile<float> &AF, ALuint &format, int &dsize)
{
    int channels = AF.getNumChannels();
    int samples = AF.getNumSamplesPerChannel();
    int bps = AF.getBitDepth();
    short int *data = new short int[channels*samples];

    int pnt = 0;
    for (int lp=0; lp<samples; lp++)
    {
        for (int lp2=0; lp2<channels; lp2++)
        {
            data[pnt] = static_cast<short> (AF.samples[lp2][lp]*32767.);
            pnt++;
        }
    }
    dsize = samples*channels;

    if (channels == 1)
    {
        if (bps == 8)
        {
            format = AL_FORMAT_MONO8;
        }
        else
        {
            format = AL_FORMAT_MONO16;
        }
    }
    else {
        if (bps == 8)
        {
            format = AL_FORMAT_STEREO8;
        }
        else {
            format = AL_FORMAT_STEREO16;
        }
    }
    return data;
}


double Find_number(string &readin, size_t pos, size_t *epos, bool *test)
{
    double number;
    string temp;
    size_t fpos = pos;
    while (readin.compare(fpos, 1, " ") == 0) fpos++;
    size_t fpos2 = readin.find_first_not_of("-0123456789.", fpos);
    temp.assign(readin, pos, (fpos2-pos));
    *epos = fpos2;
    *test = true;
    try
    {
        number = stod(temp);
    }
    catch (exception &e)
    {
        size_t sptest = fpos;
        while (readin.compare(sptest, 1, " ") !=0)
        {
            sptest++;
            if (sptest>=readin.length()) break;
        }

        temp.assign(readin, fpos, sptest-fpos);
        fprintf(stderr, "Invalid number \"%s\".\n", temp.c_str());
        error_flag = true;
        *test = false;
    }
    return number;
}

void Get_one_level(string &readin, level_data &lv, int index)
{
    bool code_test = false;
    bool test;
    levels.push_back(level_data());

    size_t epos;
    size_t pos = readin.find("Sr");               // Get number of standard rocks
    if (pos!=string::npos)
    {
        lv.num_rocks = Find_number(readin, pos+2, &epos, &test);
        code_test = true;
    }

    pos = readin.find("Fr");               // Get number of fast rocks
    if (pos!=string::npos)
    {
        lv.num_fast = Find_number(readin, pos+2, &epos, &test);
        code_test = true;
    }

    pos = readin.find("Br");               // Get number of brittle rocks
    if (pos!=string::npos)
    {
        lv.num_brittle = Find_number(readin, pos+2, &epos, &test);
        code_test = true;
    }

    pos = readin.find("Hr");               // Get number of hard rocks
    if (pos!=string::npos)
    {
        lv.num_hard = Find_number(readin, pos+2, &epos, &test);
        code_test = true;
    }

    pos = readin.find("En");               // Get number of Enemy ships
    if (pos!=string::npos)
    {
        lv.num_enemy = Find_number(readin, pos+2, &epos, &test);
        code_test = true;
    }

    pos = readin.find("Bb");               // Get number of Enemy ships
    if (pos!=string::npos)
    {
        lv.num_bullets = Find_number(readin, pos+2, &epos, &test);
        code_test = true;
    }

    pos = readin.find("Er");
    if (pos!=string::npos)
    {
        lv.range.v[0] = Find_number(readin, pos+2, &epos, &test);
        lv.range.v[1] = Find_number(readin, epos, &epos, &test);
        code_test = true;
    }

    pos = readin.find("Dr");               // Get number of drone clusters
    if (pos!=string::npos)
    {
        lv.num_clusters = Find_number(readin, pos+2, &epos, &test);
        code_test = true;
    }

    pos = readin.find("Ds");               // Get base drone speed
    if (pos!=string::npos)
    {
        lv.dr_speed = Find_number(readin, pos+2, &epos, &test);
        code_test = true;
    }

    pos = readin.find("Da");               // Drone acceleration range
    if (pos!=string::npos)
    {
        lv.dr_accel.v[0] = Find_number(readin, pos+2, &epos, &test);
        lv.dr_accel.v[1] = Find_number(readin, epos, &epos, &test);
        code_test = true;
    }
    if (!code_test)
    {
        fprintf(stderr, "No valid codes found level %i!\n", lv.level+1);
        missing_flag = true;
    }
}

bool Get_level_data()
{
    size_t pos;
    size_t epos;
    missing_flag = false;
    bool test;

    level_data lv;
    lv.num_rocks = 0;
    lv.num_fast = 0;
    lv.num_brittle = 0;
    lv.num_hard = 0;
    lv.num_enemy = 0;
    lv.num_bullets = 0;
    lv.num_clusters = 0;
    lv.range = vec2i(0, 0);
    lv.dr_speed = 0.0;
    lv.dr_accel = vec2(0.0, 0.0);

    vector<int>order;
    order.clear();

    vector<level_data>levels2;
    levels2.clear();
    vector<string>readin;
    readin.clear();

    string temp;
    int last_level=0;
    bool end_file = false;

    ifstream fileStream("levels.txt", std::ios::in);
    if(!fileStream.is_open())
    {
        fprintf(stderr, "ERROR: Could not open file \"levels.txt\"\n");
        fprintf(Debug, "ERROR: Could not open file \"levels.txt\"\n");
        return false;
    }

    while (!fileStream.eof())
    {
        temp.clear();
        getline(fileStream, temp);
        pos = temp.find("//");
        if (pos==string::npos)
        {
            readin.push_back(temp);
        }
    }

    for (int lp=0; lp<readin.size(); lp++)
    {
        bool error_flag = false;
        pos = 0;
        int index = Find_number(readin[lp], pos, &epos, &test)-1;
        if (!test)
        {
            error_flag = true;
        }
        else
        {
            if (index<0)
            {
                fprintf(stderr, "Negative or zero level number not allowed.\n");
                fprintf(Debug, "Negative or zero level number not allowed.\n");
                error_flag = true;
            }
            lv.level = index;

        }

        Get_one_level(readin[lp], lv, lp);
        if (error_flag)
        {
            fprintf(stderr,"%s\n", readin[lp].c_str());
            missing_flag = true;
        }
        levels2.push_back(lv);
        order.push_back(lv.level);
    }

    if (levels2[0].level != 0)
    {
        fprintf(stderr, "First level needs to be level 1.\n");
        fprintf(Debug, "First level needs to be level 1.\n");
        missing_flag = true;
    }
    if (levels2.size()<2)
    {
        fprintf(stderr, "Need more than one level.\n");
        fprintf(Debug, "Need more than one level.\n");
        missing_flag = true;
    }

    if (!missing_flag)
    {
        for (int lp=1; lp<order.size(); lp++)
        {
            if (order[lp]<=order[lp-1])
            {
                fprintf(stderr,"Levels are out of order or duplicated!\n");
                fprintf(Debug, "Levels are out of order or duplicated!\n");
                missing_flag = true;
            }
        }
    }

    if (!missing_flag)
    {
        int lp;
        levels.clear();
        for (lp=0; lp<levels2.size()-1; lp++)
        {
            level_data temp = levels2[lp];
            levels.push_back(levels2[lp]);

            int first = levels2[lp].level;
            int last = levels2[lp+1].level;
            if (last-first > 1)
            {
                for (int lp2=0; lp2<(last-first)-1; lp2++)
                {
                    levels.push_back(levels2[lp]);
                }
            }
        }
        levels.push_back(levels2[lp]);
    }
    int lend = levels.size()-1;
    for (int lp=0; lp<5; lp++)
    {
        levels.push_back(levels[lend]);
    }
    /*
    Debug=fopen(DEBUG_FILE,"a");
    fprintf(Debug,"size %i\n", (int) levels.size());
    for (int lp=0; lp<levels.size(); lp++)
    {
        fprintf(Debug,"%i) %i %i %i %i %i  %i %i\n",lp+1, levels[lp].num_rocks, levels[lp].num_fast, levels[lp].num_brittle, levels[lp].num_hard, levels[lp].num_bullets,
                levels[lp].range.v[0], levels[lp].range.v[1]);
    }
    fclose(Debug);
    */
    if (missing_flag)
    {
        fprintf(stderr, "\nPlease fix the \"levels.txt\" file.\n");
        fprintf(Debug, "\nPlease fix the \"levels.txt\" file.\n");
        return false;
    }
    return true;

}

int Get_next_seed()
{
    static int seconds = time(0);
    seconds-= 1000000;
    return seconds;
}

void Set_random_generator()
{
    array<int, 624> seed_data;
    random_device r;
    generate_n(seed_data.data(), seed_data.size(), Get_next_seed);
    seed_seq sd(begin(seed_data), end(seed_data));
    rndrock.seed(sd);
}

int txt_cnt;
int txt_len;

/// *********** Get characters **********
bool Get_characters()
{
    FILE *Chget;
    int num;
    float width;
    int lp, lp2;

    for(lp=0; lp<256; lp++)    // Clear character pointer tables
    {
        ascii_table[lp].num = 0;
        ascii_table[lp].start = 0;
        ascii_table[lp].width = 0;
        letter_table[lp].num = 0;
        letter_table[lp].start = 0;
        letter_table[lp].width = 0;
    }

    text_data.clear();
    txt_cnt = 0;

    Chget = fopen("Characters.txt", "r");
    if (!Chget)
    {
        fprintf(stderr,"ERROR: could not open \"Characters.txt\"\n");
        fprintf(Debug, "ERROR: could not open \"Characters.txt\"\n");
        return false;
    }
    fscanf(Chget,"%i %f", &num, &width);
    while (num != 9999)
    {
         letter_table[num].start = txt_cnt;
        Get_letter(Chget, &ascii_table[num].start, &ascii_table[num].num);
         letter_table[num].num = txt_len;
         letter_table[num].width = width;
        ascii_table[num].width = width;
        fscanf(Chget,"%i %f", &num, &width);
    }
    fclose(Chget);

    vector<float>phrase_data;
    phrase_data.clear();
    phrase_pntr.clear();

    // Read in phrases
    string line = "";
    line.reserve(100);
    ifstream fileStream("shapes/text.txt", std::ios::in);
    if (!fileStream.is_open())
    {
        fprintf(stderr, "\nError! Could not open \"shapes/text.txt\".\n");
        fprintf(Debug, "\nError! Could not open \"shapes/text.txt\".\n");
        return false;
    }
    phrase_pntr.push_back(vec2i());
    int pntr = 0;

    while (!fileStream.eof())
    {
        getline(fileStream, line);
        int sz = phrase_pntr.size()-1;
        phrase_pntr[sz].v[0] = pntr;
        float column = 0;
        int cnt = 0;
        for (lp=0; lp<line.size(); lp++)
        {
            int code = line.at(lp);
            if (code == 32)
            {
                column+= 0.5;
            }
            else
            {
                int start = ascii_table[code].start*2;
                int num = ascii_table[code].num*2;
                for (lp2=start; lp2<start+num; lp2+=2)
                {
                    phrase_data.push_back(text_data[lp2]+column);       // X coord
                    phrase_data.push_back(text_data[lp2+1]);            // Y coord
                    cnt++;
                    pntr++;
                }
                column+= (ascii_table[code].width + 0.15);
            }
        }

        phrase_pntr[sz].v[1] = cnt;
        phrase_pntr.push_back(vec2i());
    }
    fileStream.close();
    phrase_pntr.pop_back();

    int vsize = text_data.size() + phrase_data.size();
    sh_2Dtext.points = new float[vsize];
    memset(sh_2Dtext.points, 0, sizeof(float)*vsize);

    //Put character data into buffer
    for (lp = 0; lp<text_data.size(); lp++)
    {
        sh_2Dtext.points[lp] = text_data[lp];
    }
    sh_2Dtext.character_end = text_data.size();
    for (lp=0; lp<phrase_data.size(); lp++)
    {
        sh_2Dtext.points[sh_2Dtext.character_end + lp] = phrase_data[lp];
    }
    for (lp=0; lp<phrase_pntr.size(); lp++)
    {
        phrase_pntr[lp].v[0]+= (sh_2Dtext.character_end/2);
    }

    glUseProgram(sh_2Dtext.program);
    glBindVertexArray(sh_2Dtext.vao);
    sh_2Dtext.proj_mat = ortho(0.0f, g_win_width, 0.0f, g_win_height, -1.0, 1.0);
    glUniformMatrix4fv (sh_2Dtext.proj_mat_location, 1, GL_FALSE, sh_2Dtext.proj_mat.m);
    glBindBuffer(GL_ARRAY_BUFFER, sh_2Dtext.points_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vsize, sh_2Dtext.points, GL_STATIC_DRAW);
    glBindVertexArray(0);

    return true;
}

/// *********** (internal use) Read in a letter and put in temporary list *************
bool Get_letter(FILE *Chget, int *start, int *num)
{
    char str[20];
    float x, y, x2, y2;
    float r, d1, d2;
    float loop, step;
    string name;

    *start = text_data.size()/2;
    *num = 0;
    txt_len = 0;

    fscanf(Chget,"%s", str);
    name = str;
    while (name.compare("end") != 0)
    {
        if (name.compare("line") == 0)
        {
            fscanf(Chget,"%f %f %f %f", &x, &y, &x2, &y2);
            text_data.push_back(x);
            text_data.push_back(y);
            text_data.push_back(x2);
            text_data.push_back(y2);
            *num += 2;
            txt_len += 4;
            txt_cnt += 4;
        }
        if (name.compare("arc") == 0)
        {
            fscanf(Chget,"%f %f %f %f %f",&x, &y, &r, &d1, &d2);
            step = (d2 - d1) / 10.0f;
            for (loop = d1; loop < d2; loop += step)
            {
                text_data.push_back(x + sin(loop*TO_RADIANS) * r);
                text_data.push_back(y + cos(loop*TO_RADIANS) * r);
                text_data.push_back(x + sin((loop+step)*TO_RADIANS) * r);
                text_data.push_back(y + cos((loop+step)*TO_RADIANS) * r);
                *num += 2;
                txt_len +=4;
                txt_cnt +=4;
            }
        }
        fscanf(Chget,"%s", str);
        name = str;
    }
    return true;
}

int Search_integer(string &str, size_t pos, size_t *last)
{
    size_t num = str.find_first_of("-0123456789", pos);
    size_t num2 = str.find_first_not_of("0123456789", num);
    *last = num2+1;
    return stoi(str.substr(num, num2-num));
}
double Search_double(string &str, size_t pos, size_t *last)
{
    size_t num = str.find_first_of("-.0123456789", pos);
    size_t num2 = str.find_first_not_of(".0123456789", num);
    *last = num2+1;
    return stod(str.substr(num, num2-num));
}

double Search_double2(string &str, size_t pos, size_t *last)
{
    double real;

    while (str.compare(pos, 1, " ") == 0) pos++;      // Skip over leading spaces
    size_t num = pos;
    size_t num2 = pos+1;
    while (str.compare(num2, 1, " ") !=0) num2++;     // Find first space after encountering non space characters
    *last = num2;
    string number = str.substr(num, num2-num);        // Isolate non-space characters to its own string
    try
    {
        real = stod(number);                          // Convert string to a number
    }
    catch (exception &e)                              // Conversion triggered an exception, catch it.
    {
        fprintf(stderr, "Invalid number \"""%s\""".\n", number.c_str());   // Error message
        fprintf(Debug, "Invalid number \"""%s\""".\n", number.c_str());
        missing_flag = true;
    }
    return real;
}

void Missing(string setting)
{
    fprintf(stderr, "Missing \"""%s\""" setting!\n", setting.c_str());
    fprintf(Debug, "Missing \"""%s\""" setting!\n", setting.c_str());
    missing_flag = true;
}

void Missing_equal(string setting)
{
    fprintf(stderr, "Missing equal symbol for setting \"""%s\"""!\n", setting.c_str());
    fprintf(Debug, "Missing equal symbol for setting \"""%s\"""!\n", setting.c_str());
    missing_flag = true;
}

/// Find requested setting and its value
double Find_setting(string &content, string label)
{
    size_t pos;
    size_t last;
    size_t before;
    double number;

    pos = content.find(label);
    before = pos-1;

    if (pos!=string::npos && content.compare(before, 1, " ") == 0)
    {
        pos+= label.length();
        if (content.compare(pos, 1, " ") == 0 || content.compare(pos, 1, "=") == 0)
        {
            while (content.compare(pos, 1, " ") == 0) pos++;
            if (content.compare(pos, 1, "=") == 0)
            {
                pos++;
                number = Search_double2(content, pos, &last);
            }
            else Missing_equal(label);
        }
        else Missing(label);

    }
    else Missing(label);
    return number;
}

/// Find requested color setting and its RGB componets
vec3 Find_color(string &content, string label)
{
    size_t pos;
    size_t last;
    size_t before;
    vec3 col;

    pos = content.find(label);
    before = pos-1;

    if (pos!=string::npos && content.compare(before, 1, " ") == 0)
    {
        pos+= label.length();
        if (content.compare(pos, 1, " ") == 0 || content.compare(pos, 1, "=") == 0)
        {
            while (content.compare(pos, 1, " ") == 0) pos++;
            if (content.compare(pos, 1, "=") == 0)
            {
                pos++;
                col.v[0] = Search_double2(content, pos, &last);
                last++;
                col.v[1] = Search_double2(content, last, &last);
                last++;
                col.v[2] = Search_double2(content, last, &last);
            }
            else Missing_equal(label);
        }
        else Missing(label);

    }
    else Missing(label);
    return col;
}

/// *********** Get settings ************
bool Get_settings()
{
    size_t last;
    size_t pos;
    string content;

    Debug=fopen(DEBUG_FILE,"a");

    ifstream fileStream("Settings.ini", std::ios::in);
    if(!fileStream.is_open())
    {
        fprintf(stderr, "ERROR: Could not open file \"Settings.ini\"\n");

        fprintf(Debug, "ERROR: Could not open file \"Settings.ini\"\n");
        fclose(Debug);
        return false;
    }
    string line = "";
    while(!fileStream.eof()) {
        getline(fileStream, line);
        content.append(line + " ");
    }
    fileStream.close();

    missing_flag = false;

    Starting_level = Find_setting(content, "Starting_level");
    Start_ships = Find_setting(content, "Start_ships");
    Extra_ship_points = Find_setting(content, "Extra_ship_points");
    Ship_size = Find_setting(content, "Ship_size");
    Shield_size = Ship_size*1.5;
    Ship_explode_debris = Find_setting(content, "Ship_explode_debris");
    Rock_explode_debris = Find_setting(content, "Rock_explode_debris");
    Mini_explode_debris = Find_setting(content, "Mini_explode_debris");
    Impact_particles = Find_setting(content, "Impact_particles");
    Enemy_ship_big = Find_setting(content, "Enemy_ship_big");
    Enemy_ship_small = Find_setting(content, "Enemy_ship_small");
    Rock_speed_factor = Find_setting(content, "Rock_speed_factor");
    Ship_speed_factor = Find_setting(content, "Ship_speed_factor");
    Max_ship_speed = Find_setting(content, "Max_ship_speed");
    Bullet_speed_factor = Find_setting(content, "Bullet_speed_factor");
    Bomb_speed_factor = Find_setting(content, "Bomb_speed_factor");
    Ebullet_speed_factor = Find_setting(content, "Ebullet_speed_factor");
    Min_enemy_speed = Find_setting(content, "Min_enemy_speed");
    Max_enemy_speed = Find_setting(content, "Max_enemy_speed");
    Enemy_speed_factor = Find_setting(content, "Enemy_speed_factor");
    Ebullet_delay = Find_setting(content, "Ebullet_delay");
    Bomb_delay = Find_setting(content, "Bomb_delay");
    Shield_life = Find_setting(content, "Shield_life");
    Shield_charge_factor = Find_setting(content, "Shield_charge_factor");
    Ship_acceleration_factor = Find_setting(content, "Ship_acceleration_factor");
    Ship_rotate_speed = Find_setting(content, "Ship_rotate_speed");
    Ship_decay_factor = Find_setting(content, "Ship_decay_factor");
    Solid_rock = Find_setting(content, "Solid_rock");
    Rock_spawn_delay = Find_setting(content, "Rock_spawn_delay");

    Common_color = Find_color(content, "Common_color");
    Fast_color = Find_color(content, "Fast_color");
    Brittle_color = Find_color(content, "Brittle_color");
    Hard_color = Find_color(content, "Hard_color");

    Sound_volume = Find_setting(content, "Sound_volume");
    Demo_ships = Find_setting(content, "Demo_ships");
    Demo_bulletspeed_factor = Find_setting(content, "Bullet_speed_factor");
    Demo_bullet_delay = Find_setting(content, "Demo_bullet_delay");
    Player_hit_player = Find_setting(content, "Player_hit_player");
    Line_width = Find_setting(content, "Line_width");
    Rock_instancing = Find_setting(content, "Rock_instancing");
    Rock_rotation_factor = Find_setting(content, "Rock_rotation_factor");
    Vsync = Find_setting(content, "Vsync");
    Monitor_resolution = Find_setting(content, "Monitor_resolution");
    Max_rocks = Find_setting(content, "Max_rocks");
    Big_rock_shapes = Find_setting(content, "Big_rock_shapes");
    Medium_rock_shapes = Find_setting(content, "Medium_rock_shapes");
    Small_rock_shapes = Find_setting(content, "Small_rock_shapes");
    Custom_rocks_flag = Find_setting(content, "Custom_rocks_flag");
    Bullet_length = Find_setting(content, "Bullet_length");

    // Reward Points
    Points_standard_big = Find_setting(content, "Points_standard_big");
    Points_standard_medium = Find_setting(content, "Points_standard_medium");
    Points_standard_small = Find_setting(content, "Points_standard_small");
    Points_fast_big = Find_setting(content, "Points_fast_big");
    Points_fast_medium = Find_setting(content, "Points_fast_medium");
    Points_fast_small = Find_setting(content, "Points_fast_small");
    Points_brittle_big = Find_setting(content, "Points_brittle_big");
    Points_brittle_small = Find_setting(content, "Points_brittle_small");
    Points_hard_big = Find_setting(content, "Points_hard_big");
    Points_hard_medium = Find_setting(content, "Points_hard_medium");
    Points_hard_small = Find_setting(content, "Points_hard_small");
    Points_drone = Find_setting(content, "Points_drone");
    Points_enemyship = Find_setting(content, "Points_enemyship");

    Hits_standard_big = Find_setting(content, "Hits_standard_big");
    Hits_standard_medium = Find_setting(content, "Hits_standard_medium");
    Hits_standard_small = Find_setting(content, "Hits_standard_small");
    Hits_fast_big = Find_setting(content, "Hits_fast_big");
    Hits_fast_medium = Find_setting(content, "Hits_fast_medium");
    Hits_fast_small = Find_setting(content, "Hits_fast_small");
    Hits_brittle_big = Find_setting(content, "Hits_brittle_big");
    Hits_brittle_small = Find_setting(content, "Hits_brittle_small");
    Hits_hard_big = Find_setting(content, "Hits_hard_big");
    Hits_hard_medium = Find_setting(content, "Hits_hard_medium");
    Hits_hard_small = Find_setting(content, "Hits_hard_small");

    Drone_size = Find_setting(content, "Drone_size");
    Max_rock_explode = Find_setting(content, "Max_rock_explode");
    Max_ship_explode = Find_setting(content, "Max_ship_explode");
    Max_mini_explode = Find_setting(content, "Max_mini_explode");
    Max_impact = Find_setting(content, "Max_impact");
    Max_bullets = Find_setting(content, "Max_bullets");
    Max_ebullets = Find_setting(content, "Max_ebullets");

    if (missing_flag)
    {
        fprintf(stderr, "Please fix the \"Settings.ini\" file\n\n");
        fprintf(Debug, "Please fix the \"Settings.ini\" file\n\n");
        fclose(Debug);
        return false;
    }
    else
    {
        fprintf(stderr, "Settings loaded successfully\n\n");
        fprintf(Debug, "Settings loaded successfully\n\n");
        fclose(Debug);
    }
    return true;
}

bool Get_scores()
{
    char name[10];
    int points;
    player_scores p_temp;

    p_scores.clear();
    FILE *Score = fopen("Scores.txt", "r");
    if (Score == NULL)
    {
        fprintf(stderr, "\nWarning: \"Scores.txt\" not found\n");
        fprintf(Debug, "\nWarning: \"Scores.txt\" not found\n");
        return true;
    }

    fscanf(Score, "%s %i", &name, &points);
    while (!feof(Score))
    {
        p_temp.name = string(name);
        p_temp.points = points;
        p_scores.push_back(p_temp);
        fscanf(Score, "%s %i", &name, &points);
    }
    fclose(Score);
    return true;
}

void Save_scores()
{
    if (p_scores.size() > 15) p_scores.resize(15);         // Limit list of high scores to 15
    FILE *Score = fopen("Scores.txt", "w");
    for (int lp=0; lp<p_scores.size(); lp++)
    {
        fprintf(Score,"%s %i\n", p_scores[lp].name.c_str(), p_scores[lp].points);
    }
    fclose(Score);
}

float Search_float(string &str, size_t start, size_t *last)
{
    string temp;
    size_t fpos = str.find_first_of("-0123456789.", start);
    size_t fpos2 = str.find_first_not_of("-0123456789.", fpos+1);
    *last = fpos2;
    return stof(str.substr(fpos, fpos2-fpos));
}

int Place_shipdata(string &str, const string name, int sh, int pntr)
{
    vector<float>vertex;
    vertex.clear();
    size_t pos;
    float num;

    pos = str.find(name);
    pos+= name.length();
    if (pos!=string::npos)
    {
        do
        {
            num = Search_float(str, pos, &pos);
            vertex.push_back(num);
        }
        while (num<10.0);
        vertex.pop_back();
    }
    pship[sh].shape_pntr = pntr;                  // Set shape data for player ship
    pship[sh].shape.resize(vertex.size()/2);
    pship[sh].raw_shape.resize(vertex.size()/2);

    for (int lp=0; lp<vertex.size()/2; lp++)
    {
        pship[sh].raw_shape[lp] = vec2(vertex[lp*2], vertex[lp*2+1]);
        sh_pship.points[pntr] = vec2(vertex[lp*2], vertex[lp*2+1]);
        pntr++;
    }
    return pntr;
}

void Place_enemydata(string &str, const string name, int sh)
{
    vector<float>vertex;
    vertex.clear();
    size_t pos;
    float num;

    pos = str.find(name);
    if (pos!=string::npos)
    {
        do
        {
            num = Search_float(str, pos, &pos);
            vertex.push_back(num);
        }
        while (num<10.0);
        vertex.pop_back();
    }
    enemy_shapes[sh].clear();
    for (int lp=0; lp<vertex.size(); lp+=2)
    {
        enemy_shapes[sh].push_back(vec2(vertex[lp], vertex[lp+1]));
    }
}

vector<vec2> Get_rock_shapes()
{
    float num, num2;
    vector<vec2>vertex;
    vertex.clear();
    size_t pos;
    string line = "";
    string content;
    ifstream fileStream("shapes/rocks.txt", std::ios::in);
    if (!fileStream.is_open())
    {
        fprintf(stderr, "\nWarning!  Could not read file \"rocks.txt\".\n");
        fprintf(Debug, "\nWarning!  Could not read file \"rocks.txt\".\n");
        return vertex;
    }
    while (!fileStream.eof())
    {
        getline(fileStream, line);
        content.append(line + " ");
    }
    fileStream.close();

    pos = content.find("Start_data");
    if (pos!=std::string::npos)
    {
        do
        {
            num = Search_float(content, pos, &pos);
            num2 = Search_float(content, pos, &pos);
            vertex.push_back(vec2(num, num2));
        }
        while (num<9000.0);
    }
    else
    {
        fprintf(stderr, "\nCustom rock data not found!\n");
        fprintf(Debug, "\nCustom rock data not found!\n");
    }
    return vertex;
}

bool Create_playership()
{
    int lp;
    int shape_pntr = 0;
    size_t pos;
    float num;
    string temp;
    string content;
    vector<float>vertex;

    sh_pship.points = new vec2[1000];

    vertex.clear();
    ifstream fileStream("shapes/ships.txt", std::ios::in);
    if (!fileStream.is_open())
    {
        fprintf(stderr, "\nError!  Could not open file \"ships.txt\".\n");
        fprintf(Debug, "\nError!  Could not open file \"ships.txt\".\n");
        return false;
    }
    string line = "";
    while (!fileStream.eof())
    {
        getline(fileStream, line);
        content.append(line + " ");
    }
    fileStream.close();

    /// Get playership data
    shape_pntr = Place_shipdata(content, "Playership_one", 0, shape_pntr);
    shape_pntr = Place_shipdata(content, "Playership_two", 1, shape_pntr);
    shape_pntr = Place_shipdata(content, "Playership_three", 2, shape_pntr);
    shape_pntr = Place_shipdata(content, "Playership_four", 3, shape_pntr);
    shape_pntr = Place_shipdata(content, "Playership_five", 4, shape_pntr);
    shape_pntr = Place_shipdata(content, "Playership_six", 5, shape_pntr);
    shape_pntr = Place_shipdata(content, "Playership_seven", 6, shape_pntr);
    shape_pntr = Place_shipdata(content, "Playership_eight", 7, shape_pntr);
    shape_pntr = Place_shipdata(content, "Playership_nine", 8, shape_pntr);
    shape_pntr = Place_shipdata(content, "Playership_ten", 9, shape_pntr);
    shape_pntr = Place_shipdata(content, "Playership_eleven", 10, shape_pntr);
    shape_pntr = Place_shipdata(content, "Playership_twelve", 11, shape_pntr);
    shape_pntr = Place_shipdata(content, "Playership_thirteen", 12, shape_pntr);
    shape_pntr = Place_shipdata(content, "Playership_fourteen", 13, shape_pntr);
    shape_pntr = Place_shipdata(content, "Playership_fifteen", 14, shape_pntr);
    shape_pntr = Place_shipdata(content, "Playership_sixteen", 15, shape_pntr);
    shape_pntr = Place_shipdata(content, "Playership_seventeen", 16, shape_pntr);

    /// Get enemy ship data

    Place_enemydata(content, "Enemyship_one", 0);
    Place_enemydata(content, "Enemyship_two", 1);
    Place_enemydata(content, "Enemyship_three", 2);
    Place_enemydata(content, "Enemyship_four", 3);
    Place_enemydata(content, "Enemyship_five", 4);
    Place_enemydata(content, "Enemyship_six", 5);
    Place_enemydata(content, "Enemyship_seven", 6);
    Place_enemydata(content, "Cluster_ship", 7);

    int lp2;
    for (lp=0; lp<8; lp++)
    {
        enemy_shapes_pntr[lp] = shape_pntr;
        for (lp2=0; lp2<enemy_shapes[lp].size(); lp2++)
        {
            sh_pship.points[shape_pntr] = enemy_shapes[lp][lp2];
            shape_pntr++;
        }
    }
    cluster_index = 7;

    enemy_colors[0] = LIGHT_SKY_BLUE;
    enemy_colors[1] = RED;
    enemy_colors[2] = LIME;
    enemy_colors[3] = vec3(1.0, 0.5, 0.2);
    enemy_colors[4] = YELLOW;
    enemy_colors[5] = ORANGE;
    enemy_colors[6] = vec3(0.8, 0.9, 1.0);

    glUseProgram(sh_pship.program);
    glBindVertexArray(sh_pship.vao);
    glUniform1f(sh_pship.depth_location, -0.5);
    glBindBuffer(GL_ARRAY_BUFFER, sh_pship.points_vbo);
    glBufferData(GL_ARRAY_BUFFER, shape_pntr * sizeof(vec2), sh_pship.points, GL_STATIC_DRAW);
    glBindVertexArray(0);


    for (lp=0; lp<enemy_shapes[cluster_index].size(); lp++)
    {
        sh_drone.points[lp] = enemy_shapes[cluster_index][lp];
    }

    glUseProgram(sh_drone.program);
    glBindVertexArray(sh_drone.vao);
    glUniform1f(sh_drone.depth_location, -0.5);
    glBindBuffer(GL_ARRAY_BUFFER, sh_drone.points_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vec2)*enemy_shapes[cluster_index].size(), sh_drone.points, GL_STATIC_DRAW);
    glBindVertexArray(0);

    for (int lp=0; lp<num_players; lp++)     // Setup player ships
    {
        pship[lp].xpos = (Xmax/2) + lp*50;   // X position
        pship[lp].ypos = Ymax/2;             // Y position
        pship[lp].angle = 0;                 // Orientation
        pship[lp].mass = 1.0;                // Ship mass
        pship[lp].status = SHIP_ACTIVE;      // Make active
        pship[lp].thrust_flag = false;       // Engines off
        pship[lp].bomb_timer = 0;
        pship[lp].fire_pressed = false;
        shield_flag[lp] = false;
        spawn_shield_flag[lp] = false;
        shield_time[lp] = 0;
        pship[lp].shape.resize(6);
        pship[lp].lshape.resize(6);
    }
    pship[0].color = LIGHT_SKY_BLUE;         // Set player ship colors
    pship[1].color = LIME;
    pship[2].color = VIOLET;
    pship[3].color = YELLOW;
    pship[4].color = ORANGE;
    pship[5].color = TOMATO;
    pship[6].color = MAGENTA;
    pship[7].color = LAVENDER;
    pship[8].color = CHOCOLATE;
    pship[9].color = RED;
    pship[10].color = WHEAT;
    pship[11].color = SALMON;
    pship[12].color = TAN;
    pship[13].color = PINK;
    pship[14].color = THISTLE;
    pship[15].color = AQUA_MARINE;
    pship[16].color = MOCCASIN;

    pship[0].controller = KEYBOARD;          // First ship defaults to keyboard controller
    pship[0].rotate_left = k_rotate_left;
    pship[0].rotate_right = k_rotate_right;
    pship[0].thrust = k_thrust;
    pship[0].fire = k_fire;
    pship[0].bomb = k_bomb;
    pship[0].shields = k_shield;


    // Create shields
    sh_shield.points = new vec2[24];

    float x, y;
    float angle = 0;
    float step_angle = 360.0 / 24.0;
    for (int lp=0; lp<24; lp++)
    {
        x = sin(angle*TO_RADIANS);
        y = cos(angle*TO_RADIANS);
        sh_shield.points[lp] = vec2(x, y);
        angle+= step_angle;
    }


    glUseProgram(sh_shield.program);
    glBindVertexArray(sh_shield.vao);
    glUniform1f(sh_shield.depth_location, -0.6);
    glBindBuffer(GL_ARRAY_BUFFER, sh_shield.points_vbo);
    glBufferData(GL_ARRAY_BUFFER, 24 * sizeof(vec2), sh_shield.points, GL_STATIC_DRAW);
    glBindVertexArray(0);

    vertex.clear();
    pos = content.find("Ship_exhaust");
    if (pos!=string::npos)
    {
        do
        {
            num = Search_float(content, pos, &pos);
            vertex.push_back(num);
        }
        while (num<10.0);
        vertex.pop_back();
    }
    sh_exhaust.points = new vec2[vertex.size()/2];
    for (int lp=0; lp<vertex.size()/2; lp++)
    {
        sh_exhaust.points[lp] = vec2(vertex[lp*2], vertex[lp*2+1]);
    }

    vertex.clear();
    pos = content.find("Exhaust_color");
    if (pos!=string::npos)
    {
        do
        {
            num = Search_float(content, pos, &pos);
            vertex.push_back(num);
        }
        while (num<10.0);
        vertex.pop_back();
    }
    sh_exhaust.color = new vec3[vertex.size()/3];
    for (int lp=0; lp<vertex.size()/3; lp++)
    {
        sh_exhaust.color[lp] = vec3(vertex[lp*3], vertex[lp*3+1], vertex[lp*3+2]);
    }

    glUseProgram(sh_exhaust.program);
    glBindVertexArray(sh_exhaust.vao);
    vec4 color = vec4(1.0, 1.0, 153.0/255.0, 1.0);
    glUniform4fv(sh_exhaust.color_location, 1, color.v);
    glUniform1f(sh_exhaust.depth_location, -0.6);
    glBindBuffer(GL_ARRAY_BUFFER, sh_exhaust.points_vbo);
    glBufferData(GL_ARRAY_BUFFER, 3 * sizeof(vec2), sh_exhaust.points, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, sh_exhaust.color_vbo);
    glBufferData(GL_ARRAY_BUFFER, 3 * sizeof(vec3), sh_exhaust.color, GL_STATIC_DRAW);
    glBindVertexArray(0);

    bullets.clear();
    ebullets.clear();
    ship_explode.clear();

    // Set thrust pitch and fire pitch sounds
    pship[0].thrust_pitch = 1.0;
    pship[0].fire_pitch = 1.0;

    uniform_real_distribution<float>tfrnd(0.5, 2.0);

    for (lp=1; lp<17; lp++)
    {
        pship[lp].thrust_pitch = tfrnd(rndrock);
        pship[lp].fire_pitch = tfrnd(rndrock);
    }
    return true;
}

void Create_rock_shapes()
{
    uniform_real_distribution<float>radius(0.8, 1.05);

    rock_table.clear();
    s_rock_table.clear();
    rck_data.clear();
    vector<vec2>vertex;
    vector<vec2>s_vertex;
    vertex.clear();
    s_vertex.clear();
    vec2 temp;
    vec2 first;
    float x, y, r;
    float angle, step_angle;
    int pntr = 0;
    int s_pntr = 0;
    int cnt = 0;
    int s_cnt = 0;
    int lp, lp2;
    int table = 0;

    // Large rocks
    for (lp=0; lp<Big_rock_shapes; lp++)
    {
        step_angle = 360.0 / 24.0;
        angle = 0;
        cnt = 0;
        s_cnt = 0;

        rock_table.push_back(vec2i());
        s_rock_table.push_back(vec2i());
        rock_table[table].v[0] = pntr;
        s_rock_table[table].v[0] = s_pntr;

        s_vertex.push_back(vec2(0.0, 0.0));
        s_cnt++;
        s_pntr++;

        for (lp2=0; lp2<24; lp2++)
        {
            x = sin(angle*TO_RADIANS);
            y = cos(angle*TO_RADIANS);
            r = radius(rndrock);
            temp = vec2(x*r, y*r);
             if (lp2==0) first = temp;
            vertex.push_back(temp);
            s_vertex.push_back(temp);
            angle+= step_angle;
            cnt++;
            s_cnt++;
            pntr++;
            s_pntr++;
        }
        s_vertex.push_back(first);
        s_cnt++;
        s_pntr++;

        rock_table[table].v[1] = cnt;
        s_rock_table[table].v[1] = s_cnt;
        table++;
    }

    // Medium rocks
    for (lp=0; lp<Medium_rock_shapes; lp++)
    {
        step_angle = 360.0 / 16.0;
        angle = 0;
        cnt = 0;
        s_cnt = 0;

        rock_table.push_back(vec2i());
        s_rock_table.push_back(vec2i());
        rock_table[table].v[0] = pntr;
        s_rock_table[table].v[0] = s_pntr;

        s_vertex.push_back(vec2(0.0, 0.0));
        s_cnt++;
        s_pntr++;

        for (lp2=0; lp2<16; lp2++)
        {
            x = sin(angle*TO_RADIANS);
            y = cos(angle*TO_RADIANS);
            r = radius(rndrock);
            temp = vec2(x*r, y*r);
             if (lp2==0) first = temp;
            vertex.push_back(temp);
            s_vertex.push_back(temp);
            angle+= step_angle;
            cnt++;
            s_cnt++;
            pntr++;
            s_pntr++;
        }
        s_vertex.push_back(first);
        s_cnt++;
        s_pntr++;

        rock_table[table].v[1] = cnt;
        s_rock_table[table].v[1] = s_cnt;
        table++;
    }

    // Small rocks
    for (lp=0; lp<Small_rock_shapes; lp++)
    {
        step_angle = 360.0 / 9.0;
        angle = 0;
        cnt = 0;
        s_cnt = 0;

        rock_table.push_back(vec2i());
        s_rock_table.push_back(vec2i());
        rock_table[table].v[0] = pntr;
        s_rock_table[table].v[0] = s_pntr;

        s_vertex.push_back(vec2(0.0, 0.0));
        s_cnt++;
        s_pntr++;

        for (lp2=0; lp2<9; lp2++)
        {
            x = sin(angle*TO_RADIANS);
            y = cos(angle*TO_RADIANS);
            r = radius(rndrock);
            temp = vec2(x*r, y*r);
             if (lp2==0) first = temp;
            vertex.push_back(temp);
            s_vertex.push_back(temp);
            angle+= step_angle;
            cnt++;
            s_cnt++;
            pntr++;
            s_pntr++;
        }
        s_vertex.push_back(first);
        s_cnt++;
        s_pntr++;

        rock_table[table].v[1] = cnt;
        s_rock_table[table].v[1] = s_cnt;
        table++;
    }
    vector<vec2>c_vector;
    c_vector = Get_rock_shapes();               // Get custom rock shapes from file
    int c_shapes = 0;

    // Add to existing rock shape vectors.  Kind of a messy process!
    Custom_rock_shapes_pntr = table;
    cnt=0;
    s_cnt = 0;
    rock_table.push_back(vec2i());
    s_rock_table.push_back(vec2i());
    rock_table[table].v[0] = pntr;
    s_rock_table[table].v[0] = s_pntr;
    for (lp=0; lp<c_vector.size()-1; lp++)
    {
        if (cnt!= 0)
        {
            if (c_vector[lp].v[0] < 10.0)
            {
                vertex.push_back(c_vector[lp]);  // Ignore first vertex for outline shape
                pntr++;
                cnt++;
            }
        }
        else cnt=1;
        if (c_vector[lp].v[0] < 10.0)
        {
            s_vertex.push_back(c_vector[lp]);
            s_cnt++;
            s_pntr++;
        }

        if (c_vector[lp].v[0] > 10.0)
        {
            rock_table[table].v[1] = cnt-2;    // Don't count the last vertex, not needed for GL_LINE_LOOP
            s_rock_table[table].v[1] = s_cnt;

            cnt = 0;
            s_cnt = 0;

            table++;
            rock_table.push_back(vec2i());
            s_rock_table.push_back(vec2i());
            rock_table[table].v[0] = pntr;
            s_rock_table[table].v[0] = s_pntr;
            c_shapes++;
        }
    }
    custom_rock_count = c_shapes;

    for (lp=0; lp<vertex.size(); lp++)
    {
        rck_data.push_back(vertex[lp]);
    }
    for (lp=0; lp<s_vertex.size(); lp++)
    {
        rck_data.push_back(s_vertex[lp]);
    }
    for (lp=0; lp<table; lp++)
    {
        s_rock_table[lp].v[0]+= vertex.size();
    }

    if (!Rock_instancing)
    {
        sh_rocks.points = new vec2[vertex.size() + s_vertex.size()];
        for (lp=0; lp<vertex.size(); lp++)
        {
            sh_rocks.points[lp] = vertex[lp];
        }
        for (lp=0; lp<s_vertex.size(); lp++)
        {
            sh_rocks.points[lp+vertex.size()] = s_vertex[lp];
        }
    }
    else
    {
        sh_rocks2.points[0] = vec2(0.0, 0.0);
        float step_angle = 360.0/24.0;
        float step = 0;
        for (lp=1; lp<25; lp++)
        {
            sh_rocks2.points[lp].v[0] = sin(step*TO_RADIANS);
            sh_rocks2.points[lp].v[1] = cos(step*TO_RADIANS);
            step+= step_angle;
        }
        sh_rocks2.points[25] = sh_rocks2.points[1];
    }
    if (!Rock_instancing)
    {
        glUseProgram(sh_rocks.program);
        glBindVertexArray(sh_rocks.vao);
        glBindBuffer(GL_ARRAY_BUFFER, sh_rocks.points_vbo);
        glBufferData(GL_ARRAY_BUFFER, (vertex.size()+s_vertex.size()) * sizeof(vec2), sh_rocks.points, GL_STATIC_DRAW);
        glBindVertexArray(0);
    }
    else
    {
        glUseProgram(sh_rocks2.program);
        glBindVertexArray(sh_rocks2.vao);
        glBindBuffer(GL_ARRAY_BUFFER, sh_rocks2.points_vbo);
        glBufferSubData(GL_ARRAY_BUFFER, 0, 26 * sizeof(vec2), sh_rocks2.points);
        uniform_real_distribution<float>rseed(0.0, 12800.0);
        rock_seed = vec2(rseed(rndrock), rseed(rndrock));
        glUniform1f(sh_rocks2.seed1_location, rock_seed.v[0]);
        glUniform1f(sh_rocks2.seed2_location, rock_seed.v[1]);
        glBindVertexArray(0);
    }
}

/// Create a single large rock and starting position
rock_data Place_rock(int type)
{
    uniform_real_distribution<float>rock_speed(0.5, 1.2);
    uniform_real_distribution<float>color_factor(0.5, 1.0);
    uniform_real_distribution<float>angle_step(-3.0, 3.0);
    uniform_real_distribution<float>direction(1.0, 359.0);
    uniform_real_distribution<float>rock_size(37.0, 45.0);

    float angle = direction(rndrock) * TO_RADIANS;
    float speed = rock_speed(rndrock);
    rock_data rd;

    rd.type = type;
    rd.stage = BIG_ROCK;
    rd.mass = 100.0;
    rd.angle = 0;
    rd.radius = rock_size(rndrock);
    rd.angle_step = angle_step(rndrock)*TO_RADIANS;

    if (type == COMMON_ROCK)
    {
        rd.color = Common_color * color_factor(rndrock);
        rd.hits = Hits_standard_big;
        rd.score = Points_standard_big;
    }
    else if (type == FAST_ROCK)
    {
        rd.color = Fast_color * color_factor(rndrock);
        rd.hits = Hits_fast_big;
        rd.score = Points_fast_big;
        speed = 4.0;
    }
    else if (type == BRITTLE_ROCK)
    {
        rd.color = Brittle_color * color_factor(rndrock);
        rd.hits = Hits_brittle_big;
        rd.score = Points_brittle_big;
    }
    else if (type == HARD_ROCK)
    {
        rd.color = Hard_color * color_factor(rndrock);
        rd.hits = Hits_hard_big;
        rd.score = Points_hard_big;
    }

    rd.xdir = sin(angle)*speed;
    rd.ydir = cos(angle)*speed;

    // Set starting position of new big rock
    uniform_int_distribution<int>sides(0, 3);
    uniform_real_distribution<float>xstart(0.0, Xmax);
    uniform_real_distribution<float>ystart(0.0, Ymax);

    rd.status = true;
    if (!Rock_instancing)
    {
        int rock_shape;
        if (Custom_rocks_flag)
        {
            uniform_int_distribution<int>c_shape(0, custom_rock_count-1);
            rock_shape = c_shape(rndrock) + Custom_rock_shapes_pntr;
        }
        else
        {
            uniform_int_distribution<int>shape(0, Big_rock_shapes-1);
            rock_shape = shape(rndrock);
        }
        rd.pntr = rock_table[rock_shape];            // Outline rocks
        rd.s_pntr = s_rock_table[rock_shape];        // Solid rocks
    }
    else
    {
        rd.pntr = rock_table[0];            // Outline rocks
        rd.s_pntr = s_rock_table[0];        // Solid rocks
    }

    int where = sides(rndrock);
    if (where == 0)
    {
        rd.xpos = -50.0;
        rd.ypos = ystart(rndrock);
    }
    else if (where == 1)
    {
        rd.xpos = xstart(rndrock);
        rd.ypos = Ymax + 50.0;
    }
    else if (where == 2)
    {
        rd.xpos = Xmax + 50.0;
        rd.ypos = ystart(rndrock);
    }
    else if (where == 3)
    {
        rd.xpos = xstart(rndrock);
        rd.ypos = -50.0;
    }
    return rd;
}

void Create_rocks(int level)
{
    int lp;
    float angle;
    int num_rocks = (int) ((float) levels[level].num_rocks * reso_factor);
    int num_rocks2 = (int) ((float) levels[level].num_fast * reso_factor);
    int num_rocks3 = (int) ((float) levels[level].num_brittle * reso_factor);
    int num_rocks4 = (int) ((float) levels[level].num_hard * reso_factor);

    int cnt = num_rocks+num_rocks2+num_rocks3+num_rocks4;
    cnt*= 25;
    Initialize_rock_grid(cnt);

    rocks.clear();
    rocks.reserve(cnt);
    rstarts.clear();
    rstarts.reserve(cnt);
    for (lp=0; lp<num_rocks; lp++)
    {
        rstarts.push_back(COMMON_ROCK);
    }
    for (lp=0; lp<num_rocks2; lp++)
    {
        rstarts.push_back(FAST_ROCK);
    }
    for (lp=0; lp<num_rocks3; lp++)
    {
        rstarts.push_back(BRITTLE_ROCK);
    }
    for (lp=0; lp<num_rocks4; lp++)
    {
        rstarts.push_back(HARD_ROCK);
    }

    std::random_shuffle(rstarts.begin(), rstarts.end());

    if (Rock_spawn_delay>0.0)
    {
        total_rocks = 0;
        Rock_spawn_timer = Rock_spawn_delay;   // From settings file
        int sz = rstarts.size()-1;
        rocks.push_back(Place_rock(rstarts[sz]));
        rstarts.pop_back();
        total_rocks++;
    }
    else
    {
        for (lp=0; lp<rstarts.size(); lp++)
        {
            int sz = rstarts.size()-1;
            rocks.push_back(Place_rock(rstarts[sz]));
            rstarts.pop_back();
            total_rocks++;
        }
    }

    if (Rock_instancing)
    {
        int reserve = rocks.size();

         glUseProgram(sh_rocks2.program);
         glBindVertexArray(sh_rocks2.vao);

         for (lp=0; lp<reserve; lp++)
         {
            sh_rocks2.active[lp] = 1;
         }
          glBindBuffer(GL_ARRAY_BUFFER, sh_rocks2.active_vbo);
          glBufferSubData(GL_ARRAY_BUFFER, 0, reserve * sizeof(int), sh_rocks2.active);

         for (lp=0; lp<reserve; lp++)
         {
             sh_rocks2.position[lp] = vec2(rocks[lp].xpos, rocks[lp].ypos);
         }
          glBindBuffer(GL_ARRAY_BUFFER, sh_rocks2.position_vbo);
          glBufferSubData(GL_ARRAY_BUFFER, 0, reserve * sizeof(vec2), sh_rocks2.position);

          for (lp=0; lp<reserve; lp++)
         {
             sh_rocks2.color[lp] = rocks[lp].color;
         }
          glBindBuffer(GL_ARRAY_BUFFER, sh_rocks2.color_vbo);
          glBufferSubData(GL_ARRAY_BUFFER, 0, reserve * sizeof(vec3), sh_rocks2.color);

          for (lp=0; lp<reserve; lp++)
         {
             sh_rocks2.radius[lp] = rocks[lp].radius;
         }
          glBindBuffer(GL_ARRAY_BUFFER, sh_rocks2.radius_vbo);
          glBufferSubData(GL_ARRAY_BUFFER, 0, reserve * sizeof(float), sh_rocks2.radius);

          for (lp=0; lp<reserve; lp++)
         {
             sh_rocks2.angle[lp] = 0;
         }
          glBindBuffer(GL_ARRAY_BUFFER, sh_rocks2.angle_vbo);
          glBufferSubData(GL_ARRAY_BUFFER, 0, reserve * sizeof(float), sh_rocks2.angle);


          glBindVertexArray(0);
    }

    // Calculate potential number of small rocks.
    // Used to determine when to launch enemy ships
    potential_rocks = 0;
    for (lp=0; lp<rstarts.size(); lp++)
    {
        if (rstarts[lp] == BRITTLE_ROCK) potential_rocks+= 50;
        else potential_rocks+= 25;
    }

    rock_explode.clear();
    mini_explode.clear();
    impact.clear();
    drexplode.clear();
    potential_max = potential_rocks;
    Rock_buffers_changed = true;
}

/// Create a cluster of drones
void Add_cluster()
{
    int lp;
    drone_cluster dc;
    drone_ship ds;
    float sintheta;
    float costheta;
    float sz = Drone_size;
    float a;

    uniform_int_distribution<int>sides(0, 3);  //Randomly pick border cluster will spawn behind
    int s = sides(rndrock);
    if (s==0)                  // Left border
    {
        dc.xpos = -50.0;
        uniform_real_distribution<float>ypos(200.0, Ymax-200.0);
        dc.ypos = ypos(rndrock);
        uniform_real_distribution<float>angle(-30.0, 30.0);
        a = angle(rndrock);
    }
    else if (s==1)            // Right border
    {
        dc.xpos = Xmax+50.0;
        uniform_real_distribution<float>ypos(200.0, Ymax-200.0);
        dc.ypos = ypos(rndrock);
        uniform_real_distribution<float>angle(150.0, 210.0);
        a = angle(rndrock);
    }
    else if (s==2)            // Bottom border
    {
        uniform_real_distribution<float>xpos(200.0, Xmax-200.0);
        dc.xpos = xpos(rndrock);
        dc.ypos = -50.0;
        uniform_real_distribution<float>angle(60.0, 120.0);
        a = angle(rndrock);
    }
    else if (s==3)            // Top border
    {
        uniform_real_distribution<float>xpos(200.0, Xmax-200.0);
        dc.xpos = xpos(rndrock);
        dc.ypos = Ymax+50.0;
        uniform_real_distribution<float>angle(240.0, 300.0);
        a = angle(rndrock);
    }

    dc.status = true;
    dc.xdir = cos(a*TO_RADIANS)*20.0;
    dc.ydir = sin(a*TO_RADIANS)*20.0;

    cluster.push_back(dc);
    int pntr = cluster.size()-1;

    // Set 6 drone positions and orientations in cluster
    vec2 pnt1 = vec2(-0.95, 1.0);
    vec2 pnt2 = vec2(0.95, 1.0);
    ds.x_offset = pnt1.v[0];
    ds.y_offset = pnt1.v[1];
    ds.angle = 90.0*TO_RADIANS;
    drones.push_back(ds);
    ds.x_offset = pnt2.v[0];
    ds.y_offset = pnt2.v[1];
    ds.angle = -90.0*TO_RADIANS;
    drones.push_back(ds);

    pnt1 = Rotate_2f(120.0*TO_RADIANS, pnt1);
    pnt2 = Rotate_2f(120.0*TO_RADIANS, pnt2);
    ds.x_offset = pnt1.v[0];
    ds.y_offset = pnt1.v[1];
    ds.angle = 210.0*TO_RADIANS;
    drones.push_back(ds);
    ds.x_offset = pnt2.v[0];
    ds.y_offset = pnt2.v[1];
    ds.angle = 30.0*TO_RADIANS;
    drones.push_back(ds);

    pnt1 = Rotate_2f(120.0*TO_RADIANS, pnt1);
    pnt2 = Rotate_2f(120.0*TO_RADIANS, pnt2);
    ds.x_offset = pnt1.v[0];
    ds.y_offset = pnt1.v[1];
    ds.angle = 330.0*TO_RADIANS;
    drones.push_back(ds);
    ds.x_offset = pnt2.v[0];
    ds.y_offset = pnt2.v[1];
    ds.angle = 150.0*TO_RADIANS;
    drones.push_back(ds);

    vec2 dra = levels[game_level-1].dr_accel;
    uniform_real_distribution<float>ac(dra.v[0], dra.v[1]);  // Acceleration factor
    uniform_int_distribution<int>fast_drone(1, 100);

    for (lp=0; lp<6; lp++)
    {
        int slot = pntr*6 + lp;
        drones[slot].status = true;
        drones[slot].cluster = pntr;
        drones[slot].x_offset*= sz;
        drones[slot].y_offset*= sz;
        drones[slot].accel = ac(rndrock);

        // Bump up top drone speed using acceleration factor
        drones[slot].tspeed = levels[game_level-1].dr_speed + (drones[slot].accel-dra.v[0])*500.0;
        drones[slot].speed = 50.0;         // Starting speed
        drones[slot].color = WHITE;

        if (fast_drone(rndrock) < 5)    // Make a drone faster?
        {
            drones[slot].color = vec3(1.0, 0.1, 0.1);
            drones[slot].tspeed*= 2.0;
            drones[slot].accel*= 2.0;
        }
        else if (fast_drone(rndrock) == 50)
        {
            drones[slot].color = vec3(0.0, 1.0, 0.2);
            drones[slot].tspeed*= 4.0;
            drones[slot].accel*= 4.0;
        }
    }
}

/// *** Setup collision grid.  Grid size determined by maximum monitor resolution you choose ***
void Initialize_rock_grid(int rock_amount)
{
    int lp, lp2;
    int x_amount, y_amount;

    x_amount = (int) ((Xmax/300.0) + 0.5);  // Get number horizontal cells
    y_amount = (int) ((Ymax/300.0) + 0.5);  // Vertical cells

    // Create the grid.  (Grid of vectors)
    rgrid.clear();
    vector<vec2i>v_1D;
    vector<vector<vec2i>> v_2D;

    for (lp=0; lp<x_amount+1; lp++)
    {
        for (lp2=0; lp2<y_amount+1; lp2++)
        {
            v_2D.push_back(v_1D);
        }
        rgrid.push_back(v_2D);
    }
    Xgrid = (Xmax+100) / x_amount;      // Get exact size of each cell
    Ygrid = (Ymax+100) / y_amount;
    Xgrid_size = x_amount;              // Move to global variables
    Ygrid_size = y_amount;
}

void Get_joystick_status(int stick, int name)
{
    const char* jname;
    joystick[stick] = vec2i(0, 0);
    joystick[stick].v[0] = glfwJoystickPresent(name);

    if (joystick[stick].v[0])
    {
        if (glfwJoystickIsGamepad(name))
        {
            jname = glfwGetGamepadName(name);
            joystick[0].v[1] = 1;
        }
        else
        {
            jname = glfwGetJoystickName(name);
        }
        string temp(jname);
        joystick_name[stick] = temp;
    }

}

void Check_for_joysticks()
{
    joysticks = false;
    Get_joystick_status(0, GLFW_JOYSTICK_1);
    Get_joystick_status(1, GLFW_JOYSTICK_2);
    Get_joystick_status(2, GLFW_JOYSTICK_3);
    Get_joystick_status(3, GLFW_JOYSTICK_4);
    Get_joystick_status(4, GLFW_JOYSTICK_5);
    Get_joystick_status(5, GLFW_JOYSTICK_6);
    Get_joystick_status(6, GLFW_JOYSTICK_7);
    Get_joystick_status(7, GLFW_JOYSTICK_8);
    Get_joystick_status(8, GLFW_JOYSTICK_9);
    Get_joystick_status(9, GLFW_JOYSTICK_10);
    Get_joystick_status(10, GLFW_JOYSTICK_11);
    Get_joystick_status(11, GLFW_JOYSTICK_12);
    Get_joystick_status(12, GLFW_JOYSTICK_13);
    Get_joystick_status(13, GLFW_JOYSTICK_14);
    Get_joystick_status(14, GLFW_JOYSTICK_15);
    Get_joystick_status(15, GLFW_JOYSTICK_16);

    // Determine # of players due to existing number of game controllers
    max_players = 17;     // Player can use keyboard
    for (int lp=0; lp<17; lp++)
    {
        if (joystick[lp].v[0])
        {
            joysticks = true;
            pship[lp+1].controller = GAMEPAD;
            pship[lp+1].jstick = GLFW_JOYSTICK_1+lp;
            pship[lp+1].rotate_left = GLFW_GAMEPAD_AXIS_LEFT_X;
            pship[lp+1].rotate_right = GLFW_GAMEPAD_AXIS_LEFT_X;
            pship[lp+1].thrust = GLFW_GAMEPAD_AXIS_RIGHT_TRIGGER;
            pship[lp+1].fire = GLFW_GAMEPAD_BUTTON_A;
            pship[lp+1].bomb = GLFW_GAMEPAD_BUTTON_B;
            pship[lp+1].shields = GLFW_GAMEPAD_BUTTON_X;
        }
        else
        {
            pship[lp+1].controller = NPC;
        }
    }
}
