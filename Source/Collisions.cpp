#include "Collisions.h"

void Check_for_collisions()
{
    if (!demo) Check_playerships_hit_something();
    if (!demo) Check_enemyships_hit_rocks();
    if (!demo && !bullets.empty()) Check_bullets_hit_something();
    if (!ebullets.empty()) Check_ebullets_hit_something();
    if (bomb_flag) Check_bomb_bullets_hit_something();
}

vector<vec2> ast;
vector<vec2> shp;

bool Ship_rock_intersection()
{
    bool hit;
    int lp, lp2;

    for (lp=0; lp<shp.size(); lp+=2)
    {
        for (lp2=0; lp2<ast.size(); lp2+=2)
        {
            hit = Line_line_intersect2(ast[lp2], ast[lp2+1], shp[lp], shp[lp+1]);
            if (hit)
            {
                break;
            }
        }
        if (hit) break;
    }
    return hit;  // No intersecting lines, so no collision
}

float Match_gpu_rnd(vec2 cord)
{
    int32_t n = int(cord.v[0] * rock_seed.v[0] + cord.v[1] * rock_seed.v[1]);
    n = (n << 13) ^ n;
    float result = 2.0 - float( (n * (n * n * 15731 + 789221) + 1376312589) & 0x7fffffff) / 1073741824.0;
    return result*0.125 + 0.8;
}

/// ****************************************
/// * Check if player ships hits something *
/// ****************************************
void Check_playerships_hit_something()
{
    int x_index;
    int y_index;
    float x, y, r;
    int index;
    float dist;
    vec2i rcode;

    for (int sh=0; sh<num_players; sh++)
    {
        if (pship[sh].status == SHIP_ACTIVE)    // Skip inactive ship
        {
            x_index = (int) ((pship[sh].xpos+100)/Xgrid);   // Get collision grid cell you're in
             if (x_index<0) x_index = 0;
            y_index = (int) ((pship[sh].ypos+100)/Ygrid);
            int cnt = rgrid[x_index][y_index].size();       // Get # of objects in cell
            if (cnt>0)
            {
                vec2i *spnt = rgrid[x_index][y_index].data();
                int code;
                // Ship shields not on
                if (!shield_flag[sh] && !spawn_shield_flag[sh])
                {
                    for (int lp=0; lp<cnt; lp++)
                    {
                        code = spnt[lp].v[0];
                        if (code == ROCK)
                        {
                            if (Ship_hit_rock(sh, spnt[lp].v[1])) break;
                        }
                        else if (code == ENEMY)
                        {
                            if (Ship_hit_enemy(sh, spnt[lp].v[1])) break;
                        }
                        else if (code == SHIP && Player_hit_player)
                        {
                            if (Ship_hit_ship(sh, spnt[lp].v[1])) break;
                        }
                        else if (code == DRONE)
                        {
                            if (Ship_hit_drone(sh, spnt[lp].v[1])) break;
                        }
                    }
                }
                else  // Ship shields on
                {
                    for (int lp=0; lp<cnt; lp++)
                    {
                        code = spnt[lp].v[0];
                        if (code == ROCK)
                        {
                            if (Ship_shields_hit_rock(sh, spnt[lp].v[1])) break;
                        }
                        else if (code == ENEMY)
                        {
                            if (Ship_shields_hit_enemy(sh, spnt[lp].v[1])) break;
                        }
                        else if (code == SHIP && Player_hit_player)
                        {
                            if (Ship_shields_collide(sh, spnt[lp].v[1])) break;
                        }
                        else if (code == DRONE)
                        {
                            if (Ship_hit_drone(sh, spnt[lp].v[1])) break;
                        }
                    }
                }
            }
        }
    }
}

/// Check if your ship hits a rock (ship shields off)
bool Ship_hit_rock(int sh, int rk)
{
    float r = rocks[rk].radius;
    float r_travel = len2D(vec2(rocks[rk].xdir, rocks[rk].ydir));
    float s_travel = len2D(vec2(pship[sh].dirx, pship[sh].diry));
    float x = rocks[rk].xpos - pship[sh].xpos;
    float y = rocks[rk].ypos - pship[sh].ypos;
    float dist = sqrt(x*x + y*y);
    if (dist < r+Ship_size+s_travel+r_travel)
    {
        if (Precise_ship_hit_rock_collision(rk, sh))
        {
            Rock_buffers_changed = true;
            pship[sh].ships--;
            alSourceStop(thrust_sourceid[sh]);
            sound_thrust_flag[sh] = false;
            alSourcef(ship_explode_sourceid[ship_snd_pntr], AL_GAIN, Sound_volume);
            alSourcePlay(ship_explode_sourceid[ship_snd_pntr]);
            ship_snd_pntr++;
            if (ship_snd_pntr>=MAX_SE_EXPLODE) ship_snd_pntr=0;
            Start_ship_explosion(SHIP, sh, ROCK, rk, pship[sh].xpos, pship[sh].ypos);
            if (rocks[rk].stage == SMALL_ROCK)
            {
                potential_rocks--;
            }
            return true;
        }
    }
    return false;
}

/// Check if your ship hits an enemy ship (ship shields off)
bool Ship_hit_enemy(int sh, int index)
{
    if (enemy[index].status == SHIP_ACTIVE)
    {
        float r = enemy[index].xysize.v[0];
        float x = pship[sh].xpos - enemy[index].xpos;
        float y = pship[sh].ypos - enemy[index].ypos;
        float dist = sqrt(x*x + y*y);
        if (dist<r+Ship_size)
        {
            if (Precise_ship_hit_enemy_collision(index, sh))
            {
                pship[sh].ships--;
                alSourceStop(thrust_sourceid[sh]);
                sound_thrust_flag[sh] = false;

                Start_ship_explosion(SHIP, sh, ENEMY, index, pship[sh].xpos, pship[sh].ypos);
                Start_ship_explosion(ENEMY, index, -1, -1, enemy[index].xpos, enemy[index].ypos);
                alSourcef(ship_explode_sourceid[ship_snd_pntr], AL_GAIN, Sound_volume);
                alSourcePlay(ship_explode_sourceid[ship_snd_pntr]);
                ship_snd_pntr++;
                if (ship_snd_pntr>=MAX_SE_EXPLODE) ship_snd_pntr=0;
                alSourcef(enemy_explode_sourceid, AL_GAIN, Sound_volume);
                alSourcePlay(enemy_explode_sourceid);
                return true;
            }
        }
    }
    return false;
}

/// **** Give drone a player ship to chase ****
void Assign_drone(int dr)
{
   if (num_players == 1)
   {
       if (pship[0].status == SHIP_ACTIVE) drones[dr].target = 0;
       else drones[dr].target = -1;
   }
   else
   {
       vector<int>test;
       test.clear();
       for (int lp=0; lp<num_players; lp++)
       {
           if (pship[lp].status == SHIP_ACTIVE) test.push_back(lp);
       }
       if (!test.empty())
       {
           uniform_int_distribution<int>num(0, test.size());
           drones[dr].target = num(rndrock);
       }
       else drones[dr].target = -1;
   }
}

