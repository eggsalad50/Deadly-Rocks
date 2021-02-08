#include "main.h"

FILE* Debug;
FILE* Debug2;
GLFWwindow* window;
const GLFWvidmode *vmode;
bool fullscreen;

void clear_errors()
{
    while ((error = glGetError()) != GL_NO_ERROR);
}

int log_errors()
{
    int cnt = 0;
    while ((error = glGetError()) != GL_NO_ERROR)
    {
        cnt++;
        switch (error)
        {
            case GL_INVALID_ENUM: fprintf(stderr,"Invalid Enum\n");
                break;
            case GL_INVALID_VALUE: fprintf(stderr,"Invalid Value\n");
                break;
            case GL_INVALID_OPERATION: fprintf(stderr,"Invalid Operation\n");
                break;
            case GL_STACK_OVERFLOW: fprintf(stderr,"Stack Overflow\n");
                break;
            case GL_STACK_UNDERFLOW: fprintf(stderr,"Stack Underflow\n");
                break;
            case GL_OUT_OF_MEMORY: fprintf(stderr, "Out of Memory\n");
                break;
            case GL_INVALID_FRAMEBUFFER_OPERATION: fprintf(stderr,"Invalid Framebuffer operation\n");
                break;
            case GL_CONTEXT_LOST: fprintf(stderr,"Context Lost");
        }
    }
    return cnt;
}

/// Window resizing callback functions
void glfw_window_size_callback (GLFWwindow* window, int width, int height)
{
    g_win_width=width;
    g_win_height=height;
    float aspect = (float) width / (float) height;
    x_ratio = (float) g_win_width / Xmax;
    y_ratio = (float) g_win_height / Ymax;

    sh_pship.proj_mat = ortho(0.0f, width, 0.0f, height, -1.0, 1.0);
    glUseProgram(sh_pship.program);
    glUniformMatrix4fv(sh_pship.proj_mat_location, 1, GL_FALSE, sh_pship.proj_mat.m);

    sh_2Dtext.proj_mat = ortho(0.0f, width, 0.0f, height, -1.0, 1.0);
    glUseProgram(sh_2Dtext.program);
    glUniformMatrix4fv(sh_2Dtext.proj_mat_location, 1, GL_FALSE, sh_2Dtext.proj_mat.m);

    sh_bullets.proj_mat = ortho(0.0f, g_win_width, 0.0f, g_win_height, -1.0, 1.0);
    glUseProgram(sh_bullets.program);
    glUniformMatrix4fv(sh_bullets.proj_mat_location, 1, GL_FALSE, sh_bullets.proj_mat.m);

    if (!Rock_instancing)
    {
        sh_rocks.proj_mat = ortho(0.0f, g_win_width, 0.0f, g_win_height, -1.0, 1.0);
        glUseProgram(sh_rocks.program);
        glUniformMatrix4fv(sh_rocks.proj_mat_location, 1, GL_FALSE, sh_rocks.proj_mat.m);
    }
    if (Rock_instancing)
    {
        sh_rocks2.proj_mat = ortho(0.0f, g_win_width, 0.0f, g_win_height, -1.0, 1.0);
        glUseProgram(sh_rocks2.program);
        glUniformMatrix4fv(sh_rocks2.proj_mat_location, 1, GL_FALSE, sh_rocks2.proj_mat.m);
    }

    sh_rexplode.proj_mat = ortho(0.0f, g_win_width, 0.0f, g_win_height, -1.0, 1.0);
    glUseProgram(sh_rexplode.program);
    glUniformMatrix4fv(sh_rexplode.proj_mat_location, 1, GL_FALSE, sh_rexplode.proj_mat.m);

    sh_shipexplode2.proj_mat = ortho(0.0f, g_win_width, 0.0f, g_win_height, -1.0, 1.0);
    glUseProgram(sh_shipexplode2.program);
    glUniformMatrix4fv(sh_shipexplode2.proj_mat_location, 1, GL_FALSE, sh_shipexplode2.proj_mat.m);


    sh_impact.proj_mat = ortho(0.0f, g_win_width, 0.0f, g_win_height, -1.0, 1.0);
    glUseProgram(sh_impact.program);
    glUniformMatrix4fv(sh_impact.proj_mat_location, 1, GL_FALSE, sh_impact.proj_mat.m);

    sh_shield.proj_mat = ortho(0.0f, g_win_width, 0.0f, g_win_height, -1.0, 1.0);
    glUseProgram(sh_shield.program);
    glUniformMatrix4fv(sh_shield.proj_mat_location, 1, GL_FALSE, sh_shield.proj_mat.m);

    sh_exhaust.proj_mat = ortho(0.0f, g_win_width, 0.0f, g_win_height, -1.0, 1.0);
    glUseProgram(sh_exhaust.program);
    glUniformMatrix4fv(sh_exhaust.proj_mat_location, 1, GL_FALSE, sh_exhaust.proj_mat.m);

    sh_miniexplode.proj_mat = ortho(0.0f, g_win_width, 0.0f, g_win_height, -1.0, 1.0);
    glUseProgram(sh_miniexplode.program);
    glUniformMatrix4fv (sh_miniexplode.proj_mat_location, 1, GL_FALSE, sh_miniexplode.proj_mat.m);

    sh_drone.proj_mat = ortho(0.0f, g_win_width, 0.0f, g_win_height, -1.0, 1.0);
    glUseProgram(sh_drone.program);
    glUniformMatrix4fv (sh_drone.proj_mat_location, 1, GL_FALSE, sh_drone.proj_mat.m);

    sh_drexplode.proj_mat = ortho(0.0f, g_win_width, 0.0f, g_win_height, -1.0, 1.0);
    glUseProgram(sh_drexplode.program);
    glUniformMatrix4fv (sh_drexplode.proj_mat_location, 1, GL_FALSE, sh_drexplode.proj_mat.m);
}

