#include "Render.h"

void Draw_everything()
{
    Draw_rocks();
    Draw_ships();

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    Draw_shipbullets();
    Draw_ship_exploding();
    if (impact_cnt>0) Draw_impacts();
    if (explosion_cnt>0) Draw_rock_explosions();

    glDisable(GL_BLEND);
}

void Draw_rocks()
{
    int lp;
    vec2i rock_shape;

    if (!Rock_instancing)
    {
        glUseProgram(sh_rocks.program);
        glBindVertexArray(sh_rocks.vao);
        glUniform1f(sh_rocks.xratio_location, x_ratio);
        glUniform1f(sh_rocks.yratio_location, y_ratio);

        if (Solid_rock)
        {
            for (lp=0; lp<rocks.size(); lp++)
            {
                if (rocks[lp].status)
                {
                    glUniform3fv(sh_rocks.color_location, 1, rocks[lp].color.v);
                    glUniform2f(sh_rocks.pos_location, rocks[lp].xpos*x_ratio, rocks[lp].ypos*y_ratio);
                    glUniform1f(sh_rocks.radius_location, rocks[lp].radius);
                    glUniform1f(sh_rocks.angle_location, rocks[lp].angle);
                    rock_shape = rocks[lp].s_pntr;
                    glDrawArrays(GL_TRIANGLE_FAN, rock_shape.v[0], rock_shape.v[1]);
                }
            }
        }
        else
        {
            for (lp=0; lp<rocks.size(); lp++)
            {
                if (rocks[lp].status)
                {
                    glUniform3fv(sh_rocks.color_location, 1, rocks[lp].color.v);
                    glUniform2f(sh_rocks.pos_location, rocks[lp].xpos*x_ratio, rocks[lp].ypos*y_ratio);
                    glUniform1f(sh_rocks.radius_location, rocks[lp].radius);
                    glUniform1f(sh_rocks.angle_location, rocks[lp].angle);
                    rock_shape = rocks[lp].pntr;
                    glDrawArrays(GL_LINE_LOOP, rock_shape.v[0], rock_shape.v[1]);
                }
            }
        }
    }
    else
    {
        glUseProgram(sh_rocks2.program);
        glBindVertexArray(sh_rocks2.vao);

        // These buffers always change from last use
        glBindBuffer(GL_ARRAY_BUFFER, sh_rocks2.position_vbo);
        glBufferSubData(GL_ARRAY_BUFFER, 0, rocks.size() * sizeof(vec2), sh_rocks2.position);
        glBindBuffer(GL_ARRAY_BUFFER, sh_rocks2.angle_vbo);
        glBufferSubData(GL_ARRAY_BUFFER, 0, rocks.size() * sizeof(float), sh_rocks2.angle);

        // Don't update if buffer content didn't change
        if (Rock_buffers_changed)
        {
            glBindBuffer(GL_ARRAY_BUFFER, sh_rocks2.radius_vbo);
            glBufferSubData(GL_ARRAY_BUFFER, 0, rocks.size() * sizeof(float), sh_rocks2.radius);
            glBindBuffer(GL_ARRAY_BUFFER, sh_rocks2.active_vbo);
            glBufferSubData(GL_ARRAY_BUFFER, 0, rocks.size() * sizeof(int), sh_rocks2.active);
            glBindBuffer(GL_ARRAY_BUFFER, sh_rocks2.color_vbo);
            glBufferSubData(GL_ARRAY_BUFFER, 0, rocks.size() * sizeof(vec3), sh_rocks2.color);
        }
        glUniform1f(sh_rocks2.xratio_location, x_ratio);
        glUniform1f(sh_rocks2.yratio_location, y_ratio);
        if (Solid_rock)
        {
            glDrawArraysInstanced(GL_TRIANGLE_FAN, 0, 26, rocks.size());
        }
        else
        {
            glDrawArraysInstanced(GL_LINE_LOOP, 1, 25, rocks.size());
        }
    }
    Rock_buffers_changed = false;
}

