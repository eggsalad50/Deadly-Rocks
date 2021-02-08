
#include "application.h"

double elapsed_time;
float row_size;
float text_scale;
float col_size;
bool new_level_flag;
double new_level_delay;
double cpu_load;
double spawn_shield_timer[17];
vector<vec2i>player_highscore_list;

uniform_real_distribution<float>big_rock_pitch(0.7, 2.0);
uniform_real_distribution<float>small_rock_pitch(0.7, 1.3);
uniform_real_distribution<float>medium_rock_pitch(0.8, 1.2);

/// *************************  MAIN FUNCTION  ************************
/// Calls sub functions of the game
void Application(GLFWwindow* window)
{
    static double last_time = glfwGetTime();

    elapsed_time = glfwGetTime() - last_time;
    last_time = glfwGetTime();
    hiscore_timer-= elapsed_time;                // Hi score display timer
    if (hiscore_timer < HI_SCORE_OFF) hiscore_timer = NO_SHOW_STUFF;

    if (new_level_flag) Maybe_start_new_level();
    Update_ship_timers();
    Clear_collision_grid();
    Get_controller_input();
    Update_everything();
    Check_for_collisions();
    Draw_everything();
    Reduce_arrays();

    if (!demo) Check_if_gameover();
    if (score_flag) Draw_score();
    if (cpu_load_flag) Do_frame_rate();

    if (demo)
    {
        if (game_over) Game_over_text();
        else Demo_text();
        if (controls) Joystick_settings();
    }

}
/// ***************** End of main game function ****************

/// Clear collision grid
/// Collision grid purpose is to greatly reduce collision tests
void Clear_collision_grid()
{
    for (int lp=0; lp<Xgrid_size+1; lp++)
    {
        for (int lp2=0; lp2<Ygrid_size+1; lp2++)
        {
            rgrid[lp][lp2].clear();
        }
    }
}

/// Find correct moment to start next level
void Maybe_start_new_level()
{
    bool no_enemies = false;
    //bool no_drones = false;
    int lp;
    double time = glfwGetTime();
    if (time - new_level_delay > 4.0)         // Wait 4 seconds to start next level
    {
        for (lp=0; lp<num_enemy; lp++)        // Make sure there's no enemy ships around when starting next level
        {
            if (enemy[lp].status) break;
        }
        if (lp>= num_enemy)
        {
            no_enemies = true;
        }

        if (no_enemies && ebullets.empty() && !bomb_flag)
        {
            Start_level(game_level);
        }
    }
}

void Get_controller_input()
{
    for (int lp=0; lp<num_players; lp++)
    {
        if (pship[lp].controller == KEYBOARD)
        {
            Check_keyboard(window, lp);
        }
        else if (pship[lp].controller == GAMEPAD)
        {
            if (!demo) Check_joystick(lp);
        }
        else if (pship[lp].controller == NPC)     // Not implemented yet.
        {
            Computer_controled(lp);
        }
    }
}

///  **** Reduce size of arrays ****
// Removes inactive slots at end of arrays
void Reduce_arrays()
{
    if (!demo)
    {
        if (!bullets.empty())
        {
            int sz = bullets.size()-1;
            while (!bullets[sz].status)
            {
                bullets.pop_back();
                sz--;
                if (sz<0) break;
            }
        }
    }

    if (!ebullets.empty())
    {
        int sz = ebullets.size()-1;
        while (!ebullets[sz].status)
        {
            ebullets.pop_back();
            sz--;
            if (sz<0) break;
        }
    }

    if (last_bbullet>0)
    {
        if (!bbullets[last_bbullet].status) last_bbullet--;
    }

    if (!impact.empty())
    {
        int sz = impact.size()-1;
        while (!impact[sz].status)
        {
            impact.pop_back();
            sz--;
            if (sz<0) break;
        }
    }

    if (!drexplode.empty())
    {
        int sz = drexplode.size()-1;
        while (!drexplode[sz].status)
        {
            drexplode.pop_back();
            sz--;
            if (sz<0) break;
        }
    }

    if (!ship_explode.empty())
    {
        int sz = ship_explode.size()-1;
        while (!ship_explode[sz].status)
        {
            ship_explode.pop_back();
            sz--;
            if (sz<0) break;
        }
    }

    if (!rock_explode.empty())
    {
        int sz = rock_explode.size()-1;
        while (!rock_explode[sz].status)
        {
            rock_explode.pop_back();
            sz--;
            if (sz<0) break;
        }
    }

    if (!mini_explode.empty())
    {
        int sz = mini_explode.size()-1;
        while (!mini_explode[sz].status)
        {
            mini_explode.pop_back();
            sz--;
            if (sz<0) break;
        }
    }

    if (!rocks.empty())
    {
        int sz = rocks.size()-1;
        while (!rocks[sz].status)
        {
            rocks.pop_back();
            sz--;
            if (sz<0) break;
        }
    }

    if (!drones.empty())
    {
        int sz = drones.size()-1;
        while (!drones[sz].status)
        {
            drones.pop_back();
            sz--;
            if (sz<0) break;
        }
    }

    if (rocks.empty() && drones.empty())        // No rocks so set next level flag
    {
        if (new_level_flag) return;
        game_level++;
        new_level_flag = true;
        new_level_delay = glfwGetTime();
    }
}

void Check_if_gameover()
{
    int over_loop;
    for (over_loop=0; over_loop<num_players; over_loop++)
    {
        if (pship[over_loop].status) break;
    }
    if (over_loop>= num_players) pship_status = false;
    else pship_status = true;

    if (pship_status == false)            // No ships left for all players?
    {
        demo = true;
        game_over = true;
        num_enemy = Demo_ships;
        uniform_real_distribution<double>next_spawn(2.0, 20.0);
        while(enemy.size() < num_enemy)
        {
            enemy.push_back(eship_data());
            int sz = enemy.size()-1;
            enemy[sz].status = NO_SHIPS;
            enemy[sz].time = next_spawn(rndrock);
        }
        Check_for_highscores();
        Get_player_rank();
    }
}

void Update_ship_timers()
{
    int lp;

    for (lp=0; lp<num_players; lp++)
    {
        if (pship[lp].status == SHIP_SPAWN) Spawn_newship(lp);
        pship[lp].bomb_timer-= elapsed_time;
        if (!shield_flag[lp])
        {
            shield_time[lp]-= elapsed_time*Shield_charge_factor;
            if (shield_time[lp]<0.0) shield_time[lp] = 0.0;
        }
    }
}

void Do_frame_rate()
{
    static int framecount;
    static float frame_rate;
    static double Performance_delay = glfwGetTime();

    double check = glfwGetTime()-Performance_delay;
    if (check>=0.25)
    {
        frame_rate = (float (framecount) / check);
        Performance_delay = glfwGetTime();
        framecount=0;
    }
    Performance_text(frame_rate);
    framecount++;
}

/// ***** Start new game level *****
void Start_level(int level)
{
     //fprintf(stderr,"Start level IN %i\n", level);
     std::this_thread::sleep_for (std::chrono::milliseconds(1));
    if (level-1 >= levels.size()-2)
    {
        level = levels.size()-2;
        game_level = level;
    }
    time_sum=0;
    if (!demo)
    {
        num_enemy = levels[level-1].num_enemy;
        enemy.clear();
        for (int lp=0; lp<num_enemy; lp++)
        {
            enemy.push_back(eship_data());
            enemy[lp].status = NO_SHIPS;
        }
    }
    else
    {
        enemy.clear();
        num_enemy = Demo_ships;
        for (int lp=0; lp<num_enemy; lp++)
        {
            enemy.push_back(eship_data());
            enemy[lp].status = NO_SHIPS;
        }
    }
    drones.clear();
    cluster.clear();
     //fprintf(stderr,"Start level OUT %i\n", level);
    Create_rocks(level-1);
    new_level_flag = false;

    if (demo)
    {
        uniform_real_distribution<double>next_spawn(10.0, 20.0);
        for (int lp=0; lp<num_enemy; lp++)
        {
            enemy[lp].time = next_spawn(rndrock);
        }
    }
    bullet_speed = 15.0;
    alSourceStop(enemy_siren_sourceid);
    alSourceStop(enemy_siren2_sourceid);
}

///  For debugging runtime errors during development
void Draw_debug()
{
    //char c[50];
    //int cnt = 0;
    //Set_textsize(40, 60);
    //for (int lp=0; lp<MAX_ESHIPS; lp++)
    //{
    //    sprintf(c,"%i  %f %f  %f\n", enemy[lp].status, enemy[lp].xpos, enemy[lp].ypos, enemy[lp].dirx);
    //    Draw_textrow(c, 1, g_win_height - (lp+2)*row_size, 0.5, vec4(WHITE, 1.0));
    //}
    //sprintf(c,"rocks %i\n", rocks.size());
    //Draw_textrow(c, 1, g_win_height - 3*row_size, 0.5, vec4(WHITE, 1.0));

}

