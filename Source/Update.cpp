#include "Update.h"

void Update_everything()
{
    if (!demo && pship_status) Update_playerships();

    if (!rocks.empty())
    {
        if (!Rock_instancing) Update_rocks();     // Move the rocks
        else Update_instanced_rocks();
    }

    Update_enemyships();
    Update_drones();
    if (!demo && !bullets.empty()) Update_bullets();
    if (bomb_flag) Update_bomb_bullets();
    if (!ebullets.empty()) Update_enemy_bullets();
}

/// Update positions of player ships
void Update_playerships()
{
    float x, y;
    float dist;
    vec2i rcode;
    float sfactor = Ship_speed_factor*elapsed_time;

    for (int sh=0; sh<num_players; sh++)
    {
        if (pship[sh].status == SHIP_ACTIVE)            // Check if ship is active
        {
            x = pship[sh].dirx;
            y = pship[sh].diry;
            dist = sqrt(x*x + y*y);
            if (dist > Max_ship_speed)            // Limit top speed of ship
            {
                x = x/dist;
                y = y/dist;
                x*= Max_ship_speed;
                y*= Max_ship_speed;
                pship[sh].dirx = x;
                pship[sh].diry = y;
            }
            pship[sh].l_xpos = pship[sh].xpos;         // Save last ship position
            pship[sh].l_ypos = pship[sh].ypos;
            pship[sh].xpos+= pship[sh].dirx*sfactor;     // Move your ship
            pship[sh].ypos+= pship[sh].diry*sfactor;

            if (pship[sh].xpos > Xmax) pship[sh].xpos = pship[sh].xpos - Xmax;    // Wrap ship around if it goes off screen
            if (pship[sh].xpos < 0) pship[sh].xpos = Xmax + pship[sh].xpos;

            if (pship[sh].ypos > Ymax) pship[sh].ypos = pship[sh].ypos - Ymax;
            if (pship[sh].ypos < 0) pship[sh].ypos = Ymax + pship[sh].ypos;

            // Put your ship on the collision grid
            int x_index = (int) ((pship[sh].xpos+100)/Xgrid);
            int y_index = (int) ((pship[sh].ypos+100)/Ygrid);
            rcode.v[0] = SHIP;
            rcode.v[1] = sh;

            /// Put ship on collision grid
            rgrid[x_index][y_index].push_back(rcode);

            // Check for overlap into another grid cell
            int x_side = (int) ((pship[sh].xpos+100+Ship_size)/Xgrid);
            if (x_side>x_index && x_side<Xgrid_size) rgrid[x_side][y_index].push_back(rcode);
            else
            {
                x_side = (int) ((pship[sh].xpos+100-Ship_size)/Xgrid);
                if (x_side<x_index && x_side>=0) rgrid[x_side][y_index].push_back(rcode);
            }

            int y_side = (int) ((pship[sh].ypos+100+Ship_size)/Ygrid);
            if (y_side>y_index && y_side<Ygrid_size) rgrid[x_side][y_side].push_back(rcode);
            else
            {
                y_side = (int) ((pship[sh].ypos+100-Ship_size)/Ygrid);
                if (y_side<y_index && y_side>=0) rgrid[x_side][y_side].push_back(rcode);
            }

            // Update ship vertices
            vec3 pnt;
            mat4 ship = rotate_uvw(identity_mat4(), pship[sh].angle, vec3d(0, 0, 1.0));

            pship[sh].lshape = pship[sh].shape;                 // Save last vertices (for collision detection)
            for (int lp=0; lp < pship[sh].shape.size(); lp++)
            {
                pnt = ship*vec3(sh_pship.points[lp].v[0], sh_pship.points[lp].v[1], 0.0);
                pnt.v[0]*= Ship_size;
                pnt.v[1]*= Ship_size;
                pnt.v[0]+= pship[sh].xpos;
                pnt.v[1]+= pship[sh].ypos;
                pship[sh].shape[lp] = vec2(pnt.v[0], pnt.v[1]);
            }
        }
    }
}

/// Create a single large rock and starting position
rock_data Place_rock2(int type)
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