void Draw_ships()
{
    mat4 ship, eship;
    vec4 color;

    glUseProgram(sh_pship.program);
    glBindVertexArray(sh_pship.vao);
    if (!demo && pship_status)     // Draw your ship
    {
        for (int lp=0; lp<num_players; lp++)
        {
            if (pship[lp].status == SHIP_ACTIVE)
            {
                color = vec4(pship[lp].color, 1.0);
                glUniform4fv(sh_pship.color_location, 1, color.v);
                ship = rotate_uvw(identity_mat4(), pship[lp].angle, vec3d(0, 0, 1.0));   // Generate rotation matrix for ship
                ship = scale(ship, vec3(Ship_size*x_ratio, Ship_size*y_ratio, 0));       // Add ship size
                ship = translate(ship, vec3(pship[lp].xpos*x_ratio, pship[lp].ypos*y_ratio, 0)); // Add ship position
                glUniformMatrix4fv(sh_pship.matrix_mat_location, 1, false, ship.m);      // Send matrix to ship shader
                glDrawArrays(GL_LINES, pship[lp].shape_pntr, pship[lp].raw_shape.size());                                            // Draw the ship
            }
        }
    }

    for (int lp=0; lp<num_enemy; lp++)            // Draw enemy ships
    {
        if (enemy[lp].status == SHIP_ACTIVE)
        {
            color = vec4(enemy[lp].color, 1.0);
            glUniform4fv(sh_pship.color_location, 1, color.v);
            vec2 sz = enemy[lp].xysize;                                                        // Get size of enemy ship
            eship = scale(identity_mat4(), vec3(sz.v[0]*x_ratio, sz.v[1]*y_ratio, 0));         // Generate scale matrix
            eship = translate(eship, vec3(enemy[lp].xpos * x_ratio, enemy[lp].ypos * y_ratio, 0));  // Add enemy ship location
            glUniformMatrix4fv(sh_pship.matrix_mat_location, 1, false, eship.m);                    // Send matrix to shader
            glDrawArrays(GL_LINES, enemy[lp].shape_pntr, enemy[lp].raw_shape.size());                                                          // Draw enemy ship
        }
    }
    glBindVertexArray(0);

    if (!drones.empty())
    {
        int cl;
        glUseProgram(sh_drone.program);
        glBindVertexArray(sh_drone.vao);
        glUniform1f(sh_drone.xratio_location, x_ratio);
        glUniform1f(sh_drone.yratio_location, y_ratio);
        glUniform1f(sh_drone.radius_location, Drone_size);
        int cnt=0;
        for (int lp=0; lp<drones.size(); lp++)
        {
            if (drones[lp].status)
            {
                cl = drones[lp].cluster;
                if (cl>=0)
                {
                    sh_drone.position[cnt] = vec2(cluster[cl].xpos+drones[lp].x_offset, cluster[cl].ypos+drones[lp].y_offset);
                }
                else
                {
                    sh_drone.position[cnt] = vec2(drones[lp].xpos, drones[lp].ypos);
                }
                sh_drone.angle[cnt] = drones[lp].angle;
                sh_drone.color[cnt] = drones[lp].color;
                cnt++;
            }
        }
        glBindBuffer(GL_ARRAY_BUFFER, sh_drone.position_vbo);
        glBufferSubData(GL_ARRAY_BUFFER, 0, cnt * sizeof(vec2), sh_drone.position);
        glBindBuffer(GL_ARRAY_BUFFER, sh_drone.angle_vbo);
        glBufferSubData(GL_ARRAY_BUFFER, 0, cnt * sizeof(float), sh_drone.angle);
        glBindBuffer(GL_ARRAY_BUFFER, sh_drone.color_vbo);
        glBufferSubData(GL_ARRAY_BUFFER, 0, cnt * sizeof(vec3), sh_drone.color);
        glDrawArraysInstanced(GL_LINES, 0, enemy_shapes[cluster_index].size(), cnt);
        glBindVertexArray(0);
    }
    /*
    color = vec4(WHITE, 1.0);
    glUniform4fv(sh_pship.color_location, 1, color.v);

    for (int lp=0; lp<drones.size(); lp++)
    {
        if (drones[lp].status)
        {
            if (drones[lp].cluster>=0)
            {
                eship = rotate_uvw(identity_mat4(), drones[lp].angle, vec3d(0, 0, 1.0));
                eship = scale(eship, vec3(Drone_size*x_ratio, Drone_size*y_ratio, 0));
                int cl = drones[lp].cluster;
                float x = cluster[cl].xpos + drones[lp].x_offset;
                float y = cluster[cl].ypos + drones[lp].y_offset;
                eship = translate(eship, vec3(x*x_ratio, y*y_ratio, 0));
                glUniformMatrix4fv(sh_pship.matrix_mat_location, 1, false, eship.m);                    // Send matrix to shader
                glDrawArrays(GL_LINES, enemy_shapes_pntr[cluster_index], enemy_shapes[cluster_index].size());

            }
            else
            {
                eship = rotate_uvw(identity_mat4(), drones[lp].angle, vec3d(0, 0, 1.0));
                eship = scale(eship, vec3(Drone_size*x_ratio, Drone_size*y_ratio, 0));
                eship = translate(eship, vec3(drones[lp].xpos*x_ratio, drones[lp].ypos*y_ratio, 0));
                glUniformMatrix4fv(sh_pship.matrix_mat_location, 1, false, eship.m);                    // Send matrix to shader
                glDrawArrays(GL_LINES, enemy_shapes_pntr[cluster_index], enemy_shapes[cluster_index].size());

            }
        }
    }

    glBindVertexArray(0);
    */
    // Draw ship shield if active
    Draw_shields();

    // Draw ship exhaust if your ship is accelerating.
    if (!demo)
    {
        glUseProgram(sh_exhaust.program);
        glBindVertexArray(sh_exhaust.vao);
        for (int lp=0; lp<num_players; lp++)
        {
            if (pship[lp].thrust_flag && pship[lp].status == SHIP_ACTIVE)
            {
                ship = rotate_uvw(identity_mat4(), pship[lp].angle, vec3d(0, 0, 1.0));   // Generate rotation matrix for ship
                ship = scale(ship, vec3(Ship_size*x_ratio, Ship_size*y_ratio, 0));       // Add ship size
                ship = translate(ship, vec3(pship[lp].xpos*x_ratio, pship[lp].ypos*y_ratio, 0)); // Add ship position
                glUniformMatrix4fv(sh_exhaust.matrix_mat_location, 1, false, ship.m);
                glUniform1f(sh_exhaust.seed_location, glfwGetTime());
                glDrawArrays(GL_TRIANGLES, 0, 3);
            }
        }
        glBindVertexArray(0);
    }
}