void Check_keyboard(GLFWwindow *window, int sh)
{
    if (!demo)
    {
        if (glfwGetKey (window, pship[sh].rotate_left))            // Move left
        {
            pship[sh].angle+= ((Ship_rotate_speed*TO_RADIANS)*(60.0*elapsed_time));
        }
        else if (glfwGetKey (window, pship[sh].rotate_right))
        {
            pship[sh].angle-= ((Ship_rotate_speed*TO_RADIANS)*(60.0*elapsed_time));
        }
        if (glfwGetKey (window, pship[sh].thrust)) // && !shield_flag)
        {
            pship[sh].dirx+= (-sin(pship[sh].angle)*(Ship_acceleration_factor*60*elapsed_time));
            pship[sh].diry+= (cos(pship[sh].angle)*(Ship_acceleration_factor*60*elapsed_time));
            pship[sh].thrust_flag = true;
            if (!sound_thrust_flag[sh] && pship[sh].status == SHIP_ACTIVE)
            {
                alSourcef(thrust_sourceid[sh], AL_GAIN, 0.5*Sound_volume);
                alSourcef(thrust_sourceid[sh], AL_PITCH, pship[sh].thrust_pitch);
                alSourcePlay(thrust_sourceid[sh]);
                alSourcei(thrust_sourceid[sh], AL_LOOPING, AL_TRUE);
                sound_thrust_flag[sh] = true;
            }
        }
        else
        {
            // Limit to every 1/60 of second otherwise decelerates too fast
            static double skip_time = glfwGetTime();
            if (glfwGetTime() - skip_time >= (1.0/60.0))
            {
                pship[sh].dirx*= Ship_decay_factor;   //Ship speed decay
                pship[sh].diry*= Ship_decay_factor;
                pship[sh].thrust_flag = false;
                skip_time = glfwGetTime();
            }
        }
        if (glfwGetKey (window, pship[sh].shields))
        {
            shield_time[sh]+= elapsed_time;
            if (shield_time[sh] > Shield_life)
            {
                shield_time[sh] = Shield_life+0.1;
                shield_flag[sh] = false;
            }
            else shield_flag[sh] = true;
        }

        if (pship[sh].bomb_timer<0.0 && pship[sh].status)
        {
            if (glfwGetKey (window, pship[sh].bomb))
            {
                alSourcef(bomb_sourceid, AL_GAIN, Sound_volume);
                alSourcePlay(bomb_sourceid);
                Fire_bomb_bullets(sh);
                pship[sh].bomb_timer = Bomb_delay;
            }
        }
    }
}

void Check_joystick(int sh)
{
    static double skip_time[17];

    if (glfwGetGamepadState(pship[sh].jstick, &Gstate))
    {
        if (Gstate.axes[pship[sh].rotate_left] < -0.3)
        {
            pship[sh].angle+= (3.0*TO_RADIANS*(60.0*elapsed_time));
        }
        if (Gstate.axes[pship[sh].rotate_left] > 0.3)
        {
            pship[sh].angle-= (3.0*TO_RADIANS*(60.0*elapsed_time));
        }
        if (Gstate.axes[pship[sh].thrust] > 0.3)
        {
            pship[sh].dirx+= -sin(pship[sh].angle)*Ship_acceleration_factor*(60.0*elapsed_time);
            pship[sh].diry+= cos(pship[sh].angle)*Ship_acceleration_factor*(60.0*elapsed_time);
            pship[sh].thrust_flag = true;
            if (!sound_thrust_flag[sh] && pship[sh].status == SHIP_ACTIVE)
            {
                alSourcef(thrust_sourceid[sh], AL_GAIN, 0.5*Sound_volume);
                alSourcef(thrust_sourceid[sh], AL_PITCH, pship[sh].thrust_pitch);
                alSourcePlay(thrust_sourceid[sh]);
                alSourcei(thrust_sourceid[sh], AL_LOOPING, AL_TRUE);
                sound_thrust_flag[sh] = true;
            }
        }
        else
        {
            // Limit to every 1/60 of second otherwise decelerates too fast
            if (glfwGetTime() - skip_time[sh] >= (1.0/60.0))
            {
                pship[sh].dirx*= Ship_decay_factor;   //Ship speed decay
                pship[sh].diry*= Ship_decay_factor;
                pship[sh].thrust_flag = false;
                skip_time[sh] = glfwGetTime();
            }
            pship[sh].thrust_flag = false;

            if (sound_thrust_flag[sh])
            {
                sound_thrust_flag[sh] = false;
                alSourceStop(thrust_sourceid[sh]);
            }
        }

        if (Gstate.buttons[pship[sh].fire])
        {
            if (!pship[sh].fire_pressed)
            {
                Fire_bullet(sh);
                pship[sh].fire_pressed = true;
            }
        }
        else pship[sh].fire_pressed = false;

        if (pship[sh].bomb_timer<0.0 && pship[sh].status)
        {
            if (Gstate.buttons[pship[sh].bomb])
            {
                alSourcef(bomb_sourceid, AL_GAIN, Sound_volume);
                alSourcePlay(bomb_sourceid);
                Fire_bomb_bullets(sh);
                pship[sh].bomb_timer = Bomb_delay;
            }
        }
        if (Gstate.buttons[pship[sh].shields])
        {
            shield_time[sh]+= elapsed_time;
            if (shield_time[sh] > Shield_life)
            {
                shield_time[sh] = Shield_life+0.1;
                shield_flag[sh] = false;
            }
            else shield_flag[sh] = true;
        }
        else shield_flag[sh] = false;
    }
}

/// Setup up an explosion of a ship or enemy ship
void Start_ship_explosion(int ship, int ship_index, int object, int object_index, int xpos, int ypos)
{
    int lp;
    int explo;
    int explode;
    int start;
    float angle;
    float x, y;
    float rate;
    float temp, temp2;

    if (ship == SHIP)
    {
        pship[ship_index].status = SHIP_EXPLODE;
        if (object == ENEMY_BULLET)
        {
            ebullets[object_index].status = false;
        }
    }
    if (ship == ENEMY) enemy[ship_index].status = SHIP_EXPLODE;

    uniform_real_distribution<float>direction(0.0, 360.0);
    uniform_real_distribution<float>angle_speed(-3.0, 3.0);
    uniform_real_distribution<float>radius(5.0, 15.0);
    uniform_real_distribution<float>speed(10, 2000.0);

    if (ship_explode.empty())
    {
        ship_explode.push_back(ship_explode_struct());
        ship_explode[0].status = false;
        explode=0;
    }
    else
    {
        for (explode=0; explode<ship_explode.size(); explode++)
        {
            if (!ship_explode[explode].status) break;
        }
    }
    if (explode==ship_explode.size())
    {
        if (explode<Max_ship_explode)
        {
            ship_explode.push_back(ship_explode_struct());
            ship_explode[explode].status = false;
        }
    }

    if (explode<Max_ship_explode)
    {
        explo = explode;
        {
            ship_explode[explo].code = vec2i(ship, ship_index);
            ship_explode[explo].xpos = xpos;
            ship_explode[explo].ypos = ypos;
            ship_explode[explo].status = true;
            if (ship == SHIP)
            {
                ship_explode[explo].color = pship[ship_index].color;
                ship_explode[explo].xdir = pship[ship_index].dirx;
                ship_explode[explo].ydir = pship[ship_index].diry;
            }
            else if (ship == ENEMY)
            {
                ship_explode[explo].color = enemy[ship_index].color;
                ship_explode[explo].xdir = enemy[ship_index].dirx;
                ship_explode[explo].ydir = enemy[ship_index].diry;
            }

            start = explo*Ship_explode_debris;
            for (lp=0; lp<Ship_explode_debris; lp++)        // Generate random explosion debris
            {
                angle = direction(rndrock)*TO_RADIANS;
                x = sin(angle);
                y = cos(angle);
                rate = speed(rndrock);
                sh_shipexplode2.direction[start+lp] = vec2(x*rate, y*rate);
                temp = direction(rndrock)*TO_RADIANS;
                temp2 = angle_speed(rndrock)*TO_RADIANS;
                sh_shipexplode2.spin[start+lp] = vec2(temp, temp2);
                sh_shipexplode2.radius[start+lp] = radius(rndrock);
            }

            // Send the new explosion data to the GPU
             //clear_errors();
            ship_explode[explo].time = 0;
            glUseProgram(sh_shipexplode2.program);
            glBindVertexArray(sh_shipexplode2.vao);
            glUniform1f(sh_shipexplode2.xratio_location, x_ratio);
            glUniform1f(sh_shipexplode2.yratio_location, y_ratio);
            glBindBuffer(GL_ARRAY_BUFFER, sh_shipexplode2.direction_vbo);
            glBufferSubData(GL_ARRAY_BUFFER, start*sizeof(vec2), Ship_explode_debris*sizeof(vec2), &sh_shipexplode2.direction[explo*Ship_explode_debris]);
            glBindBuffer(GL_ARRAY_BUFFER, sh_shipexplode2.spin_vbo);
            glBufferSubData(GL_ARRAY_BUFFER, start*sizeof(vec2), Ship_explode_debris*sizeof(vec2), &sh_shipexplode2.spin[explo*Ship_explode_debris]);
            glBindBuffer(GL_ARRAY_BUFFER, sh_shipexplode2.radius_vbo);
            glBufferSubData(GL_ARRAY_BUFFER, start*sizeof(float), Ship_explode_debris*sizeof(float), &sh_shipexplode2.radius[explo*Ship_explode_debris]);
            glBindVertexArray(0);
             //log_errors();

            // Also explode the rock you hit
            if (object == ROCK)
            {
                rocks[object_index].status = false;
                if (Rock_instancing) sh_rocks2.active[object_index] = 0;
                total_rocks--;
                Start_rock_explosion(object_index);
                if (rocks[object_index].stage == BIG_ROCK)
                {
                    int type = rocks[object_index].type;
                    if (type == BRITTLE_ROCK)
                    {
                        Spawn_small_rocks(object_index);
                    }
                    else Spawn_medium_rocks(object_index);
                }
                else if (rocks[object_index].stage == MEDIUM_ROCK) Spawn_small_rocks(object_index);
            }
        }
    }
}