bool Ship_hit_drone(int sh, int index)
{
    if (!pship[sh].status) return false;
    if (!drones[index].status) return false;

    float x = pship[sh].xpos - drones[index].xpos;
    float y = pship[sh].ypos - drones[index].ypos;
    float dist = sqrt(x*x + y*y);

    if (shield_flag[sh] || spawn_shield_flag[sh])
    {
        if (dist<Shield_size + Drone_size)
        {
            if (Precise_shieldship_hit_drone(sh, index))
            {
                drones[index].status = false;
                alSourcef(ship_explode_sourceid[ship_snd_pntr], AL_GAIN, Sound_volume);
                alSourcePlay(ship_explode_sourceid[ship_snd_pntr]);
                ship_snd_pntr++;
                if (ship_snd_pntr>=MAX_SE_EXPLODE) ship_snd_pntr=0;
                Start_ship_explosion(DRONE, index, -1, -1, drones[index].xpos, drones[index].ypos);

                // Explosion knocks your ship away
                float ximpact = drones[index].xpos - pship[sh].xpos;
                float yimpact = drones[index].ypos - pship[sh].ypos;
                float dist = sqrt(ximpact*ximpact + yimpact*yimpact);
                ximpact/= dist;
                yimpact/= dist;
                pship[sh].dirx-= ximpact*25.0;
                pship[sh].diry-= yimpact*25.0;

                int id = drones[index].cluster;
                if (id>=0)
                {
                    cluster[id].status = false;
                    for (int lp=0; lp<drones.size(); lp++)
                    {
                        if (drones[lp].cluster == id)
                        {
                            drones[lp].cluster = -1;      // Turn drone into individual
                            Assign_drone(lp);
                        }
                    }
                }
                return true;
            }
        }
    }
    else
    {
        if (dist<Ship_size + Drone_size)
        {
            if (Precise_ship_hit_drone(sh, index))
            {
                pship[sh].ships--;
                drones[index].status = false;
                alSourceStop(thrust_sourceid[sh]);
                sound_thrust_flag[sh] = false;
                alSourcef(ship_explode_sourceid[ship_snd_pntr], AL_GAIN, Sound_volume);
                alSourcePlay(ship_explode_sourceid[ship_snd_pntr]);
                ship_snd_pntr++;
                if (ship_snd_pntr>=MAX_SE_EXPLODE) ship_snd_pntr=0;
                Start_ship_explosion(SHIP, sh, -1, -1, pship[sh].xpos, pship[sh].ypos);
                Start_ship_explosion(DRONE, index, -1, -1, drones[index].xpos, drones[index].ypos);

                int id = drones[index].cluster;
                if (id>=0)
                {
                    cluster[id].status = false;
                    for (int lp=0; lp<drones.size(); lp++)
                    {
                        if (drones[lp].cluster == id)
                        {
                            drones[lp].cluster = -1;      // Turn drone into individual
                            drones[lp].target = 0;    // Target player ship
                        }
                    }
                }
                return true;
            }
        }
    }
    return false;
}

bool Ship_hit_ship(int sh, int index)
{
    if (sh == index) return false;      // Ship not allowed to hit itself.
    if (!pship[index].status) return false;

    float x = pship[sh].xpos - pship[index].xpos;
    float y = pship[sh].ypos - pship[index].ypos;
    float dist = sqrt(x*x + y*y);

    if (shield_flag[index] || spawn_shield_flag[index])          // Other players ship shields on??
    {
        if (dist < Shield_size+Shield_size)
        {
            if (Precise_ship_hit_shieldship(sh, index))
            {
                pship[sh].ships--;
                alSourceStop(thrust_sourceid[sh]);
                sound_thrust_flag[sh] = false;
                alSourcef(ship_explode_sourceid[ship_snd_pntr], AL_GAIN, Sound_volume);
                alSourcePlay(ship_explode_sourceid[ship_snd_pntr]);
                ship_snd_pntr++;
                if (ship_snd_pntr>=MAX_SE_EXPLODE) ship_snd_pntr=0;
                Start_ship_explosion(SHIP, sh, -1, -1, pship[sh].xpos, pship[sh].ypos);

                // Explosion knocks other ship with shields away
                float ximpact = pship[index].xpos - pship[sh].xpos;
                float yimpact = pship[index].ypos - pship[sh].ypos;
                float dist = sqrt(ximpact*ximpact + yimpact*yimpact);
                ximpact/= dist;
                yimpact/= dist;
                pship[index].dirx+= ximpact*25.0;
                pship[index].diry+= yimpact*25.0;
                return true;
            }
        }
    }
    else
    {
        if (dist < Ship_size*2.0)
        {
            if (Precise_ship_hit_ship(sh, index))
            {
                pship[sh].ships--;
                pship[index].ships--;
                alSourceStop(thrust_sourceid[sh]);
                sound_thrust_flag[sh] = false;
                alSourceStop(thrust_sourceid[index]);
                sound_thrust_flag[index] = false;
                alSourcef(ship_explode_sourceid[ship_snd_pntr], AL_GAIN, Sound_volume);
                alSourcePlay(ship_explode_sourceid[ship_snd_pntr]);
                ship_snd_pntr++;
                if (ship_snd_pntr>=MAX_SE_EXPLODE) ship_snd_pntr=0;
                Start_ship_explosion(SHIP, sh, -1, -1, pship[sh].xpos, pship[sh].ypos);
                Start_ship_explosion(SHIP, index, -1, -1, pship[index].xpos, pship[index].ypos);
                return true;
            }
        }
    }
    return false;
}

/// Check if your ships shields hit a rock
bool Ship_shields_hit_rock(int sh, int index)
{
    rock_data *rd = &rocks[index];
    float x = rd->xpos - pship[sh].xpos;
    float y = rd->ypos - pship[sh].ypos;
    float r = rd->radius;
    float dist = sqrt(x*x + y*y);
    if (dist < r+Shield_size)
    {
        alSourcef(ship_bounce_sourceid, AL_GAIN, Sound_volume);
        alSourcePlay(ship_bounce_sourceid);

        float rfactor = Rock_speed_factor*elapsed_time;
        float sfactor = Ship_speed_factor*elapsed_time;

        // Back off ship and rock to undo overlap
        float overlap = 0.5*(dist - r - Ship_size*1.5);

        // Back off rock
        rd->xpos -= overlap*(rd->xpos - pship[sh].xpos)/dist;
        rd->ypos -= overlap*(rd->ypos - pship[sh].ypos)/dist;

        // Back off ship
        pship[sh].xpos += overlap*(rd->xpos - pship[sh].xpos)/dist;
        pship[sh].ypos += overlap*(rd->ypos - pship[sh].ypos)/dist;

        // Calculate collision response
        x = rd->xpos - pship[sh].xpos;
        y = rd->ypos - pship[sh].ypos;
        dist = sqrt(x*x + y*y);

        // Normal
        float nx = (pship[sh].xpos - rd->xpos)/dist;
        float ny = (pship[sh].ypos - rd->ypos)/dist;

        // Tangent
        float tx = -ny;
        float ty = nx;

        // Dot Product Tangent
        float dpTan1 = (rd->xdir*rfactor) * tx + (rd->ydir*rfactor) * ty;
        float dpTan2 = (pship[sh].dirx*sfactor) * tx + (pship[sh].diry*sfactor) * ty;

        // Dot Product Normal
        float dpNorm1 = (rd->xdir*rfactor) * nx + (rd->ydir*rfactor) * ny;
        float dpNorm2 = (pship[sh].dirx*sfactor) * nx + (pship[sh].diry*sfactor) * ny;

        // Conversation of momentum in 1D
        float m1 = (dpNorm1 * (rd->mass - pship[sh].mass) + 2.0*pship[sh].mass*dpNorm2)/(rd->mass+pship[sh].mass);
        float m2 = (dpNorm2 * (pship[sh].mass - rd->mass) + 2.0*rd->mass*dpNorm1)/(rd->mass+pship[sh].mass);

        // Update velocities
        rd->xdir = (tx*dpTan1+nx*m1)/rfactor;
        rd->ydir = (ty*dpTan1+ny*m1)/rfactor;
        pship[sh].dirx = (tx*dpTan2+nx*m2)/sfactor;
        pship[sh].diry = (ty*dpTan2+ny*m2)/sfactor;
        return true;
    }
    return false;
}