void Draw_shields()
{
    glUseProgram(sh_shield.program);
    glBindVertexArray(sh_shield.vao);
    for (int lp=0; lp<num_players; lp++)
    {
        if (pship[lp].status == SHIP_ACTIVE)
        {
            vec4 color;
            mat4 shield = identity_mat4();
            shield = scale(shield, vec3(Ship_size*1.5*x_ratio, Ship_size*1.5*y_ratio, 0));
            shield = translate(shield, vec3(pship[lp].xpos*x_ratio, pship[lp].ypos*y_ratio, 0));
            glUniformMatrix4fv(sh_shield.matrix_mat_location, 1, false, shield.m);

            color = vec4(pship[lp].color, 1.0);
            glUniform4fv(sh_shield.color_location, 1, color.v);

            if (spawn_shield_flag[lp])
            {
                double elapsed = glfwGetTime() - spawn_shield_timer[lp];
                if (elapsed > 5.0)
                {
                    spawn_shield_flag[lp] = false;
                }
                else
                {
                    double fractpart, intpart;
                    fractpart = modf(elapsed, &intpart) * 10.0;
                    int odd_even = (int) fractpart;
                    if (odd_even % 2 == 0)
                    {
                        glUniform1f(sh_shield.bright_location, 1.0);
                        glDrawArrays(GL_LINE_LOOP, 0, 24);
                    }
                }
            }
            else if (shield_flag[lp])
            {
                float bright = (Shield_life - shield_time[lp])/Shield_life;
                glUniform1f(sh_shield.bright_location, bright);
                glDrawArrays(GL_LINE_LOOP, 0, 24);
            }
        }
    }
    glBindVertexArray(0);
}

