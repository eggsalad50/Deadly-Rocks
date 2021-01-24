#ifndef OPENGL_MATH_H_INCLUDED
#define OPENGL_MATH_H_INCLUDED

#include <fstream>
#include <iostream>
/******************************************************************************\
| OpenGL 4 Example Code.                                                       |
| Accompanies written series "Anton's OpenGL 4 Tutorials"                      |
| Email: anton at antongerdelan dot net                                        |
| First version 27 Jan 2014                                                    |
| Copyright Dr Anton Gerdelan, Trinity College Dublin, Ireland.                |
| See individual libraries' separate legal notices                             |
|******************************************************************************|
| Commonly-used maths structures and functions                                 |
| Simple-as-possible. No disgusting templates.                                 |
| Structs vec3, mat4, versor. just hold arrays of floats called "v","m","q",   |
| respectively. So, for example, to get values from a mat4 do: my_mat.m        |
| A versor is the proper name for a unit quaternion.                           |
| This is C++ because it's sort-of convenient to be able to use maths operators|
\******************************************************************************/


// const used to convert degrees into radians
#define TAU 2.0 * M_PI
#define ONE_DEG_IN_RAD (2.0 * M_PI) / 360.0 // 0.017444444
#define ONE_RAD_IN_DEG 360.0 / (2.0 * M_PI) //57.2957795

struct indices;
struct vec2;
struct vec2d;
struct vec2i;
struct vec3;
struct vec3d;
struct vec4;
struct vec4d;
struct versor;

struct indices {
    indices ();
    indices (int a, int b, int c);
    int v[3];
};

struct vec2i {
    vec2i ();
    vec2i (int x, int y);
    vec2i& operator+= (const vec2i& rhs);
    vec2i operator+ (const vec2i& rhs);
    int v[2];
};

struct vec2 {
	vec2 ();
	vec2 (float x, float y);
	vec2& operator= (const vec2& rhs);
	vec2& operator+= (const vec2& rhs);
	vec2  operator* (float rhs);
	vec2  operator+ (const vec2& rhs);
	vec2  operator- (const vec2& rhs);
	float v[2];
};

struct vec2d {
	vec2d ();
	vec2d (double x, double y);
	double v[2];
};

struct vec3i {
    vec3i ();
    vec3i (int x, int y, int z);
    int v[3];
};

struct vec3 {
	vec3 ();
	// create from 3 scalars
	vec3 (float x, float y, float z);
	// create from vec2 and a scalar
	vec3 (const vec2& vv, float z);
	// create from truncated vec4
	vec3 (const vec4& vv);
	// add vector to vector
	vec3 operator+ (const vec3& rhs);
	// add scalar to vector
	vec3 operator+ (float rhs);
	// because user's expect this too
	vec3& operator+= (const vec3& rhs);
	// subtract vector from vector
	vec3 operator- (const vec3& rhs);
	// add vector to vector
	vec3 operator- (float rhs);
	// because users expect this too
	vec3& operator-= (const vec3& rhs);
	// multiply with scalar
	vec3 operator* (float rhs);
	// because users expect this too
	vec3& operator*= (float rhs);
	// divide vector by scalar
	vec3 operator/ (float rhs);
	// because users expect this too
	vec3& operator= (const vec3& rhs);
	// assign double to float
	vec3& operator= (const vec3d& rhs);

	// internal data
	float v[3];
};

struct vec3d {
    vec3d  ();
    vec3d  (double x, double y, double z);
    // subtract vector from vector
	vec3d operator- (const vec3d& rhs);
	// add vector to vector
	vec3d operator+ (const vec3d& rhs);
	// double
	vec3d& operator+= (const vec3d& rhs);
	// double
	vec3d& operator-= (const vec3d& rhs);
	// divide vector by scalar
	vec3d operator/ (double rhs);
	// because users expect this too
	vec3d& operator= (const vec3d& rhs);
	// multiply double vector by scalar
	vec3d operator* (double rhs);
	//
	vec3d& operator*= (double rhs);
	// assign single to double precision
	vec3d& operator/= (double rhs);
	vec3d& operator= (const vec3& rhs);
	//
	vec3d (const vec4& vv);
	vec3d (const vec4d& vv);
    double v[3];
};