/// Check if your ships shields hit an enemy
bool Ship_shields_hit_enemy(int sh, int index)
{
    if (enemy[index].status == SHIP_ACTIVE)
    {
        float r = enemy[index].xysize.v[0];
        float x = pship[sh].xpos - enemy[index].xpos;
        float y = pship[sh].ypos - enemy[index].ypos;
        float dist = sqrt(x*x + y*y);
        if (dist < r+Shield_size)
        {
            if (Precise_shield_hit_enemy_collision(index, sh))
            {
                Start_ship_explosion(ENEMY, index, -1, -1, enemy[index].xpos, enemy[index].ypos);
                alSourcef(enemy_explode_sourceid, AL_GAIN, Sound_volume);
                alSourcePlay(enemy_explode_sourceid);

                // Explosion knocks your ship away
                float ximpact = pship[sh].xpos - enemy[index].xpos;
                float yimpact = pship[sh].ypos - enemy[index].ypos;
                float dist = sqrt(ximpact*ximpact + yimpact*yimpact);
                ximpact/= dist;
                yimpact/= dist;
                pship[sh].dirx+= ximpact*25.0;
                pship[sh].diry+= yimpact*25.0;
                return true;
            }
        }
    }
    return false;
}

/// Check if your ships shields another ship with its shields on
bool Ship_shields_collide(int sh, int index)
{
    if (sh == index) return false;                          // Player can't hit itself!
    if (!shield_flag[index] && !spawn_shield_flag[index]) return false;

    float x = pship[sh].xpos - pship[index].xpos;
    float y = pship[sh].ypos - pship[index].ypos;
    float r = Shield_size*2.0;
    float dist = sqrt(x*x + y*y);
    if (dist < r)
    {
        alSourcef(ship_bounce_sourceid, AL_GAIN, Sound_volume);
        alSourcePlay(ship_bounce_sourceid);

        float rfactor = Ship_speed_factor*elapsed_time;
        float sfactor = Ship_speed_factor*elapsed_time;

        // Back off ships to undo overlap
        float overlap = 0.5*(dist - r);

        // Back of other ship
        pship[index].xpos -= overlap*(pship[index].xpos - pship[sh].xpos)/dist;
        pship[index].ypos -= overlap*(pship[index].ypos - pship[sh].ypos)/dist;

        // Back off ship
        pship[sh].xpos += overlap*(pship[index].xpos - pship[sh].xpos)/dist;
        pship[sh].ypos += overlap*(pship[index].ypos - pship[sh].ypos)/dist;

        // Calculate collision response
        x = pship[index].xpos - pship[sh].xpos;
        y = pship[index].ypos - pship[sh].ypos;
        dist = sqrt(x*x + y*y);

        // Normal
        float nx = (pship[sh].xpos - pship[index].xpos)/dist;
        float ny = (pship[sh].ypos - pship[index].ypos)/dist;

        // Tangent
        float tx = -ny;
        float ty = nx;

        // Dot Product Tangent
        float dpTan1 = (pship[index].dirx*rfactor) * tx + (pship[index].diry*rfactor) * ty;
        float dpTan2 = (pship[sh].dirx*sfactor) * tx + (pship[sh].diry*sfactor) * ty;

        // Dot Product Normal
        float dpNorm1 = (pship[index].dirx*rfactor) * nx + (pship[index].diry*rfactor) * ny;
        float dpNorm2 = (pship[sh].dirx*sfactor) * nx + (pship[sh].diry*sfactor) * ny;

        // Conversation of momentum in 1D
        float m1 = (dpNorm1 * (pship[index].mass - pship[sh].mass) + 2.0*pship[sh].mass*dpNorm2)/(pship[index].mass+pship[sh].mass);
        float m2 = (dpNorm2 * (pship[sh].mass - pship[index].mass) + 2.0*pship[index].mass*dpNorm1)/(pship[index].mass+pship[sh].mass);

        // Update velocities
        pship[index].dirx = (tx*dpTan1+nx*m1)/rfactor;
        pship[index].diry = (ty*dpTan1+ny*m1)/rfactor;
        pship[sh].dirx = (tx*dpTan2+nx*m2)/sfactor;
        pship[sh].diry = (ty*dpTan2+ny*m2)/sfactor;
        return true;
    }
    return false;
}

bool Precise_ship_hit_rock_collision(int index, int sh)
{
    int lp;
    float x1, y1, x2, y2;
    int start = rocks[index].pntr.v[0];
    int cnt = rocks[index].pntr.v[1];
    float sintheta = sin(rocks[index].angle);
    float costheta = cos(rocks[index].angle);
    float radius = rocks[index].radius;        // Rock size in radius
    float xrock = rocks[index].xpos;           // Get rock position
    float yrock = rocks[index].ypos;

    ast.clear();
    shp.clear();

    // Get rock shape and modify for size, location, and orientation
    if (!Rock_instancing)
    {
        for (lp=start; lp<start+cnt; lp++)
        {
            x1 = rck_data[lp].v[0]*radius;
            y1 = rck_data[lp].v[1]*radius;
            if (lp==(start+cnt-1))
            {
                x2 = rck_data[start].v[0]*radius;  // Wrap around to first vertex of rock
                y2 = rck_data[start].v[1]*radius;
            }
            else
            {
                x2 = rck_data[lp+1].v[0]*radius;  // Get endpoint of line
                y2 = rck_data[lp+1].v[1]*radius;
            }
            ast.push_back(vec2((x1*costheta - y1*sintheta) + xrock, (x1*sintheta + y1*costheta) + yrock));
            ast.push_back(vec2((x2*costheta - y2*sintheta) + xrock, (x2*sintheta + y2*costheta) + yrock));
        }
    }
    else
    {
        float n_radius;
        for (int lp2=1; lp2<25; lp2++)
        {
            x1 = sh_rocks2.points[lp2].v[0]*radius;
            y1 = sh_rocks2.points[lp2].v[1]*radius;
            n_radius = Match_gpu_rnd(vec2(x1 + (float) index, y1 + (float) index));
            x1*= n_radius;
            y1*= n_radius;

            x2 = sh_rocks2.points[lp2+1].v[0]*radius;
            y2 = sh_rocks2.points[lp2+1].v[1]*radius;
            n_radius = Match_gpu_rnd(vec2(x2 + (float) index, y2 + (float) index));
            x2*= n_radius;
            y2*= n_radius;

            ast.push_back(vec2((x1*costheta - y1*sintheta) + xrock, (x1*sintheta + y1*costheta) + yrock));
            ast.push_back(vec2((x2*costheta - y2*sintheta) + xrock, (x2*sintheta + y2*costheta) + yrock));
        }
    }

    // Put ship shape into vector
    for (lp=0; lp < pship[sh].shape.size(); lp++)
    {
        shp.push_back(pship[sh].shape[lp]);
    }
    for (lp=0; lp < pship[sh].shape.size(); lp+=2)
    {
        shp.push_back(pship[sh].shape[lp]);
        shp.push_back(pship[sh].lshape[lp]);
    }
    bool hit = Ship_rock_intersection(); // Check for ship against rock collision using line segment checks
    return hit;
}