/// Update rock status when your bullet hits one
void Bullet_hit_rock(int b_index, int r_index, circle_line *cl)
{
    bullets[b_index].status = false;
    int type = rocks[r_index].type;

    rocks[r_index].hits--;
    if (rocks[r_index].hits < 1)
    {
        Rock_buffers_changed = true;
        int sh = bullets[b_index].ship;
        pship[sh].score+= rocks[r_index].score;
        pship[sh].add_score+= rocks[r_index].score;
        pship[sh].hits++;
        if (pship[sh].add_score>Extra_ship_points)
        {
            pship[sh].ships++;
            pship[sh].add_score-= Extra_ship_points;
        }

        rocks[r_index].status = false;
        if (Rock_instancing) sh_rocks2.active[r_index] = 0;
        total_rocks--;
        Start_rock_explosion(r_index);
        if (rocks[r_index].stage == BIG_ROCK)
        {
            alSourcef(rk_explode_sourceid[rock1_pntr], AL_GAIN, Sound_volume);
            alSourcef(rk_explode_sourceid[rock1_pntr], AL_PITCH, big_rock_pitch(rndrock));
            alSourcePlay(rk_explode_sourceid[rock1_pntr]);
            rock1_pntr++;
            if (rock1_pntr>= MAX_BR_EXPLODE) rock1_pntr = 0;
            if (type == BRITTLE_ROCK)
            {
                Spawn_small_rocks(r_index);
            }
            else Spawn_medium_rocks(r_index);
        }
        else if (rocks[r_index].stage == MEDIUM_ROCK)
        {
            alSourcef(rk_explode2_sourceid[rock2_pntr], AL_GAIN, Sound_volume);
            alSourcef(rk_explode2_sourceid[rock2_pntr], AL_PITCH, medium_rock_pitch(rndrock));
            alSourcePlay(rk_explode2_sourceid[rock2_pntr]);
            rock2_pntr++;
            if (rock2_pntr>=MAX_MR_EXPLODE) rock2_pntr = 0;
            Spawn_small_rocks(r_index);
        }
        else
        {
            alSourcef(rk_explode3_sourceid[rock3_pntr], AL_GAIN, Sound_volume);
            alSourcef(rk_explode3_sourceid[rock3_pntr], AL_PITCH, small_rock_pitch(rndrock));
            alSourcePlay(rk_explode3_sourceid[rock3_pntr]);
            rock3_pntr++;
            if (rock3_pntr>=MAX_SMR_EXPLODE) rock3_pntr = 0;
            potential_rocks--;
        }
    }
    else
    {
        alSourcef(rock_impact_sourceid, AL_GAIN, 0.2*Sound_volume);
        alSourcePlay(rock_impact_sourceid);
        Start_impact(ROCK, r_index, cl);
    }
}

void Bullet_hit_enemy(int b_index, int e_index)
{
    int sh = bullets[b_index].ship;
    game_score+= Points_enemyship;
    pship[sh].add_score+= Points_enemyship;
    pship[sh].score+= Points_enemyship;
    pship[sh].hits++;
    bullets[b_index].status = false;

    Start_ship_explosion(ENEMY, e_index, -1, -1, enemy[e_index].xpos, enemy[e_index].ypos);
    alSourcef(enemy_explode_sourceid, AL_GAIN, 1.0*Sound_volume);
    alSourcePlay(enemy_explode_sourceid);
}

void Fire_bullet(int sh)
{
    if (demo) return;
    if (!pship[sh].status) return;

    int cnt = levels[game_level-1].num_bullets;

    alSourcef(ship_bullet_sourceid[ship_fire_pntr], AL_PITCH, pship[sh].fire_pitch);
    alSourcef(ship_bullet_sourceid[ship_fire_pntr], AL_GAIN, 0.2*Sound_volume);   // 20% sound volume
    alSourcePlay(ship_bullet_sourceid[ship_fire_pntr]);
    ship_fire_pntr++;
    if (ship_fire_pntr>MAX_FIRE_SND) ship_fire_pntr = 0;

    if (cnt == 1)
    {
        Fire_single_bullet(sh);
        return;
    }

    float start;
    int odd_even = cnt % 2;    // Check if bullet count it odd or even
    if (odd_even == 0)
    {
        start = (float) (cnt / -2) + 0.5;
    }
    else
    {
        start = (float) (cnt / -2);
    }
    for (int lp=0; lp<cnt; lp++)
    {
        Fire_mutliple_bullet(pship[sh].angle + start*TO_RADIANS, sh);
        start+= 1.0;
    }
}

void Fire_single_bullet(int sh)
{
    int lp;
    ship_bullets temp;

    for (lp=0; lp<bullets.size(); lp++)      // Find empty slot for new bullet
    {
        if (!bullets[lp].status)
        {
            break;
        }
    }
    if (lp>= bullets.size())
    {
        bullets.push_back(ship_bullets());
    }

    float x = pship[sh].xpos - pship[sh].l_xpos;
    float y = pship[sh].ypos - pship[sh].l_ypos;

    temp.status = true;
    temp.ship = sh;                    // Ship bullet was fired from
    temp.life = 0;
    temp.l_xpos = pship[sh].xpos;
    temp.l_ypos = pship[sh].ypos;
    temp.xstart = pship[sh].xpos;
    temp.ystart = pship[sh].ypos;
    temp.x_norm = -sin(pship[sh].angle);
    temp.y_norm = cos(pship[sh].angle);
    temp.xdir = temp.x_norm*bullet_speed;
    temp.ydir = temp.y_norm*bullet_speed;
    temp.sx_offset = x;
    temp.sy_offset = y;
    temp.xpos = temp.l_xpos;
    temp.ypos = temp.l_ypos;
    temp.color = pship[sh].color*2.0;
    temp.edge = false;
    bullets[lp] = temp;

    pship[sh].fired++;             // Add to number of bullets fired from this ship
}

void Fire_mutliple_bullet(float angle, int sh)
{
    int lp;
    ship_bullets temp;

    for (lp=0; lp<bullets.size(); lp++)
    {
        if (!bullets[lp].status)
        {
            break;
        }
    }
    if (lp>=bullets.size())
    {
        bullets.push_back(ship_bullets());
    }

    float x = pship[sh].xpos - pship[sh].l_xpos;
    float y = pship[sh].ypos - pship[sh].l_ypos;

    temp.status = true;
    temp.ship = sh;                    // Ship bullet was fired from
    temp.life = 0;
    temp.xpos = pship[sh].xpos;
    temp.ypos = pship[sh].ypos;
    temp.xstart = pship[sh].xpos;
    temp.ystart = pship[sh].ypos;
    temp.x_norm = -sin(angle);
    temp.y_norm = cos(angle);
    temp.xdir = temp.x_norm*bullet_speed;
    temp.ydir = temp.y_norm*bullet_speed;
    temp.sx_offset = x;
    temp.sy_offset = y;
    temp.color = pship[sh].color*2.0;
    temp.edge = false;
    bullets[lp] = temp;

    pship[sh].fired++;
}

