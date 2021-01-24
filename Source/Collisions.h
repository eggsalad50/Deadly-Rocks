#ifndef COLLISIONS_H_INCLUDED
#define COLLISIONS_H_INCLUDED

#include "main.h"

void Check_playerships_hit_something();
bool Ship_hit_rock(int sh, int rk);
bool Ship_hit_enemy(int sh, int index);
bool Ship_hit_ship(int sh, int index);
bool Ship_hit_drone(int sh, int index);
bool Ship_shields_hit_rock(int sh, int index);
bool Ship_shields_hit_enemy(int sh, int index);
bool Ship_shields_collide(int sh, int index);
bool Precise_ship_hit_rock_collision(int index, int sh);
bool Precise_ship_hit_enemy_collision(int index, int sh);
bool Precise_ship_hit_ship(int sh, int sh2);
bool Precise_ship_hit_shieldship(int sh, int sh2);
bool Precise_shield_hit_enemy_collision(int index, int sh);
bool Precise_ship_hit_drone(int sh, int dr);
bool Precise_shieldship_hit_drone(int sh, int dr);
void Check_enemyships_hit_rocks();
bool Precise_enemyship_hit_rock_collision(int e_index, int r_index);
void Check_bullets_hit_something();
bool Bullet_rock_collision(int bl, int rk, circle_line *cl);
bool Bullet_player_collision(int bl, int sh, circle_line *cl);
bool Bullet_enemy_collision(int bl, int en, circle_line *cl);
bool Bullet_drone_collision(int bl, int d_index, circle_line *cl);
bool Precise_bullet_hit_rock(int r_index, circle_line *cl);
bool Precise_bullet_hit_enemy(int index, circle_line *cl);
bool Precise_bullet_hit_ship(int sh, circle_line *cl);
bool Precise_bullet_hit_drone(int d_index, circle_line *cl);
void Check_ebullets_hit_something();
bool Ebullet_rock_collision(int bl, int rk, circle_line *cl);
bool Ebullet_ship_collision(int bl, int sh, circle_line *cl);
void Check_bomb_bullets_hit_something();
bool Bbullet_rock_collision(int bl, int rk, circle_line *cl);
bool Bbullet_enemy_collision(int bl, int en, circle_line *cl);
bool Bbullet_drone_collision(int bl, int d_index, circle_line *cl);

#endif // COLLISIONS_H_INCLUDED