/// **** Check if geometry of drone intersects geometry of player ship ****
bool Precise_ship_hit_drone(int sh, int dr)
{
    bool hit;
    int lp, lp2;
    float angle = drones[dr].angle;
    float sintheta = sin(angle);
    float costheta = cos(angle);
    vec2 dstart, dend;
    float xpos = drones[dr].xpos;
    float ypos = drones[dr].ypos;
    float x1, y1, x2, y2;

    for (lp=0; lp<enemy_shapes[cluster_index].size(); lp+=2)
    {
        x1=enemy_shapes[cluster_index][lp].v[0]*Drone_size;
        y1=enemy_shapes[cluster_index][lp].v[1]*Drone_size;
        x2=enemy_shapes[cluster_index][lp+1].v[0]*Drone_size;
        y2=enemy_shapes[cluster_index][lp+1].v[1]*Drone_size;
        dstart = vec2((x1*costheta - y1*sintheta) + xpos, (x1*sintheta + y1*costheta) + ypos);
        dend = vec2((x2*costheta - y2*sintheta) + xpos, (x2*sintheta + y2*costheta) + ypos);

        for (lp2=0; lp2 < pship[sh].raw_shape.size(); lp2+=2)
        {
            hit = Line_line_intersect2(pship[sh].shape[lp2], pship[sh].shape[lp2+1], dstart, dend);
            if (hit) break;
        }
        if (hit) break;
    }
    return hit;
}

/// **** Check if geometry of drone intersects ship shields ****
bool Precise_shieldship_hit_drone(int sh, int dr)
{
    float angle = drones[dr].angle;
    float sintheta = sin(angle);
    float costheta = cos(angle);
    float xpos = drones[dr].xpos;
    float ypos = drones[dr].ypos;
    float x1, y1, x2, y2, xtest, ytest;

    for (int lp=0; lp<enemy_shapes[cluster_index].size(); lp++)
    {
        x1=enemy_shapes[cluster_index][lp].v[0]*Drone_size;
        y1=enemy_shapes[cluster_index][lp].v[1]*Drone_size;
        x2 = (x1*costheta - y1*sintheta) + xpos;
        y2 = (x1*sintheta + y1*costheta) + ypos;
        xtest = x2-pship[sh].xpos;
        ytest = y2-pship[sh].ypos;
        float dist = sqrt(xtest*xtest + ytest*ytest);
        if (dist<Shield_size) return true;
    }
    return false;
}

bool Precise_ship_hit_enemy_collision(int index, int sh)
{
    int lp;
    int cnt = enemy[index].raw_shape.size();
    int start = enemy[index].shape_pntr;
    vec2 bship[cnt];
    bool hit;

    float xsize = enemy[index].xysize.v[0];
    float ysize = enemy[index].xysize.v[1];

    for (lp=0; lp<cnt; lp++)
    {
        bship[lp].v[0] = sh_pship.points[start+lp].v[0]*xsize + enemy[index].xpos;
        bship[lp].v[1] = sh_pship.points[start+lp].v[1]*ysize + enemy[index].ypos;
    }

    for (lp=0; lp < pship[sh].raw_shape.size(); lp+=2)
    {
        for (int lp2=0; lp2<cnt; lp2+=2)
        {
            hit = Line_line_intersect2(pship[sh].shape[lp], pship[sh].shape[lp+1], bship[lp2], bship[lp2+1]);
            if (hit) break;
        }
        if (hit) break;
    }
    return hit;
}

/// Check if your ship's geometry intersects another nearby player ship
bool Precise_ship_hit_ship(int sh, int sh2)
{
    int lp, lp2;
    vec2 a, b, c, d;
    bool hit;

    for (lp=0; lp<pship[sh].shape.size(); lp+=2)
    {
        a = pship[sh].shape[lp];
        b = pship[sh].shape[lp+1];
        for (lp2=0; lp2<pship[sh2].shape.size(); lp2+=2)
        {
            c = pship[sh2].shape[lp2];
            d = pship[sh2].shape[lp2+1];
            hit = Line_line_intersect2(a, b, c, d);
            if (hit) break;
        }
        if (hit) break;
    }
    return hit;
}

bool Precise_ship_hit_shieldship(int sh, int sh2)
{
    float x, y;
    bool hit = false;
    float dist;
    int cnt = pship[sh].shape.size();

    for (int lp=0; lp<cnt; lp++)
    {
        x = pship[sh].shape[lp].v[0] - pship[sh2].xpos;
        y = pship[sh].shape[lp].v[1] - pship[sh2].ypos;
        dist = sqrt(x*x + y*y);
        if (dist < Shield_size)
        {
            hit = true;
            break;
        }
    }
    return hit;
}

bool Precise_shield_hit_enemy_collision(int index, int sh)
{
    float x, y;
    bool hit = false;
    float dist;
    int cnt = enemy[index].raw_shape.size();
    int start = enemy[index].shape_pntr;

    float xsize = enemy[index].xysize.v[0];
    float ysize = enemy[index].xysize.v[1];
    for (int lp=0; lp<cnt; lp++)
    {
        x = sh_pship.points[start+lp].v[0]*xsize + enemy[index].xpos - pship[sh].xpos;
        y = sh_pship.points[start+lp].v[1]*ysize + enemy[index].ypos - pship[sh].ypos;
        dist = sqrt(x*x + y*y);
        if (dist < Shield_size)
        {
            hit = true;
            break;
        }
    }
    return hit;
}

void Check_enemyships_hit_rocks()
{
    for (int lp=0; lp<num_enemy; lp++)
    {
        if (enemy[lp].status == SHIP_ACTIVE)
        {
            int x_index = (int) ((enemy[lp].xpos+100)/Xgrid);
            int y_index = (int) ((enemy[lp].ypos+100)/Ygrid);
            int cnt = rgrid[x_index][y_index].size();
            if (cnt>0)
            {
                vec2i *spnt = rgrid[x_index][y_index].data();
                for (int lp2=0; lp2<cnt; lp2++)
                {
                    if (spnt[lp2].v[0] == ROCK)
                    {
                        // Check if ship vertices inside the rock
                        int index = spnt[lp2].v[1];
                        float r = rocks[index].radius*1.2;
                        float x = rocks[index].xpos - enemy[lp].xpos;
                        float y = rocks[index].ypos - enemy[lp].ypos;
                        float dist = sqrt(x*x + y*y);
                        if (dist < r + enemy[lp].xysize.v[0])
                        {
                            if (Precise_enemyship_hit_rock_collision(lp, index))
                            {
                                Rock_buffers_changed = true;
                                Start_ship_explosion(ENEMY, lp, ROCK, index, enemy[lp].xpos, enemy[lp].ypos);
                                if (rocks[index].stage == SMALL_ROCK)
                                {
                                    potential_rocks--;
                                }
                                alSourcef(enemy_explode_sourceid, AL_GAIN, Sound_volume);
                                alSourcePlay(enemy_explode_sourceid);
                                break;
                            }
                        }
                    }
                }
            }
        }
    }
}