void Fire_bomb_bullets(int sh)
{
    if (!pship[sh].status) return;

    float angle = 0;
    bomb_flag = true;
    int index = 0;

    float x = pship[sh].xpos - pship[sh].l_xpos;
    float y = pship[sh].ypos - pship[sh].l_ypos;

    for (int lp=0; lp<BOMB_BULLETS; lp++)
    {
        while (bbullets[index].status) index++;

        if (index>=last_bbullet) last_bbullet++;
        bbullets[index].status = true;
        bbullets[index].life = 0;
        bbullets[index].xpos = pship[sh].xpos;
        bbullets[index].ypos = pship[sh].ypos;
        bbullets[index].xstart = pship[sh].xpos;
        bbullets[index].ystart = pship[sh].ypos;
        bbullets[index].x_norm = -sin(angle);
        bbullets[index].y_norm = cos(angle);
        bbullets[index].xdir = bbullets[index].x_norm*bullet_speed;
        bbullets[index].ydir = bbullets[index].y_norm*bullet_speed;
        bbullets[index].sx_offset = x;
        bbullets[index].sy_offset = y;
        bbullets[index].color = pship[sh].color*2.0;

        bbullets[index].edge = false;
        angle+= TO_RADIANS;
        index++;

        if (index>=MAX_BBULLETS) break;
    }
}

/// ** Bomb Bullet hits a rock.  No points awarded **
void Bbullet_hit_rock(int b_index, int r_index, circle_line *cl)
{
    bbullets[b_index].status = false;

    rocks[r_index].hits--;
    if (rocks[r_index].hits < 1)
    {
        Rock_buffers_changed = true;
        rocks[r_index].status = false;
        if (Rock_instancing) sh_rocks2.active[r_index] = 0;
        total_rocks--;
        Start_rock_explosion(r_index);
        if (rocks[r_index].stage == BIG_ROCK)
        {
            alSourcef(rk_explode_sourceid[rock1_pntr], AL_GAIN, Sound_volume);
            alSourcef(rk_explode_sourceid[rock1_pntr], AL_PITCH, big_rock_pitch(rndrock));
            alSourcePlay(rk_explode_sourceid[rock1_pntr]);
            rock1_pntr++;
            if (rock1_pntr>= MAX_BR_EXPLODE) rock1_pntr = 0;
            if (rocks[r_index].type == BRITTLE_ROCK)
            {
                Spawn_small_rocks(r_index);
            }
            else Spawn_medium_rocks(r_index);
        }
        else if (rocks[r_index].stage == MEDIUM_ROCK)
        {
            alSourcef(rk_explode2_sourceid[rock2_pntr], AL_GAIN, Sound_volume);
            alSourcef(rk_explode2_sourceid[rock2_pntr], AL_PITCH, medium_rock_pitch(rndrock));
            alSourcePlay(rk_explode2_sourceid[rock2_pntr]);
            rock2_pntr++;
            if (rock2_pntr>=MAX_MR_EXPLODE) rock2_pntr = 0;
            Spawn_small_rocks(r_index);
        }
        else
        {
            alSourcef(rk_explode3_sourceid[rock3_pntr], AL_GAIN, Sound_volume);
            alSourcef(rk_explode3_sourceid[rock3_pntr], AL_PITCH, small_rock_pitch(rndrock));
            alSourcePlay(rk_explode3_sourceid[rock3_pntr]);
            rock3_pntr++;
            if (rock3_pntr>=MAX_SMR_EXPLODE) rock3_pntr = 0;
            potential_rocks--;
        }
    }
    else
    {
        Start_impact(ROCK, r_index, cl);
    }
}

void Bbullet_hit_enemy(int b_index, int e_index)
{
    bbullets[b_index].status = false;
    Start_ship_explosion(ENEMY, e_index, -1, -1, enemy[e_index].xpos, enemy[e_index].ypos);
    alSourcef(enemy_explode_sourceid, AL_GAIN, Sound_volume);
    alSourcePlay(enemy_explode_sourceid);
}

void Spawn_medium_rocks(int index)
{
    int lp;
    bool overflow = false;
    float x_start = rocks[index].xpos;
    float y_start = rocks[index].ypos;
    vec3 color = rocks[index].color;
    int type = rocks[index].type;

    uniform_int_distribution<int>shape(0, Medium_rock_shapes-1);
    uniform_int_distribution<int>c_shape(0, custom_rock_count-1);
    uniform_real_distribution<float>angle_step(-3.0, 3.0);
    uniform_real_distribution<float>direction(1.0, 359.0);
    uniform_real_distribution<float>rock_size(18.0, 24.0);
    uniform_real_distribution<float>rock_speed(1.0, 2.5);

    int slot = 0;
    for (lp=0; lp<5; lp++)
    {
        if (!overflow)
        {
            while (rocks[slot].status)
            {
                slot++;
                if (slot>= rocks.size())
                {
                    rocks.push_back(rock_data());
                    rocks[slot].status = false;
                    overflow = true;
                }
            }
        }
        else
        {
            slot++;
            rocks.push_back(rock_data());
        }
        rocks[slot].hits = Hits_standard_medium;
        rocks[slot].score = Points_standard_medium;
        if (type == HARD_ROCK)
        {
            rocks[slot].hits = Hits_hard_medium;
            rocks[slot].score = Points_hard_medium;
        }
        rocks[slot].mass = 30.0;
        rocks[slot].xpos = x_start;
        rocks[slot].ypos = y_start;
        rocks[slot].angle = 0;
        rocks[slot].angle_step = angle_step(rndrock)*TO_RADIANS;
        float angle = direction(rndrock) * TO_RADIANS;
        float speed = rock_speed(rndrock);
        rocks[slot].type = type;
        if (type == FAST_ROCK)
        {
            speed = 8.0;
            rocks[slot].score = Points_fast_medium;
            rocks[slot].hits = Hits_fast_medium;
        }
        rocks[slot].color = color;
        rocks[slot].xdir = sin(angle) * speed;
        rocks[slot].ydir = cos(angle) * speed;
        rocks[slot].radius = rock_size(rndrock);
        rocks[slot].status = true;
        rocks[slot].stage = MEDIUM_ROCK;
        int rock_shape;
        if (Custom_rocks_flag)
        {
            rock_shape = c_shape(rndrock) + Custom_rock_shapes_pntr;
        }
        else
        {
            rock_shape = shape(rndrock) + Big_rock_shapes;
        }
        rocks[slot].pntr = rock_table[rock_shape];
        rocks[slot].s_pntr = rock_table[rock_shape];

        if (Rock_instancing)
        {
            sh_rocks2.active[slot] = 1;
            sh_rocks2.position[slot] = vec2(x_start, y_start);
            sh_rocks2.angle[slot] = 0;
            sh_rocks2.radius[slot] = rocks[slot].radius;
            sh_rocks2.color[slot] = color;
        }
    }
    total_rocks+= 5;
    Rock_buffers_changed = true;
}

void Spawn_small_rocks(int index)
{
    int lp;
    bool overflow = false;
    float x_start = rocks[index].xpos;
    float y_start = rocks[index].ypos;
    vec3 color = rocks[index].color;
    int type = rocks[index].type;
    int amount = 5;
    if (type == BRITTLE_ROCK)
    {
        amount = 50;
        type = COMMON_ROCK;
        color = Brittle_color;

    }
    uniform_int_distribution<int>shape(0, Small_rock_shapes-1);
    uniform_int_distribution<int>c_shape(0, custom_rock_count-1);
    uniform_real_distribution<float>angle_step(-3.0, 3.0);
    uniform_real_distribution<float>direction(1.0, 359.0);
    uniform_real_distribution<float>rock_size(8.0, 11.0);
    uniform_real_distribution<float>rock_speed(2.0, 3.5);

    int slot = 0;
    for (lp=0; lp<amount; lp++)
    {
        if (!overflow)
        {
            while (rocks[slot].status)
            {
                slot++;
                if (slot>= rocks.size())
                {
                    rocks.push_back(rock_data());
                    rocks[slot].status = false;
                    overflow = true;
                }
            }
        }
        else
        {
            slot++;
            rocks.push_back(rock_data());
        }

        if (type == BRITTLE_ROCK)
        {
            rocks[slot].hits = Hits_brittle_small;
            rocks[slot].score = Points_brittle_small;
        }
        if (type == COMMON_ROCK)
        {
            rocks[slot].hits = Hits_standard_small;
            rocks[slot].score = Points_standard_small;
        }
        if (type == HARD_ROCK)
        {
            rocks[slot].hits = Hits_hard_small;
            rocks[slot].score = Points_hard_small;
        }
        rocks[slot].mass = 10.0;
        rocks[slot].xpos = x_start;
        rocks[slot].ypos = y_start;
        rocks[slot].angle = 0;
        rocks[slot].angle_step = angle_step(rndrock)*TO_RADIANS;
        float angle = direction(rndrock) * TO_RADIANS;
        float speed = rock_speed(rndrock);
        rocks[slot].type = type;
        if (type == FAST_ROCK)
        {
            speed = 12.0;
            rocks[slot].score = Points_fast_small;
            rocks[slot].hits = Hits_fast_small;
        }
        rocks[slot].color = color;
        rocks[slot].xdir = sin(angle) * speed;
        rocks[slot].ydir = cos(angle) * speed;
        rocks[slot].radius = rock_size(rndrock);
        rocks[slot].status = true;
        rocks[slot].stage = SMALL_ROCK;
        int rock_shape;
        if (Custom_rocks_flag)
        {
            rock_shape = c_shape(rndrock) + Custom_rock_shapes_pntr;
        }
        else
        {
            rock_shape = shape(rndrock) + Big_rock_shapes + Medium_rock_shapes;
        }
        rocks[slot].pntr = rock_table[rock_shape];
        rocks[slot].s_pntr = rock_table[rock_shape];

        if (Rock_instancing)
        {
            sh_rocks2.active[slot] = 1;
            sh_rocks2.position[slot] = vec2(x_start, y_start);
            sh_rocks2.angle[slot] = 0;
            sh_rocks2.radius[slot] = rocks[slot].radius;
            sh_rocks2.color[slot] = color;
        }
    }
    total_rocks+= amount;
    Rock_buffers_changed = true;
}