/// Move rocks and place in collision grid
void Update_rocks()
{
    int lp;
    float x, y;
    float radius;
    int x_index, y_index;
    int x_side, y_side;
    vec2i rcode;
    bool left, right, up, down;
    float sfactor = Rock_speed_factor*elapsed_time;
    float rfactor = Rock_rotation_factor*elapsed_time;
    time_sum+= sfactor;

    for (lp=0; lp<rocks.size(); lp++)
    {
        rocks[lp].xpos+= rocks[lp].xdir*sfactor;
        rocks[lp].ypos+= rocks[lp].ydir*sfactor;
    }
    for (lp=0; lp<rocks.size(); lp++)
    {
        if (rocks[lp].status)
        {
            radius = rocks[lp].radius;
            x = rocks[lp].xpos;
            y = rocks[lp].ypos;
            rocks[lp].angle+= rocks[lp].angle_step*rfactor;

            if (x > Xmax+radius)
            {
                x=-radius;
                rocks[lp].xpos = x;
            }
            else if (x<-radius)
            {
                x=Xmax+radius;
                rocks[lp].xpos = x;
            }
            if (y > Ymax+radius)
            {
                y = -radius;
                rocks[lp].ypos = y;
            }
            else if (y<-radius)
            {
                y=Ymax+radius;
                rocks[lp].ypos = y;
            }

            x_index = (int) ((x+100)/Xgrid);
             if (x_index<0) x_index = 0;
             else if (x_index>=Xgrid_size) x_index = Xgrid_size-1;
            y_index = (int) ((y+100)/Ygrid);
             if (y_index<0) y_index = 0;
             else if (y_index>=Ygrid_size) y_index = Ygrid_size-1;

            rcode.v[0] = ROCK;
            rcode.v[1] = lp;
            rgrid[x_index][y_index].push_back(rcode);

            // Check for overlap into another grid cell
            up = false;
            down = false;
            left = false;
            right = false;
            x_side = (int) ((x+100+radius)/Xgrid);
            if (x_side>x_index && x_side<Xgrid_size)
            {
                rgrid[x_side][y_index].push_back(rcode);
                right = true;
            }
            else
            {
                x_side = (int) ((x+100-radius)/Xgrid);
                if (x_side<x_index && x_side>=0)
                {
                    rgrid[x_side][y_index].push_back(rcode);
                    left = true;
                }
            }

            y_side = (int) ((y+100+radius)/Ygrid);
            if (y_side>y_index && y_side<Ygrid_size)
            {
                rgrid[x_index][y_side].push_back(rcode);
                up = true;
            }
            else
            {
                y_side = (int) ((y+100-radius)/Ygrid);
                if (y_side<y_index && y_side>=0)
                {
                    rgrid[x_index][y_side].push_back(rcode);
                    down = true;
                }
            }

            // Check if overlap into adjacent diagonal cells
            if (left || right)
            {
                if (up || down)
                {
                    rgrid[x_side][y_side].push_back(rcode);
                }
            }
        }
    }

    // Spawn new rock if it's time for it
    if (!rstarts.empty())
    {
        Rock_spawn_timer-= elapsed_time;
        if (Rock_spawn_timer<0.0)
        {
            Rock_spawn_timer = Rock_spawn_delay;
            int sz = rstarts.size()-1;
            rocks.push_back(Place_rock2(rstarts[sz]));
            rstarts.pop_back();
            total_rocks++;
        }
    }
}