bool Precise_enemyship_hit_rock_collision(int e_index, int r_index)
{
    int lp, lp2;
    vector<vec2>trock;
    vector<vec2>eship;
    trock.clear();
    eship.clear();
    float x1, y1;
    float a, b;
    int start = rocks[r_index].pntr.v[0];
    int cnt = rocks[r_index].pntr.v[1];
    float sintheta = sin(rocks[r_index].angle);
    float costheta = cos(rocks[r_index].angle);
    float radius = rocks[r_index].radius;
    float xpos = rocks[r_index].xpos;
    float ypos = rocks[r_index].ypos;
    bool hit;

    if (!Rock_instancing)
    {
        for (lp=start; lp<start+cnt; lp++)
        {
            x1 = rck_data[lp].v[0]*radius;
            y1 = rck_data[lp].v[1]*radius;
            a = (x1*costheta - y1*sintheta) + xpos;
            b = (x1*sintheta + y1*costheta) + ypos;
            trock.push_back(vec2(a, b));
        }
    }
    else
    {
        float n_radius;
        for (int lp2=1; lp2<26; lp2++)
        {
            x1 = sh_rocks2.points[lp2].v[0]*radius;
            y1 = sh_rocks2.points[lp2].v[1]*radius;
            n_radius = Match_gpu_rnd(vec2(x1 + (float) r_index, y1 + (float) r_index));
            x1*= n_radius;
            y1*= n_radius;
            trock.push_back(vec2(x1+xpos, y1+ypos));
        }
    }

    float xsize = enemy[e_index].xysize.v[0];
    float ysize = enemy[e_index].xysize.v[1];
    int ecnt = enemy[e_index].raw_shape.size();    // # of vertices in enemy ship
    int estart = enemy[e_index].shape_pntr;
    for (lp=0; lp<ecnt; lp++)
    {
        x1 = sh_pship.points[estart+lp].v[0]*xsize + enemy[e_index].xpos;
        y1 = sh_pship.points[estart+lp].v[1]*ysize + enemy[e_index].ypos;
        eship.push_back(vec2(x1, y1));
    }
    for (lp=0; lp<trock.size(); lp++)
    {
        for (lp2=0; lp2<eship.size(); lp2+=2)
        {
            hit = Line_line_intersect2(trock[lp], trock[lp+1], eship[lp2], eship[lp2+1]);
            if (hit) break;
        }
        if (hit) break;
    }
    return hit;
}

/// ************************************************
/// * Check if player ship bullets hits something  *
/// ************************************************
void Check_bullets_hit_something()
{
    int lp;
    int lp2;
    int x_index, y_index;
    float x, y;
    float bdist1;
    float blen;
    circle_line cl;

    for (lp=0; lp<bullets.size(); lp++)
    {
        if (bullets[lp].status)
        {
            cl.px1 = bullets[lp].l_xpos;  // Load bullet: current and last position
            cl.py1 = bullets[lp].l_ypos;
            cl.px2 = bullets[lp].xpos;
            cl.py2 = bullets[lp].ypos;

            x_index = (int) ((bullets[lp].xpos + 100)/Xgrid);  // Get cell bullet is in
             if (x_index<0) x_index = 0;
             else if (x_index>=Xgrid_size) x_index = Xgrid_size-1;
            y_index = (int) ((bullets[lp].ypos + 100)/Ygrid);
             if (y_index<0) y_index = 0;
             else if (y_index>=Ygrid_size) y_index = Ygrid_size-1;
            int cnt = rgrid[x_index][y_index].size();           // Get number of objects in the cell
            if (cnt>0)
            {
                int code;
                vec2i *spnt = rgrid[x_index][y_index].data();
                for (lp2=0; lp2<cnt; lp2++)
                {
                    code = spnt[lp2].v[0];
                    if (code == ROCK)
                    {
                        if (Bullet_rock_collision(lp, spnt[lp2].v[1], &cl)) break;
                    }
                    else if (code == ENEMY)
                    {
                        if (Bullet_enemy_collision(lp, spnt[lp2].v[1], &cl)) break;
                    }
                    else if (code == SHIP && Player_hit_player)
                    {
                        if (Bullet_player_collision(lp, spnt[lp2].v[1], &cl)) break;
                    }
                    else if (code == DRONE)
                    {
                        if (Bullet_drone_collision(lp, spnt[lp2].v[1], &cl)) break;
                    }
                }
            }
        }
    }
}

bool Bullet_rock_collision(int bl, int rk, circle_line *cl)
{
    if (!rocks[rk].status) return false;

    cl->cx = rocks[rk].xpos;   // Load rock
    cl->cy = rocks[rk].ypos;
    cl->r = rocks[rk].radius*1.2;
    if (Circle_line_intersect(cl))   // Check if line of bullet intersect rock
    {
        float x = cl->px2 - cl->cx;
        float y = cl->py2 - cl->cy;
        float bdist1 = sqrt(x*x + y*y);
        x = cl->px2 - cl->px1;
        y = cl->py2 - cl->py1;
        float blen = sqrt(x*x + y*y);
        if (bdist1 < (cl->r + blen))              // Check current bullet position
        {
            if (Precise_bullet_hit_rock(rk, cl))
            {
                Bullet_hit_rock(bl, rk, cl);
                return true;
            }
        }
    }
    return false;
}

bool Bullet_player_collision(int bl, int sh, circle_line *cl)
{
    if (pship[sh].status != SHIP_ACTIVE) return false;
    if (sh == bullets[bl].ship) return false;                   // Can't shoot yourself
    bool shield = false;
    if (shield_flag[sh] || spawn_shield_flag[sh]) shield = true;

    cl->cx = pship[sh].xpos;   // Load enemy ship location
    cl->cy = pship[sh].ypos;
    if (!shield) cl->r = Ship_size;   // Radius of enemy ship
    else cl->r = Shield_size;

    if (Circle_line_intersect(cl))
    {
        float x = cl->px2 - cl->cx;
        float y = cl->py2 - cl->cy;
        float bdist1 = sqrt(x*x + y*y);
        x = cl->px2 - cl->px1;
        y = cl->py2 - cl->py1;
        float blen = sqrt(x*x + y*y);
        if (bdist1< (cl->r+blen))              // Check current bullet position
        {
            if (!shield)
            {
                if (Precise_bullet_hit_ship(sh, cl))
                {
                    pship[sh].ships--;
                    alSourceStop(thrust_sourceid[sh]);
                    sound_thrust_flag[sh] = false;
                    alSourcef(ship_explode_sourceid[ship_snd_pntr], AL_GAIN, Sound_volume);
                    alSourcePlay(ship_explode_sourceid[ship_snd_pntr]);
                    ship_snd_pntr++;
                    if (ship_snd_pntr>=MAX_SE_EXPLODE) ship_snd_pntr = 0;

                    Start_ship_explosion(SHIP, sh, SHIP_BULLET, bl, pship[sh].xpos, pship[sh].ypos);
                    return true;
                }
            }
            else
            {
                // Shield on.  Do impact effect.
                x = bullets[bl].l_xpos - bullets[bl].xdir*200.0;
                y = bullets[bl].l_ypos - bullets[bl].ydir*200.0;
                bdist1 = sqrt((x-cl->fx)*(x-cl->fx) + (y-cl->fy)*(y-cl->fy));
                float bdist2 = sqrt((x-cl->gx)*(x-cl->gx) + (y-cl->gy)*(y-cl->gy));
                if (bdist1 < bdist2)
                {
                    cl->tx = cl->fx;
                    cl->ty = cl->fy;
                }
                else
                {
                    cl->tx = cl->gx;
                    cl->ty = cl->gy;
                }
                Start_impact(SHIP, sh, cl);
                bullets[bl].status = false;

                alSourcef(ship_bounce_sourceid, AL_GAIN, Sound_volume);
                alSourcePlay(ship_bounce_sourceid);
                return true;
            }
        }
    }
    return false;
}

