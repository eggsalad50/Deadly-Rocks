#include "Text.h"

// Entering initials
int k_first_initial;
int k_middle_initial;
int k_last_initial;
int initial_pos;

void Draw_score()
{
    char c[50];

    glUseProgram(sh_2Dtext.program);
    glBindVertexArray(sh_2Dtext.vao);

    Set_textsize(60, 90);
    sprintf(c,"Score:\n");
    Draw_textrow(c, 1, g_win_height - 2*row_size, 0.5, vec4(DEEP_SKY_BLUE, 1.0));
    if (!demo)
    {
        sprintf(c,"Ships:\n");
        Draw_textrow(c, 1.3, g_win_height - 3.2*row_size, 0.5, vec4(DEEP_SKY_BLUE, 1.0));
    }
    else
    {
        sprintf(c,"Rank:\n");
        Draw_textrow(c, 1.7, g_win_height - 3.2*row_size, 0.5, vec4(DEEP_SKY_BLUE, 1.0));
        sprintf(c,"Skill:\n");
        Draw_textrow(c, 1.9, g_win_height - 4.4*row_size, 0.5, vec4(DEEP_SKY_BLUE, 1.0));
    }
    for (int lp=0; lp<num_players; lp++)
    {
        sprintf(c,"%i\n", pship[lp].score);
        Draw_textrow(c, 5.2+(lp*5), g_win_height - 2*row_size, 0.5, vec4(pship[lp].color, 1.0));
        if (pship[lp].ships > 0)
        {
            sprintf(c,"%i\n", pship[lp].ships);
            Draw_textrow(c, 5.2+(lp*5), g_win_height - 3.2*row_size, 0.5, vec4(pship[lp].color, 1.0));
        }
        else
        {
            if (!demo)
            {
                sprintf(c,"Empty\n");
                Draw_textrow(c, 5.2+(lp*5), g_win_height - 3.2*row_size, 0.5, vec4(pship[lp].color, 1.0));
            }
            else
            {
                sprintf(c,"%s\n", pship[lp].srank.c_str());
                Draw_textrow(c, 5.2+(lp*5), g_win_height - 3.2*row_size, 0.5, vec4(pship[lp].color, 1.0));
                float dv = (float) pship[lp].hits;
                float nu = (float) pship[lp].fired;
                if (nu==0.0) nu=1.0;
                sprintf(c,"%0.1f\%\n",  (dv/nu)*100.0);
                Draw_textrow(c, 5.2+(lp*5), g_win_height - 4.4*row_size, 0.5, vec4(pship[lp].color, 1.0));
            }
        }
    }
    Bonus_ship_text();
    glBindVertexArray(0);
}

void Performance_text(float frames)
{
    glUseProgram(sh_2Dtext.program);
    glBindVertexArray(sh_2Dtext.vao);

    char c[50];
    Set_textsize(60, 90);
    sprintf(c, "Frame rate  %0.1f  bullets %i\n", frames, ebullets.size());
    Draw_textrow(c, 1.0, g_win_height - 6.5*row_size, 0.5, vec4(WHITE, 1.0));
    glBindVertexArray(0);
}

/// Show bonus ship points message periodically.  Also show current game level.
void Bonus_ship_text()
{
    static double time = glfwGetTime();
    double time2 = glfwGetTime() - time;

    if (time2<5.0)
    {
        char c[50];
        Set_textsize(90,120);
        sprintf(c, "Bonus ship every %i points\n", Extra_ship_points);
        Draw_textrow(c, 50.0, g_win_height - 6.5*row_size, 0.5, vec4(WHITE, 1.0));
    }
    else if (time2>15.0 and time2<20.0)
    {
        char c[50];
        Set_textsize(90, 120);
        sprintf(c, "Level %i\n", game_level);
        Draw_textrow(c, 52.0, g_win_height - 6.5*row_size, 0.5, vec4(WHITE, 1.0));
    }
    else
    {
        if (time2>30.0) time = glfwGetTime();
    }
}

void Demo_text()
{
    vec4 tcolor = vec4 (WHITE, 1.0);

    glUseProgram(sh_2Dtext.program);
    glBindVertexArray(sh_2Dtext.vao);
     Set_textsize(15, 20);
    Draw_phrase(TITLE, 5.0, g_win_height - 2*row_size, 0.5, tcolor);    // Deadly rocks
     Set_textsize(30, 40);
    Draw_phrase(F1_TO_START, 17.3, g_win_height - 6*row_size, 0.5, tcolor);    // F1 to start
    Set_textsize(30, 40);
    Draw_phrase(F2_SETTINGS, 15.0, g_win_height - 7.8*row_size, 0.5, tcolor);     // F2 settings
    Draw_phrase(F3_MISC_SETTINGS, 15.0, g_win_height - 9.6*row_size, 0.5, tcolor);
    glBindVertexArray(0);

    if (controls) Controls_text(15.0);
    else if (miscell) Misc_settings_text(15.0);
    else
    {
        if (hiscore_timer < CREDITS_ON && hiscore_timer > CREDITS_OFF) Draw_credits(13);
        if (hiscore_timer < HI_SCORE_ON && hiscore_timer > HI_SCORE_OFF) Draw_scores_text(13);
        if (hiscore_timer < RANK_ON && hiscore_timer > RANK_OFF) Draw_ranks(13);
    }
}

string Gamepad_buttons_text(int code)
{
    string text;
    switch(code)
    {
        case GLFW_GAMEPAD_BUTTON_A:
            text="Button A\n";
            break;
        case GLFW_GAMEPAD_BUTTON_B:
            text="Button B\n";
            break;
        case GLFW_GAMEPAD_BUTTON_X:
            text="Button X\n";
            break;
        case GLFW_GAMEPAD_BUTTON_Y:
            text="Button Y\n";
            break;
        case GLFW_GAMEPAD_BUTTON_LEFT_BUMPER:
            text="Button left bumper\n";
            break;
        case GLFW_GAMEPAD_BUTTON_RIGHT_BUMPER:
            text="Button right bumper\n";
            break;
        case GLFW_GAMEPAD_BUTTON_BACK:
            text="Button Back\n";
            break;
        case GLFW_GAMEPAD_BUTTON_START:
            text="Button Start\n";
            break;
        case GLFW_GAMEPAD_BUTTON_GUIDE:
            text="Button Guide\n";
            break;
        case GLFW_GAMEPAD_BUTTON_LEFT_THUMB:
            text="Button left thumb\n";
            break;
        case GLFW_GAMEPAD_BUTTON_DPAD_UP:
            text="Button Dpad Up\n";
            break;
        case GLFW_GAMEPAD_BUTTON_DPAD_RIGHT:
            text="Button Dpad Right\n";
            break;
        case GLFW_GAMEPAD_BUTTON_DPAD_DOWN:
            text="Button Dpad Down\n";
            break;
        case GLFW_GAMEPAD_BUTTON_DPAD_LEFT:
            text="Button Dpad Left\n";
    }
    return text;
}