/// Move rocks and place in collision grid (Instanced mode)
void Update_instanced_rocks()
{
    int lp;
    float x, y;
    float radius;
    int x_index, y_index;
    int x_side, y_side;
    vec2i rcode;
    bool left, right, up, down;
    float sfactor = Rock_speed_factor*elapsed_time;
    float rfactor = Rock_rotation_factor*elapsed_time;
    time_sum+= sfactor;

    for (lp=0; lp<rocks.size(); lp++)
    {
        rocks[lp].xpos+= rocks[lp].xdir*sfactor;
        rocks[lp].ypos+= rocks[lp].ydir*sfactor;
    }

    for (lp=0; lp<rocks.size(); lp++)
    {
        if (rocks[lp].status)
        {
            radius = rocks[lp].radius;
            x = rocks[lp].xpos;
            y = rocks[lp].ypos;
            rocks[lp].angle+= rocks[lp].angle_step*rfactor;
            sh_rocks2.angle[lp] = rocks[lp].angle;     // Update OpenGL buffer

            if (x > Xmax+radius)
            {
                x=-radius;
                rocks[lp].xpos = x;
            }
            else if (x<-radius)
            {
                x=Xmax+radius;
                rocks[lp].xpos = x;
            }
            if (y > Ymax+radius)
            {
                y = -radius;
                rocks[lp].ypos = y;
            }
            else if (y<-radius)
            {
                y=Ymax+radius;
                rocks[lp].ypos = y;
            }

            sh_rocks2.position[lp] = vec2(x, y);       // Update OpenGL buffer

            x_index = (int) ((x+100)/Xgrid);
             if (x_index<0) x_index = 0;
             else if (x_index>=Xgrid_size) x_index = Xgrid_size-1;
            y_index = (int) ((y+100)/Ygrid);
             if (y_index<0) y_index = 0;
             else if (y_index>=Ygrid_size) y_index = Ygrid_size-1;

            rcode.v[0] = ROCK;
            rcode.v[1] = lp;
            rgrid[x_index][y_index].push_back(rcode);

            // Check for overlap into another grid cell
            up = false;
            down = false;
            left = false;
            right = false;
            x_side = (int) ((x+100+radius)/Xgrid);
            if (x_side>x_index && x_side<Xgrid_size)
            {
                rgrid[x_side][y_index].push_back(rcode);
                right = true;
            }
            else
            {
                x_side = (int) ((x+100-radius)/Xgrid);
                if (x_side<x_index && x_side>=0)
                {
                    rgrid[x_side][y_index].push_back(rcode);
                    left = true;
                }
            }

            y_side = (int) ((y+100+radius)/Ygrid);
            if (y_side>y_index && y_side<Ygrid_size)
            {
                rgrid[x_index][y_side].push_back(rcode);
                up = true;
            }
            else
            {
                y_side = (int) ((y+100-radius)/Ygrid);
                if (y_side<y_index && y_side>=0)
                {
                    rgrid[x_index][y_side].push_back(rcode);
                    down = true;
                }
            }

            // Check if overlap into adjacent diagonal cells
            if (left || right)
            {
                if (up || down)
                {
                    rgrid[x_side][y_side].push_back(rcode);
                }
            }
        }
    }

    // Spawn new rock if it's time for it
    if (!rstarts.empty())
    {
        Rock_spawn_timer-= elapsed_time;
        if (Rock_spawn_timer<0.0)
        {
            Rock_spawn_timer = Rock_spawn_delay;
            int sz = rstarts.size()-1;
            rocks.push_back(Place_rock2(rstarts[sz]));
            rstarts.pop_back();
            total_rocks++;
            Rock_buffers_changed = true;

            sz = rocks.size()-1;
            sh_rocks2.active[sz] = 1;
            sh_rocks2.position[sz] = vec2(rocks[sz].xpos, rocks[sz].ypos);
            sh_rocks2.angle[sz] = 0;
            sh_rocks2.radius[sz] = rocks[sz].radius;
            sh_rocks2.color[sz] = rocks[sz].color;
        }
    }
}


void New_enemy_ship(int index)
{
    uniform_real_distribution<float>zero_one(0.0, 1.0);
    uniform_real_distribution<float>y_position(50, Ymax - 150);
    uniform_real_distribution<float>speed(Min_enemy_speed, Max_enemy_speed);
    int first = levels[game_level-1].range.v[0];
    int last = levels[game_level-1].range.v[1];
    uniform_int_distribution<int>ship_choice(first, last);

    enemy[index].status = SHIP_ACTIVE;
    int shape = ship_choice(rndrock);     // Determine ship appearance
    enemy[index].code = shape;
    enemy[index].shape_pntr = enemy_shapes_pntr[shape];
    enemy[index].raw_shape = enemy_shapes[shape];
    enemy[index].color = enemy_colors[shape];

    int test = potential_max / 4;
    if  (potential_rocks < test)
    {
        if (zero_one(rndrock) < 0.8)
        {
            if (!siren_flag)               // Small enemy ship siren sound
            {
                if (!demo)
                {
                    alSourcef(enemy_siren_sourceid, AL_GAIN, Sound_volume);
                    alSourcePlay(enemy_siren_sourceid);
                    alSourcei(enemy_siren_sourceid, AL_LOOPING, AL_TRUE);
                    siren_flag = true;
                }
            }
            enemy[index].type = SMALL_ENEMY;
            enemy[index].xysize = vec2(Enemy_ship_small, Enemy_ship_small*0.5);
        }
        else
        {
            if (!siren2_flag)               // Enemy ship siren sound
            {
                if (!demo)
                {
                    alSourcef(enemy_siren2_sourceid, AL_GAIN, 0.5*Sound_volume);
                    alSourcePlay(enemy_siren2_sourceid);
                    alSourcei(enemy_siren2_sourceid, AL_LOOPING, AL_TRUE);
                    siren2_flag = true;
                }
            }
            enemy[index].type = BIG_ENEMY;
            enemy[index].xysize = vec2(Enemy_ship_big, Enemy_ship_big*0.5);
        }
    }
    else
    {
        if (!siren2_flag)               // Big enemy ship siren sound
        {
            if (!demo)
            {
                alSourcef(enemy_siren2_sourceid, AL_GAIN, 0.5*Sound_volume);
                alSourcePlay(enemy_siren2_sourceid);
                alSourcei(enemy_siren2_sourceid, AL_LOOPING, AL_TRUE);
                siren2_flag = true;
            }
        }
        enemy[index].type = BIG_ENEMY;
        enemy[index].xysize = vec2(Enemy_ship_big, Enemy_ship_big*0.5);
    }

    if (zero_one(rndrock) < 0.5)
    {
        enemy[index].speed = speed(rndrock);
        enemy[index].xpos = -98.0;
        enemy[index].dirx = enemy[index].speed;
        enemy[index].diry = 0.0;
        enemy[index].left_right = RIGHT;
    }
    else
    {
        enemy[index].speed = speed(rndrock);
        enemy[index].xpos = Xmax+98;
        enemy[index].dirx = enemy[index].speed*-1.0;
        enemy[index].diry = 0.0;
        enemy[index].left_right = LEFT;
    }
    enemy[index].change_time = 1.5;

    enemy[index].ypos = y_position(rndrock);
    enemy[index].bullet_delay = 0;
}