bool Bullet_enemy_collision(int bl, int en, circle_line *cl)
{
    if (enemy[en].status != SHIP_ACTIVE) return false;

    cl->cx = enemy[en].xpos;   // Load enemy ship location
    cl->cy = enemy[en].ypos;
    cl->r = enemy[en].xysize.v[0];   // Radius of enemy ship
    if (Circle_line_intersect(cl))
    {
        float x = cl->px2 - cl->cx;
        float y = cl->py2 - cl->cy;
        float bdist1 = sqrt(x*x + y*y);
        x = cl->px2 - cl->px1;
        y = cl->py2 - cl->py1;
        float blen = sqrt(x*x + y*y);
        if (bdist1< (cl->r+blen))              // Check current bullet position
        {
            if (Precise_bullet_hit_enemy(en, cl))
            {
                Bullet_hit_enemy(bl, en);
                return true;
            }
        }
    }
    return false;
}

bool Bullet_drone_collision(int bl, int d_index, circle_line *cl)
{
    if (!drones[d_index].status) return false;
    cl->cx = drones[d_index].xpos;
    cl->cy = drones[d_index].ypos;
    cl->r = Drone_size;
    if (Circle_line_intersect(cl))
    {
        float x = cl->px2 - cl->cx;
        float y = cl->py2 - cl->cy;
        float bdist1 = sqrt(x*x + y*y);
        x = cl->px2 - cl->px1;
        y = cl->py2 - cl->py1;
        float blen = sqrt(x*x + y*y);
        if (bdist1< (cl->r+blen))
        {
            if (Precise_bullet_hit_drone(d_index, cl))
            {
                int id = drones[d_index].cluster;
                if (id>=0)
                {
                    cluster[id].status = false;
                    for (int lp=0; lp<drones.size(); lp++)
                    {
                        if (drones[lp].cluster == id)
                        {
                            drones[lp].cluster = -1;      // Turn drone into individual
                            Assign_drone(lp);
                        }
                    }
                    bullets[bl].status = false;
                    return true;
                }
                else
                {
                    drones[d_index].status = false;
                    bullets[bl].status = false;
                    int sh = bullets[bl].ship;
                    pship[sh].score+= Points_drone;
                    pship[sh].add_score+= Points_drone;
                    pship[sh].hits++;
                    if (pship[sh].add_score>Extra_ship_points)
                    {
                        pship[sh].ships++;
                        pship[sh].add_score-= Extra_ship_points;
                    }
                    alSourcef(ship_explode_sourceid[ship_snd_pntr], AL_GAIN, Sound_volume);
                    alSourcePlay(ship_explode_sourceid[ship_snd_pntr]);
                    ship_snd_pntr++;
                    if (ship_snd_pntr>=MAX_SE_EXPLODE) ship_snd_pntr = 0;

                    Start_ship_explosion(DRONE, d_index, -1, -1, drones[d_index].xpos, drones[d_index].ypos);
                    return true;
                }
            }
        }
    }
    return false;
}

/// Get collision point on surface of rock
bool Precise_bullet_hit_rock(int r_index, circle_line *cl)
{
    float x1, y1, x2, y2;
    float a, b, c, d;
    int start = rocks[r_index].pntr.v[0];        // Get pointer to shape of rock
    int cnt = rocks[r_index].pntr.v[1];          // Number of vertices for shape of rock
    float sintheta = sin(rocks[r_index].angle);  // Get trig of angle rotation of rock
    float costheta = cos(rocks[r_index].angle);
    float radius = rocks[r_index].radius;        // Size of rock (radius)
    float xpos = rocks[r_index].xpos;            // Get center position of rock
    float ypos = rocks[r_index].ypos;
    bool hit;

    vector<vec2>ht;
    ht.clear();

    float xstart = cl->fx;   // Get start and endpoint of bullet
    float ystart = cl->fy;
    float xend = cl->gx;
    float yend = cl->gy;

    if (!Rock_instancing)
    {
        for (int lp2=start; lp2<start+cnt; lp2++)    // Loop through vertices of rock
        {
            x1 = rck_data[lp2].v[0]*radius;   // Get start point of line
            y1 = rck_data[lp2].v[1]*radius;
            if (lp2==(start+cnt-1))
            {
                x2 = rck_data[start].v[0]*radius;  // Wrap around to first vertex of rock
                y2 = rck_data[start].v[1]*radius;
            }
            else
            {
                x2 = rck_data[lp2+1].v[0]*radius;  // Get endpoint of line
                y2 = rck_data[lp2+1].v[1]*radius;
            }
            a = (x1*costheta - y1*sintheta) + xpos;   // Rotate line and add rock position
            b = (x1*sintheta + y1*costheta) + ypos;
            c = (x2*costheta - y2*sintheta) + xpos;
            d = (x2*sintheta + y2*costheta) + ypos;
            vec2 intersect = Line_line_intersect(vec2(a, b), vec2(c, d), vec2(xstart, ystart), vec2(xend, yend), &hit);
            if (hit)
            {
                ht.push_back(intersect);
            }
        }
    }
    else
    {
        float n_radius;
        for (int lp2=1; lp2<25; lp2++)
        {
            x1 = sh_rocks2.points[lp2].v[0]*radius;
            y1 = sh_rocks2.points[lp2].v[1]*radius;
            n_radius = Match_gpu_rnd(vec2(x1 + (float) r_index, y1 + (float) r_index));
            x1*= n_radius;
            y1*= n_radius;

            x2 = sh_rocks2.points[lp2+1].v[0]*radius;
            y2 = sh_rocks2.points[lp2+1].v[1]*radius;
            n_radius = Match_gpu_rnd(vec2(x2 + (float) r_index, y2 + (float) r_index));
            x2*= n_radius;
            y2*= n_radius;

            a = (x1*costheta - y1*sintheta) + xpos;   // Rotate line and add rock position
            b = (x1*sintheta + y1*costheta) + ypos;
            c = (x2*costheta - y2*sintheta) + xpos;
            d = (x2*sintheta + y2*costheta) + ypos;
            vec2 intersect = Line_line_intersect(vec2(a, b), vec2(c, d), vec2(xstart, ystart), vec2(xend, yend), &hit);
            if (hit)
            {
                ht.push_back(intersect);
            }
        }
    }

    if (ht.empty()) return false;
    else if (ht.size() == 2)
    {
        float dist = sqrt((ht[0].v[0]-xstart)*(ht[0].v[0]-xstart) + (ht[0].v[1]-ystart)*(ht[0].v[1]-ystart));
        float dist1 = sqrt((ht[1].v[0]-xstart)*(ht[1].v[0]-xstart) + (ht[1].v[1]-ystart)*(ht[1].v[1]-ystart));
        if (dist<dist1)         // Get closest intersection to last position of bullet
        {
            cl->tx = ht[0].v[0];
            cl->ty = ht[0].v[1];
        }
        else
        {
            cl->tx = ht[1].v[0];
            cl->ty = ht[1].v[1];
        }
        return true;
    }
    else if (ht.size() == 1)
    {
        cl->tx = ht[0].v[0];
        cl->ty = ht[0].v[1];
        return true;
    }
    else return false;
}