string Gamepad_axis_text(int code)
{
    string text;

    switch (code)
    {
        case GLFW_GAMEPAD_AXIS_LEFT_X:
            text="Left axis X\n";
            break;
        case GLFW_GAMEPAD_AXIS_LEFT_Y:
            text="Left axis Y\n";
            break;
        case GLFW_GAMEPAD_AXIS_RIGHT_X:
            text="Right axis X\n";
            break;
        case GLFW_GAMEPAD_AXIS_RIGHT_Y:
            text="Right axis Y\n";
            break;
        case GLFW_GAMEPAD_AXIS_LEFT_TRIGGER:
            text="Trigger axis Left\n";
            break;
        case GLFW_GAMEPAD_AXIS_RIGHT_TRIGGER:
            text="Trigger axis Right\n";
    }
    return text;
}

void Controls_text(float row)
{
    vec4 tcolor = vec4 (WHITE, 1.0);
    vec4 hcolor = vec4 (GOLD, 1.0);
    vec4 color;
    char c[50];
    float lmargin = 23.0;
    Set_textsize(40, 60);
    string controller;
    string text;
    string players = to_string(k_num_players);
    bool gamepad = false;

    glUseProgram(sh_2Dtext.program);
    glBindVertexArray(sh_2Dtext.vao);

    if (pship[player_menu].controller == GAMEPAD) gamepad = true;

    if (settings_row == 0) color = hcolor;
    else color = tcolor;
    sprintf(c,"Number of players: %s\n", players.c_str());
    Draw_textrow(c, lmargin, g_win_height - row*row_size, 0.5, color);

    if (settings_row == 1) color = hcolor;
    else color = tcolor;
    row+= 1.5;
    sprintf(c,"Player: %i\n", player_menu+1);
    Draw_textrow(c, lmargin, g_win_height - row*row_size, 0.5, color);

    if (settings_row == 2) color = hcolor;
    else color = tcolor;
    row+= 1.5;
    if (pship[player_menu].controller == KEYBOARD) controller = "KeyBoard\n";
    if (pship[player_menu].controller == NPC) controller = "NPC";
    if (pship[player_menu].controller == GAMEPAD) controller = joystick_name[0];
    sprintf(c,"Controller: %s\n", controller.c_str());
    Draw_textrow(c, lmargin, g_win_height - row*row_size, 0.5, color);

    if (settings_row == 3) color = hcolor;
    else color = tcolor;
    row+=1.5;
    if (pship[player_menu].controller == KEYBOARD)
    {
        if (k_rotate_left == 32) sprintf(c,"Rotate left: SPACE\n");
        else sprintf(c,"Rotate left: %c\n", k_rotate_left);
        Draw_textrow(c, lmargin, g_win_height - row*row_size, 0.5, color);
    }
    else
    {
        if (gamepad)
        {
            text = Gamepad_axis_text(pship[player_menu].rotate_left);
            sprintf(c,"Rotate left: %s\n", text.c_str());
        }
        else sprintf(c,"Rotate left: Auto\n");
        Draw_textrow(c, lmargin, g_win_height - row*row_size, 0.5, color);
    }

    if (settings_row == 4) color = hcolor;
    else color = tcolor;
    row+= 1.5;
    if (pship[player_menu].controller == KEYBOARD)
    {
        if (k_rotate_right == 32) sprintf(c,"Rotate right: SPACE\n");
        else sprintf(c,"Rotate right: %c\n", k_rotate_right);
        Draw_textrow(c, lmargin, g_win_height - row*row_size, 0.5, color);
    }
    else
    {
        if (gamepad)
        {
            text = Gamepad_axis_text(pship[player_menu].rotate_right);
            sprintf(c,"Rotate right: %s\n", text.c_str());
        }
        else sprintf(c,"Rotate right: Auto\n");
        Draw_textrow(c, lmargin, g_win_height - row*row_size, 0.5, color);
    }

    if (settings_row == 5) color = hcolor;
    else color = tcolor;
    row+= 1.5;
    if (pship[player_menu].controller == KEYBOARD)
    {
        if (k_thrust == 32) sprintf(c,"Thrust: SPACE\n");
        else sprintf(c,"Thrust: %c\n", k_thrust);
        Draw_textrow(c, lmargin, g_win_height - row*row_size, 0.5, color);
    }
    else
    {
        if (gamepad)
        {
            text = Gamepad_axis_text(pship[player_menu].thrust);
            sprintf(c,"Thrust: %s\n", text.c_str());
        }
        else sprintf(c, "Thrust: Auto\n");
        Draw_textrow(c, lmargin, g_win_height - row*row_size, 0.5, color);
    }

    if (settings_row == 6) color = hcolor;
    else color = tcolor;
    row+= 1.5;
    if (pship[player_menu].controller == KEYBOARD)
    {
        if (k_fire == 32) sprintf(c,"Fire: SPACE\n");
        else sprintf(c,"Fire: %c\n", k_fire);
        Draw_textrow(c, lmargin, g_win_height - row*row_size, 0.5, color);
    }
    else
    {
        if (gamepad)
        {
            text = Gamepad_buttons_text(pship[player_menu].fire);
            sprintf(c,"Fire: %s\n", text.c_str());
        }
        else sprintf(c,"Fire: Auto\n");
        Draw_textrow(c, lmargin, g_win_height - row*row_size, 0.5, color);
    }

    if (settings_row == 7) color = hcolor;
    else color = tcolor;
    row+= 1.5;
    if (pship[player_menu].controller == KEYBOARD)
    {
        if (k_bomb == 32) sprintf(c,"Bomb: SPACE\n");
        else sprintf(c,"Bomb: %c\n", k_bomb);
        Draw_textrow(c, lmargin, g_win_height - row*row_size, 0.5, color);
    }
    else
    {
        if (gamepad)
        {
            text = Gamepad_buttons_text(pship[player_menu].bomb);
            sprintf(c,"Bomb: %s\n", text.c_str());
        }
        else sprintf(c,"Bomb: Auto\n");
        Draw_textrow(c, lmargin, g_win_height - row*row_size, 0.5, color);
    }

    if (settings_row == 8) color = hcolor;
    else color = tcolor;
    row+= 1.5;
    if (pship[player_menu].controller == KEYBOARD)
    {
        if (k_shield == 32) sprintf(c,"Shields: SPACE\n");
        else sprintf(c,"Shields: %c\n", k_shield);
        Draw_textrow(c, lmargin, g_win_height - row*row_size, 0.5, color);
    }
    else
    {
        if (gamepad)
        {
            text = Gamepad_buttons_text(pship[player_menu].shields);
            sprintf(c,"Shields: %s\n", text.c_str());
        }
        else sprintf(c, "Shields: Auto\n");
        Draw_textrow(c, lmargin, g_win_height - row*row_size, 0.5, color);
    }

    if (settings_row == 9) color = hcolor;
    else color = tcolor;
    row+= 2.0;
    if (!Vsync)
    {
        sprintf(c,"Sync to monitor: Disabled\n");
    }
    else
    {
        sprintf(c,"Sync to monitor: Enabled\n");
    }
    Draw_textrow(c, lmargin, g_win_height - row*row_size, 0.5, color);

    if (settings_row == 10) color = hcolor;
    else color = tcolor;
    sprintf(c,"Resolution: %s\n", reso_text.c_str());
    row+=2.0;
    Draw_textrow(c, lmargin, g_win_height - row*row_size, 0.5, color);

    if (settings_row == 11) color = hcolor;
    else color = tcolor;
    sprintf(c,"Sound Volume: %i\%\n", (int) (Sound_volume*100.0));
    row+=2.0;
    Draw_textrow(c, lmargin, g_win_height - row*row_size, 0.5, color);

    glBindVertexArray(0);
}