/// *** Draw ship and enemy ship bullets
void Draw_shipbullets()
{
    int lp;
    int cnt;
    int vertices;
    int x, y;
    int x2, y2;
    //float lfactor = (1.0/60.0) / elapsed_time;

    if (!bullets.empty() || !ebullets.empty() || bomb_flag)   // Check if any active bullets
    {
        int start = 0;
        glUseProgram(sh_bullets.program);               // Use shader program and buffers for bullets
        glBindVertexArray(sh_bullets.vao);
        if (!bullets.empty())
        {
            cnt = 0;
            vertices = 0;
            for (lp=0; lp<bullets.size(); lp++)
            {
                if (bullets[lp].status)
                {
                    x = bullets[lp].xpos;
                    y = bullets[lp].ypos;
                    x2 = x-bullets[lp].x_norm*Bullet_length;
                    y2 = y-bullets[lp].y_norm*Bullet_length;
                    sh_bullets.points[cnt] = vec2(x*x_ratio, y*y_ratio);
                    sh_bullets.points[cnt+1] = vec2(x2*x_ratio, y2*y_ratio);
                    sh_bullets.color[cnt] = bullets[lp].color;
                    sh_bullets.color[cnt+1] = vec3(0, 0, 0);
                    cnt+=2;
                    vertices+=2;
                }
            }
            glBindBuffer(GL_ARRAY_BUFFER, sh_bullets.points_vbo);
            glBufferSubData(GL_ARRAY_BUFFER, start, vertices * sizeof(vec2), sh_bullets.points);
            glBindBuffer(GL_ARRAY_BUFFER, sh_bullets.color_vbo);
            glBufferSubData(GL_ARRAY_BUFFER, start, vertices * sizeof(vec3), sh_bullets.color);
            glDrawArrays(GL_LINES, start, vertices);
        }
        if (!ebullets.empty())
        {
            cnt = Max_bullets*2;
            start = cnt;
            vertices = 0;

            for (lp=0; lp<ebullets.size(); lp++)
            {
                if (ebullets[lp].status)
                {
                    x = ebullets[lp].xpos;
                    y = ebullets[lp].ypos;
                    x2 = x-ebullets[lp].x_norm*Bullet_length;
                    y2 = y-ebullets[lp].y_norm*Bullet_length;
                    sh_bullets.points[cnt] = vec2(x*x_ratio, y*y_ratio);
                    sh_bullets.points[cnt+1] = vec2(x2*x_ratio, y2*y_ratio);
                    sh_bullets.color[cnt] = ebullets[lp].color;
                    sh_bullets.color[cnt+1] = vec3(0.0, 0.0, 0.0);
                    cnt+=2;
                    vertices+=2;
                }
            }
            glBindBuffer(GL_ARRAY_BUFFER, sh_bullets.points_vbo);
            glBufferSubData(GL_ARRAY_BUFFER, start*sizeof(vec2), vertices * sizeof(vec2), &sh_bullets.points[start]);
            glBindBuffer(GL_ARRAY_BUFFER, sh_bullets.color_vbo);
            glBufferSubData(GL_ARRAY_BUFFER, start*sizeof(vec3), vertices * sizeof(vec3), &sh_bullets.color[start]);
            glDrawArrays(GL_LINES, start, vertices);
        }
        if (bomb_flag)
        {
            vertices = 0;
            cnt = (Max_bullets+Max_ebullets)*2;
            start = cnt;
            for (lp=0; lp<last_bbullet; lp++)
            {
                if (bbullets[lp].status)
                {
                    x = bbullets[lp].xpos;
                    y = bbullets[lp].ypos;
                    x2 = x-bbullets[lp].x_norm*Bullet_length;
                    y2 = y-bbullets[lp].y_norm*Bullet_length;
                    sh_bullets.points[cnt] = vec2(x*x_ratio, y*y_ratio);
                    sh_bullets.points[cnt+1] = vec2(x2*x_ratio, y2*y_ratio);
                    sh_bullets.color[cnt] = bbullets[lp].color;
                    sh_bullets.color[cnt+1] = vec3(0, 0, 0);
                    cnt+=2;
                    vertices+=2;
                }
            }
            glBindBuffer(GL_ARRAY_BUFFER, sh_bullets.points_vbo);
            glBufferSubData(GL_ARRAY_BUFFER, start*sizeof(vec2), vertices * sizeof(vec2), &sh_bullets.points[start]);
            glBindBuffer(GL_ARRAY_BUFFER, sh_bullets.color_vbo);
            glBufferSubData(GL_ARRAY_BUFFER, start*sizeof(vec3), vertices * sizeof(vec3), &sh_bullets.color[start]);
            glDrawArrays(GL_LINES, start, vertices);
        }
    }
    glBindVertexArray(0);
}