/// **** Update enemy ships ****
void Update_enemyships()
{
    int lp;
    float sfactor = Enemy_speed_factor*elapsed_time;

    if (!demo)
    {
        int test = potential_max / 2;
        if (potential_rocks > test) return;
    }

    for (lp=0; lp<num_enemy; lp++)
    {
        if (!enemy[lp].status)
        {
            enemy[lp].time-= elapsed_time;
            if (enemy[lp].time < 0.0)
            {
                if (total_rocks>=1) New_enemy_ship(lp);      // Only spawn enemy ships if there's still rocks
            }
        }
        else
        {
            enemy[lp].xpos+= (enemy[lp].dirx*sfactor);
            enemy[lp].ypos+= (enemy[lp].diry*sfactor);
            if (enemy[lp].ypos<-20) enemy[lp].ypos = Ymax+19;
            if (enemy[lp].ypos > Ymax+20) enemy[lp].ypos = -19;

            if (enemy[lp].xpos<-100 || enemy[lp].xpos > Xmax+100)
            {
                enemy[lp].status = 0;  // Off screen, disable
                if (!demo)
                {
                    uniform_real_distribution<double>next_spawn(Min_enemy_speed, Max_enemy_speed);
                    enemy[lp].time = next_spawn(rndrock);
                }
                else enemy[lp].time = 1.0;
            }
            else
            {
                // Change direction of enemy ship?
                enemy[lp].change_time-= elapsed_time;
                if (enemy[lp].change_time<0)
                {
                    uniform_real_distribution<double>dir_change(1.5, 3.0);
                    enemy[lp].change_time = dir_change(rndrock);
                    if (enemy[lp].left_right == RIGHT)
                    {
                        uniform_real_distribution<float>rangle(-60.0, 60.0);
                        float angle = rangle(rndrock)*TO_RADIANS;
                        enemy[lp].dirx = cos(angle)*enemy[lp].speed;
                        enemy[lp].diry = sin(angle)*enemy[lp].speed;
                    }
                    else
                    {
                        uniform_real_distribution<float>rangle(120.0, 240.0);
                        float angle = rangle(rndrock)*TO_RADIANS;
                        enemy[lp].dirx = cos(angle)*enemy[lp].speed;
                        enemy[lp].diry = sin(angle)*enemy[lp].speed;
                    }
                }

                // Put enemy ship in the collision grid
                int x = enemy[lp].xpos;
                int y = enemy[lp].ypos;
                int radius = enemy[lp].xysize.v[0];

                int x_index = (int) ((x+100)/Xgrid);
                 if (x_index<0) x_index = 0;
                 else if (x_index>=Xgrid_size) x_index = Xgrid_size-1;
                int y_index = (int) ((y+100)/Ygrid);
                 if (y_index<0) y_index = 0;
                 else if (y_index>=Ygrid_size) y_index = Ygrid_size-1;
                vec2i rcode;
                rcode.v[0] = ENEMY;
                rcode.v[1] = lp;
                rgrid[x_index][y_index].push_back(rcode);

                // Check for overlap into another grid cell
                int x_side = (int) ((x+100+radius)/Xgrid);
                if (x_side>x_index && x_side<Xgrid_size) rgrid[x_side][y_index].push_back(rcode);
                else
                {
                    x_side = (int) ((x+100-radius)/Xgrid);
                    if (x_side<x_index && x_side>=0) rgrid[x_side][y_index].push_back(rcode);
                }

                int y_side = (int) ((y+100+radius)/Ygrid);
                if (y_side>y_index && y_side<Ygrid_size) rgrid[x_side][y_side].push_back(rcode);
                else
                {
                    y_side = (int) ((y+100-radius)/Ygrid);
                    if (y_side<y_index && y_side>=0) rgrid[x_side][y_side].push_back(rcode);
                }

                // Fire an enemy bullet
                if (enemy[lp].status == SHIP_ACTIVE)
                {
                    enemy[lp].bullet_delay+= elapsed_time;

                    float delay;
                    if (!demo) delay = Ebullet_delay;
                    else delay = Demo_bullet_delay;
                    if (enemy[lp].bullet_delay > delay)
                    {
                        Fire_enemy_bullet(lp);
                        enemy[lp].bullet_delay = 0;
                    }
                }
            }
        }
    }

    if (siren_flag)
    {
        bool no_enemy = true;
        for (lp=0; lp<num_enemy; lp++)
        {
            if (enemy[lp].status == SHIP_ACTIVE && enemy[lp].type == SMALL_ENEMY)
            {
                no_enemy = false;
            }
        }
        if (no_enemy)
        {
            alSourceStop(enemy_siren_sourceid);
            siren_flag = false;
        }
    }

    if (siren2_flag)
    {
        bool no_enemy = true;
        for (lp=0; lp<num_enemy; lp++)
        {
            if (enemy[lp].status == SHIP_ACTIVE && enemy[lp].type == BIG_ENEMY)
            {
                no_enemy = false;
            }
        }
        if (no_enemy)
        {
            alSourceStop(enemy_siren2_sourceid);
            siren2_flag = false;
        }
    }
}