void Misc_settings_text(float row)
{
    vec4 tcolor = vec4 (WHITE, 1.0);
    vec4 hcolor = vec4 (GOLD, 1.0);
    vec4 color;
    char c[50];
    float lmargin = 23.0;
    Set_textsize(40, 60);

    glUseProgram(sh_2Dtext.program);
    glBindVertexArray(sh_2Dtext.vao);

    if (misc_row == 0) color = hcolor;
    else color = tcolor;
    sprintf(c,"Ship Inventory: %i\n", Start_ships);
    Draw_textrow(c, lmargin, g_win_height - row*row_size, 0.5, color);

    if (misc_row == 1) color = hcolor;
    else color = tcolor;
    row+= 1.5;
    sprintf(c,"Starting level: %i\n", Starting_level);
    Draw_textrow(c, lmargin, g_win_height - row*row_size, 0.5, color);

    if (misc_row == 2) color = hcolor;
    else color = tcolor;
    row+= 1.5;
    sprintf(c,"Bonus ship at %i  points\n", Extra_ship_points);
    Draw_textrow(c, lmargin, g_win_height - row*row_size, 0.5, color);

    if (misc_row == 3) color = hcolor;
    else color = tcolor;
    row+= 1.5;
    if (!Player_hit_player) sprintf(c,"Teamwork: True\n");
    else sprintf(c, "Teamwork: False\n");
    Draw_textrow(c, lmargin, g_win_height - row*row_size, 0.5, color);

    if (misc_row == 4) color = hcolor;
    else color = tcolor;
    row+= 1.5;
    sprintf(c,"Shield life: %0.1f seconds\n", Shield_life);
    Draw_textrow(c, lmargin, g_win_height - row*row_size, 0.5, color);

    if (misc_row == 5) color = hcolor;
    else color = tcolor;
    row+= 1.5;
    sprintf(c,"Bomb recharge time: %0.1f seconds\n", Bomb_delay);
    Draw_textrow(c, lmargin, g_win_height - row*row_size, 0.5, color);

    if (misc_row == 6) color = hcolor;
    else color = tcolor;
    row+= 1.5;
    sprintf(c,"Line width: %0.1f\n", Line_width);
    Draw_textrow(c, lmargin, g_win_height - row*row_size, 0.5, color);

    if (misc_row == 7) color = hcolor;
    else color = tcolor;
    row+= 1.5;
    sprintf(c,"Rock speed factor: %0.1f\n", Rock_speed_factor);
    Draw_textrow(c, lmargin, g_win_height - row*row_size, 0.5, color);

    if (misc_row == 8) color = hcolor;
    else color = tcolor;
    row+= 1.5;
    sprintf(c,"Enemy speed factor: %0.1f\n", Enemy_speed_factor);
    Draw_textrow(c, lmargin, g_win_height - row*row_size, 0.5, color);
}

void Draw_credits(float row)
{
    vec4 tcolor = vec4(WHITE, 1.0);

    glUseProgram(sh_2Dtext.program);
    glBindVertexArray(sh_2Dtext.vao);

    Set_textsize(30, 40);
    Draw_phrase(PROGRAMMER, 11.0, g_win_height - row*row_size, 0.5, tcolor);

    Set_textsize(45, 60);
    row*=1.5;
    row+=2;
    Draw_phrase(PROGRAM_TOOLS, 24.0, g_win_height - row*row_size, 0.5, tcolor);
    row+=1.5;
    Draw_phrase(COMPILER, 26.0, g_win_height - row*row_size, 0.5, tcolor);
    row+=1.5;
    Draw_phrase(OPENGL, 26.0, g_win_height - row*row_size, 0.5, tcolor);
    row+=1.5;
    Draw_phrase(GLFW32, 26.0, g_win_height - row*row_size, 0.5, tcolor);
    row+=1.5;
    Draw_phrase(OPENAL11, 26.0, g_win_height - row*row_size, 0.5, tcolor);
     row+=1.5;
    Draw_phrase(CODE_BLOCKS, 26.0, g_win_height - row*row_size, 0.5, tcolor);

    glBindVertexArray(0);
}

void Draw_scores_text(float row)
{
    char c[20];
    vec4 tcolor = vec4(WHITE, 1.0);

    glUseProgram(sh_2Dtext.program);
    glBindVertexArray(sh_2Dtext.vao);

    Set_textsize(28, 38);
    sprintf(c, "High Scores\n");
    Draw_textrow(c, 15.7, g_win_height - row*row_size, 0.5, tcolor);

    row*=1.57;

    Set_textsize(40, 60);
    for (int lp=0; lp<p_scores.size(); lp++)
    {
        sprintf(c, "%s  %i\n", p_scores[lp].name.c_str(), p_scores[lp].points);
        Draw_textrow(c, 27.0, g_win_height - row*row_size, 0.5, tcolor);
        row+= 1.3;
    }
    glBindVertexArray(0);
}

