#include "shaders.h"

player_ship sh_pship;      // 2d line drawing and text
text2D_shader sh_2Dtext;
bullet_shader sh_bullets;
rock_shader sh_rocks;
rockexplode_shader sh_rexplode;
miniexplode_shader sh_miniexplode;
shipexplode_shader sh_shipexplode;
shield_ship sh_shield;
exhaust sh_exhaust;
impact_shader sh_impact;
rock_shader2 sh_rocks2;
moverock_shader sh_mrock;
drone_shader sh_drone;

string vertshader;
string fragshader;
string compshader;

/// ********** Load and setup all the shaders **********
bool Get_all_shaders()
{
     //fprintf(stderr,"Shaders IN\n");
    if (!Get_2Dline_shader()) return false;
    if (!Get_2Dtext_shader()) return false;   // Text shader
    if (!Get_bullets_shader()) return false;
    if (!Rock_instancing)
    {
        if (!Get_rock_shader()) return false;
    }
    if (!Get_rockexplode_shader()) return false;
    if (!Get_shipexplode_shader()) return false;
    if (!Get_shield_shader()) return false;
    if (!Get_impact_shader()) return false;
    if (!Get_miniexplode_shader()) return false;
    if (!Get_exhaust_shader()) return false;
    if (Rock_instancing)
    {
        if (!Get_rock2_shader()) return false;
    }
    if (!Get_drone_shader()) return false;

     fprintf(stderr,"\nShaders compiled and linked successfully\n");
    return true;
}

bool Compile_shaders(GLuint &program, string &vert, string &frag)
{
    GLuint vs;
    GLuint fs;

    const char* vertex_shader;
    const char* fragment_shader;
    bool success = true;

    vertex_shader = vert.c_str();
    fragment_shader = frag.c_str();

    // Compile vertex shader program
    vs = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vs, 1, &vertex_shader, NULL);
    glCompileShader(vs);
    int params=-1;
    glGetShaderiv (vs, GL_COMPILE_STATUS, &params);
    if (GL_TRUE != params)
    {
        fprintf(stderr, "ERROR: GL vertex shader index %i did not compile: File> %s\n", vs, vert);
        print_shader_info_log(vs);
        success = false;
    }

    // Compile fragment shader program
    fs = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fs, 1, &fragment_shader, NULL);
    glCompileShader(fs);
    glGetShaderiv (fs, GL_COMPILE_STATUS, &params);
    if (GL_TRUE != params)
    {
        fprintf(stderr, "ERROR: GL fragment shader index %i did not compile: File> %s\n", fs, frag);
        print_shader_info_log(fs);
        success = false;
    }

    // Link vertex and shader program
    program = glCreateProgram();
    glAttachShader(program, fs);
    glAttachShader(program, vs);
    glLinkProgram(program);

    //Check if linked correctly.
    glGetProgramiv(program, GL_LINK_STATUS, &params);
    if (GL_TRUE !=params)
    {
        fprintf (stderr, "ERROR: could not link shader programme GL index %u\n", program);
        print_programme_info_log(program);
        success = false;
    }
    print_all(program);
    return success;
}

bool Compile_compute_shaders(GLuint &program, string &compute)
{
    GLuint cs;

    const char* compute_shader;
    bool success = true;

    compute_shader = compute.c_str();

    // Compile vertex shader program
    cs = glCreateShader(GL_COMPUTE_SHADER);
    glShaderSource(cs, 1, &compute_shader, NULL);
    glCompileShader(cs);
    int params=-1;
    glGetShaderiv (cs, GL_COMPILE_STATUS, &params);
    if (GL_TRUE != params)
    {
        fprintf(stderr, "ERROR: GL compute shader index %i did not compile: File> %s\n", cs, compute);
        print_shader_info_log(cs);
        success = false;
    }

    // Link vertex and shader program
    program = glCreateProgram();
    glAttachShader(program, cs);
    glLinkProgram(program);

    //Check if linked correctly.
    glGetProgramiv(program, GL_LINK_STATUS, &params);
    if (GL_TRUE !=params)
    {
        fprintf (stderr, "ERROR: could not link shader programme GL index %u\n", program);
        print_programme_info_log(program);
        success = false;
    }
    print_all(program);
    return success;
}