void Start_rock_explosion(int index)
{
    int explode;
    int lp, lp2;
    int start;
    float angle;
    float speed;
    float x, y;

    uniform_real_distribution<float>direction(0.0, 360.0);
    uniform_real_distribution<float>rock_speed(20.0, 1000.0);

    if (rock_explode.empty())
    {
        rock_explode.push_back(rock_explode_struct());
        rock_explode[0].status = false;
        explode=0;
    }
    else
    {
        for (explode=0; explode<rock_explode.size(); explode++)
        {
            if (!rock_explode[explode].status) break;
        }
    }
    if (explode==rock_explode.size())
    {
        if (explode<Max_rock_explode)
        {
            rock_explode.push_back(rock_explode_struct());
            rock_explode[explode].status = false;
        }
    }

    if (explode<Max_rock_explode)
    {
        lp = explode;
        explosion_cnt++;
        start = lp*Rock_explode_debris;
        rock_explode[lp].status = true;
        rock_explode[lp].xpos = rocks[index].xpos;
        rock_explode[lp].ypos = rocks[index].ypos;
        rock_explode[lp].xdir = rocks[index].xdir;
        rock_explode[lp].ydir = rocks[index].ydir;
        rock_explode[lp].time = glfwGetTime();
        rock_explode[lp].color = rocks[index].color;
        for (lp2=0; lp2<Rock_explode_debris; lp2++)
        {
            speed = rock_speed(rndrock);
            angle = direction(rndrock)*TO_RADIANS;
            x = sin(angle)*speed;
            y = cos(angle)*speed;
            sh_rexplode.direction[start+lp2] = vec2(x, y);
        }
        glUseProgram(sh_rexplode.program);
        glBindVertexArray(sh_rexplode.vao);
        glBindBuffer(GL_ARRAY_BUFFER, sh_rexplode.direction_vbo);
        glBufferSubData(GL_ARRAY_BUFFER, start*sizeof(vec2), Rock_explode_debris * sizeof(vec2), &sh_rexplode.direction[start]);
        glBindVertexArray(0);
    }

    uniform_real_distribution<float>mini_speed(10.0, 500.0);
    uniform_real_distribution<float>mini_size(3.0, 5.0);
    uniform_real_distribution<float>radius(0.8, 1.05);

    if (mini_explode.empty())
    {
        mini_explode.push_back(mini_explode_struct());
        mini_explode[0].status = false;
        explode=0;
    }
    else
    {
        for (explode=0; explode<mini_explode.size(); explode++)
        {
            if (!mini_explode[explode].status) break;
        }
    }
    if (explode==mini_explode.size())
    {
        if (explode<Max_mini_explode)
        {
            mini_explode.push_back(mini_explode_struct());
            mini_explode[explode].status = false;
        }
    }

    if (explode<Max_mini_explode)
    {
        lp = explode;
        start = lp*Mini_explode_debris;
        mini_explode[lp].status = true;
        mini_explode[lp].xpos = rocks[index].xpos;
        mini_explode[lp].ypos = rocks[index].ypos;
        mini_explode[lp].xdir = rocks[index].xdir;
        mini_explode[lp].ydir = rocks[index].ydir;
        mini_explode[lp].time = glfwGetTime();
        mini_explode[lp].color = rocks[index].color;
        for (lp2=0; lp2<Mini_explode_debris; lp2++)
        {
            speed = mini_speed(rndrock);
            angle = direction(rndrock)*TO_RADIANS;
            x = sin(angle)*speed;
            y = cos(angle)*speed;
            sh_miniexplode.direction[start+lp2] = vec2(x, y);
            sh_miniexplode.angle[start+lp2] = direction(rndrock)*TO_RADIANS;
            sh_miniexplode.radius[start+lp2] = mini_size(rndrock);
        }

        // For outline rocks
        float step_angle = 360.0 / 10.0;
        float angle = 0;
        for (lp2=0; lp2<10; lp2++)
        {
            x = sin(angle*TO_RADIANS);
            y = cos(angle*TO_RADIANS);
            float r = radius(rndrock);
            vec2 temp = vec2(x*r, y*r);
            sh_miniexplode.points[lp*10 + lp2] = temp;
            angle+= step_angle;
        }

        // For solid rocks
        sh_miniexplode.points[lp*10 + 10*Max_mini_explode] = vec2(0, 0);
        for (lp2=0; lp2<10; lp2++)
        {
            sh_miniexplode.points[lp*10 + 10*Max_mini_explode + lp2+1] = sh_miniexplode.points[lp*10 + lp2];
        }
        sh_miniexplode.points[lp*10 + 10*Max_mini_explode + 11] = sh_miniexplode.points[lp*10];

        glUseProgram(sh_miniexplode.program);
        glBindVertexArray(sh_miniexplode.vao);
        glBindBuffer(GL_ARRAY_BUFFER, sh_miniexplode.direction_vbo);
        glBufferSubData(GL_ARRAY_BUFFER, start*sizeof(vec2), Mini_explode_debris * sizeof(vec2), &sh_miniexplode.direction[start]);
        glBindBuffer(GL_ARRAY_BUFFER, sh_miniexplode.angle_vbo);
        glBufferSubData(GL_ARRAY_BUFFER, start*sizeof(float), Mini_explode_debris * sizeof(float), &sh_miniexplode.angle[start]);
        glBindBuffer(GL_ARRAY_BUFFER, sh_miniexplode.radius_vbo);
        glBufferSubData(GL_ARRAY_BUFFER, start*sizeof(float), Mini_explode_debris * sizeof(float), &sh_miniexplode.radius[start]);

        glBindBuffer(GL_ARRAY_BUFFER, sh_miniexplode.points_vbo);
        glBufferSubData(GL_ARRAY_BUFFER, lp*10*sizeof(vec2), 10*sizeof(vec2), &sh_miniexplode.points[lp*10]);
        glBufferSubData(GL_ARRAY_BUFFER, (lp*12 + 10*Max_mini_explode)*sizeof(vec2), 12*sizeof(vec2), &sh_miniexplode.points[lp*12 + 10*Max_mini_explode]);
        glBindVertexArray(0);
    }
}

/// Initiates bullet impact if bullet doesn't destroy a rock or ship
void Start_impact(int object, int r_index, circle_line *cl)
{
    int lp, lp2;
    int explode;
    int start;
    float angle, angle2;
    float speed;
    float x, y;

    uniform_real_distribution<float>direction(-25.0, 25.0);
    uniform_real_distribution<float>rock_speed(10.0, 500.0);

    if (impact.empty())
    {
        impact.push_back(impact_struct());
        impact[0].status = false;
        explode=0;
    }
    else
    {
        for (explode=0; explode<impact.size(); explode++)
        {
            if (!impact[explode].status) break;
        }
    }
    if (explode==impact.size())
    {
        if (explode<Max_impact)
        {
            impact.push_back(impact_struct());
            impact[explode].status = false;
        }
    }

    if (explode>=Max_impact) return;

    lp = explode;
    impact_cnt++;
    start = lp*Impact_particles;
    impact[lp].status = true;
    impact[lp].rock_pntr = r_index;

    if (object == ROCK)
    {
        impact[lp].xpos = cl->tx - rocks[r_index].xpos;
        impact[lp].ypos = cl->ty - rocks[r_index].ypos;
        angle = atan2(rocks[r_index].ypos - cl->ty, rocks[r_index].xpos - cl->tx);

        impact[lp].xrock = rocks[r_index].xpos;
        impact[lp].yrock = rocks[r_index].ypos;
        impact[lp].xdir = rocks[r_index].xdir;
        impact[lp].ydir = rocks[r_index].ydir;
        impact[lp].color = rocks[r_index].color;
    }
    else if (object == SHIP)
    {
        impact[lp].xpos = cl->tx - pship[r_index].xpos;
        impact[lp].ypos = cl->ty - pship[r_index].ypos;
        angle = atan2(pship[r_index].ypos - cl->ty, pship[r_index].xpos - cl->tx);

        impact[lp].xrock = pship[r_index].xpos;
        impact[lp].yrock = pship[r_index].ypos;
        impact[lp].xdir = pship[r_index].dirx;
        impact[lp].ydir = pship[r_index].diry;
        impact[lp].color = pship[r_index].color;
    }

    impact[lp].time = glfwGetTime();
    for (lp2=0; lp2<Impact_particles; lp2++)
    {
        speed = rock_speed(rndrock);
        angle2 = angle + direction(rndrock)*TO_RADIANS;
        angle2+= 180.0*TO_RADIANS;
        x = cos(angle2)*speed;
        y = sin(angle2)*speed;
        sh_impact.direction[start+lp2] = vec2(x, y);
    }
    glUseProgram(sh_impact.program);
    glBindVertexArray(sh_impact.vao);
    glBindBuffer(GL_ARRAY_BUFFER, sh_impact.direction_vbo);
    glBufferSubData(GL_ARRAY_BUFFER, start*sizeof(vec2), Impact_particles * sizeof(vec2), &sh_impact.direction[start]);
    glBindVertexArray(0);
}