void Draw_ranks(float row)
{
    Set_textsize(50, 80);
    row*= 50.0/30.0;
    vec4 tcolor = vec4(WHITE, 1.0);
    char c[100];
    float c1 = 30.0;
    float c2 = 40.0;
    float add_row = 1.2;

    glUseProgram(sh_2Dtext.program);
    glBindVertexArray(sh_2Dtext.vao);

    Draw_phrase(RANK_TEXT, 32.0, g_win_height - row*row_size, 0.5, tcolor);

    row+=add_row*2.0;
    Draw_phrase(SR_SHORT, c1, g_win_height - row*row_size, 0.5, tcolor);
    Draw_phrase(SR_TEXT, c2, g_win_height - row*row_size, 0.5, tcolor);

    row+=add_row;
    Draw_phrase(SB_SHORT, c1, g_win_height - row*row_size, 0.5, tcolor);
    Draw_phrase(SB_TEXT, c2, g_win_height - row*row_size, 0.5, tcolor);

    row+=add_row;
    Draw_phrase(SMN_SHORT, c1, g_win_height - row*row_size, 0.5, tcolor);
    Draw_phrase(SMN_TEXT, c2, g_win_height - row*row_size, 0.5, tcolor);

    row+=add_row;
    Draw_phrase(S1C_SHORT, c1, g_win_height - row*row_size, 0.5, tcolor);
    Draw_phrase(S1C_TEXT, c2, g_win_height - row*row_size, 0.5, tcolor);

    row+=add_row;
    Draw_phrase(SSRS_SHORT, c1, g_win_height - row*row_size, 0.5, tcolor);
    Draw_phrase(SSRS_TEXT, c2, g_win_height - row*row_size, 0.5, tcolor);

    row+=add_row;
    Draw_phrase(SSGT_SHORT, c1, g_win_height - row*row_size, 0.5, tcolor);
    Draw_phrase(SSGT_TEXT, c2, g_win_height - row*row_size, 0.5, tcolor);

    row+=add_row;
    Draw_phrase(TSGT_SHORT, c1, g_win_height - row*row_size, 0.5, tcolor);
    Draw_phrase(TSGT_TEXT, c2, g_win_height - row*row_size, 0.5, tcolor);

    row+=add_row;
    Draw_phrase(MSGT_SHORT, c1, g_win_height - row*row_size, 0.5, tcolor);
    Draw_phrase(MSGT_TEXT, c2, g_win_height - row*row_size, 0.5, tcolor);

    row+=add_row;
    Draw_phrase(SMSGT_SHORT, c1, g_win_height - row*row_size, 0.5, tcolor);
    Draw_phrase(SMSGT_TEXT, c2, g_win_height - row*row_size, 0.5, tcolor);

    row+=add_row;
    Draw_phrase(CMSGT_SHORT, c1, g_win_height - row*row_size, 0.5, tcolor);
    Draw_phrase(CMSGT_TEXT, c2, g_win_height - row*row_size, 0.5, tcolor);

    row+=add_row;
    Draw_phrase(LT2D_SHORT, c1, g_win_height - row*row_size, 0.5, tcolor);
    Draw_phrase(LT2D_TEXT, c2, g_win_height - row*row_size, 0.5, tcolor);

    row+=add_row;
    Draw_phrase(LT1D_SHORT, c1, g_win_height - row*row_size, 0.5, tcolor);
    Draw_phrase(LT1D_TEXT, c2, g_win_height - row*row_size, 0.5, tcolor);

    row+=add_row;
    Draw_phrase(CAPT_SHORT, c1, g_win_height - row*row_size, 0.5, tcolor);
    Draw_phrase(CAPT_TEXT, c2, g_win_height - row*row_size, 0.5, tcolor);

    row+=add_row;
    Draw_phrase(MAJ_SHORT, c1, g_win_height - row*row_size, 0.5, tcolor);
    Draw_phrase(MAJ_TEXT, c2, g_win_height - row*row_size, 0.5, tcolor);

    row+=add_row;
    Draw_phrase(LTCOL_SHORT, c1, g_win_height - row*row_size, 0.5, tcolor);
    Draw_phrase(LTCOL_TEXT, c2, g_win_height - row*row_size, 0.5, tcolor);

    row+=add_row;
    Draw_phrase(COL_SHORT, c1, g_win_height - row*row_size, 0.5, tcolor);
    Draw_phrase(COL_TEXT, c2, g_win_height - row*row_size, 0.5, tcolor);

    row+=add_row;
    Draw_phrase(BRGEN_SHORT, c1, g_win_height - row*row_size, 0.5, tcolor);
    Draw_phrase(BRGEN_TEXT, c2, g_win_height - row*row_size, 0.5, tcolor);

    row+=add_row;
    Draw_phrase(MJGEN_SHORT, c1, g_win_height - row*row_size, 0.5, tcolor);
    Draw_phrase(MJGEN_TEXT, c2, g_win_height - row*row_size, 0.5, tcolor);

    row+=add_row;
    Draw_phrase(LTGEN_SHORT, c1, g_win_height - row*row_size, 0.5, tcolor);
    Draw_phrase(LTGEN_TEXT, c2, g_win_height - row*row_size, 0.5, tcolor);

    row+=add_row;
    Draw_phrase(GEN_SHORT, c1, g_win_height - row*row_size, 0.5, tcolor);
    Draw_phrase(GEN_TEXT, c2, g_win_height - row*row_size, 0.5, tcolor);

    glBindVertexArray(0);
}

