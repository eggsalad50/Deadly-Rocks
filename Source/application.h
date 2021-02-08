#ifndef APPLICATION_H_INCLUDED
#define APPLICATION_H_INCLUDED

#include "main.h"

void Clear_collision_grid();
void Maybe_start_new_level();
void Get_controller_input();
void Reduce_arrays();
void Check_if_gameover();
void Update_ship_timers();
void Do_frame_rate();
void DrawText();
void Screen_shot(int width, int height);
void Check_keyboard(GLFWwindow *window, int sh);
void Check_joystick(int sh);
void Spawn_medium_rocks(int index);
void Spawn_small_rocks(int index);
void Start_rock_explosion(int index);
void Spawn_newship(int num);
float Draw_textrow(char text_line[], float column, float y, float z, vec4 color);
void Set_textsize(float rows, float cols);
void Bullet_hit_rock(int b_index, int r_index, circle_line *cl);
void Ebullet_hit_rock(int b_index, int r_index, circle_line *cl);
void Fire_enemy_bullet(int index);
void Fire_single_bullet(int sh);
void Fire_mutliple_bullet(float angle, int sh);
void Fire_bullet(int sh);
void Fire_bomb_bullets(int sh);
void Draw_debug();
void Start_new_game();
void Enter_player_name(float row);
void Check_for_highscores();
void Draw_ranks(float row);
void Get_player_rank();
void Set_ship_positions(int num);
void Start_drexplode(int index);

#endif // APPLICATION_H_INCLUDED