void Update_bullets()
{
    int lp;
    float sfactor = Bullet_speed_factor*elapsed_time;

    for (lp=0; lp<bullets.size(); lp++)
    {
        if (bullets[lp].status)
        {
            if (bullets[lp].life > bullet_life)
            {
                bullets[lp].status = false;    // Bullet life expired
            }
            else
            {
                if (!bullets[lp].edge)
                {
                    bullets[lp].l_xpos = bullets[lp].xpos;  // Save current position
                    bullets[lp].l_ypos = bullets[lp].ypos;
                }
                else
                {
                    bullets[lp].l_xpos = bullets[lp].xstart;  // Save current position
                    bullets[lp].l_ypos = bullets[lp].ystart;
                    bullets[lp].xpos = bullets[lp].xstart;
                    bullets[lp].ypos = bullets[lp].ystart;
                }
                bullets[lp].xpos+= (bullets[lp].xdir*sfactor + bullets[lp].sx_offset);    // Move the bullet
                bullets[lp].ypos+= (bullets[lp].ydir*sfactor + bullets[lp].sy_offset);

                bullets[lp].life+= bullet_speed*sfactor;
                bullets[lp].edge = false;

                // Handle bullets going beyond edge of screen
                // Move start point of bullet to edge of screen if bullet goes off screen
                if (bullets[lp].xpos < 0)
                {
                    vec2 collide = Line_line_intersect3(vec2(0, 0), vec2(0, Ymax), vec2(bullets[lp].l_xpos, bullets[lp].l_ypos), vec2(bullets[lp].xpos, bullets[lp].ypos));
                    bullets[lp].xstart = Xmax;
                    bullets[lp].ystart = collide.v[1];
                    bullets[lp].edge = true;
                }
                else if (bullets[lp].xpos >= Xmax)
                {
                    vec2 collide = Line_line_intersect3(vec2(Xmax, 0), vec2(Xmax, Ymax), vec2(bullets[lp].l_xpos, bullets[lp].l_ypos), vec2(bullets[lp].xpos, bullets[lp].ypos));
                    bullets[lp].xstart = 0;
                    bullets[lp].ystart = collide.v[1];
                    bullets[lp].edge = true;
                }
                if (bullets[lp].ypos < 0)
                {
                    vec2 collide = Line_line_intersect3(vec2(0, 0), vec2(Xmax, 0), vec2(bullets[lp].l_xpos, bullets[lp].l_ypos), vec2(bullets[lp].xpos, bullets[lp].ypos));
                    bullets[lp].ystart = Ymax;
                    bullets[lp].xstart = collide.v[0];
                    bullets[lp].edge = true;
                }
                else if (bullets[lp].ypos >= Ymax)
                {
                    vec2 collide = Line_line_intersect3(vec2(0, Ymax), vec2(Xmax, Ymax), vec2(bullets[lp].l_xpos, bullets[lp].l_ypos), vec2(bullets[lp].xpos, bullets[lp].ypos));
                    bullets[lp].ystart = 0;
                    bullets[lp].xstart = collide.v[0];
                    bullets[lp].edge = true;
                }
            }
        }
    }
}