void Game_over_text()
{
    vec4 tcolor = vec4 (WHITE, 1.0);
    char c[50];

    glUseProgram(sh_2Dtext.program);
    glBindVertexArray(sh_2Dtext.vao);

    Set_textsize(15, 20);
    Draw_phrase(GAMEOVER, 6.0, g_win_height - 2*row_size, 0.5, tcolor);

    Set_textsize(30, 40);

    Draw_phrase(PLAY_AGAIN, 15.3, g_win_height - 8*row_size, 0.5, tcolor);
    Draw_phrase(F2_SETTINGS, 15.0, g_win_height - 9.8*row_size, 0.5, tcolor);
    Draw_phrase(F3_MISC_SETTINGS, 15.0, g_win_height - 11.6*row_size, 0.5, tcolor);
    glBindVertexArray(0);

    if (new_high_score)
    {
        Enter_player_name(15);
    }
    else
    {
        if (controls) Controls_text(17.5);
        else if (miscell) Misc_settings_text(17.5);
        else
        {
            if (hiscore_timer < CREDITS_ON && hiscore_timer > CREDITS_OFF) Draw_credits(13.5);
            if (hiscore_timer < HI_SCORE_ON && hiscore_timer > HI_SCORE_OFF) Draw_scores_text(13.5);
            if (hiscore_timer < RANK_ON && hiscore_timer > RANK_OFF) Draw_ranks(13.5);
        }
    }
}

void Enter_player_name(float row)
{
    char c[30];
    int player = player_highscore_list[new_high_score-1].v[0];
    vec3 color = pship[player].color;
    vec4 tcolor = vec4(color, 1.0);
    vec4 tcolor1 = vec4 (color, 1.0);
    vec4 tcolor2 = vec4 (color, 1.0);
    vec4 tcolor3 = vec4 (color, 1.0);

    glUseProgram(sh_2Dtext.program);
    glBindVertexArray(sh_2Dtext.vao);

    double time = glfwGetTime();
    time = time - (double) ((int) time);

    if (initial_pos == 0)
    {
        if (time<0.5) tcolor1 = vec4(HALF_WHITE, 1.0);
        else tcolor1 =  vec4(WHITE, 1.0);
    }
    if (initial_pos == 1)
    {
        if (time<0.5) tcolor2 =  vec4(HALF_WHITE, 1.0);
        else tcolor2 =  vec4(WHITE, 1.0);
    }
    if (initial_pos == 2)
    {
        if (time<0.5) tcolor3 =  vec4(HALF_WHITE, 1.0);
        else tcolor3 =  vec4(WHITE, 1.0);
    }

    Set_textsize(30, 40);
    sprintf(c,"Player %i New High Score!!\n", player+1);
    Draw_textrow(c, 12, g_win_height - row*row_size, 0.5, tcolor);

    row+=1.5;
    sprintf(c,"Enter your initials below\n");
    Draw_textrow(c, 13, g_win_height - row*row_size, 0.5, tcolor);

    Set_textsize(20.0, 30.0);
    row*= 0.75;
    row+= 1.0;

    sprintf(c,"%c\n", k_first_initial);
    Draw_textrow(c, 13.0, g_win_height - row*row_size, 0.5, tcolor1);
    sprintf(c,"%c\n", k_middle_initial);
    Draw_textrow(c, 14.0, g_win_height - row*row_size, 0.5, tcolor2);
    sprintf(c,"%c\n", k_last_initial);
    Draw_textrow(c, 15.0, g_win_height - row*row_size, 0.5, tcolor3);\

    glBindVertexArray(0);
}

bool sort_points(const player_scores &lhs, const player_scores &rhs)
{
    return lhs.points > rhs.points;
}

void Sort_high_score(vector<player_scores> &scores)
{
    sort(scores.begin(), scores.end(), sort_points);
}

void Check_for_highscores()
{
    int lp;
    vector<int> score;
    player_highscore_list.clear();
    score.clear();
    for (lp=0; lp<p_scores.size(); lp++)
    {
        score.push_back(p_scores[lp].points);
    }
    for (lp=0; lp<num_players; lp++)
    {
        score.push_back(pship[lp].score);
    }
    sort(score.begin(), score.end(), greater<int>());
    if (score.size()>15) score.resize(15);

    // Get players that are on high score list
    for (lp=0; lp<num_players; lp++)
    {
        for (int lp2=0; lp2<score.size(); lp2++)
        {
            if (pship[lp].score == score[lp2])
            {
                vec2i ps = vec2i(lp, score[lp2]);
                player_highscore_list.push_back(ps);
                break;
            }
        }
    }

    if (!player_highscore_list.empty())
    {
        new_high_score = 1;
        initial_pos=0;
        k_first_initial = GLFW_KEY_A;
        k_middle_initial = GLFW_KEY_A;
        k_last_initial = GLFW_KEY_A;
    }
}