/// Read in shader file
string readFile(const char *filePath)
{
    string content;
    ifstream fileStream(filePath, std::ios::in);

    if(!fileStream.is_open()) {
        cerr << "Could not read file " << filePath << ". File does not exist." << endl;
        return "";
    }

    string line = "";
    while(!fileStream.eof()) {
        getline(fileStream, line);
        content.append(line + "\n");
    }

    fileStream.close();
    return content;
}


/// Read in shader file
string readFile2(string filePath)
{
    string content;
    ifstream fileStream(filePath, std::ios::in);

    if(!fileStream.is_open()) {
        cerr << "Could not read file " << filePath << ". File does not exist." << endl;
        return "";
    }

    string line = "";
    while(!fileStream.eof()) {
        getline(fileStream, line);
        content.append(line + "\n");
    }

    fileStream.close();
    return content;
}

bool Get_2Dline_shader()
{
    string vert = "shaders/line2D.vert.txt";
    string frag = "shaders/line2D.frag.txt";
    vertshader = readFile2(vert);
    fragshader = readFile2(frag);
    if (!(Compile_shaders(sh_pship.program, vertshader, fragshader))) return false;

    glUseProgram(sh_pship.program);
    glGenVertexArrays(1, &sh_pship.vao);
    glBindVertexArray(sh_pship.vao);

    sh_pship.proj_mat = ortho(0.0f, g_win_width, 0.0f, g_win_height, -1.0, 1.0);
    sh_pship.proj_mat_location = glGetUniformLocation (sh_pship.program, "proj");
     glUniformMatrix4fv (sh_pship.proj_mat_location, 1, GL_FALSE, sh_pship.proj_mat.m);
    sh_pship.matrix_mat_location = glGetUniformLocation (sh_pship.program, "matrix");

    sh_pship.color_location = glGetUniformLocation (sh_pship.program, "color");
    sh_pship.depth_location = glGetUniformLocation (sh_pship.program, "depth");

    glGenBuffers(1, &sh_pship.points_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, sh_pship.points_vbo);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, NULL);

    glEnableVertexAttribArray(0);
    glBindVertexArray(0);

    return true;
}

bool Get_2Dtext_shader()
{
    string vert = "shaders/text2D.vert.txt";
    string frag = "shaders/text2D.frag.txt";
    vertshader = readFile2(vert);
    fragshader = readFile2(frag);
    if (!(Compile_shaders(sh_2Dtext.program, vertshader, fragshader))) return false;

    glUseProgram(sh_2Dtext.program);
    glGenVertexArrays(1, &sh_2Dtext.vao);
    glBindVertexArray(sh_2Dtext.vao);

    sh_2Dtext.proj_mat_location = glGetUniformLocation (sh_2Dtext.program, "proj");
    sh_2Dtext.matrix_mat_location = glGetUniformLocation (sh_2Dtext.program, "matrix");

    sh_2Dtext.color_location = glGetUniformLocation (sh_2Dtext.program, "color");
    sh_2Dtext.depth_location = glGetUniformLocation (sh_2Dtext.program, "depth");

    glGenBuffers(1, &sh_2Dtext.points_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, sh_2Dtext.points_vbo);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, NULL);

    glEnableVertexAttribArray(0);
    glBindVertexArray(0);

    return true;
}

