#ifndef INITIALIZE_H_INCLUDED
#define INITIALIZE_H_INCLUDED

#include "main.h"

float Fcoef();
bool Get_characters();
bool Get_settings();
bool Get_letter(FILE *Chget, int *start, int *num);
vector<string> ReadTextFile(const char *filePath);
bool Create_playership();
void Create_rocks(int amount);
void Create_rock_shapes();
vector<vec2> Get_rock_shapes();
void Initialize_rock_grid(int rock_amount);
void Set_random_generator();
bool Get_level_data();
bool Initialize_audio();
static void list_audio_devices(const ALCchar *devices);
void Show_sound_error(ALCenum error);
void Load_sound_files();
short int *Convert_to_sound_data(AudioFile<float> &AF, ALuint &format, int &dsize);
bool Get_scores();
void Save_scores();
void Check_for_joysticks();
double Search_double2(string &str, size_t pos, size_t *last);
void Set_starting_monitor();

#endif // INITIALIZE_H_INCLUDED