/// Keyboard callback function
void glfw_keyboard(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (action == GLFW_PRESS)
    {
        if (key == GLFW_KEY_F6)
        {
            if (!cpu_load_flag) cpu_load_flag = true;
            else cpu_load_flag = false;
        }
        if (key == GLFW_KEY_F4)
        {
            if (fullscreen)
            {
                fullscreen = false;
                g_win_width = (int) ((float) vmode->width*0.75);
                g_win_height = (int) ((float) vmode->height*0.75);
                int x = (int) ((float) vmode->width*0.125);
                int y = (int) ((float) vmode->height*0.125);
                glfwSetWindowMonitor(window, NULL, x, y, g_win_width, g_win_height, GLFW_DONT_CARE);
                glfwSwapInterval(Vsync);
            }
            else
            {
                fullscreen = true;
                glfwSetWindowMonitor(window, glfwGetPrimaryMonitor(), 0, 0, vmode->width, vmode->height, GLFW_DONT_CARE);
                g_win_width = vmode->width;
                g_win_height = vmode->height;
                glfwSwapInterval(Vsync);
            }

        }

        if (!demo)
        {
            if (pship[0].status == SHIP_ACTIVE && pship[0].controller == KEYBOARD) // && !shield_flag)
            {
                if (key == k_fire)
                {
                    Fire_bullet(0);
                }
            }
        }

        if (!new_high_score)
        {
            if (key == GLFW_KEY_F1 && demo)         // Start new game
            {
                Start_new_game();
                controls = false;
            }
            else if (key == GLFW_KEY_F2)        // Enable or disable settings text
            {
                if (!controls)
                {
                    controls = true;
                    miscell = false;
                }
                else controls = false;
            }
            else if (key == GLFW_KEY_F3)
            {
                if (!miscell)
                {
                    miscell = true;
                    controls = false;
                }
                else miscell = false;
            }
            else if (controls)                 // In settings mode?
            {
                // selecting game setting to change
                if (key == GLFW_KEY_DOWN)
                {
                    settings_row++;
                    if (settings_row>11) settings_row = 0;
                }
                else if (key == GLFW_KEY_UP)
                {
                    settings_row--;
                    if (settings_row<0) settings_row = 11;
                }
                else
                {
                    // Changing game settings
                    if (settings_row == 0)
                    {
                        if (key == GLFW_KEY_RIGHT)
                        {
                            num_players++;
                            if (num_players>max_players) num_players = 1;
                            k_num_players = num_players;
                        }
                        if (key == GLFW_KEY_LEFT)
                        {
                            num_players--;
                            if (num_players<1) num_players = max_players;
                            k_num_players = num_players;
                        }
                    }
                    if (settings_row == 1)
                    {
                        if (key == GLFW_KEY_RIGHT)
                        {
                            player_menu++;
                            if (player_menu>=num_players) player_menu = 0;
                        }
                        if (key == GLFW_KEY_LEFT)
                        {
                            player_menu--;
                            if (player_menu<0) player_menu = num_players-1;
                        }
                    }
                    if (settings_row == 2)
                    {
                        if (key == GLFW_KEY_RIGHT)
                        {
                            Control_selection();
                        }
                    }
                    if (settings_row == 3)
                    {
                        if (pship[player_menu].controller == KEYBOARD)
                        {
                            if (No_same_keys(key) && Valid_key(key)) k_rotate_left = key;
                        }
                    }
                    if (settings_row == 4)
                    {
                        if (pship[player_menu].controller == KEYBOARD)
                        {
                            if (No_same_keys(key)) k_rotate_right = key;
                        }
                    }
                    if (settings_row == 5)
                    {
                        if (pship[player_menu].controller == KEYBOARD)
                        {
                            if (No_same_keys(key)) k_thrust = key;
                        }
                    }
                    if (settings_row == 6)
                    {
                        if (pship[player_menu].controller == KEYBOARD)
                        {
                            if (No_same_keys(key)) k_fire = key;
                        }
                    }
                    if (settings_row == 7)
                    {
                        if (pship[player_menu].controller == KEYBOARD)
                        {
                            if (No_same_keys(key)) k_bomb = key;
                        }
                    }
                    if (settings_row == 8)
                    {
                        if (pship[player_menu].controller == KEYBOARD)
                        {
                            if (No_same_keys(key)) k_shield = key;
                        }
                    }
                    if (settings_row == 9)
                    {
                        if (key == GLFW_KEY_RIGHT || key == GLFW_KEY_LEFT)
                        {
                            Vsync++;
                            if (Vsync>1) Vsync = 0;
                            glfwSwapInterval(Vsync);
                        }
                    }
                    if (settings_row == 10)
                    {
                        if (key == GLFW_KEY_RIGHT) resolution++;
                        if (key == GLFW_KEY_LEFT) resolution--;
                        if (resolution>4) resolution = 0;
                        if (resolution<0) resolution = 4;
                        if (resolution == 0)
                        {
                            reso_text = "1080p";
                            pre_Xmax = 1920;
                            pre_Ymax = 1080;
                            pre_reso_factor = 1.0;
                            bullet_life = pre_Xmax*0.8;
                            pre_ebullet_life = pre_Xmax*1.0;
                        }
                        if (resolution == 1)
                        {
                            reso_text = "1440p";
                            pre_Xmax = 2560;
                            pre_Ymax = 1440;
                            pre_reso_factor = 2.0;
                            bullet_life = pre_Xmax*0.8;
                            pre_ebullet_life = pre_Xmax*1.0;
                        }
                        if (resolution == 2)
                        {
                            reso_text = "2160p";
                            pre_Xmax = 3840;
                            pre_Ymax = 2160;
                            pre_reso_factor = 8.3/2.1;
                            bullet_life = pre_Xmax*0.8;
                            pre_ebullet_life = pre_Xmax*1.0;
                        }
                        if (resolution == 3)
                        {
                            reso_text = "4320p";
                            pre_Xmax = 7680;
                            pre_Ymax = 4320;
                            pre_reso_factor = 33.2/2.1;
                            bullet_life = pre_Xmax*0.8;
                            pre_ebullet_life = pre_Xmax*1.0;
                        }
                        if (resolution == 4)
                        {
                            reso_text = "8640p";
                            pre_Xmax = 15360;
                            pre_Ymax = 8640;
                            pre_reso_factor = 132.7 /2.1;
                            bullet_life = pre_Xmax*0.8;
                            pre_ebullet_life = pre_Xmax*1.0;
                        }
                    }
                    if (settings_row == 11)
                    {
                        if (key == GLFW_KEY_RIGHT) Sound_volume+=0.05;
                        if (key == GLFW_KEY_LEFT) Sound_volume-=0.05;
                        if (Sound_volume>1.0) Sound_volume = 0.0;
                        if (Sound_volume<0.0) Sound_volume = 1.0;
                    }
                }
            }
            else if (miscell)
            {
                if (key == GLFW_KEY_UP)
                {
                    misc_row--;
                    if (misc_row<0) misc_row = 8;
                }
                if (key == GLFW_KEY_DOWN)
                {
                    misc_row++;
                    if (misc_row>8) misc_row = 0;
                }
                if (misc_row == 0)
                {
                    if (key == GLFW_KEY_RIGHT) Start_ships++;
                    if (key == GLFW_KEY_LEFT)
                    {
                        Start_ships--;
                        if (Start_ships<0) Start_ships = 0;
                    }
                }
                if (misc_row == 1)
                {
                    if (key == GLFW_KEY_RIGHT)
                    {
                        Starting_level++;
                        if (Starting_level>=levels.size()) Starting_level = 1;
                    }
                    if (key == GLFW_KEY_LEFT)
                    {
                        Starting_level--;
                        if (Starting_level<1) Starting_level = levels.size()-1;
                    }
                }
                if (misc_row == 2)
                {
                    if (key == GLFW_KEY_RIGHT) Extra_ship_points+=100;
                    if (key == GLFW_KEY_LEFT)
                    {
                        Extra_ship_points-=100;
                        if (Extra_ship_points<100) Extra_ship_points = 100;
                    }
                }
                if (misc_row == 3)
                {
                    if (key == GLFW_KEY_RIGHT || key == GLFW_KEY_LEFT)
                    {
                        Player_hit_player++;
                        if (Player_hit_player>1) Player_hit_player = 0;
                    }
                }
                if (misc_row == 4)
                {
                    if (key == GLFW_KEY_RIGHT) Shield_life+= 1.0;
                    if (key == GLFW_KEY_LEFT)
                    {
                        Shield_life-=1.0;
                        if (Shield_life<1.0) Shield_life = 1.0;
                    }
                }
                if (misc_row == 5)
                {
                    if (key == GLFW_KEY_RIGHT) Bomb_delay+= 1.0;
                    if (key == GLFW_KEY_LEFT)
                    {
                        Bomb_delay-=1.0;
                        if (Bomb_delay<1.0) Bomb_delay = 1.0;
                    }
                }
                if (misc_row == 6)
                {
                    if (key == GLFW_KEY_RIGHT)
                    {
                        Line_width+= 0.5;
                        glLineWidth(Line_width);
                    }
                    if (key == GLFW_KEY_LEFT)
                    {
                        Line_width-=0.5;
                        if (Line_width<1.0) Line_width = 1.0;
                        glLineWidth(Line_width);
                    }
                }
                if (misc_row == 7)
                {
                    if (key == GLFW_KEY_RIGHT) Rock_speed_factor+= 10.0;
                    if (key == GLFW_KEY_LEFT)
                    {
                        Rock_speed_factor-=10.0;
                        if (Rock_speed_factor<10.0) Rock_speed_factor = 10.0;
                    }
                }
                if (misc_row == 8)
                {
                    if (key == GLFW_KEY_RIGHT) Enemy_speed_factor+= 10.0;
                    if (key == GLFW_KEY_LEFT)
                    {
                        Enemy_speed_factor-=10.0;
                        if (Enemy_speed_factor<10.0) Enemy_speed_factor = 10.0;
                    }
                }

            }
        }
        else
        {
            Get_high_score(key);
        }
    }

    if (action == GLFW_RELEASE)
    {
        if (key == k_shield)
        {
            shield_flag[0] = false;
            //shield_flag[1] = false;
        }

        if (key == k_thrust)
        {
            pship[0].thrust_flag = false;
            if (sound_thrust_flag[0])
            {
                sound_thrust_flag[0] = false;
                alSourceStop(thrust_sourceid[0]);
            }
        }
        if (key == GLFW_KEY_F5)
        {
            if (Solid_rock) Solid_rock = false;
            else Solid_rock = true;
        }
    }
}