bool Get_bullets_shader()
{
    int mem = (Max_bullets+Max_ebullets+MAX_BBULLETS)*2;
    sh_bullets.points = new vec2[mem];
    sh_bullets.color = new vec3[mem];

    string vert = "shaders/bullets.vert.txt";
    string frag = "shaders/bullets.frag.txt";
    vertshader = readFile2(vert);
    fragshader = readFile2(frag);
    if (!(Compile_shaders(sh_bullets.program, vertshader, fragshader))) return false;

    glUseProgram(sh_bullets.program);
    glGenVertexArrays(1, &sh_bullets.vao);
    glBindVertexArray(sh_bullets.vao);

    sh_bullets.proj_mat = ortho(0.0f, g_win_width, 0.0f, g_win_height, -1.0, 1.0);
    sh_bullets.proj_mat_location = glGetUniformLocation (sh_bullets.program, "proj");
     glUniformMatrix4fv (sh_bullets.proj_mat_location, 1, GL_FALSE, sh_bullets.proj_mat.m);
    sh_bullets.matrix_mat_location = glGetUniformLocation (sh_bullets.program, "matrix");
     mat4 matrix = identity_mat4();
     glUniformMatrix4fv (sh_bullets.matrix_mat_location, 1, GL_FALSE, matrix.m);

    sh_bullets.depth_location = glGetUniformLocation (sh_bullets.program, "depth");
     glUniform1f(sh_bullets.depth_location, -0.5);

    glGenBuffers(1, &sh_bullets.points_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, sh_bullets.points_vbo);
    glBufferData(GL_ARRAY_BUFFER, mem * sizeof(vec2), sh_bullets.points, GL_DYNAMIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, NULL);

    glGenBuffers(1, &sh_bullets.color_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, sh_bullets.color_vbo);
    glBufferData(GL_ARRAY_BUFFER, mem * sizeof(vec3), sh_bullets.color, GL_DYNAMIC_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glBindVertexArray(0);

    return true;
}

bool Get_rock_shader()
{
    string vert = "shaders/rocks.vert.txt";
    string frag = "shaders/rocks.frag.txt";
    vertshader = readFile2(vert);
    fragshader = readFile2(frag);
    if (!(Compile_shaders(sh_rocks.program, vertshader, fragshader))) return false;

    glUseProgram(sh_rocks.program);
    glGenVertexArrays(1, &sh_rocks.vao);
    glBindVertexArray(sh_rocks.vao);

    sh_rocks.proj_mat = ortho(0.0f, g_win_width, 0.0f, g_win_height, -1.0, 1.0);
    sh_rocks.proj_mat_location = glGetUniformLocation (sh_rocks.program, "proj");
     glUniformMatrix4fv (sh_rocks.proj_mat_location, 1, GL_FALSE, sh_rocks.proj_mat.m);
    sh_rocks.matrix_mat_location = glGetUniformLocation (sh_rocks.program, "matrix");
     mat4 matrix = identity_mat4();
     glUniformMatrix4fv (sh_rocks.matrix_mat_location, 1, GL_FALSE, matrix.m);

    sh_rocks.color_location = glGetUniformLocation (sh_rocks.program, "color");
     vec4 color = vec4(WHITE, 1.0);
     glUniform4fv(sh_rocks.color_location, 1, color.v);
    sh_rocks.depth_location = glGetUniformLocation (sh_rocks.program, "depth");
     glUniform1f(sh_rocks.depth_location, -0.5);
    sh_rocks.radius_location = glGetUniformLocation (sh_rocks.program, "radius");
    sh_rocks.pos_location = glGetUniformLocation (sh_rocks.program, "pos");
    sh_rocks.xratio_location = glGetUniformLocation (sh_rocks.program, "xratio");
    sh_rocks.yratio_location = glGetUniformLocation (sh_rocks.program, "yratio");
    sh_rocks.angle_location = glGetUniformLocation (sh_rocks.program, "angle");

    glGenBuffers(1, &sh_rocks.points_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, sh_rocks.points_vbo);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, NULL);

    glEnableVertexAttribArray(0);
    glBindVertexArray(0);

    return true;
}

bool Get_rockexplode_shader()
{

    string vert = "shaders/rockexplode.vert.txt";
    string frag = "shaders/rockexplode.frag.txt";
    vertshader = readFile2(vert);
    fragshader = readFile2(frag);
    if (!(Compile_shaders(sh_rexplode.program, vertshader, fragshader))) return false;

    glUseProgram(sh_rexplode.program);
    glGenVertexArrays(1, &sh_rexplode.vao);
    glBindVertexArray(sh_rexplode.vao);

    sh_rexplode.proj_mat = ortho(0.0f, g_win_width, 0.0f, g_win_height, -1.0, 1.0);
    sh_rexplode.proj_mat_location = glGetUniformLocation (sh_rexplode.program, "proj");
     glUniformMatrix4fv (sh_rexplode.proj_mat_location, 1, GL_FALSE, sh_rexplode.proj_mat.m);
    sh_rexplode.matrix_mat_location = glGetUniformLocation (sh_rexplode.program, "matrix");
     mat4 matrix = identity_mat4();
     glUniformMatrix4fv (sh_rexplode.matrix_mat_location, 1, GL_FALSE, matrix.m);

    sh_rexplode.color_location = glGetUniformLocation (sh_rexplode.program, "color");

    sh_rexplode.depth_location = glGetUniformLocation (sh_rexplode.program, "depth");
     glUniform1f(sh_rexplode.depth_location, -0.5);
    sh_rexplode.center_location = glGetUniformLocation (sh_rexplode.program, "center");
    sh_rexplode.time_locaton = glGetUniformLocation (sh_rexplode.program, "time");
    sh_rexplode.xratio_location = glGetUniformLocation (sh_rexplode.program, "xratio");
    sh_rexplode.yratio_location = glGetUniformLocation (sh_rexplode.program, "yratio");

    glGenBuffers(1, &sh_rexplode.direction_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, sh_rexplode.direction_vbo);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(0);
    glBindVertexArray(0);

    return true;
}