struct vec3ld
{
    vec3ld  ();
    vec3ld  (long double x, long double y, long double z);
    // subtract vector from vector
	vec3ld operator- (const vec3ld& rhs);
	// add vector to vector
	vec3ld operator+ (const vec3ld& rhs);
	// double
	vec3ld& operator+= (const vec3ld& rhs);
	// double
	vec3ld& operator-= (const vec3ld& rhs);
	// divide vector by scalar
	vec3ld operator/ (long double rhs);
	// because users expect this too
	vec3ld& operator= (const vec3ld& rhs);
	// multiply double vector by scalar
	vec3ld operator* (long double rhs);
	//
	vec3ld& operator*= (long double rhs);
	// assign single to double precision
	vec3ld& operator/= (long double rhs);
	vec3ld& operator= (const vec3d& rhs);
	vec3ld& operator= (const vec3& rhs);
	//
	//vec3ld (const vec4& vv);
    long double v[3];
};

struct vec4 {
	vec4 ();
	vec4 (float x, float y, float z, float w);
	vec4 (const vec2& vv, float z, float w);
	vec4 (const vec3& vv, float w);
	float v[4];
};

struct vec4d {
	vec4d ();
	vec4d (double x, double y, double z, double w);
	vec4d (const vec2d& vv, double z, double w);
	vec4d (const vec3d& vv, double w);
	double v[4];
};

/* stored like this:
a d g
b e h
c f i */
struct mat3 {
	mat3 ();
	mat3 (float a, float b, float c,
				float d, float e, float f,
				float g, float h, float i);
	float m[9];
};

/* stored like this:
0 4 8  12
1 5 9  13
2 6 10 14
3 7 11 15*/
struct mat4 {
	mat4 ();
	// note! this is entering components in ROW-major order
	mat4 (float a, float b, float c, float d,
				float e, float f, float g, float h,
				float i, float j, float k, float l,
				float mm, float n, float o, float p);
	vec4 operator* (const vec4& rhs);
	vec3 operator* (const vec3& rhs);
	mat4 operator* (const mat4& rhs);
	mat4& operator= (const mat4& rhs);
	float m[16];
};

struct mat4d {
	mat4d ();
	// note! this is entering components in ROW-major order
	mat4d (double a, double b, double c, double d,
				double e, double f, double g, double h,
				double i, double j, double k, double l,
				double mm, double n, double o, double p);
	vec4d operator* (const vec4d& rhs);
	vec3d operator* (const vec3d& rhs);
	mat4d operator* (const mat4d& rhs);
	mat4d& operator= (const mat4d& rhs);
	double m[16];
};


struct versor {
	versor ();
	versor operator/ (float rhs);
	versor operator* (float rhs);
	versor operator* (const versor& rhs);
	versor operator+ (const versor& rhs);
	float q[4];
};

struct sphere_plane
{
    double A;                    // Plane equation
    double B;
    double C;
    double D;
    double x, y, z, r;           // sphere center and radius
    double cx, cy, cz, radius;   // return intersection circle center and radius
    double dist;                 // return circle center distance from sphere center
};

struct circle_circle
{
    double cx1, cy1, r1;   // center and radius of circle 1
    double cx2, cy2, r2;   // center and radius of circle 2
    double dist;           // distance in between circle centers
    double px1, py1;       // return intersection point 1
    double px2, py2;       // return intersection point 2
};

struct circle_line
{
    double px1, py1;       // line point 1
    double px2, py2;       // line point 2
    double cx, cy, r;      // circle center and radius
    double fx, fy;         // intersection point 1
    double gx, gy;         // intersection point 2
    double tx, ty;         // true intersection (calculated later if needed)
};


void print (const vec2& v);
void print (const vec3& v);
void print (const vec4& v);
void print (const mat3& m);
void print (const mat4& m, FILE* debug);
// vector functions
float len2D (const vec2& v);
float length (const vec3& v);    // single precision
float length2 (const vec3& v);  // double precision
double lengthd (const vec3d& v);
double length2d (const vec3d& v);