/// Check if bullet intersects geometry of enemy ship
bool Precise_bullet_hit_enemy(int index, circle_line *cl)
{
    int lp;
    int start = enemy[index].shape_pntr;
    int cnt = enemy[index].raw_shape.size();
    vec2 bship[cnt];
    bool hit;
    float xsize = enemy[index].xysize.v[0];
    float ysize = enemy[index].xysize.v[1];
    vec2 bstart = vec2(cl->px1, cl->py1);
    vec2 bend = vec2(cl->px2, cl->py2);

    for (lp=0; lp<cnt; lp++)
    {
        bship[lp].v[0] = sh_pship.points[start+lp].v[0]*xsize + enemy[index].xpos;
        bship[lp].v[1] = sh_pship.points[start+lp].v[1]*ysize + enemy[index].ypos;
    }

    for (int lp2=0; lp2<cnt; lp2+=2)
    {
        hit = Line_line_intersect2(bstart, bend, bship[lp2], bship[lp2+1]);
        if (hit) break;
    }
    return hit;
}

/// Check if bullet intersects geometry of player ship
bool Precise_bullet_hit_ship(int sh, circle_line *cl)
{
    int cnt = pship[sh].shape.size();
    bool hit;
    vec2 bstart = vec2(cl->px1, cl->py1);
    vec2 bend = vec2(cl->px2, cl->py2);

    for (int lp2=0; lp2<cnt; lp2+=2)
    {
        hit = Line_line_intersect2(bstart, bend, pship[sh].shape[lp2], pship[sh].shape[lp2+1]);
        if (hit) break;
    }
    return hit;
}

/// Check if bullet intersect geometry of a drone
bool Precise_bullet_hit_drone(int d_index, circle_line *cl)
{
    bool hit;
    int lp;
    float a, b, c, d;
    float x1, y1, x2, y2;
    float xpos, ypos;
    float angle = drones[d_index].angle;
    float sintheta = sin(angle);
    float costheta = cos(angle);
    float radius = Drone_size;

    float xstart = cl->fx;   // Get start and endpoint of bullet
    float ystart = cl->fy;
    float xend = cl->gx;
    float yend = cl->gy;

    for (lp=0; lp<enemy_shapes[cluster_index].size(); lp+=2)
    {
        x1 = enemy_shapes[cluster_index][lp].v[0] * radius;
        y1 = enemy_shapes[cluster_index][lp].v[1] * radius;
        x2 = enemy_shapes[cluster_index][lp+1].v[0] * radius;
        y2 = enemy_shapes[cluster_index][lp+1].v[1] * radius;
        a = (x1*costheta - y1*sintheta) + cl->cx;   // Rotate line and add rock position
        b = (x1*sintheta + y1*costheta) + cl->cy;
        c = (x2*costheta - y2*sintheta) + cl->cx;
        d = (x2*sintheta + y2*costheta) + cl->cy;
        vec2 intersect = Line_line_intersect(vec2(a, b), vec2(c, d), vec2(xstart, ystart), vec2(xend, yend), &hit);
        if (hit)
        {
            if (hit) break;
        }
    }
    return hit;
}

/// Check for enemy bullets hitting rocks or your ship
void Check_ebullets_hit_something()
{
    int lp;
    int lp2;
    int x_index, y_index;
    float x, y;
    float bdist1;
    float blen;             // Length of bullet
    circle_line cl;

    for (lp=0; lp<ebullets.size(); lp++)
    {
        if (ebullets[lp].status)
        {
            cl.px1 = ebullets[lp].l_xpos;  // Last bullet position
            cl.py1 = ebullets[lp].l_ypos;
            cl.px2 = ebullets[lp].xpos;    // Current bullet position
            cl.py2 = ebullets[lp].ypos;

            x_index = (int) ((ebullets[lp].xpos + 100)/Xgrid);  // Get cell bullet is in
             if (x_index<0) x_index=0;
             else if (x_index>=Xgrid_size) x_index = Xgrid_size-1;
            y_index = (int) ((ebullets[lp].ypos + 100)/Ygrid);
             if (y_index<0) y_index=0;
             else if (y_index>=Ygrid_size) y_index = Ygrid_size-1;

            int cnt = rgrid[x_index][y_index].size();           // Get number of rocks in the cell
            if (cnt>0)
            {
                vec2i *spnt = rgrid[x_index][y_index].data();
                for (lp2=0; lp2<cnt; lp2++)
                {
                    if (spnt[lp2].v[0] == ROCK)
                    {
                        if (Ebullet_rock_collision(lp, spnt[lp2].v[1], &cl)) break;
                    }
                    else if (spnt[lp2].v[0] == SHIP)
                    {
                        if (Ebullet_ship_collision(lp, spnt[lp2].v[1], &cl)) break;
                    }
                }
            }
        }
    }
}

bool Ebullet_rock_collision(int bl, int rk, circle_line *cl)
{
    if (!rocks[rk].status) return false;

    cl->cx = rocks[rk].xpos;   // Get rock position
    cl->cy = rocks[rk].ypos;
    cl->r = rocks[rk].radius;  // Get rock radius (size in radius)
    if (Circle_line_intersect(cl))   // Check if line of bullet intersects rock
    {
        float x = cl->px2 - cl->cx;
        float y = cl->py2 - cl->cy;
        float bdist1 = sqrt(x*x + y*y);
        x = cl->px2 - cl->px1;
        y = cl->py2 - cl->py1;
        float blen = sqrt(x*x + y*y);
        if (bdist1 < (cl->r+blen))              // Check current bullet position
        {
            if (Precise_bullet_hit_rock(rk, cl))
            {
                Ebullet_hit_rock(bl, rk, cl);
                return true;
            }
        }
    }
    return false;
}

