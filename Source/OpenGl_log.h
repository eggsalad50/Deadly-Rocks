#ifndef OPENGL_LOG_H_INCLUDED
#define OPENGL_LOG_H_INCLUDED

#include <stdio.h>
using namespace std;
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <time.h>
#include <stdarg.h>
#include <string.h>

#include <GL/glew.h> // include GLEW and new version of GL on Windows
#include <GL/glfw3.h> // GLFW helper library

#define GL_LOG_FILE "gl.log"
#define DEBUG_FILE "debug.log"
#define DEBUG_FILE2 "debug2.log"

bool restart_gl_log();
bool gl_log (const char* message, ...);
bool gl_log_err (const char* message, ...);
void log_gl_params();
void glfw_error_callback (int error, const char* description);
void print_shader_info_log (GLuint shader_index);
void print_programme_info_log(GLuint programme);
const char* GL_type_to_string (GLenum type);
void print_all (GLuint programme);


#endif // OPENGL_LOG_H_INCLUDED