void Draw_ship_exploding()
{
    int explo;
    vec4 color;

    glUseProgram(sh_shipexplode.program);
    glBindVertexArray(sh_shipexplode.vao);

    for (explo=0; explo<ship_explode.size(); explo++)     // Loop through explosion array to see if any active explosions going on.
    {
        if (ship_explode[explo].status)
        {
            ship_explode[explo].time+= elapsed_time;
            if (ship_explode[explo].time < 1.5)        // Update ship explosion
            {
                color = vec4(ship_explode[explo].color, 1.0 - ship_explode[explo].time*0.67);
                glUniform4fv(sh_shipexplode.color_location, 1, color.v);
                glUniform2f(sh_shipexplode.center_location, ship_explode[explo].xpos, ship_explode[explo].ypos);
                glUniform1f(sh_shipexplode.time_locaton, ship_explode[explo].time);
                glDrawArraysInstancedBaseInstance(GL_LINES, 0, 2, Ship_explode_debris, explo*Ship_explode_debris);
            }
            else
            {
                int code = ship_explode[explo].code.v[0];
                int index = ship_explode[explo].code.v[1];
                if (code == SHIP)
                {
                    if (pship[index].ships<0) pship[index].status = NO_SHIPS;
                    else pship[index].status = SHIP_SPAWN;
                }
                if (code == ENEMY)
                {
                    enemy[index].status = 0;
                    uniform_real_distribution<double>next_spawn(3.0, 5.0);
                    enemy[index].time = next_spawn(rndrock);
                }

                ship_explode[explo].status = false;
            }
        }
    }
    glBindVertexArray(0);
}