vec2 normalise2 (const vec2& v);
vec3 normalise (const vec3& v);
vec3d normalised (const vec3d& v);
float dot2 (vec2 a, vec2 b);
float dot (const vec3& a, const vec3& b);
double dot_d ( const vec3d& a, const vec3d& b);
double dot2_d (const vec3d& a);
float Cross (const vec2& a, const vec2& b);
vec3 cross (const vec3& a, const vec3& b);
vec3d crossd (const vec3d& a, const vec3d& b);
int clampi (int a, int small, int big);
double clamp (double a, double small, double big);
vec3i clamp_vec3i(vec3i a, int top);
float get_squared_dist (vec3 from, vec3 to);
double distance_d (vec3d from, vec3d to);
float distance_f (vec3 from, vec3 to);
float get_distance_2i (vec2i from, vec2i to);
float direction_to_heading (vec3 d);
vec3 heading_to_direction (float degrees);
// matrix functions
mat3 zero_mat3 ();
mat3 identity_mat3 ();
mat4 zero_mat4 ();
mat4d zero_mat4d ();
mat4 identity_mat4 ();
mat4d identity_mat4d ();
float determinant (const mat4& mm);
double determinant_d (const mat4d& mm);
mat4 inverse (const mat4& mm);
mat4d inverse_d (const mat4d& mm);
mat4 transpose (const mat4& mm);
// affine functions
mat4 translate (const mat4& m, const vec3& v);
mat4d translate_d (const mat4d& m, const vec3d& v);
mat4 rotate_x_deg (const mat4& m, float deg);
mat4 rotate_y_deg (const mat4& m, float deg);
mat4 rotate_z_deg (const mat4& m, float deg);
mat4 rotate_uvw(const mat4& m, double rad, vec3d axis);
mat4d rotate_uvw_d(const mat4d& m, double rad, vec3d axis);
mat4 scale (const mat4& m, const vec3& v);
// camera functions
mat4 look_at (vec3 cam_pos, vec3d fwd, vec3d rgt, vec3d up);
mat4d look_at_d (vec3d cam_pos, vec3d fwd, vec3d rgt, vec3d up);
mat4 look_at_fix (vec3 cam_pos, vec3d target, vec3d &rgt, vec3d &up);
mat4 perspective (float fovy, float aspect, float near, float far);
mat4 ortho (float left, float right, float bottom, float top, float near, float far);
// quaternion functions
versor quat_from_axis_rad (float radians, float x, float y, float z);
versor quat_from_axis_deg (float degrees, float x, float y, float z);
mat4 quat_to_mat4 (const versor& q);
float dot (const versor& q, const versor& r);
versor slerp (const versor& q, const versor& r);
// stupid overloading wouldn't let me use const
versor normalise (versor& q);
void print (const versor& q);
versor slerp (versor& q, versor& r, float t);

void create_versor (float* q, float a, float x, float y, float z);
void quat_to_mat4 (float* m, float* q);
void normalise_quat (float* q);
void mult_quat_quat (float* result, float* r, float* s);
float angle_of_twoVecsf(vec2 a, vec2 b);
double angle_of_twoVecs(vec3d vec1, vec3d vec2);
vec3d normal_of_twoVecs(vec3d vec1, vec3d vec2);
vec2 Point3d_to_point2d(mat4 matx, vec3 pnt, float width, float height);
vec3 Point2d_to_point3d(mat4 proj, mat4 view, vec2 pnt, float dist, float width, float height);
float Triangle_area_2d(vec2 t1, vec2 t2, vec2 t3);
vec3d float_to_double(vec3 con);
vec3 double_to_float(vec3d con);
vec4 double_to_float(vec4d con);
vec3ld double_to_longdouble(vec3d con);
vec3d longdouble_to_double(vec3ld con);
vec3 longdouble_to_float(vec3ld con);
void Circle_circle_intersect(circle_circle *cc);
bool Circle_line_intersect(circle_line *cl);
vec2 Line_line_intersect(vec2 p1, vec2 p2, vec2 p3, vec2 p4, bool *flag);
bool Line_line_intersect2(vec2 p1, vec2 p2, vec2 p3, vec2 p4);
vec2 Line_line_intersect3(vec2 p1, vec2 p2, vec2 p3, vec2 p4);
vec2 Rotate_2f(float angle, vec2 point);
vec2d Rotate_2d(double angle, vec2d point);
vec3 normal_of_twoVecsf(vec3 vec1, vec3 vec2);

double Angle_A_LawOfCosines(double a, double b, double c);
double Angle_B_LawOfCosines(double a, double b, double c);
double Angle_C_LawOfCosines(double a, double b, double c);
double Side_A_LawOfCosines(double A, double b, double c);
double Side_B_LawOfCosines(double B, double a, double c);
double Side_C_LawOfCosines(double C, double a, double b);


#endif // OPENGL_MATH_H_INCLUDED