/// Initiates bullet impact if bullet doesn't destroy a rock or ship
void Start_drexplode(int index)
{
    int lp, lp2;
    int explode;
    int start;
    float angle, angle2;
    float speed;
    float x, y;

    uniform_real_distribution<float>direction(0.0, 359.99);
    uniform_real_distribution<float>rock_speed(10.0, 1000.0);
    uniform_real_distribution<float>cfactor(0.5, 1.0);
    uniform_real_distribution<float>psize(1.5, 4.0);

    if (drexplode.empty())
    {
        drexplode.push_back(drexplode_struct());
        drexplode[0].status = false;
        explode=0;
    }
    else
    {
        for (explode=0; explode<drexplode.size(); explode++)
        {
            if (!drexplode[explode].status) break;
        }
    }
    if (explode==drexplode.size())
    {
        if (explode<MAX_EXDRONE)
        {
            drexplode.push_back(drexplode_struct());
            drexplode[explode].status = false;
        }
    }

    if (explode>=MAX_EXDRONE) return;

    lp = explode;
    start = lp*DRONE_PARTICLES;
    drexplode[lp].status = true;
    drexplode[lp].xdrone = drones[index].xpos;
    drexplode[lp].ydrone = drones[index].ypos;
    drexplode[lp].xdir = -sin(drones[index].angle) * drones[index].speed;
    drexplode[lp].ydir = cos(drones[index].angle) * drones[index].speed;
    drexplode[lp].color = drones[index].color;
    drexplode[lp].time = glfwGetTime();
    for (lp2=0; lp2<DRONE_PARTICLES; lp2++)
    {
        speed = rock_speed(rndrock);
        angle = direction(rndrock)*TO_RADIANS;
        x = cos(angle)*speed;
        y = sin(angle)*speed;
        sh_drexplode.direction[start+lp2] = vec2(x, y);
        sh_drexplode.col[start+lp2] = drexplode[lp].color * cfactor(rndrock);
        sh_drexplode.psize[start+lp2] = psize(rndrock);
    }
    glUseProgram(sh_drexplode.program);
    glBindVertexArray(sh_drexplode.vao);
    glBindBuffer(GL_ARRAY_BUFFER, sh_drexplode.direction_vbo);
    glBufferSubData(GL_ARRAY_BUFFER, start*sizeof(vec2), DRONE_PARTICLES * sizeof(vec2), &sh_drexplode.direction[start]);
    glBindBuffer(GL_ARRAY_BUFFER, sh_drexplode.col_vbo);
    glBufferSubData(GL_ARRAY_BUFFER, start*sizeof(vec3), DRONE_PARTICLES * sizeof(vec3), &sh_drexplode.col[start]);
    glBindBuffer(GL_ARRAY_BUFFER, sh_drexplode.psize_vbo);
    glBufferSubData(GL_ARRAY_BUFFER, start*sizeof(float), DRONE_PARTICLES * sizeof(float), &sh_drexplode.psize[start]);
    glBindVertexArray(0);
}

struct sort_rocks
{
    int index;
    float dist;
};
vector<sort_rocks> rsort;

bool rocksort(const sort_rocks &lhs, const sort_rocks &rhs)
{
    return lhs.dist < rhs.dist;
}

void Spawn_newship(int num)
{
    float x, y;
    float tdist;
    float xspawn, yspawn;
    float left_test = (Xmax/2.0 - Xmax/4.0);
    float right_test = (Xmax/2.0 + Xmax/4.0);
    float up_test = (Ymax/2.0 + Ymax/4.0);
    float down_test = (Ymax/2.0 - Ymax/4.0);
    uniform_real_distribution<float>xtry(left_test, right_test);
    uniform_real_distribution<float>ytry(down_test, up_test);

    rsort.clear();
    sort_rocks rs;
    xspawn = xtry(rndrock);
    yspawn = ytry(rndrock);
    int x_index = (int) ((xspawn+100)/Xgrid);
    int y_index = (int) ((yspawn+100)/Ygrid);

    int cnt = rgrid[x_index][y_index].size();
    vec2i *spnt = rgrid[x_index][y_index].data();
    for (int lp=0; lp<cnt; lp++)
    {
        if (spnt[lp].v[0] == ROCK)
        {
            int rck = spnt[lp].v[1];
            rs.index = rck;
            x = rocks[rck].xpos;
            y = rocks[rck].ypos;
            rs.dist = sqrt((x-xspawn)*(x-xspawn) + (y-yspawn)*(y-yspawn));
            rsort.push_back(rs);
        }
    }

    if (!rsort.empty())
    {
        sort(rsort.begin(), rsort.end(), rocksort);
        tdist = 100;
        pship[num].spawn_tries++;
        if (pship[num].spawn_tries>100)
        {
            tdist = 100-(pship[num].spawn_tries-100);
            if (tdist<0) tdist = 0;
        }
        if (rsort[0].dist > tdist)
        {
            pship[num].status = SHIP_ACTIVE;
            pship[num].angle = 0;
            pship[num].xpos = xspawn;
            pship[num].ypos = yspawn;
            pship[num].dirx = 0;
            pship[num].diry = 0;
            spawn_shield_flag[num] = true;
            spawn_shield_timer[num] = glfwGetTime();
            shield_time[num] = 0.0;
            pship[num].spawn_tries = 0;
        }
    }
    else
    {
        pship[num].status = SHIP_ACTIVE;
        pship[num].angle = 0;
        pship[num].xpos = xspawn;
        pship[num].ypos = yspawn;
        pship[num].dirx = 0;
        pship[num].diry = 0;
        spawn_shield_flag[num] = true;
        spawn_shield_timer[num] = glfwGetTime();
        shield_time[num] = 0.0;
        pship[num].spawn_tries = 0;
    }
}

void Start_new_game()
{
    int lp;
    Xmax = pre_Xmax;
    Ymax = pre_Ymax;
    x_ratio = (float) g_win_width / Xmax;
    y_ratio = (float) g_win_height / Ymax;
    reso_factor = pre_reso_factor;
    ebullet_life = pre_ebullet_life;

    for (lp=0; lp<num_players; lp++)
    {
        pship[lp].angle = 0;
        pship[lp].mass = 1.0;
        pship[lp].status = true;
        pship[lp].ships = Start_ships;
        pship[lp].add_score = 0;
        pship[lp].score = 0;
        pship[lp].fired = 0;
        pship[lp].hits = 0;
        pship[lp].dirx = 0.0;
        pship[lp].diry = 0.0;
        pship[lp].spawn_tries = 0;
    }

    Set_ship_positions(num_players);

    pship_status = true;
    bullets.clear();
    ebullets.clear();
    mini_explode.clear();
    impact.clear();
    bullets.clear();
    demo = false;
    game_level = Starting_level;
    num_enemy = levels[game_level-1].num_enemy;
    enemy.clear();

    for (lp=0; lp<num_players; lp++)
    {
        pship[lp].add_score = 0;
    }
    game_score = 0;
    Start_level(game_level);
    game_over = false;
    score_flag = true;
}

void Fire_single_random_enemy_bullet(float angle, int index)
{
    int lp;
    enemy_bullets eb;

    eb.status = true;
    eb.edge = false;
    eb.life = 0;
    eb.xpos = enemy[index].xpos;
    eb.ypos = enemy[index].ypos;
    eb.xstart = enemy[index].xpos;
    eb.ystart = enemy[index].ypos;
    eb.color = enemy[index].color*2.0;
    eb.x_norm = -sin(angle);
    eb.y_norm = cos(angle);
    eb.xdir = eb.x_norm*bullet_speed + enemy[index].dirx;
    eb.ydir = eb.y_norm*bullet_speed + enemy[index].diry;

    for (lp=0; lp<ebullets.size(); lp++)
    {
        if (!ebullets[lp].status)
        {
            break;
        }
    }
    if (lp>= ebullets.size()) ebullets.push_back(enemy_bullets());
    ebullets[lp] = eb;
}