void Get_high_score(int key)
{
    // Entering high score initials
    if (key == GLFW_KEY_LEFT)
    {
        initial_pos--;
        if (initial_pos<0) initial_pos = 2;
    }
    else if (key == GLFW_KEY_RIGHT)
    {
        initial_pos++;
        if (initial_pos>2) initial_pos = 0;
    }
    else if (key == GLFW_KEY_ENTER)
    {
        char c[3];
        player_scores ptemp;
        sprintf(c, "%c%c%c", k_first_initial, k_middle_initial, k_last_initial);
        string name(c);
        name.resize(3);
        ptemp.name =  name;
        ptemp.points = player_highscore_list[new_high_score-1].v[1];
        p_scores.push_back(ptemp);
        new_high_score++;
        if (new_high_score > player_highscore_list.size())
        {
            hiscore_timer = 10.0;
            Sort_high_score(p_scores);
            Save_scores();
            new_high_score = 0;
        }
    }
    else
    {
        if (initial_pos == 0)
        {
            if (Valid_key(key)) k_first_initial = key;
        }
        if (initial_pos == 1)
        {
            if (Valid_key(key)) k_middle_initial = key;
        }
        if (initial_pos == 2)
        {
            if (Valid_key(key)) k_last_initial = key;
        }
    }
}

bool No_same_keys(int key)
{
    if (key != k_rotate_left && key != k_rotate_right && key != k_thrust && key != k_fire
        && key != k_bomb && key != k_shield) return true;
    else return false;
}

bool Valid_key(int key)
{
    if (key == GLFW_KEY_SPACE) return true;
    if (key == GLFW_KEY_APOSTROPHE) return true;
    if (key>= GLFW_KEY_COMMA && key<= GLFW_KEY_9) return true;
    if (key == GLFW_KEY_SEMICOLON) return true;
    if (key == GLFW_KEY_EQUAL) return true;
    if (key>= GLFW_KEY_A && key<= GLFW_KEY_RIGHT_BRACKET) return true;
    return false;
}

void Set_to_controller(int device)
{
    pship[player_menu].controller = device;
    pship[player_menu].rotate_left = GLFW_GAMEPAD_AXIS_LEFT_X;
    pship[player_menu].rotate_right = GLFW_GAMEPAD_AXIS_LEFT_X;
    pship[player_menu].thrust = GLFW_GAMEPAD_AXIS_RIGHT_TRIGGER;
    pship[player_menu].fire = GLFW_GAMEPAD_BUTTON_A;
    pship[player_menu].bomb = GLFW_GAMEPAD_BUTTON_X;
    pship[player_menu].shields = GLFW_GAMEPAD_BUTTON_B;
}


/// Select input device for ship control
/// Only first player can use keyboard.
/// Other players can use a joystick, game pad, or is NPC
void Control_selection()
{
    if (player_menu==0)
    {
        if (joystick[0].v[1])
        {
            if (pship[0].controller == KEYBOARD)
            {
                if (num_players>1)
                {
                    for (int lp=0; lp<16; lp++)
                    {
                        pship[lp].controller = pship[lp+1].controller;
                        pship[lp].jstick = pship[lp+1].jstick;
                        pship[lp].rotate_left = pship[lp+1].rotate_left;
                        pship[lp].rotate_right = pship[lp+1].rotate_right;
                        pship[lp].thrust = pship[lp+1].thrust;
                        pship[lp].fire = pship[lp+1].fire;
                        pship[lp].bomb = pship[lp+1].bomb;
                        pship[lp].shields = pship[lp+1].shields;
                    }
                    pship[num_players].controller = NPC;
                }
                else Set_to_controller(GAMEPAD);

            }
            else
            {
                if (num_players>1)
                {
                    for (int lp=15; lp>=0; lp--)
                    {
                        pship[lp+1].controller = pship[lp].controller;
                        pship[lp+1].jstick = pship[lp].jstick;
                        pship[lp+1].rotate_left = pship[lp].rotate_left;
                        pship[lp+1].rotate_right = pship[lp].rotate_right;
                        pship[lp+1].thrust = pship[lp].thrust;
                        pship[lp+1].fire = pship[lp].fire;
                        pship[lp+1].bomb = pship[lp].bomb;
                        pship[lp+1].shields = pship[lp].shields;
                    }
                }
                pship[0].controller = KEYBOARD;
                pship[0].rotate_left = k_rotate_left;
                pship[0].rotate_right = k_rotate_right;
                pship[0].thrust = k_thrust;
                pship[0].fire = k_fire;
                pship[0].bomb = k_bomb;
                pship[0].shields = k_shield;
            }
        }
    }
    else
    {
        if (joystick[player_menu].v[0])
        {
            if (pship[player_menu].controller == GAMEPAD)
            {
                pship[player_menu].controller = NPC;
            }
            else
            {
                pship[player_menu].controller = GAMEPAD;
                //Set_to_controller(GAMEPAD);
            }
        }
    }
}