bool Ebullet_ship_collision(int bl, int sh, circle_line *cl)
{
    bool shield = false;
    if (pship[sh].status != SHIP_ACTIVE) return false;
    if (shield_flag[sh] || spawn_shield_flag[sh]) shield = true;

    cl->cx = pship[sh].xpos;                    // Get ship position
    cl->cy = pship[sh].ypos;
    if (shield) cl->r = Shield_size;
    else cl->r = Ship_size;

    if (Circle_line_intersect(cl))
    {
        float x = cl->px2 - cl->cx;
        float y = cl->py2 - cl->cy;
        float bdist1 = sqrt(x*x + y*y);
        x = cl->px2 - cl->px1;
        y = cl->py2 - cl->py1;
        float blen = sqrt(x*x + y*y);
        if (bdist1 < (cl->r+blen))              // Check current bullet position
        {
            if (!shield)
            {
                // Shields off.  Ship is destroyed if there's a direct hit
                if (Precise_bullet_hit_ship(sh, cl))
                {
                    pship[sh].ships--;
                    alSourceStop(thrust_sourceid[sh]);
                    sound_thrust_flag[sh] = false;
                    alSourcef(ship_explode_sourceid[ship_snd_pntr], AL_GAIN, Sound_volume);
                    alSourcePlay(ship_explode_sourceid[ship_snd_pntr]);
                    ship_snd_pntr++;
                    if (ship_snd_pntr>=MAX_SE_EXPLODE) ship_snd_pntr = 0;

                    Start_ship_explosion(SHIP, sh, ENEMY_BULLET, bl, pship[sh].xpos, pship[sh].ypos);
                    return true;
                }
            }
            else
            {
                // Shield on.  Do impact effect.
                x = ebullets[bl].l_xpos - ebullets[bl].xdir*200.0;
                y = ebullets[bl].l_ypos - ebullets[bl].ydir*200.0;
                bdist1 = sqrt((x-cl->fx)*(x-cl->fx) + (y-cl->fy)*(y-cl->fy));
                float bdist2 = sqrt((x-cl->gx)*(x-cl->gx) + (y-cl->gy)*(y-cl->gy));
                if (bdist1 < bdist2)   // Get closest intersection
                {
                    cl->tx = cl->fx;
                    cl->ty = cl->fy;
                }
                else
                {
                    cl->tx = cl->gx;
                    cl->ty = cl->gy;
                }
                Start_impact(SHIP, sh, cl);
                ebullets[bl].status = false;

                alSourcef(ship_bounce_sourceid, AL_GAIN, Sound_volume);
                alSourcePlay(ship_bounce_sourceid);
                return true;
            }
        }
    }
    return false;
}

void Check_bomb_bullets_hit_something()
{
    int lp;
    int lp2;
    int x_index, y_index;
    float x, y;
    float bdist1;
    float blen;
    circle_line cl;

    for (lp=0; lp<MAX_BBULLETS; lp++)
    {
        if (bbullets[lp].status)
        {
            cl.px1 = bbullets[lp].l_xpos;  // Load bullet: current and last position
            cl.py1 = bbullets[lp].l_ypos;
            cl.px2 = bbullets[lp].xpos;
            cl.py2 = bbullets[lp].ypos;

            x_index = (int) ((bbullets[lp].xpos + 100)/Xgrid);  // Get cell bullet it in
             if (x_index<0) x_index=0;
            y_index = (int) ((bbullets[lp].ypos + 100)/Ygrid);
             if (y_index<0) y_index=0;
            int cnt = rgrid[x_index][y_index].size();           // Get number of rocks in the cell
            if (cnt>0)
            {
                int code;
                vec2i *spnt = rgrid[x_index][y_index].data();
                for (lp2=0; lp2<cnt; lp2++)
                {
                    code = spnt[lp2].v[0];
                    if (spnt[lp2].v[0] == ROCK)
                    {
                        if (Bbullet_rock_collision(lp, spnt[lp2].v[1], &cl)) break;
                    }
                    else if (spnt[lp2].v[0] == ENEMY)
                    {
                        if (Bbullet_enemy_collision(lp, spnt[lp2].v[1], &cl)) break;
                    }
                    else if (spnt[lp2].v[0] == DRONE)
                    {
                        if (Bbullet_drone_collision(lp, spnt[lp2].v[1], &cl)) break;
                    }
                }
            }
        }
    }
}

bool Bbullet_rock_collision(int bl, int rk, circle_line *cl)
{
    if (!rocks[rk].status) return false;

    cl->cx = rocks[rk].xpos;   // Load rock
    cl->cy = rocks[rk].ypos;
    cl->r = rocks[rk].radius*1.2;
    if (Circle_line_intersect(cl))   // Check if line of bullet intersect rock
    {
        float x = cl->px2 - cl->cx;
        float y = cl->py2 - cl->cy;
        float bdist1 = sqrt(x*x + y*y);
        x = cl->px2 - cl->px1;
        y = cl->py2 - cl->py1;
        float blen = sqrt(x*x + y*y);
        if (bdist1 < (cl->r + blen))              // Check current bullet position
        {
            if (Precise_bullet_hit_rock(rk, cl))
            {
                Bbullet_hit_rock(bl, rk, cl);
                return true;
            }
        }
    }
    return false;
}

bool Bbullet_enemy_collision(int bl, int en, circle_line *cl)
{
    if (enemy[en].status != SHIP_ACTIVE) return false;

    cl->cx = enemy[en].xpos;   // Load rock
    cl->cy = enemy[en].ypos;
    cl->r = enemy[en].xysize.v[0];  // Radius of enemy ship
    if (Circle_line_intersect(cl))
    {
        float x = cl->px2 - cl->cx;
        float y = cl->py2 - cl->cy;
        float bdist1 = sqrt(x*x + y*y);
        x = cl->px2 - cl->px1;
        y = cl->py2 - cl->py1;
        float blen = sqrt(x*x + y*y);
        if (bdist1 < (cl->r + blen))              // Check current bullet position
        {
            if (Precise_bullet_hit_enemy(en, cl))
            {
                Bbullet_hit_enemy(bl, en);
                return true;
            }
        }
    }
    return false;
}



bool Bbullet_drone_collision(int bl, int d_index, circle_line *cl)
{
    if (!drones[d_index].status) return false;
    cl->cx = drones[d_index].xpos;
    cl->cy = drones[d_index].ypos;
    cl->r = Drone_size;
    if (Circle_line_intersect(cl))
    {
        float x = cl->px2 - cl->cx;
        float y = cl->py2 - cl->cy;
        float bdist1 = sqrt(x*x + y*y);
        x = cl->px2 - cl->px1;
        y = cl->py2 - cl->py1;
        float blen = sqrt(x*x + y*y);
        if (bdist1< (cl->r+blen))
        {
            if (Precise_bullet_hit_drone(d_index, cl))
            {
                int id = drones[d_index].cluster;
                if (id>=0)
                {
                    cluster[id].status = false;
                    for (int lp=0; lp<drones.size(); lp++)
                    {
                        if (drones[lp].cluster == id)
                        {
                            drones[lp].cluster = -1;      // Turn drone into individual
                            Assign_drone(lp);             // Give drone a player ship to chase
                        }
                    }
                    bbullets[bl].status = false;
                    return true;
                }
                else
                {
                    drones[d_index].status = false;
                    bbullets[bl].status = false;

                    alSourcef(ship_explode_sourceid[ship_snd_pntr], AL_GAIN, Sound_volume);
                    alSourcePlay(ship_explode_sourceid[ship_snd_pntr]);
                    ship_snd_pntr++;
                    if (ship_snd_pntr>=MAX_SE_EXPLODE) ship_snd_pntr = 0;

                    Start_ship_explosion(DRONE, d_index, -1, -1, drones[d_index].xpos, drones[d_index].ypos);
                    return true;
                }
            }
        }
    }
    return false;
}
