#ifndef TEXT_H_INCLUDED
#define TEXT_H_INCLUDED

#include "main.h"

void Draw_score();
void Demo_text();
void Controls_text(float row);
void Draw_credits(float row);
void Draw_scores_text(float row);
void Draw_credits(float row);
void Draw_ranks(float row);
void Game_over_text();
void Enter_player_name(float row);
void Check_for_highscores();
bool No_same_keys(int key);
bool Valid_key(int key);
void Get_high_score(int key);
void Control_selection();
void Joystick_settings();
void Performance_text(float frames);
void Draw_phrase(int phrase, float column, float y, float z, vec4 color);
void Bonus_ship_text();
void Misc_settings_text(float row);

#endif // TEXT_H_INCLUDED