/// Change settings with joysticks if present
void Joystick_settings()
{
    static bool dpad_up = false;
    static bool dpad_down = false;
    static bool dpad_right = false;
    static bool dpad_left = false;
    int lp;

    if (pship[player_menu].controller == GAMEPAD)
    {
        if( glfwGetGamepadState(pship[player_menu].jstick, &Gstate))
        {
            if (Gstate.buttons[GLFW_GAMEPAD_BUTTON_DPAD_UP])
            {
                if (!dpad_up)
                {
                    settings_row--;
                    if (settings_row<0) settings_row = 8;
                    dpad_up = true;
                }
            }
            else dpad_up = false;

            if (Gstate.buttons[GLFW_GAMEPAD_BUTTON_DPAD_DOWN])
            {
                if (!dpad_down)
                {
                    settings_row++;
                    if (settings_row>8) settings_row = 0;
                    dpad_down = true;
                }
            }
            else dpad_down = false;
        }

        if (Gstate.buttons[GLFW_GAMEPAD_BUTTON_DPAD_RIGHT])
        {
            if (!dpad_right)
            {
                if (settings_row == 3 or settings_row == 4)
                {
                    int t=pship[player_menu].rotate_left;
                    t++;
                    if (t>3) t=0;
                    pship[player_menu].rotate_left = t;
                    pship[player_menu].rotate_right = t;
                    dpad_right = true;
                }
                if (settings_row == 5)
                {
                    int t=pship[player_menu].thrust;
                    t++;
                    if (t>5) t=0;
                    pship[player_menu].thrust=t;
                    dpad_right = true;
                }
                if (settings_row == 6)
                {
                    int t=pship[player_menu].fire;
                    t++;
                    if (t>14) t=0;
                    else if(t==4) t=11;
                    pship[player_menu].fire = t;
                    dpad_right = true;
                }
                if (settings_row == 7)
                {
                    int t=pship[player_menu].bomb;
                    t++;
                    if (t>14) t=0;
                    else if(t==4) t=11;
                    pship[player_menu].bomb = t;
                    dpad_right = true;
                }
                if (settings_row == 8)
                {
                    int t=pship[player_menu].shields;
                    t++;
                    if (t>14) t=0;
                    else if(t==4) t=11;
                    pship[player_menu].shields = t;
                    dpad_right = true;
                }
            }
        }
        else dpad_right = false;

        if (Gstate.buttons[GLFW_GAMEPAD_BUTTON_DPAD_LEFT])
        {
            if (!dpad_left)
            {
                if (settings_row == 3 or settings_row == 4)
                {
                    int t=pship[player_menu].rotate_left;
                    t--;
                    if (t<0) t=3;
                    pship[player_menu].rotate_left = t;
                    pship[player_menu].rotate_right = t;
                    dpad_left = true;
                }
                if (settings_row == 5)
                {
                    int t=pship[player_menu].thrust;
                    t--;
                    if (t<0) t=5;
                    pship[player_menu].thrust=t;
                    dpad_left = true;
                }
                if (settings_row == 6)
                {
                    int t=pship[player_menu].fire;
                    t--;
                    if (t<0) t=14;
                    else if(t==10) t=3;
                    pship[player_menu].fire = t;
                    dpad_left = true;
                }
                if (settings_row == 7)
                {
                    int t=pship[player_menu].bomb;
                    t--;
                    if (t<0) t=14;
                    else if(t==10) t=3;
                    pship[player_menu].bomb = t;
                    dpad_left = true;
                }
                if (settings_row == 8)
                {
                    int t=pship[player_menu].shields;
                    t--;
                    if (t<0) t=14;
                    else if(t==10) t=3;
                    pship[player_menu].shields = t;
                    dpad_left = true;
                }
            }
        }
        else dpad_left = false;
   }
}

/// Draw line of text whole line at once (2D opengl rendering)
void Draw_phrase(int phrase, float column, float y, float z, vec4 color)
{
    mat4 matrix;

    column*= col_size;
    matrix = identity_mat4();
    matrix = scale(matrix, vec3(col_size, text_scale, 0));
    matrix.m[12] = column;
    matrix.m[13] = y;
    matrix.m[14] = z;

    glUniform4fv(sh_2Dtext.color_location, 1, color.v);
    glUniform1f(sh_2Dtext.depth_location, z);

    glUniformMatrix4fv (sh_2Dtext.matrix_mat_location, 1, GL_FALSE, matrix.m);
    glDrawArrays(GL_LINES, phrase_pntr[phrase].v[0], phrase_pntr[phrase].v[1]);
}

/// Set text size by total # of rows in screen
void Set_textsize(float rows, float cols)
{
     row_size = g_win_height / rows;
     text_scale = row_size * 0.75;
     col_size = g_win_width / cols;
}

/// Draw line of text by individual characters (2D opengl rendering)
float Draw_textrow(char text_line[], float column, float y, float z, vec4 color)
{
    mat4 matrix;
    int lp;
    int code;

    column*= col_size;
    matrix = identity_mat4();
    matrix = scale(matrix, vec3(col_size, text_scale, 0));

    glUniform4fv(sh_2Dtext.color_location, 1, color.v);
    glUniform1f(sh_2Dtext.depth_location, z);

    for (lp=0; lp<100; lp++)
    {
        code = text_line[lp];
        if (code == 10) break;
        else if (code == 32 || code == 95)
        {
            column+= 0.5 * col_size;
        }
        else
        {
            matrix.m[12] = column;
            matrix.m[13] = y;
            matrix.m[14] = z;
            column+= (ascii_table[code].width + 0.15) * col_size;
            glUniformMatrix4fv (sh_2Dtext.matrix_mat_location, 1, GL_FALSE, matrix.m);
            glDrawArrays(GL_LINES, ascii_table[code].start, ascii_table[code].num);
        }

    }
    return column;
}