void Update_bomb_bullets()
{
    bool no_bullets = true;
    int lp;
    float sfactor = Bomb_speed_factor*elapsed_time;

    for (lp=0; lp<last_bbullet; lp++)
    {
        if (bbullets[lp].status)
        {
            if (bbullets[lp].life > bullet_life)
            {
                bbullets[lp].status = false;    // Bullet life expired
            }
            else
            {
                no_bullets = false;
                if (!bbullets[lp].edge)
                {
                    bbullets[lp].l_xpos = bbullets[lp].xpos;  // Save current position
                    bbullets[lp].l_ypos = bbullets[lp].ypos;
                }
                else
                {
                    bbullets[lp].l_xpos = bbullets[lp].xstart;  // Save current position
                    bbullets[lp].l_ypos = bbullets[lp].ystart;
                    bbullets[lp].xpos = bbullets[lp].xstart;
                    bbullets[lp].ypos = bbullets[lp].ystart;
                }
                bbullets[lp].xpos+= (bbullets[lp].xdir*sfactor + bbullets[lp].sx_offset);    // Move the bullet
                bbullets[lp].ypos+= (bbullets[lp].ydir*sfactor + bbullets[lp].sy_offset);
                bbullets[lp].life+= bullet_speed*sfactor;
                bbullets[lp].edge = false;

                // Handle bullets going beyond edge of screen
                // Move start point of bullet to edge of screen if bullet goes off screen
                if (bbullets[lp].xpos < 0)
                {
                    bool hit;
                    vec2 collide = Line_line_intersect(vec2(0, 0), vec2(0, Ymax), vec2(bbullets[lp].l_xpos, bbullets[lp].l_ypos), vec2(bbullets[lp].xpos, bbullets[lp].ypos), &hit);
                    bbullets[lp].xstart = Xmax;
                    bbullets[lp].ystart = collide.v[1];
                    bbullets[lp].edge = true;
                }
                else if (bbullets[lp].xpos > Xmax)
                {
                    bool hit;
                    vec2 collide = Line_line_intersect(vec2(Xmax, 0), vec2(Xmax, Ymax), vec2(bbullets[lp].l_xpos, bbullets[lp].l_ypos), vec2(bbullets[lp].xpos, bbullets[lp].ypos), &hit);
                    bbullets[lp].xstart = 0;
                    bbullets[lp].ystart = collide.v[1];
                    bbullets[lp].edge = true;
                }
                if (bbullets[lp].ypos < 0)
                {
                    bool hit;
                    vec2 collide = Line_line_intersect(vec2(0, 0), vec2(Xmax, 0), vec2(bbullets[lp].l_xpos, bbullets[lp].l_ypos), vec2(bbullets[lp].xpos, bbullets[lp].ypos), &hit);
                    bbullets[lp].ystart = Ymax;
                    bbullets[lp].xstart = collide.v[0];
                    bbullets[lp].edge = true;
                }
                else if (bbullets[lp].ypos > Ymax)
                {
                    bool hit;
                    vec2 collide = Line_line_intersect(vec2(0, Ymax), vec2(Xmax, Ymax), vec2(bbullets[lp].l_xpos, bbullets[lp].l_ypos), vec2(bbullets[lp].xpos, bbullets[lp].ypos), &hit);
                    bbullets[lp].ystart = 0;
                    bbullets[lp].xstart = collide.v[0];
                    bbullets[lp].edge = true;
                }
            }
        }
    }
    if (no_bullets) bomb_flag = false;
}