bool Get_miniexplode_shader()
{
    sh_miniexplode.points = new vec2[10*Max_mini_explode + 12*Max_mini_explode];
    sh_miniexplode.direction = new vec2[Mini_explode_debris * Max_mini_explode];
    sh_miniexplode.angle = new float[Mini_explode_debris * Max_mini_explode];
    sh_miniexplode.radius = new float[Mini_explode_debris * Max_mini_explode];

    string vert = "shaders/miniexplode.vert.txt";
    string frag = "shaders/miniexplode.frag.txt";
    vertshader = readFile2(vert);
    fragshader = readFile2(frag);
    if (!(Compile_shaders(sh_miniexplode.program, vertshader, fragshader))) return false;

    glUseProgram(sh_miniexplode.program);
    glGenVertexArrays(1, &sh_miniexplode.vao);
    glBindVertexArray(sh_miniexplode.vao);

    sh_miniexplode.proj_mat = ortho(0.0f, g_win_width, 0.0f, g_win_height, -1.0, 1.0);
    sh_miniexplode.proj_mat_location = glGetUniformLocation (sh_miniexplode.program, "proj");
     glUniformMatrix4fv (sh_miniexplode.proj_mat_location, 1, GL_FALSE, sh_miniexplode.proj_mat.m);
    sh_miniexplode.matrix_mat_location = glGetUniformLocation (sh_miniexplode.program, "matrix");
     mat4 matrix = identity_mat4();
     glUniformMatrix4fv (sh_miniexplode.matrix_mat_location, 1, GL_FALSE, matrix.m);

    sh_miniexplode.color_location = glGetUniformLocation (sh_miniexplode.program, "color");
     vec4 color = vec4(WHITE, 1.0);
     glUniform4fv(sh_miniexplode.color_location, 1, color.v);
    sh_miniexplode.depth_location = glGetUniformLocation (sh_miniexplode.program, "depth");
     glUniform1f(sh_miniexplode.depth_location, -0.5);
    sh_miniexplode.center_location = glGetUniformLocation (sh_miniexplode.program, "center");
    sh_miniexplode.time_locaton = glGetUniformLocation (sh_miniexplode.program, "time");
    sh_miniexplode.xratio_location = glGetUniformLocation (sh_miniexplode.program, "xratio");
    sh_miniexplode.yratio_location = glGetUniformLocation (sh_miniexplode.program, "yratio");

    glGenBuffers(1, &sh_miniexplode.points_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, sh_miniexplode.points_vbo);
    glBufferData(GL_ARRAY_BUFFER,  (10*Max_mini_explode + 12*Max_mini_explode) * sizeof(vec2), sh_miniexplode.points, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, NULL);

    glGenBuffers(1, &sh_miniexplode.direction_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, sh_miniexplode.direction_vbo);
    glBufferData(GL_ARRAY_BUFFER,  Mini_explode_debris * Max_mini_explode * sizeof(vec2), sh_miniexplode.direction, GL_STATIC_DRAW);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, NULL);
    glVertexAttribDivisor(1, 1);

    glGenBuffers(1, &sh_miniexplode.angle_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, sh_miniexplode.angle_vbo);
    glBufferData(GL_ARRAY_BUFFER, Mini_explode_debris * Max_mini_explode * sizeof(float), sh_miniexplode.angle, GL_STATIC_DRAW);
    glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, 0, NULL);
    glVertexAttribDivisor(2, 1);

    glGenBuffers(1, &sh_miniexplode.radius_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, sh_miniexplode.radius_vbo);
    glBufferData(GL_ARRAY_BUFFER, Mini_explode_debris * Max_mini_explode * sizeof(float), sh_miniexplode.radius, GL_STATIC_DRAW);
    glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, 0, NULL);
    glVertexAttribDivisor(3, 1);

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    glEnableVertexAttribArray(3);
    glBindVertexArray(0);

    return true;
}