void glfw_frambuffer_resize_callback (GLFWwindow* window, int width, int height)
{
    g_fb_width=width;
    g_fb_height=height;
}


/// Main Program
int main()
{
    int lp;

    Debug=fopen(DEBUG_FILE,"w");
    if (!restart_gl_log())
    {
        return false;
    }

    fprintf(Debug,"************ Debug information *****************\n");
    fclose(Debug);


    // start GL context and O/S window using the GLFW helper library
    gl_log("starting GLFW\n%s\n", glfwGetVersionString());
    // register the error call-back function that we wrote, above
    glfwSetErrorCallback (glfw_error_callback);
    // start GL context and O/S window using the GLFW helper library
    if (!glfwInit())
    {
       fprintf(stderr, "ERROR: could not start GLFW3\n");

       Debug=fopen(DEBUG_FILE,"a");
       fprintf(stderr, "ERROR: could not start GLFW3\n");
       fclose(Debug);
       return false;
    }

    if (!Get_settings()) return false;    // Load in values from settings file

    glfwWindowHint (GLFW_SAMPLES, 4);
    vmode = glfwGetVideoMode(glfwGetPrimaryMonitor());         // Get current monitor mode

    // Position the OpenGL window on desktop
    g_win_width = (int) ((float) vmode->width*0.75);
    g_win_height = (int) ((float) vmode->height*0.75);
    int x = (int) ((float) vmode->width*0.125);
    int y = (int) ((float) vmode->height*0.125);
    fullscreen = false;

    window=glfwCreateWindow (g_win_width, g_win_height, "Deadly rocks", NULL, NULL);
    if (!window)
    {
       fprintf(stderr, "ERROR: could not open window with GLFW3\n");

       Debug=fopen(DEBUG_FILE,"a");
       fprintf(Debug, "ERROR: could not open window with GLFW3\n");
       fclose(Debug);

       glfwTerminate();
       return false;
    }

    glfwMakeContextCurrent(window);
    log_gl_params();  //Get OpenGL parameters and print to log file.

    // Register window resizing callback functions
    glfwSetWindowSizeCallback (window, glfw_window_size_callback);
    glfwSetFramebufferSizeCallback (window, glfw_frambuffer_resize_callback);

    // Callback keyboard function
    glfwSetKeyCallback (window, glfw_keyboard);
    glfwSetWindowPos(window, x, y);

    // start GLEW extension handler
    glewExperimental = GL_TRUE;
    glewInit();

    // get version info
    const GLubyte* renderer = glGetString(GL_RENDERER); // get renderer string
    const GLubyte* version = glGetString(GL_VERSION); // version as a string
    printf("Renderer: %s\n", renderer);
    printf("OpenGL version supported %s\n\n", version);

    Debug=fopen(DEBUG_FILE,"a");
    fprintf(Debug, "Renderer: %s\n", renderer);
    fprintf(Debug, "OpenGL version supported %s\n\n", version);
    fclose(Debug);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glViewport(0, 0, g_win_width, g_win_height);
    glfwSwapBuffers(window);

    if (!Get_all_shaders()) return false;
    if (!Initialize())
    {
        fclose(Debug);
        return false;
    }
    fclose(Debug);

    /// *********** FRAME RATE SYNC **************
    glfwSwapInterval(Vsync);  //Turn on or off Vsync (0=off, 1=on)

    glClearColor (0.0, 0.0, 0.0, 1.0); // Grey background to help spot mistakes
    glEnable (GL_PROGRAM_POINT_SIZE);
    glEnable (GL_SMOOTH);
    glEnable (GL_DEPTH_TEST);
    glEnable (GL_LINE_SMOOTH);
    glDepthFunc (GL_ALWAYS);
    glEnable (GL_FRONT_AND_BACK);
    glLineWidth(Line_width);

    // GLFW window loop
    while(!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glViewport(0, 0, g_win_width, g_win_height);

        // Main application function
        Application(window);

        // Update other events like input handling
        glfwPollEvents();
        // put the stuff we've been drawing onto the display
        glfwSwapBuffers(window);
        if (GLFW_PRESS == glfwGetKey (window, GLFW_KEY_ESCAPE))
        {
            glfwSetWindowShouldClose (window, 1);
        }
    }

    // Close OpenAL
    ALCdevice *device;
    ALCcontext *context;
    context = alcGetCurrentContext();
    device = alcGetContextsDevice(context);
    alcMakeContextCurrent(NULL);
    alcDestroyContext(context);
    alcCloseDevice(device);

    glfwTerminate();
    fclose(Debug);
    return 0;
}