void Draw_impacts()
{
    int lp;
    float time;
    float current_time = glfwGetTime();
    vec4 color;
    float sfactor = Rock_speed_factor*elapsed_time;

    glUseProgram(sh_impact.program);
    glBindVertexArray(sh_impact.vao);
    glUniform1f(sh_impact.xratio_location, x_ratio);
    glUniform1f(sh_impact.yratio_location, y_ratio);

    for (lp=0; lp<impact.size(); lp++)
    {
        if (impact[lp].status)
        {
           if (current_time < impact[lp].time+1.5)
           {
               impact[lp].xrock+= impact[lp].xdir*sfactor;
               impact[lp].yrock+= impact[lp].ydir*sfactor;

               time = current_time - impact[lp].time;
               glUniform2f(sh_impact.center_location, (impact[lp].xrock+impact[lp].xpos), (impact[lp].yrock+impact[lp].ypos));
               glUniform1f(sh_impact.time_locaton, time);
               color = vec4(impact[lp].color*2.0, 1.0 - time*0.67);
               glUniform4fv(sh_impact.color_location, 1, color.v);
               glDrawArrays(GL_POINTS, lp*Impact_particles, Impact_particles);
           }
           else
           {
               impact[lp].status = false;
               impact_cnt--;
           }
        }
    }
    glBindVertexArray(0);
}

void Draw_rock_explosions()
{
    int lp;
    float time;
    float current_time = glfwGetTime();
    vec4 color;
    float sfactor = Rock_speed_factor*elapsed_time;

    glUseProgram(sh_rexplode.program);
    glBindVertexArray(sh_rexplode.vao);
    glUniform1f(sh_rexplode.xratio_location, x_ratio);
    glUniform1f(sh_rexplode.yratio_location, y_ratio);

    for (lp=0; lp<rock_explode.size(); lp++)
    {
        if (rock_explode[lp].status)
        {
           if (current_time < rock_explode[lp].time+EXPLODE_TIME)
           {
               time = current_time - rock_explode[lp].time;
               rock_explode[lp].xpos+= rock_explode[lp].xdir*sfactor;
               rock_explode[lp].ypos+= rock_explode[lp].ydir*sfactor;
               glUniform2f(sh_rexplode.center_location, rock_explode[lp].xpos, rock_explode[lp].ypos);
               glUniform1f(sh_rexplode.time_locaton, time);
               color = vec4(rock_explode[lp].color*2.0, 1.0-time*(1.0/EXPLODE_TIME));
               glUniform4fv(sh_rexplode.color_location, 1, color.v);
               glDrawArrays(GL_POINTS, lp*Rock_explode_debris, Rock_explode_debris);
           }
           else
           {
               rock_explode[lp].status = false;
               explosion_cnt--;
           }
        }
    }
    glBindVertexArray(0);

    glUseProgram(sh_miniexplode.program);
    glBindVertexArray(sh_miniexplode.vao);
    glUniform1f(sh_miniexplode.xratio_location, x_ratio);
    glUniform1f(sh_miniexplode.yratio_location, y_ratio);

    for (lp=0; lp<mini_explode.size(); lp++)
    {
        if (mini_explode[lp].status)
        {
           if (current_time < mini_explode[lp].time+EXPLODE_TIME)
           {
               time = current_time - mini_explode[lp].time;
               mini_explode[lp].xpos+= mini_explode[lp].xdir*sfactor;
               mini_explode[lp].ypos+= mini_explode[lp].ydir*sfactor;
               glUniform2f(sh_miniexplode.center_location, mini_explode[lp].xpos, mini_explode[lp].ypos);
               glUniform1f(sh_miniexplode.time_locaton, time);
               color = vec4(mini_explode[lp].color*2.0, 1.0 - time*(1.0/EXPLODE_TIME));
               glUniform4fv(sh_miniexplode.color_location, 1, color.v);
               if (!Solid_rock)
               {
                    glDrawArraysInstancedBaseInstance(GL_LINE_LOOP, lp*10, 10, Mini_explode_debris, lp*Mini_explode_debris);
               }
               else
               {
                   glDrawArraysInstancedBaseInstance(GL_TRIANGLE_FAN, lp*12 + 10*Max_mini_explode, 12, Mini_explode_debris, lp*Mini_explode_debris);
               }
           }
           else
           {
               mini_explode[lp].status = false;
           }
        }
    }
    glBindVertexArray(0);
}