bool Get_impact_shader()
{
    sh_impact.direction = new vec2[Max_impact * Impact_particles];

    string vert = "shaders/impact.vert.txt";
    string frag = "shaders/impact.frag.txt";
    vertshader = readFile2(vert);
    fragshader = readFile2(frag);
    if (!(Compile_shaders(sh_impact.program, vertshader, fragshader))) return false;

    glUseProgram(sh_impact.program);
    glGenVertexArrays(1, &sh_impact.vao);
    glBindVertexArray(sh_impact.vao);

    sh_impact.proj_mat = ortho(0.0f, g_win_width, 0.0f, g_win_height, -1.0, 1.0);
    sh_impact.proj_mat_location = glGetUniformLocation (sh_impact.program, "proj");
     glUniformMatrix4fv (sh_impact.proj_mat_location, 1, GL_FALSE, sh_impact.proj_mat.m);
    sh_impact.matrix_mat_location = glGetUniformLocation (sh_impact.program, "matrix");
     mat4 matrix = identity_mat4();
     glUniformMatrix4fv (sh_impact.matrix_mat_location, 1, GL_FALSE, matrix.m);

    sh_impact.color_location = glGetUniformLocation (sh_impact.program, "color");

    sh_impact.depth_location = glGetUniformLocation (sh_impact.program, "depth");
     glUniform1f(sh_impact.depth_location, -0.5);
    sh_impact.center_location = glGetUniformLocation (sh_impact.program, "center");
    sh_impact.time_locaton = glGetUniformLocation (sh_impact.program, "time");
    sh_impact.xratio_location = glGetUniformLocation (sh_impact.program, "xratio");
    sh_impact.yratio_location = glGetUniformLocation (sh_impact.program, "yratio");

    glGenBuffers(1, &sh_impact.direction_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, sh_impact.direction_vbo);
    glBufferData(GL_ARRAY_BUFFER,  Max_impact*Impact_particles * sizeof(vec2), sh_impact.direction, GL_DYNAMIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(0);
    glBindVertexArray(0);
    return true;
}

bool Get_shipexplode_shader()
{
    sh_shipexplode.points = new vec2[2];
    sh_shipexplode.direction = new vec2[Ship_explode_debris * Max_ship_explode];
    sh_shipexplode.spin = new vec2[Ship_explode_debris * Max_ship_explode];
    sh_shipexplode.radius = new float[Ship_explode_debris * Max_ship_explode];

    sh_shipexplode.points[0] = vec2(-1.0, 0.0);
    sh_shipexplode.points[1] = vec2(1.0, 0.0);

    string vert = "shaders/shipexplode.vert.txt";
    string frag = "shaders/shipexplode.frag.txt";
    vertshader = readFile2(vert);
    fragshader = readFile2(frag);
    if (!(Compile_shaders(sh_shipexplode.program, vertshader, fragshader))) return false;

    glUseProgram(sh_shipexplode.program);
    glGenVertexArrays(1, &sh_shipexplode.vao);
    glBindVertexArray(sh_shipexplode.vao);

    sh_shipexplode.proj_mat = ortho(0.0f, g_win_width, 0.0f, g_win_height, -1.0, 1.0);
    sh_shipexplode.proj_mat_location = glGetUniformLocation (sh_shipexplode.program, "proj");
     glUniformMatrix4fv (sh_shipexplode.proj_mat_location, 1, GL_FALSE, sh_shipexplode.proj_mat.m);
    sh_shipexplode.matrix_mat_location = glGetUniformLocation (sh_shipexplode.program, "matrix");
     mat4 matrix = identity_mat4();
     glUniformMatrix4fv (sh_shipexplode.matrix_mat_location, 1, GL_FALSE, matrix.m);

    sh_shipexplode.color_location = glGetUniformLocation (sh_shipexplode.program, "color");
     vec4 color = vec4(LIGHT_SKY_BLUE, 1.0);
     glUniform4fv(sh_shipexplode.color_location, 1, color.v);
    sh_shipexplode.depth_location = glGetUniformLocation (sh_shipexplode.program, "depth");
     glUniform1f(sh_shipexplode.depth_location, -0.5);
    sh_shipexplode.center_location = glGetUniformLocation (sh_shipexplode.program, "center");
    sh_shipexplode.time_locaton = glGetUniformLocation (sh_shipexplode.program, "time");
    sh_shipexplode.xratio_location = glGetUniformLocation (sh_shipexplode.program, "xratio");
    sh_shipexplode.yratio_location = glGetUniformLocation (sh_shipexplode.program, "yratio");

    glGenBuffers(1, &sh_shipexplode.points_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, sh_shipexplode.points_vbo);
    glBufferData(GL_ARRAY_BUFFER,  2 * sizeof(vec2), sh_shipexplode.points, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, NULL);

    glGenBuffers(1, &sh_shipexplode.direction_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, sh_shipexplode.direction_vbo);
    glBufferData(GL_ARRAY_BUFFER,  Ship_explode_debris*Max_ship_explode * sizeof(vec2), sh_shipexplode.direction, GL_STATIC_DRAW);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, NULL);
    glVertexAttribDivisor(1, 1);

    glGenBuffers(1, &sh_shipexplode.spin_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, sh_shipexplode.spin_vbo);
    glBufferData(GL_ARRAY_BUFFER, Ship_explode_debris*Max_ship_explode * sizeof(vec2), sh_shipexplode.spin, GL_STATIC_DRAW);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, NULL);
    glVertexAttribDivisor(2, 1);

    glGenBuffers(1, &sh_shipexplode.radius_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, sh_shipexplode.radius_vbo);
    glBufferData(GL_ARRAY_BUFFER, Ship_explode_debris*Max_ship_explode * sizeof(float), sh_shipexplode.radius, GL_STATIC_DRAW);
    glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, 0, NULL);
    glVertexAttribDivisor(3, 1);

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    glEnableVertexAttribArray(3);
    glBindVertexArray(0);

    return true;
}