void Fire_random_enemy_bullets(int index)
{
    uniform_real_distribution<float>angle(0.0, 360.0);

    int code = enemy[index].code;
    float dir = angle(rndrock)*TO_RADIANS;

    if (code == 0)
    {
        Fire_single_random_enemy_bullet(dir, index);
        return;
    }
    code++;
    float start;
    int odd_even = code % 2;
    if (odd_even == 0)
    {
        start = (float) (code/-2) + 0.5;
    }
    else
    {
        start = (float) (code/-2);
    }
    for (int lp=0; lp<code; lp++)
    {
        Fire_single_random_enemy_bullet(dir + start*TO_RADIANS, index);
        start+= 1.0;
    }
}

void Fire_direct_single_enemy_bullet(int en, vec2 dir, vec2 norm)
{
    int lp;
    enemy_bullets eb;

    eb.status = true;
    eb.edge = false;
    eb.life = 0;
    eb.xpos = enemy[en].xpos;
    eb.ypos = enemy[en].ypos;
    eb.xstart = enemy[en].xpos;
    eb.ystart = enemy[en].ypos;
    eb.color = enemy[en].color*2.0;
    eb.x_norm = norm.v[0];
    eb.y_norm = norm.v[1];
    eb.xdir = dir.v[0];
    eb.ydir = dir.v[1];

    for (lp=0; lp<ebullets.size(); lp++)
    {
        if (!ebullets[lp].status)
        {
            break;
        }
    }
    if (lp>= ebullets.size()) ebullets.push_back(enemy_bullets());
    ebullets[lp] = eb;
}

void Fire_direct_enemy_bullets(int sh, int en)
{
    enemy_bullets eb;
    int code = enemy[en].code;
    vec2 r_dir;
    vec2 r_norm;

    float dirx = pship[sh].xpos - enemy[en].xpos;
    float diry = pship[sh].ypos - enemy[en].ypos;
    float len = sqrt(dirx*dirx + diry*diry);
    float x_norm = dirx/len;
    float y_norm = diry/len;
    dirx = x_norm*bullet_speed;
    diry = y_norm*bullet_speed;

    if (code == 0)
    {
        Fire_direct_single_enemy_bullet(en, vec2(dirx, diry), vec2(x_norm, y_norm));
        return;
    }
    code++;
    int odd_even = code % 2;
    float start;
    if (odd_even == 0)
    {
        start = (float) (code/-2)+0.5;
    }
    else
    {
        start = (float) (code/-2);
    }
    for (int lp=0; lp<code; lp++)
    {
        r_dir = Rotate_2f(start*TO_RADIANS, vec2(dirx, diry));
        r_norm = Rotate_2f(start*TO_RADIANS, vec2(x_norm, y_norm));
        Fire_direct_single_enemy_bullet(en, r_dir, r_norm);
        start+= 1.0;
    }
}

void Fire_demo_bullets(int rck, int en)
{
    enemy_bullets eb;
    vec2 r_dir;
    vec2 r_norm;
    int code = enemy[en].code;

    float xdir = rocks[rck].xpos - enemy[en].xpos;   // Direction to rock
    float ydir = rocks[rck].ypos - enemy[en].ypos;
    float dist = sqrt(xdir*xdir + ydir*ydir);           // Distance to rock
    float rtime = dist / (bullet_speed*(Bullet_speed_factor*0.0166));  // Enemy bullet time to rock

    float rxpos = (rocks[rck].xpos + rocks[rck].xdir*(Rock_speed_factor*0.0166)*rtime);
    float rypos = (rocks[rck].ypos + rocks[rck].ydir*(Rock_speed_factor*0.0166)*rtime);

    xdir = rxpos - enemy[en].xpos;
    ydir = rypos - enemy[en].ypos;
    dist = sqrt(xdir*xdir + ydir*ydir);

    xdir/= dist;
    ydir/= dist;
    float x_norm = xdir;
    float y_norm = ydir;
    xdir*= bullet_speed;
    ydir*= bullet_speed;

    if (code == 0)
    {
        Fire_direct_single_enemy_bullet(en, vec2(xdir, ydir), vec2(x_norm, y_norm));
        return;
    }
    code++;
    int odd_even = code % 2;
    float start;
    if (odd_even == 0)
    {
        start = (float) (code/-2)+0.5;
    }
    else
    {
        start = (float) (code/-2);
    }
    for (int lp=0; lp<code; lp++)
    {
        r_dir = Rotate_2f(start*TO_RADIANS, vec2(xdir, ydir));
        r_norm = Rotate_2f(start*TO_RADIANS, vec2(x_norm, y_norm));
        Fire_direct_single_enemy_bullet(en, r_dir, r_norm);
        start+= 1.0;
    }
}

void Fire_enemy_bullet(int index)
{
    int lp2;
    static int cnt = 0;

    if (demo)
    {
        if (total_rocks<1) return;
    }

    if (!demo)
    {
        if (enemy[index].type == BIG_ENEMY)
        {
            Fire_random_enemy_bullets(index);
            alSourcef(enemy_bullet_sourceid, AL_GAIN, Sound_volume);
            alSourcePlay(enemy_bullet_sourceid);
        }
        else
        {
            // Enemy ship fires at you
            int sh;
            vector<int>active;
            active.clear();
            for (int el=0; el<num_players; el++)
            {
                if (pship[el].status == SHIP_ACTIVE) active.push_back(el);
            }
            if (!active.empty())
            {
                if (active.size() == 1) sh = active[0];
                else
                {
                    uniform_int_distribution<int>ship(1, active.size());
                    sh = active[ship(rndrock)-1];
                }
                Fire_direct_enemy_bullets(sh, index);
                alSourcef(enemy_bullet_sourceid, AL_GAIN, Sound_volume);
                alSourcePlay(enemy_bullet_sourceid);
            }
            else return;  // No ships to shoot at.
        }
    }
    else
    {
        // Demo mode:  enemy ship aims for a rock
        for (lp2=cnt; lp2<rocks.size(); lp2++)
        {
            if (rocks[lp2].status)
            {
                Fire_demo_bullets(lp2, index);
                cnt = lp2+1;
                break;
            }
        }
        if (lp2>=rocks.size())
        {
            cnt=0;
            return;
        }
    }
}

/// ** Bullet hit a rock **
void Ebullet_hit_rock(int b_index, int r_index, circle_line *cl)
{
    ebullets[b_index].status = false;     // Disable bullet that hit rock
    rocks[r_index].hits--;
    if (rocks[r_index].hits < 1)
    {
        Rock_buffers_changed = true;
        rocks[r_index].status = false;
        if (Rock_instancing) sh_rocks2.active[r_index] = 0;
        total_rocks--;
        Start_rock_explosion(r_index);
        if (rocks[r_index].stage == BIG_ROCK)
        {
            if (!demo)
            {
                alSourcef(rk_explode_sourceid[rock1_pntr], AL_GAIN, Sound_volume);
                alSourcef(rk_explode_sourceid[rock1_pntr], AL_PITCH, big_rock_pitch(rndrock));
                alSourcePlay(rk_explode_sourceid[rock1_pntr]);
                rock1_pntr++;
                if (rock1_pntr>= MAX_BR_EXPLODE) rock1_pntr = 0;
            }
            if (rocks[r_index].type == BRITTLE_ROCK)
            {
                Spawn_small_rocks(r_index);
            }
            else Spawn_medium_rocks(r_index);
        }
        else if (rocks[r_index].stage == MEDIUM_ROCK)
        {
            if (!demo)
            {
                alSourcef(rk_explode2_sourceid[rock2_pntr], AL_GAIN, Sound_volume);
                alSourcef(rk_explode2_sourceid[rock2_pntr], AL_PITCH, medium_rock_pitch(rndrock));
                alSourcePlay(rk_explode2_sourceid[rock2_pntr]);
                rock2_pntr++;
                if (rock2_pntr>=MAX_MR_EXPLODE) rock2_pntr = 0;
            }
            Spawn_small_rocks(r_index);
        }
        else
        {
            if (!demo)
            {
                alSourcef(rk_explode3_sourceid[rock3_pntr], AL_GAIN, Sound_volume);
                alSourcef(rk_explode3_sourceid[rock3_pntr], AL_PITCH, small_rock_pitch(rndrock));
                alSourcePlay(rk_explode3_sourceid[rock3_pntr]);
                rock3_pntr++;
                if (rock3_pntr>=MAX_SMR_EXPLODE) rock3_pntr = 0;
            }
            potential_rocks--;
        }
    }
    else
    {
        if (!demo)
        {
            alSourcef(rock_impact_sourceid, AL_GAIN, Sound_volume);
            alSourcePlay(rock_impact_sourceid);
        }
        Start_impact(ROCK, r_index, cl);
    }
}