/// *** Move enemy bullets if there's any ***
void Update_enemy_bullets()
{
    int lp;
    float sfactor = Ebullet_speed_factor*elapsed_time;

    for (lp=0; lp<ebullets.size(); lp++)       // Loop through array of enemy bullets
    {
        if (ebullets[lp].status)            // Active?
        {
            if (ebullets[lp].life > ebullet_life)   // Check if bullet at maximum range
            {
                ebullets[lp].status = false;    // At max range, disable.
            }
            else
            {
                if (!ebullets[lp].edge)
                {
                    ebullets[lp].l_xpos = ebullets[lp].xpos;  // Save current position
                    ebullets[lp].l_ypos = ebullets[lp].ypos;

                    ebullets[lp].xpos+= ebullets[lp].xdir*sfactor;    // Move the bullet
                    ebullets[lp].ypos+= ebullets[lp].ydir*sfactor;
                    ebullets[lp].life+= bullet_speed*sfactor;         // Add distance bullet traveled
                }
                else
                {
                    ebullets[lp].l_xpos = ebullets[lp].xstart - ebullets[lp].x_norm*bullet_speed;  // Save current position
                    ebullets[lp].l_ypos = ebullets[lp].ystart - ebullets[lp].y_norm*bullet_speed;
                    ebullets[lp].xpos = ebullets[lp].xstart;
                    ebullets[lp].ypos = ebullets[lp].ystart;
                    ebullets[lp].edge = false;
                }


                // Handle bullets going beyond edge of screen
                // Move start point of bullet to edge of screen if bullet goes off screen
                if (ebullets[lp].xpos < 0)
                {
                    vec2 collide = Line_line_intersect3(vec2(0, 0), vec2(0, Ymax), vec2(ebullets[lp].l_xpos, ebullets[lp].l_ypos), vec2(ebullets[lp].xpos, ebullets[lp].ypos));
                    ebullets[lp].xstart = Xmax;
                    ebullets[lp].ystart = collide.v[1];
                    ebullets[lp].edge = true;
                }
                else if (ebullets[lp].xpos > Xmax)
                {
                    vec2 collide = Line_line_intersect3(vec2(Xmax, 0), vec2(Xmax, Ymax), vec2(ebullets[lp].l_xpos, ebullets[lp].l_ypos), vec2(ebullets[lp].xpos, ebullets[lp].ypos));
                    ebullets[lp].xstart = 0;
                    ebullets[lp].ystart = collide.v[1];
                    ebullets[lp].edge = true;
                }
                if (ebullets[lp].ypos < 0)
                {
                    vec2 collide = Line_line_intersect3(vec2(0, 0), vec2(Xmax, 0), vec2(ebullets[lp].l_xpos, ebullets[lp].l_ypos), vec2(ebullets[lp].xpos, ebullets[lp].ypos));
                    ebullets[lp].ystart = Ymax;
                    ebullets[lp].xstart = collide.v[0];
                    ebullets[lp].edge = true;
                }
                else if (ebullets[lp].ypos > Ymax)
                {
                    vec2 collide = Line_line_intersect3(vec2(0, Ymax), vec2(Xmax, Ymax), vec2(ebullets[lp].l_xpos, ebullets[lp].l_ypos), vec2(ebullets[lp].xpos, ebullets[lp].ypos));
                    ebullets[lp].ystart = 0;
                    ebullets[lp].xstart = collide.v[0];
                    ebullets[lp].edge = true;
                }
            }
        }
    }
}

/// **** Give drone a player ship to chase ****
void Assign_drone2(int dr)
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

/// ***** Rotate drone toward its target ship (Calculates angle to target) *****
// Process is rather messy because angle jumps back to zero at full revolution of a circle!
float Get_new_drone_angle(int d_index, float turn_angle)
{
    float drone_angle = drones[d_index].angle;             // Current angle heading of drone
    int target = drones[d_index].target;                   // Player ship drone is chasing after
    float tx = pship[target].xpos - drones[d_index].xpos;  // Get vector direction to target ship
    float ty = pship[target].ypos - drones[d_index].ypos;

    if (tx>Xmax*0.5) tx-=Xmax;                             // Go opposite direction if your ship is half screen distant or more
    else if (tx<-Xmax*0.5) tx+=Xmax;
    if (ty>Ymax*0.5) ty-=Ymax;
    else if (ty<-Ymax*0.5) ty+=Ymax;

    float to_angle = atan2(ty, tx);                        // Calculate angle from vector
    if (to_angle<0) to_angle+= 2.0*M_PI;                   // Adjust to positive angle and compensate for -sine
    to_angle-=0.5*M_PI;
    if (to_angle<0.0) to_angle+= 2.0*M_PI;

    // Find angle between angle heading of drone and angle to target ship
    vec2 toship_vec = vec2(tx, ty);
    vec2 drone_vec = vec2(-sin(drones[d_index].angle), cos(drones[d_index].angle));
    float angle = angle_of_twoVecsf(toship_vec, drone_vec);

    // Advance drone heading a little bit so we can figure which way to rotate drone to target ship
    vec2 test_vec = Rotate_2f(TO_RADIANS, drone_vec);
    float new_angle = angle_of_twoVecsf(toship_vec, test_vec);

    if (new_angle > angle)   // Rotate drone clockwise?
    {
        drone_angle-=turn_angle;
        if (drone_angle<0.0) drone_angle = 2.0*M_PI + drone_angle;              // Rotated pass zero degrees?
        if (abs(drone_angle - to_angle) < turn_angle) drone_angle = to_angle;   // Don't overshoot angle to target ship
    }
    else                     // Rotate counter clockwise
    {
        drone_angle+=turn_angle;
        if (drone_angle>2.0*M_PI) drone_angle-= 2.0*M_PI;                       // Rotated pass 360 degrees?
        if (abs(drone_angle - to_angle) < turn_angle) drone_angle = to_angle;   // Don't overshoot angle to target ship
    }
    return drone_angle;
}