bool Get_shield_shader()
{
    string vert = "shaders/shields.vert.txt";
    string frag = "shaders/shields.frag.txt";
    vertshader = readFile2(vert);
    fragshader = readFile2(frag);
    if (!(Compile_shaders(sh_shield.program, vertshader, fragshader))) return false;

    glUseProgram(sh_shield.program);
    glGenVertexArrays(1, &sh_shield.vao);
    glBindVertexArray(sh_shield.vao);

    sh_shield.proj_mat = ortho(0.0f, g_win_width, 0.0f, g_win_height, -1.0, 1.0);
    sh_shield.proj_mat_location = glGetUniformLocation (sh_shield.program, "proj");
     glUniformMatrix4fv (sh_shield.proj_mat_location, 1, GL_FALSE, sh_shield.proj_mat.m);
    sh_shield.matrix_mat_location = glGetUniformLocation (sh_shield.program, "matrix");
     mat4 matrix = identity_mat4();
    glUniformMatrix4fv (sh_shield.matrix_mat_location, 1, GL_FALSE, matrix.m);

    sh_shield.color_location = glGetUniformLocation (sh_shield.program, "color");
    sh_shield.depth_location = glGetUniformLocation (sh_shield.program, "depth");
    sh_shield.bright_location = glGetUniformLocation (sh_shield.program, "bright");

    glGenBuffers(1, &sh_shield.points_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, sh_shield.points_vbo);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, NULL);

    glEnableVertexAttribArray(0);
    glBindVertexArray(0);

    return true;
}