///  ***** Position player ships at beginning of game *****
void Set_ship_positions(int num)
{
    float x = Xmax/2.0;
    float y = Ymax/2.0;
    float lp;
    int index=0;

    switch(num)
    {
        case 1:
            pship[0].xpos = x;
            pship[0].ypos = y;
            break;
        case 2:
            pship[0].xpos = x-50.0;
            pship[0].ypos = y;
            pship[1].xpos = x+50.0;
            pship[1].ypos = y;
            break;
        case 3:
            for (lp=0.0; lp<CIRCLE; lp+= (CIRCLE/3.0))
            {
                pship[index].xpos = sin(lp)*100+x;
                pship[index].ypos = cos(lp)*100+y;
                index++;
            }
            break;
        case 4:
            for (lp=0.0; lp<CIRCLE; lp+= (CIRCLE/4.0))
            {
                pship[index].xpos = sin(lp)*100+x;
                pship[index].ypos = cos(lp)*100+y;
                index++;
            }
            break;
        case 5:
            for (lp=0.0; lp<CIRCLE; lp+= (CIRCLE/5.0))
            {
                pship[index].xpos = sin(lp)*110+x;
                pship[index].ypos = cos(lp)*110+y;
                index++;
            }
            break;
        case 6:
            for (lp=0.0; lp<CIRCLE; lp+= (CIRCLE/6.0))
            {
                pship[index].xpos = sin(lp)*110+x;
                pship[index].ypos = cos(lp)*110+y;
                index++;
            }
            break;
        case 7:
            for (lp=0.0; lp<CIRCLE; lp+= (CIRCLE/7.0))
            {
                pship[index].xpos = sin(lp)*120+x;
                pship[index].ypos = cos(lp)*120+y;
                index++;
            }
            break;
        case 8:
            for (lp=0.0; lp<CIRCLE; lp+= (CIRCLE/8.0))
            {
                pship[index].xpos = sin(lp)*120+x;
                pship[index].ypos = cos(lp)*120+y;
                index++;
            }
            break;
        case 9:
            for (lp=0.0; lp<CIRCLE; lp+= (CIRCLE/9.0))
            {
                pship[index].xpos = sin(lp)*130+x;
                pship[index].ypos = cos(lp)*130+y;
                index++;
            }
            break;
        case 10:
            for (lp=0.0; lp<CIRCLE; lp+= (CIRCLE/10.0))
            {
                pship[index].xpos = sin(lp)*130+x;
                pship[index].ypos = cos(lp)*130+y;
                index++;
            }
            break;
        case 11:
            for (lp=0.0; lp<CIRCLE; lp+= (CIRCLE/11.0))
            {
                pship[index].xpos = sin(lp)*140+x;
                pship[index].ypos = cos(lp)*140+y;
                index++;
            }
            break;
        case 12:
            for (lp=0.0; lp<CIRCLE; lp+= (CIRCLE/12.0))
            {
                pship[index].xpos = sin(lp)*140+x;
                pship[index].ypos = cos(lp)*140+y;
                index++;
            }
            break;
        case 13:
            for (lp=0.0; lp<CIRCLE; lp+= (CIRCLE/13.0))
            {
                pship[index].xpos = sin(lp)*150+x;
                pship[index].ypos = cos(lp)*150+y;
                index++;
            }
            break;
        case 14:
            for (lp=0.0; lp<CIRCLE; lp+= (CIRCLE/14.0))
            {
                pship[index].xpos = sin(lp)*150+x;
                pship[index].ypos = cos(lp)*150+y;
                index++;
            }
            break;
        case 15:
            for (lp=0.0; lp<CIRCLE; lp+= (CIRCLE/15.0))
            {
                pship[index].xpos = sin(lp)*160+x;
                pship[index].ypos = cos(lp)*160+y;
                index++;
            }
            break;
        case 16:
            for (lp=0.0; lp<CIRCLE; lp+= (CIRCLE/16.0))
            {
                pship[index].xpos = sin(lp)*160+x;
                pship[index].ypos = cos(lp)*160+y;
                index++;
            }
            break;
        case 17:
            for (lp=0.0; lp<CIRCLE; lp+= (CIRCLE/17.0))
            {
                pship[index].xpos = sin(lp)*170+x;
                pship[index].ypos = cos(lp)*170+y;
                index++;
            }
            break;
    }
}

void Get_player_rank()
{
    for (int lp=0; lp<num_players; lp++)
    {
        int s = pship[lp].score;
        string r;

        if (s<500) r="SR";
        else if (s<750) r="SB";
        else if (s<1000) r="Smn";
        else if (s<1500) r="S1C";
        else if (s<1800) r="SSrS";
        else if (s<2100) r="SSgt";
        else if (s<2500) r="TSgt";
        else if (s<3000) r="MSgt";
        else if (s<3500) r="SMSgt";
        else if (s<4200) r="CMSgt";
        else if (s<5000) r="2d Lt";
        else if (s<6000) r="1st Lt";
        else if (s<7200) r="Capt";
        else if (s<9000) r="Maj";
        else if (s<12000) r="Lt Col";
        else if (s<14000) r="Col";
        else if (s<17000) r="Br Gen";
        else if (s>=17000) r="Gen";

        pship[lp].srank = r;
    }
}

const int bytesPerPixel = 3; /// red, green, blue
const int fileHeaderSize = 14;
const int infoHeaderSize = 40;

unsigned char* createBitmapFileHeader(int height, int width){
    int fileSize = fileHeaderSize + infoHeaderSize + bytesPerPixel*height*width;

    static unsigned char fileHeader[] = {
        0,0, /// signature
        0,0,0,0, /// image file size in bytes
        0,0,0,0, /// reserved
        0,0,0,0, /// start of pixel array
    };

    fileHeader[ 0] = (unsigned char)('B');
    fileHeader[ 1] = (unsigned char)('M');
    fileHeader[ 2] = (unsigned char)(fileSize    );
    fileHeader[ 3] = (unsigned char)(fileSize>> 8);
    fileHeader[ 4] = (unsigned char)(fileSize>>16);
    fileHeader[ 5] = (unsigned char)(fileSize>>24);
    fileHeader[10] = (unsigned char)(fileHeaderSize + infoHeaderSize);

    return fileHeader;
}

unsigned char* createBitmapInfoHeader(int height, int width){
    static unsigned char infoHeader[] = {
        0,0,0,0, /// header size
        0,0,0,0, /// image width
        0,0,0,0, /// image height
        0,0, /// number of color planes
        0,0, /// bits per pixel
        0,0,0,0, /// compression
        0,0,0,0, /// image size
        0,0,0,0, /// horizontal resolution
        0,0,0,0, /// vertical resolution
        0,0,0,0, /// colors in color table
        0,0,0,0, /// important color count
    };

    infoHeader[ 0] = (unsigned char)(infoHeaderSize);
    infoHeader[ 4] = (unsigned char)(width    );
    infoHeader[ 5] = (unsigned char)(width>> 8);
    infoHeader[ 6] = (unsigned char)(width>>16);
    infoHeader[ 7] = (unsigned char)(width>>24);
    infoHeader[ 8] = (unsigned char)(height    );
    infoHeader[ 9] = (unsigned char)(height>> 8);
    infoHeader[10] = (unsigned char)(height>>16);
    infoHeader[11] = (unsigned char)(height>>24);
    infoHeader[12] = (unsigned char)(1);
    infoHeader[14] = (unsigned char)(bytesPerPixel*8);

    return infoHeader;
}

void generateBitmapImage(unsigned char *image, int height, int width, char* imageFileName){

    unsigned char* fileHeader = createBitmapFileHeader(height, width);
    unsigned char* infoHeader = createBitmapInfoHeader(height, width);
    unsigned char padding[3] = {0, 0, 0};
    int paddingSize = (4-(width*bytesPerPixel)%4)%4;

    FILE* imageFile = fopen(imageFileName, "wb");

    fwrite(fileHeader, 1, fileHeaderSize, imageFile);
    fwrite(infoHeader, 1, infoHeaderSize, imageFile);

    int i;
    for(i=0; i<height; i++){
        fwrite(image+(i*width*bytesPerPixel), bytesPerPixel, width, imageFile);
        fwrite(padding, 1, paddingSize, imageFile);
    }

    fclose(imageFile);
}

void Screen_shot(int width, int height)
{
    static int file_num = 0;

    string fname = "screenshot/Picture" + to_string(file_num) + ".bmp";
    file_num++;
    char c[40];
    sprintf(c,"%s", fname.c_str());

    unsigned char *screen_data = new unsigned char[width * height * 3 + (3*height)];
    glReadPixels(0, 0, width, height, GL_BGR, GL_UNSIGNED_BYTE, screen_data);
    generateBitmapImage(screen_data, height, width, c);

    delete[] screen_data;
}