/// Update drones or cluster of drones
void Update_drones()
{
    if (!demo)
    {
        int test = potential_max / 8;
        if (potential_rocks > test) return;

        if (cluster.empty())
        {
            for (int lp=0; lp<levels[game_level-1].num_clusters*(int)(reso_factor+0.4); lp++)
            {
                Add_cluster();
            }
        }
        // Spawn more clusters if they are all gone and there's still rocks
        if (!rocks.empty())
        {
           bool no_drones = true;
           for (int lp=0; lp<drones.size(); lp++)
           {
               if (drones[lp].status)
               {
                    no_drones = false;
                    break;
               }
           }
           if (no_drones)
           {
               cluster.clear();
               drones.clear();
           }
        }
    }
    if (drones.empty()) return;

    int lp;
    float x, y, s;
    float angle;
    int x_index, y_index;
    vec2i rcode;
    float ang_factor = 60.0*elapsed_time;

    for (lp=0; lp<cluster.size(); lp++)
    {
        if (cluster[lp].status)
        {
            x = cluster[lp].xpos;
            y = cluster[lp].ypos;
            x+= cluster[lp].xdir*elapsed_time;
            y+= cluster[lp].ydir*elapsed_time;

            if (!demo)
            {
                if (x<-101.0) x = Xmax+99.0;
                else if (x>Xmax+101.0) x=-99.0;
                if (y<-101.0) y = Ymax+99.0;
                else if (y>Ymax+101.0) y=-99.0;
            }
            else if (x<-100.0 || x>Xmax+100.0 || y<-100.0 || y>Ymax+100.0)
            {
                cluster[lp].status = false;
                for (int lp2=0; lp2<6; lp2++)
                {
                    drones[lp*6 + lp2].status = false;
                }
            }
            cluster[lp].xpos = x;
            cluster[lp].ypos = y;
        }
    }

    for (lp=0; lp<drones.size(); lp++)
    {
        if (drones[lp].status)
        {
            if (drones[lp].cluster>=0)
            {
                int c = drones[lp].cluster;
                x = drones[lp].x_offset + cluster[c].xpos;
                y = drones[lp].y_offset + cluster[c].ypos;

                drones[lp].xpos = x;
                drones[lp].ypos = y;

                int x_index = (int) ((x+100)/Xgrid);
                if (x_index<0) x_index = 0;
                else if (x_index>=Xgrid_size) x_index = Xgrid_size-1;
                int y_index = (int) ((y+100)/Ygrid);
                if (y_index<0) y_index=0;
                else if (y_index>=Ygrid_size) y_index = Ygrid_size-1;
                rcode.v[0] = DRONE;
                rcode.v[1] = lp;
                rgrid[x_index][y_index].push_back(rcode);
            }
            else
            {
                if (!demo)
                {
                    int tg = drones[lp].target;
                    if (tg>=0)
                    {
                        if (pship[tg].status == SHIP_ACTIVE)
                        {
                            angle = Get_new_drone_angle(lp, 3.0*TO_RADIANS * ang_factor);
                            drones[lp].angle = angle;
                        }
                        else
                        {
                            Assign_drone2(lp);
                            angle = drones[lp].angle;
                        }
                    }
                    else
                    {
                        Assign_drone2(lp);
                        angle = drones[lp].angle;
                    }
                }
                else
                {
                    angle = drones[lp].angle;
                }

                x = drones[lp].xpos;
                y = drones[lp].ypos;
                s = drones[lp].speed;
                x+= -sin(angle)*s*elapsed_time;
                y+= cos(angle)*s*elapsed_time;
                if (!demo)
                {
                    if (x<0) x = Xmax+x;
                    else if (x>Xmax) x = x - Xmax;
                    if (y<0) y = Ymax+y;
                    else if (y>Ymax) y = y - Ymax;
                }
                else
                {
                    if (x<0 || x>Xmax || y<0 || y>Ymax) drones[lp].status = false;
                }
                drones[lp].xpos = x;
                drones[lp].ypos = y;
                if (drones[lp].speed < drones[lp].tspeed)
                {
                    s+= drones[lp].accel * ang_factor;
                    if (s>drones[lp].tspeed) s = drones[lp].tspeed;
                    drones[lp].speed = s;
                }

                int x_index = (int) ((x+100)/Xgrid);
                int y_index = (int) ((y+100)/Ygrid);
                rcode.v[0] = DRONE;
                rcode.v[1] = lp;
                rgrid[x_index][y_index].push_back(rcode);
            }
        }
    }
}