bool Get_exhaust_shader()
{
    string vert = "shaders/exhaust.vert.txt";
    string frag = "shaders/exhaust.frag.txt";
    vertshader = readFile2(vert);
    fragshader = readFile2(frag);
    if (!(Compile_shaders(sh_exhaust.program, vertshader, fragshader))) return false;

    glUseProgram(sh_exhaust.program);
    glGenVertexArrays(1, &sh_exhaust.vao);
    glBindVertexArray(sh_exhaust.vao);

    sh_exhaust.proj_mat = ortho(0.0f, g_win_width, 0.0f, g_win_height, -1.0, 1.0);
    sh_exhaust.proj_mat_location = glGetUniformLocation (sh_exhaust.program, "proj");
     glUniformMatrix4fv (sh_exhaust.proj_mat_location, 1, GL_FALSE, sh_exhaust.proj_mat.m);
    sh_exhaust.matrix_mat_location = glGetUniformLocation (sh_exhaust.program, "matrix");
     mat4 matrix = identity_mat4();
    glUniformMatrix4fv (sh_exhaust.matrix_mat_location, 1, GL_FALSE, matrix.m);

    sh_exhaust.color_location = glGetUniformLocation (sh_exhaust.program, "color");
    sh_exhaust.depth_location = glGetUniformLocation (sh_exhaust.program, "depth");
    sh_exhaust.seed_location = glGetUniformLocation (sh_exhaust.program, "seed");

    glGenBuffers(1, &sh_exhaust.points_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, sh_exhaust.points_vbo);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, NULL);

    glGenBuffers(1, &sh_exhaust.color_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, sh_exhaust.color_vbo);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glBindVertexArray(0);

    return true;
}

bool Get_rock2_shader()
{
    int reserve = Max_rocks;
    sh_rocks2.active = new int[reserve];
    sh_rocks2.points = new vec2[30];
    sh_rocks2.position = new vec2[reserve];
    sh_rocks2.direction = new vec2[reserve];
    sh_rocks2.radius = new float[reserve];
    sh_rocks2.angle = new float[reserve];
    sh_rocks2.color = new vec3[reserve];

    string vert = "shaders/rocks2.vert.txt";
    string frag = "shaders/rocks2.frag.txt";
    vertshader = readFile2(vert);
    fragshader = readFile2(frag);
    if (!(Compile_shaders(sh_rocks2.program, vertshader, fragshader))) return false;

     clear_errors();
    glUseProgram(sh_rocks2.program);
    glGenVertexArrays(1, &sh_rocks2.vao);
    glBindVertexArray(sh_rocks2.vao);

    sh_rocks2.proj_mat = ortho(0.0f, g_win_width, 0.0f, g_win_height, -1.0, 1.0);
    sh_rocks2.proj_mat_location = glGetUniformLocation (sh_rocks2.program, "proj");
     glUniformMatrix4fv (sh_rocks2.proj_mat_location, 1, GL_FALSE, sh_rocks2.proj_mat.m);
    sh_rocks2.matrix_location = glGetUniformLocation (sh_rocks2.program, "matrix");
     mat4 matrix = identity_mat4();
     glUniformMatrix4fv (sh_rocks2.matrix_location, 1, GL_FALSE, matrix.m);

    sh_rocks2.depth_location = glGetUniformLocation (sh_rocks2.program, "depth");
     glUniform1f(sh_rocks2.depth_location, -0.5);
    sh_rocks2.xratio_location = glGetUniformLocation (sh_rocks2.program, "xratio");
    sh_rocks2.yratio_location = glGetUniformLocation (sh_rocks2.program, "yratio");
    sh_rocks2.seed1_location = glGetUniformLocation (sh_rocks2.program, "seed1");
    sh_rocks2.seed2_location = glGetUniformLocation (sh_rocks2.program, "seed2");

    glGenBuffers(1, &sh_rocks2.points_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, sh_rocks2.points_vbo);
    glBufferData(GL_ARRAY_BUFFER, 26*sizeof(vec2), sh_rocks2.points, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, NULL);

    glGenBuffers(1, &sh_rocks2.position_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, sh_rocks2.position_vbo);
    glBufferData(GL_ARRAY_BUFFER,  reserve * sizeof(vec2), sh_rocks2.position, GL_DYNAMIC_DRAW);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, NULL);
    glVertexAttribDivisor(1, 1);

    glGenBuffers(1, &sh_rocks2.color_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, sh_rocks2.color_vbo);
    glBufferData(GL_ARRAY_BUFFER,  reserve * sizeof(vec3), sh_rocks2.color, GL_DYNAMIC_DRAW);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glVertexAttribDivisor(2, 1);

    glGenBuffers(1, &sh_rocks2.radius_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, sh_rocks2.radius_vbo);
    glBufferData(GL_ARRAY_BUFFER,  reserve * sizeof(float), sh_rocks2.radius, GL_DYNAMIC_DRAW);
    glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, 0, NULL);
    glVertexAttribDivisor(3, 1);

    glGenBuffers(1, &sh_rocks2.angle_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, sh_rocks2.angle_vbo);
    glBufferData(GL_ARRAY_BUFFER,  reserve * sizeof(float), sh_rocks2.angle, GL_DYNAMIC_DRAW);
    glVertexAttribPointer(4, 1, GL_FLOAT, GL_FALSE, 0, NULL);
    glVertexAttribDivisor(4, 1);

    glGenBuffers(1, &sh_rocks2.active_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, sh_rocks2.active_vbo);
    glBufferData(GL_ARRAY_BUFFER,  reserve * sizeof(int), sh_rocks2.active, GL_DYNAMIC_DRAW);
    glVertexAttribPointer(5, 1, GL_INT, GL_FALSE, 0, NULL);
    glVertexAttribDivisor(5, 1);

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    glEnableVertexAttribArray(3);
    glEnableVertexAttribArray(4);
    glEnableVertexAttribArray(5);
    glBindVertexArray(0);
    return true;
}

bool Get_drone_shader()
{
     //clear_errors();
    int reserve = 2000;
    sh_drone.points = new vec2[30];
    sh_drone.position = new vec2[reserve];
    sh_drone.angle = new float[reserve];
    sh_drone.color = new vec3[reserve];

    string vert = "shaders/drone.vert.txt";
    string frag = "shaders/drone.frag.txt";
    vertshader = readFile2(vert);
    fragshader = readFile2(frag);
    if (!(Compile_shaders(sh_drone.program, vertshader, fragshader))) return false;

    glUseProgram(sh_drone.program);
    glGenVertexArrays(1, &sh_drone.vao);
    glBindVertexArray(sh_drone.vao);

    sh_drone.proj_mat = ortho(0.0f, g_win_width, 0.0f, g_win_height, -1.0, 1.0);
    sh_drone.proj_mat_location = glGetUniformLocation (sh_drone.program, "proj");
     glUniformMatrix4fv (sh_drone.proj_mat_location, 1, GL_FALSE, sh_drone.proj_mat.m);
    sh_drone.matrix_location = glGetUniformLocation (sh_drone.program, "matrix");
     mat4 matrix = identity_mat4();
     glUniformMatrix4fv (sh_drone.matrix_location, 1, GL_FALSE, matrix.m);

    sh_drone.depth_location = glGetUniformLocation (sh_drone.program, "depth");
     glUniform1f(sh_drone.depth_location, -0.5);
    sh_drone.xratio_location = glGetUniformLocation (sh_drone.program, "xratio");
    sh_drone.yratio_location = glGetUniformLocation (sh_drone.program, "yratio");
    sh_drone.radius_location = glGetUniformLocation (sh_drone.program, "radius");

    glGenBuffers(1, &sh_drone.points_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, sh_drone.points_vbo);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, NULL);

    glGenBuffers(1, &sh_drone.position_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, sh_drone.position_vbo);
    glBufferData(GL_ARRAY_BUFFER,  reserve * sizeof(vec2), sh_drone.position, GL_DYNAMIC_DRAW);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, NULL);
    glVertexAttribDivisor(1, 1);

    glGenBuffers(1, &sh_drone.color_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, sh_drone.color_vbo);
    glBufferData(GL_ARRAY_BUFFER,  reserve * sizeof(vec3), sh_drone.color, GL_DYNAMIC_DRAW);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glVertexAttribDivisor(2, 1);

    glGenBuffers(1, &sh_drone.angle_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, sh_drone.angle_vbo);
    glBufferData(GL_ARRAY_BUFFER,  reserve * sizeof(float), sh_drone.angle, GL_DYNAMIC_DRAW);
    glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, 0, NULL);
    glVertexAttribDivisor(3, 1);

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    glEnableVertexAttribArray(3);
    glBindVertexArray(0);
     //log_errors();
    return true;
}
