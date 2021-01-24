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
\******************************************************************************/
#include "OpenGl_math.h"
#include <stdio.h>
#define _USE_MATH_DEFINES
#include <math.h>

/*--------------------------------CONSTRUCTORS--------------------------------*/
indices::indices () {}   // Triangle face

indices::indices (int a, int b, int c) {
    v[0] = a;
    v[1] = b;
    v[2] = c;
}

vec2::vec2 () {}

vec2::vec2 (float x, float y) {
	v[0] = x;
	v[1] = y;
}

// Single precision assign
vec2& vec2::operator= (const vec2& rhs) {
	v[0] = rhs.v[0];
	v[1] = rhs.v[1];
	return *this;
}

// single precision add
vec2& vec2::operator+= (const vec2& rhs) {
	v[0] += rhs.v[0];
	v[1] += rhs.v[1];
	return *this; // return self
}

// Single precision multiply
vec2 vec2::operator* (float rhs) {
	vec2 vc;
	vc.v[0] = v[0] * rhs;
	vc.v[1] = v[1] * rhs;
	return vc;
}

// Single precision add vector
vec2 vec2::operator+ (const vec2& rhs) {
	vec2 vc;
	vc.v[0] = v[0] + rhs.v[0];
	vc.v[1] = v[1] + rhs.v[1];
	return vc;
}

// Single precision add vector
vec2 vec2::operator- (const vec2& rhs) {
	vec2 vc;
	vc.v[0] = v[0] - rhs.v[0];
	vc.v[1] = v[1] - rhs.v[1];
	return vc;
}

vec2d::vec2d () {}

vec2d::vec2d (double x, double y) {
	v[0] = x;
	v[1] = y;
}

vec2i::vec2i () {}

vec2i::vec2i (int x, int y) {
    v[0] = x;
    v[1] = y;
}

// integer add
vec2i& vec2i::operator+= (const vec2i& rhs) {
	v[0] += rhs.v[0];
	v[1] += rhs.v[1];
	return *this; // return self
}

vec2i vec2i::operator+ (const vec2i& rhs) {
	vec2i vc;
	vc.v[0] = v[0] + rhs.v[0];
	vc.v[1] = v[1] + rhs.v[1];
	return vc;
}

vec3i::vec3i () {}

vec3i::vec3i (int x, int y, int z) {
    v[0] = x;
    v[1] = y;
    v[2] = z;
}

vec3::vec3 () {}

vec3::vec3 (float x, float y, float z) {
	v[0] = x;
	v[1] = y;
	v[2] = z;
}


vec3::vec3 (const vec2& vv, float z) {
	v[0] = vv.v[0];
	v[1] = vv.v[1];
	v[2] = z;
}

vec3::vec3 (const vec4& vv) {
	v[0] = vv.v[0];
	v[1] = vv.v[1];
	v[2] = vv.v[2];
}

vec3d::vec3d () {}

vec3d::vec3d (double x, double y, double z) {
    v[0] = x;
    v[1] = y;
    v[2] = z;
}

// Turn a single precision vec4 to double vec3
vec3d::vec3d (const vec4& vv) {
    v[0] = vv.v[0];
    v[1] = vv.v[1];
    v[2] = vv.v[2];
}

vec3d::vec3d (const vec4d& vv) {
    v[0] = vv.v[0];
    v[1] = vv.v[1];
    v[2] = vv.v[2];
}

vec3ld::vec3ld () {}

vec3ld::vec3ld (long double x, long double y, long double z) {
    v[0] = x;
    v[1] = y;
    v[2] = z;
}

vec4::vec4 () {}

vec4::vec4 (float x, float y, float z, float w) {
	v[0] = x;
	v[1] = y;
	v[2] = z;
	v[3] = w;
}

vec4::vec4 (const vec2& vv, float z, float w) {
	v[0] = vv.v[0];
	v[1] = vv.v[1];
	v[2] = z;
	v[3] = w;
}

vec4::vec4 (const vec3& vv, float w) {
	v[0] = vv.v[0];
	v[1] = vv.v[1];
	v[2] = vv.v[2];
	v[3] = w;
}

vec4d::vec4d () {}

vec4d::vec4d (double x, double y, double z, double w) {
	v[0] = x;
	v[1] = y;
	v[2] = z;
	v[3] = w;
}

vec4d::vec4d (const vec2d& vv, double z, double w) {
	v[0] = vv.v[0];
	v[1] = vv.v[1];
	v[2] = z;
	v[3] = w;
}

vec4d::vec4d (const vec3d& vv, double w) {
	v[0] = vv.v[0];
	v[1] = vv.v[1];
	v[2] = vv.v[2];
	v[3] = w;
}

mat3::mat3 () {}

/* note: entered in COLUMNS */
mat3::mat3 (float a, float b, float c,
						float d, float e, float f,
						float g, float h, float i) {
	m[0] = a;
	m[1] = b;
	m[2] = c;
	m[3] = d;
	m[4] = e;
	m[5] = f;
	m[6] = g;
	m[7] = h;
	m[8] = i;
}

mat4::mat4 () {}

/* note: entered in COLUMNS */
mat4::mat4 (float a, float b, float c, float d,
						float e, float f, float g, float h,
						float i, float j, float k, float l,
						float mm, float n, float o, float p) {
	m[0] = a;
	m[1] = b;
	m[2] = c;
	m[3] = d;
	m[4] = e;
	m[5] = f;
	m[6] = g;
	m[7] = h;
	m[8] = i;
	m[9] = j;
	m[10] = k;
	m[11] = l;
	m[12] = mm;
	m[13] = n;
	m[14] = o;
	m[15] = p;
}

mat4d::mat4d () {}

/* note: entered in COLUMNS */
mat4d::mat4d (double a, double b, double c, double d,
						double e, double f, double g, double h,
						double i, double j, double k, double l,
						double mm, double n, double o, double p) {
	m[0] = a;
	m[1] = b;
	m[2] = c;
	m[3] = d;
	m[4] = e;
	m[5] = f;
	m[6] = g;
	m[7] = h;
	m[8] = i;
	m[9] = j;
	m[10] = k;
	m[11] = l;
	m[12] = mm;
	m[13] = n;
	m[14] = o;
	m[15] = p;
}

/*-----------------------------PRINT FUNCTIONS--------------------------------*/
void print (const vec2& v) {
	printf ("[%.2f, %.2f]\n", v.v[0], v.v[1]);
}

void print (const vec3& v) {
	printf ("[%.2f, %.2f, %.2f]\n", v.v[0], v.v[1], v.v[2]);
}

void print (const vec4& v) {
	printf ("[%.2f, %.2f, %.2f, %.2f]\n", v.v[0], v.v[1], v.v[2], v.v[3]);
}

void print (const mat3& m) {
	printf("\n");
	printf ("[%.2f][%.2f][%.2f]\n", m.m[0], m.m[3], m.m[6]);
	printf ("[%.2f][%.2f][%.2f]\n", m.m[1], m.m[4], m.m[7]);
	printf ("[%.2f][%.2f][%.2f]\n", m.m[2], m.m[5], m.m[8]);
}

void print (const mat4& m, FILE* debug) {
	fprintf(debug,"\n");
	fprintf (debug,"[%.2f][%.2f][%.2f][%.2f]\n", m.m[0], m.m[4], m.m[8], m.m[12]);
	fprintf (debug,"[%.2f][%.2f][%.2f][%.2f]\n", m.m[1], m.m[5], m.m[9], m.m[13]);
	fprintf (debug,"[%.2f][%.2f][%.2f][%.2f]\n", m.m[2], m.m[6], m.m[10], m.m[14]);
	fprintf (debug,"[%.2f][%.2f][%.2f][%.2f]\n", m.m[3], m.m[7], m.m[11], m.m[15]);
}

/*------------------------------VECTOR FUNCTIONS------------------------------*/
// single precision
float len2D (const vec2& v) {
	return sqrt (v.v[0]*v.v[0] + v.v[1]*v.v[1]);
}

// single precision
float length (const vec3& v) {
	return sqrt (v.v[0] * v.v[0] + v.v[1] * v.v[1] + v.v[2] * v.v[2]);
}

// double precision
double lengthd (const vec3d& v) {
	return sqrt (v.v[0] * v.v[0] + v.v[1] * v.v[1] + v.v[2] * v.v[2]);
}

// squared length single precision
float length2 (const vec3& v) {
	return v.v[0] * v.v[0] + v.v[1] * v.v[1] + v.v[2] * v.v[2];
}

// squared length double precision
double length2d (const vec3d& v) {
	return (v.v[0] * v.v[0] + v.v[1] * v.v[1] + v.v[2] * v.v[2]);
}

// note: proper spelling (hehe)
vec2 normalise2 (const vec2& v) {
	vec2 vb;
	float l = len2D(v);
	if (0.0f == l) {
		return vec2 (0.0f, 0.0f);
	}
	vb.v[0] = v.v[0] / l;
	vb.v[1] = v.v[1] / l;
	return vb;
}

// note: proper spelling (hehe)
vec3 normalise (const vec3& v) {
	vec3 vb;
	float l = length (v);
	if (0.0f == l) {
		return vec3 (0.0f, 0.0f, 0.0f);
	}
	vb.v[0] = v.v[0] / l;
	vb.v[1] = v.v[1] / l;
	vb.v[2] = v.v[2] / l;
	return vb;
}

vec3d normalised (const vec3d& v) {
	vec3d vb;
	double l = lengthd (v);
	if (0.0f == l) {
		return vec3d (0.0f, 0.0f, 0.0f);
	}
	vb.v[0] = v.v[0] / l;
	vb.v[1] = v.v[1] / l;
	vb.v[2] = v.v[2] / l;
	return vb;
}

// Single precision add vector
vec3 vec3::operator+ (const vec3& rhs) {
	vec3 vc;
	vc.v[0] = v[0] + rhs.v[0];
	vc.v[1] = v[1] + rhs.v[1];
	vc.v[2] = v[2] + rhs.v[2];
	return vc;
}

// Double precision add vector
vec3d vec3d::operator+ (const vec3d& rhs) {
	vec3d vc;
	vc.v[0] = v[0] + rhs.v[0];
	vc.v[1] = v[1] + rhs.v[1];
	vc.v[2] = v[2] + rhs.v[2];
	return vc;
}

// Long Double precision add vector
vec3ld vec3ld::operator+ (const vec3ld& rhs) {
	vec3ld vc;
	vc.v[0] = v[0] + rhs.v[0];
	vc.v[1] = v[1] + rhs.v[1];
	vc.v[2] = v[2] + rhs.v[2];
	return vc;
}


// single precision add
vec3& vec3::operator+= (const vec3& rhs) {
	v[0] += rhs.v[0];
	v[1] += rhs.v[1];
	v[2] += rhs.v[2];
	return *this; // return self
}

// double precision add
vec3d& vec3d::operator+= (const vec3d& rhs) {
	v[0] += rhs.v[0];
	v[1] += rhs.v[1];
	v[2] += rhs.v[2];
	return *this; // return self
}

// long double precision add
vec3ld& vec3ld::operator+= (const vec3ld& rhs) {
	v[0] += rhs.v[0];
	v[1] += rhs.v[1];
	v[2] += rhs.v[2];
	return *this; // return self
}

// Single precision subtract vector from vector
vec3 vec3::operator- (const vec3& rhs) {
	vec3 vc;
	vc.v[0] = v[0] - rhs.v[0];
	vc.v[1] = v[1] - rhs.v[1];
	vc.v[2] = v[2] - rhs.v[2];
	return vc;
}

// Double precision subtract vector from vector
vec3d vec3d::operator- (const vec3d& rhs) {
	vec3d vc;
	vc.v[0] = v[0] - rhs.v[0];
	vc.v[1] = v[1] - rhs.v[1];
	vc.v[2] = v[2] - rhs.v[2];
	return vc;
}

// Long Double precision subtract vector from vector
vec3ld vec3ld::operator- (const vec3ld& rhs) {
	vec3ld vc;
	vc.v[0] = v[0] - rhs.v[0];
	vc.v[1] = v[1] - rhs.v[1];
	vc.v[2] = v[2] - rhs.v[2];
	return vc;
}

// Single precision
vec3& vec3::operator-= (const vec3& rhs) {
	v[0] -= rhs.v[0];
	v[1] -= rhs.v[1];
	v[2] -= rhs.v[2];
	return *this;
}

// Double precision
vec3d& vec3d::operator-= (const vec3d& rhs) {
	v[0] -= rhs.v[0];
	v[1] -= rhs.v[1];
	v[2] -= rhs.v[2];
	return *this;
}

// Long Double precision
vec3ld& vec3ld::operator-= (const vec3ld& rhs) {
	v[0] -= rhs.v[0];
	v[1] -= rhs.v[1];
	v[2] -= rhs.v[2];
	return *this;
}

// single precision add
vec3 vec3::operator+ (float rhs) {
	vec3 vc;
	vc.v[0] = v[0] + rhs;
	vc.v[1] = v[1] + rhs;
	vc.v[2] = v[2] + rhs;
	return vc;
}

// single precision subtract
vec3 vec3::operator- (float rhs) {
	vec3 vc;
	vc.v[0] = v[0] - rhs;
	vc.v[1] = v[1] - rhs;
	vc.v[2] = v[2] - rhs;
	return vc;
}

// Single precision multiply
vec3 vec3::operator* (float rhs) {
	vec3 vc;
	vc.v[0] = v[0] * rhs;
	vc.v[1] = v[1] * rhs;
	vc.v[2] = v[2] * rhs;
	return vc;
}

// Double precision multiply
vec3d vec3d::operator* (double rhs) {
    vec3d vc;
    vc.v[0] = v[0] * rhs;
    vc.v[1] = v[1] * rhs;
    vc.v[2] = v[2] * rhs;
    return vc;
}

// Double precision multiply
vec3ld vec3ld::operator* (long double rhs) {
    vec3ld vc;
    vc.v[0] = v[0] * rhs;
    vc.v[1] = v[1] * rhs;
    vc.v[2] = v[2] * rhs;
    return vc;
}

// Single precision division
vec3 vec3::operator/ (float rhs) {
	vec3 vc;
	vc.v[0] = v[0] / rhs;
	vc.v[1] = v[1] / rhs;
	vc.v[2] = v[2] / rhs;
	return vc;
}

// Double precision division
vec3d vec3d::operator/ (double rhs) {
	vec3d vc;
	vc.v[0] = v[0] / rhs;
	vc.v[1] = v[1] / rhs;
	vc.v[2] = v[2] / rhs;
	return vc;
}

// Double precision division
vec3ld vec3ld::operator/ (long double rhs) {
	vec3ld vc;
	vc.v[0] = v[0] / rhs;
	vc.v[1] = v[1] / rhs;
	vc.v[2] = v[2] / rhs;
	return vc;
}

// single
vec3& vec3::operator*= (float rhs) {
	v[0] = v[0] * rhs;
	v[1] = v[1] * rhs;
	v[2] = v[2] * rhs;
	return *this;
}

// double
vec3d& vec3d::operator*= (double rhs) {
	v[0] = v[0] * rhs;
	v[1] = v[1] * rhs;
	v[2] = v[2] * rhs;
	return *this;
}

// double
vec3ld& vec3ld::operator*= (long double rhs) {
	v[0] = v[0] * rhs;
	v[1] = v[1] * rhs;
	v[2] = v[2] * rhs;
	return *this;
}

vec3d& vec3d::operator/= (double rhs) {
	v[0] = v[0] / rhs;
	v[1] = v[1] / rhs;
	v[2] = v[2] / rhs;
	return *this;
}

// Single precision assign
vec3& vec3::operator= (const vec3& rhs) {
	v[0] = rhs.v[0];
	v[1] = rhs.v[1];
	v[2] = rhs.v[2];
	return *this;
}

// Double precision assign
vec3d& vec3d::operator= (const vec3d& rhs) {
	v[0] = rhs.v[0];
	v[1] = rhs.v[1];
	v[2] = rhs.v[2];
	return *this;
}

// Double precision assign
vec3ld& vec3ld::operator= (const vec3ld& rhs) {
	v[0] = rhs.v[0];
	v[1] = rhs.v[1];
	v[2] = rhs.v[2];
	return *this;
}

// Assign single to double precision
vec3d& vec3d::operator= (const vec3& rhs) {
	v[0] =  rhs.v[0];
	v[1] =  rhs.v[1];
	v[2] =  rhs.v[2];
	return *this;
}
 // Assign double to single precision
vec3& vec3::operator= (const vec3d& rhs) {
	v[0] =   rhs.v[0];
	v[1] =   rhs.v[1];
	v[2] =   rhs.v[2];
	return *this;
}

vec3d float_to_double(vec3 con)            // Convert 3D Float to 3D Double
{
    return vec3d(con.v[0], con.v[1], con.v[2]);
}
vec3 double_to_float(vec3d con)            // Convert 3D Double to 3D Float
{
    return vec3(con.v[0], con.v[1], con.v[2]);
}
vec4 double_to_float(vec4d con)
{
    return vec4(con.v[0], con.v[1], con.v[2], con.v[3]);
}
vec3ld double_to_longdouble(vec3d con)
{
    return vec3ld(con.v[0], con.v[1], con.v[2]);
}
vec3d longdouble_to_double(vec3ld con)
{
    return vec3d(con.v[0], con.v[1], con.v[2]);
}
vec3 longdouble_to_float(vec3ld con)
{
    return vec3(con.v[0], con.v[1], con.v[2]);
}

// Dot 2d

// Dot single precision
float dot2 (vec2 a, vec2 b) {
	return a.v[0] * b.v[0] + a.v[1] * b.v[1];
}

// Dot single precision
float dot (const vec3& a, const vec3& b) {
	return a.v[0] * b.v[0] + a.v[1] * b.v[1] + a.v[2] * b.v[2];
}

// Dot double precision
double dot_d (const vec3d& a, const vec3d& b) {
	return a.v[0] * b.v[0] + a.v[1] * b.v[1] + a.v[2] * b.v[2];
}

// Dot of one vector
double dot2_d (const vec3d& a)
{
    return dot_d(a, a);
}

// Restricts input to a certain range
int clampi (int a, int small, int big)
{
    if (a<small) return small;
    if (a>big) return big;
    return a;
}

// Restricts input to a certain range
double clamp (double a, double small, double big)
{
    if (a<small) return small;
    if (a>big) return big;
    return a;
}

vec3i clamp_vec3i(vec3i a, int top)
{
    if (a.v[0] > top) a.v[0] = top;
    if (a.v[1] > top) a.v[1] = top;
    if (a.v[2] > top) a.v[2] = top;
    return a;
}

// Integer color mix
vec3i mix_colori (vec3i color, vec3i color2, float a)
{
    vec3i temp;

    temp.v[0] = (int) ((float) color.v[0] * (1.0 - a) + (float) color2.v[0] * a);
    temp.v[1] = (int) ((float) color.v[1] * (1.0 - a) + (float) color2.v[1] * a);
    temp.v[2] = (int) ((float) color.v[2] * (1.0 - a) + (float) color2.v[2] * a);
    return temp;
}

float Cross (const vec2& a, const vec2& b)
{
    return a.v[0] * b.v[1] - a.v[1] * b.v[0];
}

// Single precision
vec3 cross (const vec3& a, const vec3& b) {
	float x = a.v[1] * b.v[2] - a.v[2] * b.v[1];
	float y = a.v[2] * b.v[0] - a.v[0] * b.v[2];
	float z = a.v[0] * b.v[1] - a.v[1] * b.v[0];
	return vec3 (x, y, z);
}

// Double precision
vec3d crossd (const vec3d& a, const vec3d& b) {
	double x = a.v[1] * b.v[2] - a.v[2] * b.v[1];
	double y = a.v[2] * b.v[0] - a.v[0] * b.v[2];
	double z = a.v[0] * b.v[1] - a.v[1] * b.v[0];
	return vec3d (x, y, z);
}

float get_distance_2i (vec2i from, vec2i to)
{
    float x = (to.v[0] - from.v[0]) * (to.v[0] - from.v[0]);
    float y = (to.v[1] - from.v[1]) * (to.v[1] - from.v[1]);
    return sqrt(x + y);
}

float get_squared_dist (vec3 from, vec3 to) {
	double x = (to.v[0] - from.v[0]) * (to.v[0] - from.v[0]);
	double y = (to.v[1] - from.v[1]) * (to.v[1] - from.v[1]);
	double z = (to.v[2] - from.v[2]) * (to.v[2] - from.v[2]);
	return (float) x + y + z;
}

float distance_f (vec3 from, vec3 to) {
	float x = (to.v[0] - from.v[0]) * (to.v[0] - from.v[0]);
	float y = (to.v[1] - from.v[1]) * (to.v[1] - from.v[1]);
	float z = (to.v[2] - from.v[2]) * (to.v[2] - from.v[2]);
	return sqrt( x + y + z);
}

double distance_d (vec3d from, vec3d to) {
    double x = (to.v[0] - from.v[0]) * (to.v[0] - from.v[0]);
	double y = (to.v[1] - from.v[1]) * (to.v[1] - from.v[1]);
	double z = (to.v[2] - from.v[2]) * (to.v[2] - from.v[2]);
	return sqrt(x + y + z);
}


/* converts an un-normalised direction into a heading in degrees
NB i suspect that the z is backwards here but i've used in in
several places like this. d'oh! */
float direction_to_heading (vec3 d) {
	return atan2 (-d.v[0], -d.v[2]) * ONE_RAD_IN_DEG;
}

vec3 heading_to_direction (float degrees) {
	float rad = degrees * ONE_DEG_IN_RAD;
	return vec3 (-sinf (rad), 0.0f, -cosf (rad));
}

/*-----------------------------MATRIX FUNCTIONS-------------------------------*/
mat3 zero_mat3 () {
	return mat3 (
		0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f
	);
}

mat3 identity_mat3 () {
	return mat3 (
		1.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 1.0f
	);
}

mat4 zero_mat4 () {
	return mat4 (
		0.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f
	);
}

// double precision
mat4d zero_mat4d () {
	return mat4d (
		0.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f
	);
}

mat4 identity_mat4 () {
	return mat4 (
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	);
}

mat4d identity_mat4d () {
	return mat4d (
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	);
}

/* mat4 array layout
 0  4  8 12
 1  5  9 13
 2  6 10 14
 3  7 11 15
*/

vec4 mat4::operator* (const vec4& rhs) {
	// 0x + 4y + 8z + 12w
	float x =
		m[0] * rhs.v[0] +
		m[4] * rhs.v[1] +
		m[8] * rhs.v[2] +
		m[12] * rhs.v[3];
	// 1x + 5y + 9z + 13w
	float y = m[1] * rhs.v[0] +
		m[5] * rhs.v[1] +
		m[9] * rhs.v[2] +
		m[13] * rhs.v[3];
	// 2x + 6y + 10z + 14w
	float z = m[2] * rhs.v[0] +
		m[6] * rhs.v[1] +
		m[10] * rhs.v[2] +
		m[14] * rhs.v[3];
	// 3x + 7y + 11z + 15w
	float w = m[3] * rhs.v[0] +
		m[7] * rhs.v[1] +
		m[11] * rhs.v[2] +
		m[15] * rhs.v[3];
	return vec4 (x, y, z, w);
}

vec4d mat4d::operator* (const vec4d& rhs) {
	// 0x + 4y + 8z + 12w
	double x =
		m[0] * rhs.v[0] +
		m[4] * rhs.v[1] +
		m[8] * rhs.v[2] +
		m[12] * rhs.v[3];
	// 1x + 5y + 9z + 13w
	double y = m[1] * rhs.v[0] +
		m[5] * rhs.v[1] +
		m[9] * rhs.v[2] +
		m[13] * rhs.v[3];
	// 2x + 6y + 10z + 14w
	double z = m[2] * rhs.v[0] +
		m[6] * rhs.v[1] +
		m[10] * rhs.v[2] +
		m[14] * rhs.v[3];
	// 3x + 7y + 11z + 15w
	double w = m[3] * rhs.v[0] +
		m[7] * rhs.v[1] +
		m[11] * rhs.v[2] +
		m[15] * rhs.v[3];
	return vec4d (x, y, z, w);
}

vec3 mat4::operator* (const vec3& rhs) {
	// 0x + 4y + 8z + 12w
	float x =
		m[0] * rhs.v[0] +
		m[4] * rhs.v[1] +
		m[8] * rhs.v[2] +
		m[12] * rhs.v[3];
	// 1x + 5y + 9z + 13w
	float y = m[1] * rhs.v[0] +
		m[5] * rhs.v[1] +
		m[9] * rhs.v[2] +
		m[13] * rhs.v[3];
	// 2x + 6y + 10z + 14w
	float z = m[2] * rhs.v[0] +
		m[6] * rhs.v[1] +
		m[10] * rhs.v[2] +
		m[14] * rhs.v[3];
	// 3x + 7y + 11z + 15w
	//float w = m[3] * rhs.v[0] +
	//	m[7] * rhs.v[1] +
	//	m[11] * rhs.v[2] +
	//	m[15] * rhs.v[3];
	return vec3 (x, y, z);
}

vec3d mat4d::operator* (const vec3d& rhs) {
	// 0x + 4y + 8z + 12w
	float x =
		m[0] * rhs.v[0] +
		m[4] * rhs.v[1] +
		m[8] * rhs.v[2] +
		m[12] * 1.0; //rhs.v[3];
	// 1x + 5y + 9z + 13w
	float y = m[1] * rhs.v[0] +
		m[5] * rhs.v[1] +
		m[9] * rhs.v[2] +
		m[13] * 1.0; //rhs.v[3];
	// 2x + 6y + 10z + 14w
	float z = m[2] * rhs.v[0] +
		m[6] * rhs.v[1] +
		m[10] * rhs.v[2] +
		m[14] * 1.0; //rhs.v[3];
	// 3x + 7y + 11z + 15w
	//float w = m[3] * rhs.v[0] +
	//	m[7] * rhs.v[1] +
	//	m[11] * rhs.v[2] +
	//	m[15] * rhs.v[3];
	return vec3d (x, y, z);
}



mat4 mat4::operator* (const mat4& rhs) {
	mat4 r = zero_mat4 ();
	int r_index = 0;
	for (int col = 0; col < 4; col++) {
		for (int row = 0; row < 4; row++) {
			float sum = 0.0f;
			for (int i = 0; i < 4; i++) {
				sum += rhs.m[i + col * 4] * m[row + i * 4];
			}
			r.m[r_index] = sum;
			r_index++;
		}
	}
	return r;
}

// double precision
mat4d mat4d::operator* (const mat4d& rhs) {
	mat4d r = zero_mat4d ();
	int r_index = 0;
	for (int col = 0; col < 4; col++) {
		for (int row = 0; row < 4; row++) {
			double sum = 0.0f;
			for (int i = 0; i < 4; i++) {
				sum += rhs.m[i + col * 4] * m[row + i * 4];
			}
			r.m[r_index] = sum;
			r_index++;
		}
	}
	return r;
}

mat4& mat4::operator= (const mat4& rhs) {
	for (int i = 0; i < 16; i++) {
		m[i] = rhs.m[i];
	}
	return *this;
}

// double precision
mat4d& mat4d::operator= (const mat4d& rhs) {
	for (int i = 0; i < 16; i++) {
		m[i] = rhs.m[i];
	}
	return *this;
}

// returns a scalar value with the determinant for a 4x4 matrix
// see http://www.euclideanspace.com/maths/algebra/matrix/functions/determinant/fourD/index.htm
float determinant (const mat4& mm) {
	return
		mm.m[12] * mm.m[9] * mm.m[6] * mm.m[3] -
		mm.m[8] * mm.m[13] * mm.m[6] * mm.m[3] -
		mm.m[12] * mm.m[5] * mm.m[10] * mm.m[3] +
		mm.m[4] * mm.m[13] * mm.m[10] * mm.m[3] +
		mm.m[8] * mm.m[5] * mm.m[14] * mm.m[3] -
		mm.m[4] * mm.m[9] * mm.m[14] * mm.m[3] -
		mm.m[12] * mm.m[9] * mm.m[2] * mm.m[7] +
		mm.m[8] * mm.m[13] * mm.m[2] * mm.m[7] +
		mm.m[12] * mm.m[1] * mm.m[10] * mm.m[7] -
		mm.m[0] * mm.m[13] * mm.m[10] * mm.m[7] -
		mm.m[8] * mm.m[1] * mm.m[14] * mm.m[7] +
		mm.m[0] * mm.m[9] * mm.m[14] * mm.m[7] +
		mm.m[12] * mm.m[5] * mm.m[2] * mm.m[11] -
		mm.m[4] * mm.m[13] * mm.m[2] * mm.m[11] -
		mm.m[12] * mm.m[1] * mm.m[6] * mm.m[11] +
		mm.m[0] * mm.m[13] * mm.m[6] * mm.m[11] +
		mm.m[4] * mm.m[1] * mm.m[14] * mm.m[11] -
		mm.m[0] * mm.m[5] * mm.m[14] * mm.m[11] -
		mm.m[8] * mm.m[5] * mm.m[2] * mm.m[15] +
		mm.m[4] * mm.m[9] * mm.m[2] * mm.m[15] +
		mm.m[8] * mm.m[1] * mm.m[6] * mm.m[15] -
		mm.m[0] * mm.m[9] * mm.m[6] * mm.m[15] -
		mm.m[4] * mm.m[1] * mm.m[10] * mm.m[15] +
		mm.m[0] * mm.m[5] * mm.m[10] * mm.m[15];
}

double determinant_d (const mat4d& mm) {
	return
		mm.m[12] * mm.m[9] * mm.m[6] * mm.m[3] -
		mm.m[8] * mm.m[13] * mm.m[6] * mm.m[3] -
		mm.m[12] * mm.m[5] * mm.m[10] * mm.m[3] +
		mm.m[4] * mm.m[13] * mm.m[10] * mm.m[3] +
		mm.m[8] * mm.m[5] * mm.m[14] * mm.m[3] -
		mm.m[4] * mm.m[9] * mm.m[14] * mm.m[3] -
		mm.m[12] * mm.m[9] * mm.m[2] * mm.m[7] +
		mm.m[8] * mm.m[13] * mm.m[2] * mm.m[7] +
		mm.m[12] * mm.m[1] * mm.m[10] * mm.m[7] -
		mm.m[0] * mm.m[13] * mm.m[10] * mm.m[7] -
		mm.m[8] * mm.m[1] * mm.m[14] * mm.m[7] +
		mm.m[0] * mm.m[9] * mm.m[14] * mm.m[7] +
		mm.m[12] * mm.m[5] * mm.m[2] * mm.m[11] -
		mm.m[4] * mm.m[13] * mm.m[2] * mm.m[11] -
		mm.m[12] * mm.m[1] * mm.m[6] * mm.m[11] +
		mm.m[0] * mm.m[13] * mm.m[6] * mm.m[11] +
		mm.m[4] * mm.m[1] * mm.m[14] * mm.m[11] -
		mm.m[0] * mm.m[5] * mm.m[14] * mm.m[11] -
		mm.m[8] * mm.m[5] * mm.m[2] * mm.m[15] +
		mm.m[4] * mm.m[9] * mm.m[2] * mm.m[15] +
		mm.m[8] * mm.m[1] * mm.m[6] * mm.m[15] -
		mm.m[0] * mm.m[9] * mm.m[6] * mm.m[15] -
		mm.m[4] * mm.m[1] * mm.m[10] * mm.m[15] +
		mm.m[0] * mm.m[5] * mm.m[10] * mm.m[15];
}

/* returns a 16-element array that is the inverse of a 16-element array (4x4
matrix). see http://www.euclideanspace.com/maths/algebra/matrix/functions/inverse/fourD/index.htm */
mat4 inverse (const mat4& mm) {
	float det = determinant (mm);
	/* there is no inverse if determinant is zero (not likely unless scale is
	broken) */
	if (0.0f == det) {
		fprintf (stderr, "WARNING. matrix has no determinant. can not invert\n");
		return mm;
	}
	float inv_det = 1.0f / det;

	return mat4 (
		inv_det * (
			mm.m[9] * mm.m[14] * mm.m[7] - mm.m[13] * mm.m[10] * mm.m[7] +
			mm.m[13] * mm.m[6] * mm.m[11] - mm.m[5] * mm.m[14] * mm.m[11] -
			mm.m[9] * mm.m[6] * mm.m[15] + mm.m[5] * mm.m[10] * mm.m[15]
		),
		inv_det * (
			mm.m[13] * mm.m[10] * mm.m[3] - mm.m[9] * mm.m[14] * mm.m[3] -
			mm.m[13] * mm.m[2] * mm.m[11] + mm.m[1] * mm.m[14] * mm.m[11] +
			mm.m[9] * mm.m[2] * mm.m[15] - mm.m[1] * mm.m[10] * mm.m[15]
		),
		inv_det * (
			mm.m[5] * mm.m[14] * mm.m[3] - mm.m[13] * mm.m[6] * mm.m[3] +
			mm.m[13] * mm.m[2] * mm.m[7] - mm.m[1] * mm.m[14] * mm.m[7] -
			mm.m[5] * mm.m[2] * mm.m[15] + mm.m[1] * mm.m[6] * mm.m[15]
		),
		inv_det * (
			mm.m[9] * mm.m[6] * mm.m[3] - mm.m[5] * mm.m[10] * mm.m[3] -
			mm.m[9] * mm.m[2] * mm.m[7] + mm.m[1] * mm.m[10] * mm.m[7] +
			mm.m[5] * mm.m[2] * mm.m[11] - mm.m[1] * mm.m[6] * mm.m[11]
		),
		inv_det * (
			mm.m[12] * mm.m[10] * mm.m[7] - mm.m[8] * mm.m[14] * mm.m[7] -
			mm.m[12] * mm.m[6] * mm.m[11] + mm.m[4] * mm.m[14] * mm.m[11] +
			mm.m[8] * mm.m[6] * mm.m[15] - mm.m[4] * mm.m[10] * mm.m[15]
		),
		inv_det * (
			mm.m[8] * mm.m[14] * mm.m[3] - mm.m[12] * mm.m[10] * mm.m[3] +
			mm.m[12] * mm.m[2] * mm.m[11] - mm.m[0] * mm.m[14] * mm.m[11] -
			mm.m[8] * mm.m[2] * mm.m[15] + mm.m[0] * mm.m[10] * mm.m[15]
		),
		inv_det * (
			mm.m[12] * mm.m[6] * mm.m[3] - mm.m[4] * mm.m[14] * mm.m[3] -
			mm.m[12] * mm.m[2] * mm.m[7] + mm.m[0] * mm.m[14] * mm.m[7] +
			mm.m[4] * mm.m[2] * mm.m[15] - mm.m[0] * mm.m[6] * mm.m[15]
		),
		inv_det * (
			mm.m[4] * mm.m[10] * mm.m[3] - mm.m[8] * mm.m[6] * mm.m[3] +
			mm.m[8] * mm.m[2] * mm.m[7] - mm.m[0] * mm.m[10] * mm.m[7] -
			mm.m[4] * mm.m[2] * mm.m[11] + mm.m[0] * mm.m[6] * mm.m[11]
		),
		inv_det * (
			mm.m[8] * mm.m[13] * mm.m[7] - mm.m[12] * mm.m[9] * mm.m[7] +
			mm.m[12] * mm.m[5] * mm.m[11] - mm.m[4] * mm.m[13] * mm.m[11] -
			mm.m[8] * mm.m[5] * mm.m[15] + mm.m[4] * mm.m[9] * mm.m[15]
		),
		inv_det * (
			mm.m[12] * mm.m[9] * mm.m[3] - mm.m[8] * mm.m[13] * mm.m[3] -
			mm.m[12] * mm.m[1] * mm.m[11] + mm.m[0] * mm.m[13] * mm.m[11] +
			mm.m[8] * mm.m[1] * mm.m[15] - mm.m[0] * mm.m[9] * mm.m[15]
		),
		inv_det * (
			mm.m[4] * mm.m[13] * mm.m[3] - mm.m[12] * mm.m[5] * mm.m[3] +
			mm.m[12] * mm.m[1] * mm.m[7] - mm.m[0] * mm.m[13] * mm.m[7] -
			mm.m[4] * mm.m[1] * mm.m[15] + mm.m[0] * mm.m[5] * mm.m[15]
		),
		inv_det * (
			mm.m[8] * mm.m[5] * mm.m[3] - mm.m[4] * mm.m[9] * mm.m[3] -
			mm.m[8] * mm.m[1] * mm.m[7] + mm.m[0] * mm.m[9] * mm.m[7] +
			mm.m[4] * mm.m[1] * mm.m[11] - mm.m[0] * mm.m[5] * mm.m[11]
		),
		inv_det * (
			mm.m[12] * mm.m[9] * mm.m[6] - mm.m[8] * mm.m[13] * mm.m[6] -
			mm.m[12] * mm.m[5] * mm.m[10] + mm.m[4] * mm.m[13] * mm.m[10] +
			mm.m[8] * mm.m[5] * mm.m[14] - mm.m[4] * mm.m[9] * mm.m[14]
		),
		inv_det * (
			mm.m[8] * mm.m[13] * mm.m[2] - mm.m[12] * mm.m[9] * mm.m[2] +
			mm.m[12] * mm.m[1] * mm.m[10] - mm.m[0] * mm.m[13] * mm.m[10] -
			mm.m[8] * mm.m[1] * mm.m[14] + mm.m[0] * mm.m[9] * mm.m[14]
		),
		inv_det * (
			mm.m[12] * mm.m[5] * mm.m[2] - mm.m[4] * mm.m[13] * mm.m[2] -
			mm.m[12] * mm.m[1] * mm.m[6] + mm.m[0] * mm.m[13] * mm.m[6] +
			mm.m[4] * mm.m[1] * mm.m[14] - mm.m[0] * mm.m[5] * mm.m[14]
		),
		inv_det * (
			mm.m[4] * mm.m[9] * mm.m[2] - mm.m[8] * mm.m[5] * mm.m[2] +
			mm.m[8] * mm.m[1] * mm.m[6] - mm.m[0] * mm.m[9] * mm.m[6] -
			mm.m[4] * mm.m[1] * mm.m[10] + mm.m[0] * mm.m[5] * mm.m[10]
		)
	);
}

mat4d inverse_d (const mat4d& mm) {
	double det = determinant_d (mm);
	/* there is no inverse if determinant is zero (not likely unless scale is
	broken) */
	if (0.0f == det) {
		fprintf (stderr, "WARNING. matrix has no determinant. can not invert\n");
		return mm;
	}
	double inv_det = 1.0d / det;

	return mat4d (
		inv_det * (
			mm.m[9] * mm.m[14] * mm.m[7] - mm.m[13] * mm.m[10] * mm.m[7] +
			mm.m[13] * mm.m[6] * mm.m[11] - mm.m[5] * mm.m[14] * mm.m[11] -
			mm.m[9] * mm.m[6] * mm.m[15] + mm.m[5] * mm.m[10] * mm.m[15]
		),
		inv_det * (
			mm.m[13] * mm.m[10] * mm.m[3] - mm.m[9] * mm.m[14] * mm.m[3] -
			mm.m[13] * mm.m[2] * mm.m[11] + mm.m[1] * mm.m[14] * mm.m[11] +
			mm.m[9] * mm.m[2] * mm.m[15] - mm.m[1] * mm.m[10] * mm.m[15]
		),
		inv_det * (
			mm.m[5] * mm.m[14] * mm.m[3] - mm.m[13] * mm.m[6] * mm.m[3] +
			mm.m[13] * mm.m[2] * mm.m[7] - mm.m[1] * mm.m[14] * mm.m[7] -
			mm.m[5] * mm.m[2] * mm.m[15] + mm.m[1] * mm.m[6] * mm.m[15]
		),
		inv_det * (
			mm.m[9] * mm.m[6] * mm.m[3] - mm.m[5] * mm.m[10] * mm.m[3] -
			mm.m[9] * mm.m[2] * mm.m[7] + mm.m[1] * mm.m[10] * mm.m[7] +
			mm.m[5] * mm.m[2] * mm.m[11] - mm.m[1] * mm.m[6] * mm.m[11]
		),
		inv_det * (
			mm.m[12] * mm.m[10] * mm.m[7] - mm.m[8] * mm.m[14] * mm.m[7] -
			mm.m[12] * mm.m[6] * mm.m[11] + mm.m[4] * mm.m[14] * mm.m[11] +
			mm.m[8] * mm.m[6] * mm.m[15] - mm.m[4] * mm.m[10] * mm.m[15]
		),
		inv_det * (
			mm.m[8] * mm.m[14] * mm.m[3] - mm.m[12] * mm.m[10] * mm.m[3] +
			mm.m[12] * mm.m[2] * mm.m[11] - mm.m[0] * mm.m[14] * mm.m[11] -
			mm.m[8] * mm.m[2] * mm.m[15] + mm.m[0] * mm.m[10] * mm.m[15]
		),
		inv_det * (
			mm.m[12] * mm.m[6] * mm.m[3] - mm.m[4] * mm.m[14] * mm.m[3] -
			mm.m[12] * mm.m[2] * mm.m[7] + mm.m[0] * mm.m[14] * mm.m[7] +
			mm.m[4] * mm.m[2] * mm.m[15] - mm.m[0] * mm.m[6] * mm.m[15]
		),
		inv_det * (
			mm.m[4] * mm.m[10] * mm.m[3] - mm.m[8] * mm.m[6] * mm.m[3] +
			mm.m[8] * mm.m[2] * mm.m[7] - mm.m[0] * mm.m[10] * mm.m[7] -
			mm.m[4] * mm.m[2] * mm.m[11] + mm.m[0] * mm.m[6] * mm.m[11]
		),
		inv_det * (
			mm.m[8] * mm.m[13] * mm.m[7] - mm.m[12] * mm.m[9] * mm.m[7] +
			mm.m[12] * mm.m[5] * mm.m[11] - mm.m[4] * mm.m[13] * mm.m[11] -
			mm.m[8] * mm.m[5] * mm.m[15] + mm.m[4] * mm.m[9] * mm.m[15]
		),
		inv_det * (
			mm.m[12] * mm.m[9] * mm.m[3] - mm.m[8] * mm.m[13] * mm.m[3] -
			mm.m[12] * mm.m[1] * mm.m[11] + mm.m[0] * mm.m[13] * mm.m[11] +
			mm.m[8] * mm.m[1] * mm.m[15] - mm.m[0] * mm.m[9] * mm.m[15]
		),
		inv_det * (
			mm.m[4] * mm.m[13] * mm.m[3] - mm.m[12] * mm.m[5] * mm.m[3] +
			mm.m[12] * mm.m[1] * mm.m[7] - mm.m[0] * mm.m[13] * mm.m[7] -
			mm.m[4] * mm.m[1] * mm.m[15] + mm.m[0] * mm.m[5] * mm.m[15]
		),
		inv_det * (
			mm.m[8] * mm.m[5] * mm.m[3] - mm.m[4] * mm.m[9] * mm.m[3] -
			mm.m[8] * mm.m[1] * mm.m[7] + mm.m[0] * mm.m[9] * mm.m[7] +
			mm.m[4] * mm.m[1] * mm.m[11] - mm.m[0] * mm.m[5] * mm.m[11]
		),
		inv_det * (
			mm.m[12] * mm.m[9] * mm.m[6] - mm.m[8] * mm.m[13] * mm.m[6] -
			mm.m[12] * mm.m[5] * mm.m[10] + mm.m[4] * mm.m[13] * mm.m[10] +
			mm.m[8] * mm.m[5] * mm.m[14] - mm.m[4] * mm.m[9] * mm.m[14]
		),
		inv_det * (
			mm.m[8] * mm.m[13] * mm.m[2] - mm.m[12] * mm.m[9] * mm.m[2] +
			mm.m[12] * mm.m[1] * mm.m[10] - mm.m[0] * mm.m[13] * mm.m[10] -
			mm.m[8] * mm.m[1] * mm.m[14] + mm.m[0] * mm.m[9] * mm.m[14]
		),
		inv_det * (
			mm.m[12] * mm.m[5] * mm.m[2] - mm.m[4] * mm.m[13] * mm.m[2] -
			mm.m[12] * mm.m[1] * mm.m[6] + mm.m[0] * mm.m[13] * mm.m[6] +
			mm.m[4] * mm.m[1] * mm.m[14] - mm.m[0] * mm.m[5] * mm.m[14]
		),
		inv_det * (
			mm.m[4] * mm.m[9] * mm.m[2] - mm.m[8] * mm.m[5] * mm.m[2] +
			mm.m[8] * mm.m[1] * mm.m[6] - mm.m[0] * mm.m[9] * mm.m[6] -
			mm.m[4] * mm.m[1] * mm.m[10] + mm.m[0] * mm.m[5] * mm.m[10]
		)
	);
}

// returns a 16-element array flipped on the main diagonal
mat4 transpose (const mat4& mm) {
	return mat4 (
		mm.m[0], mm.m[4], mm.m[8], mm.m[12],
		mm.m[1], mm.m[5], mm.m[9], mm.m[13],
		mm.m[2], mm.m[6], mm.m[10], mm.m[14],
		mm.m[3], mm.m[7], mm.m[11], mm.m[15]
	);
}

/*--------------------------AFFINE MATRIX FUNCTIONS---------------------------*/
// translate a 4d matrix with xyz array
mat4 translate (const mat4& m, const vec3& v) {
	mat4 m_t = identity_mat4 ();
	m_t.m[12] = v.v[0];
	m_t.m[13] = v.v[1];
	m_t.m[14] = v.v[2];
	return m_t * m;
}

// double precision version of translate
mat4d translate_d (const mat4d& m, const vec3d& v) {
	mat4d m_t = identity_mat4d ();
	m_t.m[12] = v.v[0];
	m_t.m[13] = v.v[1];
	m_t.m[14] = v.v[2];
	return m_t * m;
}

// rotate around x axis by an angle in degrees
mat4 rotate_x_deg (const mat4& m, float deg) {
	// convert to radians
	float rad = deg * ONE_DEG_IN_RAD;
	mat4 m_r = identity_mat4 ();
	m_r.m[5] = cos (rad);
	m_r.m[9] = -sin (rad);
	m_r.m[6] = sin (rad);
	m_r.m[10] = cos (rad);
	return m_r * m;
}

// rotate around y axis by an angle in degrees
mat4 rotate_y_deg (const mat4& m, float deg) {
	// convert to radians
	float rad = deg * ONE_DEG_IN_RAD;
	mat4 m_r = identity_mat4 ();
	m_r.m[0] = cos (rad);
	m_r.m[8] = sin (rad);
	m_r.m[2] = -sin (rad);
	m_r.m[10] = cos (rad);
	return m_r * m;
}

// rotate around z axis by an angle in degrees
mat4 rotate_z_deg (const mat4& m, float deg) {
	// convert to radians
	float rad = deg * ONE_DEG_IN_RAD;
	mat4 m_r = identity_mat4 ();
	m_r.m[0] = cos (rad);
	m_r.m[4] = -sin (rad);
	m_r.m[1] = sin (rad);
	m_r.m[5] = cos (rad);
	return m_r * m;
}

// rotate around arbitrary axis (normalized) at origin
mat4 rotate_uvw(const mat4& m, double rad, vec3d axis)
{
    float x = (float) axis.v[0];
    float y = (float) axis.v[1];
    float z = (float) axis.v[2];
    float sine = sinf((float)rad);
    float cosine = cosf((float)rad);

    mat4 m_r = identity_mat4();
    m_r.m[0] = cosine + x*x * (1.0 - cosine);
    m_r.m[1] = y*x * (1.0 - cosine) + z*sine;
    m_r.m[2] = z*x * (1.0 - cosine) - y*sine;
    m_r.m[4] = x*y * (1.0 - cosine) - z*sine;
    m_r.m[5] = cosine + y*y * (1.0 - cosine);
    m_r.m[6] = z*y * (1.0 - cosine) + x*sine;
    m_r.m[8] = x*z * (1.0 - cosine) + y*sine;
    m_r.m[9] = y*z * (1.0 - cosine) - x*sine;
    m_r.m[10] = cosine + z*z * (1.0 - cosine);
    return m_r * m;
}

// double precision version of above
mat4d rotate_uvw_d(const mat4d& m, double rad, vec3d axis)
{
    double x = axis.v[0];
    double y = axis.v[1];
    double z = axis.v[2];
    double sine = sin(rad);
    double cosine = cos(rad);

    mat4d m_r = identity_mat4d();
    m_r.m[0] = cosine + x*x * (1 - cosine);
    m_r.m[1] = y*x * (1 - cosine) + z*sine;
    m_r.m[2] = z*x * (1 - cosine) - y*sine;
    m_r.m[4] = x*y * (1 - cosine) - z*sine;
    m_r.m[5] = cosine + y*y * (1 - cosine);
    m_r.m[6] = z*y * (1 - cosine) + x*sine;
    m_r.m[8] = x*z * (1 - cosine) + y*sine;
    m_r.m[9] = y*z * (1 - cosine) - x*sine;
    m_r.m[10] = cosine + z*z * (1 - cosine);
    return m_r * m;
}

// scale a matrix by [x, y, z]
mat4 scale (const mat4& m, const vec3& v) {
	mat4 a = identity_mat4 ();
	a.m[0] = v.v[0];
	a.m[5] = v.v[1];
	a.m[10] = v.v[2];
	return a * m;
}



// returns a view matrix using the opengl lookAt style. COLUMN ORDER.
mat4 look_at (vec3 cam_pos, vec3d fwd, vec3d rgt, vec3d up) {
	// inverse translation
	mat4 p = identity_mat4 ();
	p = translate (p, cam_pos);

	mat4 ori = identity_mat4 ();
	ori.m[0] = rgt.v[0];
	ori.m[4] = rgt.v[1];
	ori.m[8] = rgt.v[2];
	ori.m[1] = up.v[0];
	ori.m[5] = up.v[1];
	ori.m[9] = up.v[2];
	ori.m[2] = -fwd.v[0];
	ori.m[6] = -fwd.v[1];
	ori.m[10] = -fwd.v[2];

	return ori * p;//p * ori;
}

// double precision version of look_at
mat4d look_at_d (vec3d cam_pos, vec3d fwd, vec3d rgt, vec3d up) {
	// inverse translation
	mat4d p = identity_mat4d ();
	p = translate_d (p, cam_pos);

	mat4d ori = identity_mat4d ();
	ori.m[0] = rgt.v[0];
	ori.m[4] = rgt.v[1];
	ori.m[8] = rgt.v[2];
	ori.m[1] = up.v[0];
	ori.m[5] = up.v[1];
	ori.m[9] = up.v[2];
	ori.m[2] = -fwd.v[0];
	ori.m[6] = -fwd.v[1];
	ori.m[10] = -fwd.v[2];

	return ori * p;//p * ori;
}

// returns a view matrix using the opengl lookAt style. COLUMN ORDER.
mat4 look_at_fix (vec3 cam_pos, vec3d target, vec3d &rgt, vec3d &up) {
	// inverse translation
	mat4 p = identity_mat4 ();
	p = translate (p, cam_pos);
	// distance vector
	//vec3d d = target;
	// forward vector
	vec3d f = normalised (target);
	// right vector
	rgt = normalised (crossd (f, up));
	// real up vector
	up = normalised (crossd (rgt, f));
	mat4 ori = identity_mat4 ();
	ori.m[0] = rgt.v[0];
	ori.m[4] = rgt.v[1];
	ori.m[8] = rgt.v[2];
	ori.m[1] = up.v[0];
	ori.m[5] = up.v[1];
	ori.m[9] = up.v[2];
	ori.m[2] = -f.v[0];
	ori.m[6] = -f.v[1];
	ori.m[10] = -f.v[2];

	return ori * p;//p * ori;
}


// returns a perspective function mimicking the opengl projection style.
mat4 perspective (float fovy, float aspect, float near, float far) {
	float fov_rad = fovy * ONE_DEG_IN_RAD;
	float range = tan (fov_rad / 2.0f) * near;
	float sx = (2.0f * near) / (range * aspect + range * aspect);
	float sy = near / range;
	float sz = -(far + near) / (far - near);
	float pz = -(2.0f * far * near) / (far - near);
	mat4 m = zero_mat4 (); // make sure bottom-right corner is zero
	m.m[0] = sx;
	m.m[5] = sy;
	m.m[10] = sz;
	m.m[14] = pz;
	m.m[11] = -1.0f;
	return m;
}

mat4 ortho (float left, float right, float bottom, float top, float near, float far) {
   float x_orth = 2 / (right - left);
   float y_orth = 2 / (top - bottom);
   float z_orth = -2 / (far - near);
   float tx = -(right + left) / (right - left);
   float ty = -(top + bottom) / (top - bottom);
   float tz = -(far + near) / (far - near);
   mat4 m = zero_mat4();

   m.m[0] = x_orth;
   m.m[5] = y_orth;
   m.m[10] = z_orth;
   m.m[12] = tx;
   m.m[13] = ty;
   m.m[14] = tz;
   m.m[15] = 1.0f;

   return m;
}

/*----------------------------HAMILTON IN DA HOUSE!---------------------------*/
versor::versor () { }

versor versor::operator/ (float rhs) {
	versor result;
	result.q[0] = q[0] / rhs;
	result.q[1] = q[1] / rhs;
	result.q[2] = q[2] / rhs;
	result.q[3] = q[3] / rhs;
	return result;
}

versor versor::operator* (float rhs) {
	versor result;
	result.q[0] = q[0] * rhs;
	result.q[1] = q[1] * rhs;
	result.q[2] = q[2] * rhs;
	result.q[3] = q[3] * rhs;
	return result;
}

void print (const versor& q) {
	printf ("[%.2f ,%.2f, %.2f, %.2f]\n", q.q[0], q.q[1], q.q[2], q.q[3]);
}

versor versor::operator* (const versor& rhs) {
	versor result;
	result.q[0] = rhs.q[0] * q[0] - rhs.q[1] * q[1] -
		rhs.q[2] * q[2] - rhs.q[3] * q[3];
	result.q[1] = rhs.q[0] * q[1] + rhs.q[1] * q[0] -
		rhs.q[2] * q[3] + rhs.q[3] * q[2];
	result.q[2] = rhs.q[0] * q[2] + rhs.q[1] * q[3] +
		rhs.q[2] * q[0] - rhs.q[3] * q[1];
	result.q[3] = rhs.q[0] * q[3] - rhs.q[1] * q[2] +
		rhs.q[2] * q[1] + rhs.q[3] * q[0];
	// re-normalise in case of mangling
	return normalise (result);
}

versor versor::operator+ (const versor& rhs) {
	versor result;
	result.q[0] = rhs.q[0] + q[0];
	result.q[1] = rhs.q[1] + q[1];
	result.q[2] = rhs.q[2] + q[2];
	result.q[3] = rhs.q[3] + q[3];
	// re-normalise in case of mangling
	return normalise (result);
}

versor quat_from_axis_rad (float radians, float x, float y, float z) {
	versor result;
	result.q[0] = cos (radians / 2.0);
	result.q[1] = sin (radians / 2.0) * x;
	result.q[2] = sin (radians / 2.0) * y;
	result.q[3] = sin (radians / 2.0) * z;
	return result;
}

versor quat_from_axis_deg (float degrees, float x, float y, float z) {
	return quat_from_axis_rad (ONE_DEG_IN_RAD * degrees, x, y, z);
}

mat4 quat_to_mat4 (const versor& q) {
	float w = q.q[0];
	float x = q.q[1];
	float y = q.q[2];
	float z = q.q[3];
	return mat4 (
		1.0f - 2.0f * y * y - 2.0f * z * z,
		2.0f * x * y + 2.0f * w * z,
		2.0f * x * z - 2.0f * w * y,
		0.0f,
		2.0f * x * y - 2.0f * w * z,
		1.0f - 2.0f * x * x - 2.0f * z * z,
		2.0f * y * z + 2.0f * w * x,
		0.0f,
		2.0f * x * z + 2.0f * w * y,
		2.0f * y * z - 2.0f * w * x,
		1.0f - 2.0f * x * x - 2.0f * y * y,
		0.0f,
		0.0f,
		0.0f,
		0.0f,
		1.0f
	);
}

versor normalise (versor& q) {
	// norm(q) = q / magnitude (q)
	// magnitude (q) = sqrt (w*w + x*x...)
	// only compute sqrt if interior sum != 1.0
	float sum =
		q.q[0] * q.q[0] + q.q[1] * q.q[1] +
		q.q[2] * q.q[2] + q.q[3] * q.q[3];
	// NB: floats have min 6 digits of precision
	const float thresh = 0.0001f;
	if (fabs (1.0f - sum) < thresh) {
		return q;
	}
	float mag = sqrt (sum);
	return q / mag;
}

float dot (const versor& q, const versor& r) {
	return q.q[0] * r.q[0] + q.q[1] * r.q[1] + q.q[2] * r.q[2] + q.q[3] * r.q[3];
}

versor slerp (versor& q, versor& r, float t) {
	// angle between q0-q1
	float cos_half_theta = dot (q, r);
	// as found here http://stackoverflow.com/questions/2886606/flipping-issue-when-interpolating-rotations-using-quaternions
	// if dot product is negative then one quaternion should be negated, to make
	// it take the short way around, rather than the long way
	// yeah! and furthermore Susan, I had to recalculate the d.p. after this
	if (cos_half_theta < 0.0f) {
		for (int i = 0; i < 4; i++) {
			q.q[i] *= -1.0f;
		}
		cos_half_theta = dot (q, r);
	}
	// if qa=qb or qa=-qb then theta = 0 and we can return qa
	if (fabs (cos_half_theta) >= 1.0f) {
		return q;
	}
	// Calculate temporary values
	float sin_half_theta = sqrt (1.0f - cos_half_theta * cos_half_theta);
	// if theta = 180 degrees then result is not fully defined
	// we could rotate around any axis normal to qa or qb
	versor result;
	if (fabs (sin_half_theta) < 0.001f) {
		for (int i = 0; i < 4; i++) {
			result.q[i] = (1.0f - t) * q.q[i] + t * r.q[i];
		}
		return result;
	}
	float half_theta = acos (cos_half_theta);
	float a = sin ((1.0f - t) * half_theta) / sin_half_theta;
	float b = sin (t * half_theta) / sin_half_theta;
	for (int i = 0; i < 4; i++) {
		result.q[i] = q.q[i] * a + r.q[i] * b;
	}
	return result;
}

// ========================== INDEPENDANT MATH FUNCTIONS =========================

/* create a unit quaternion q from an angle in degrees a, and an axis x,y,z */
void create_versor (float* q, float a, float x, float y, float z)
{
	float rad = ONE_DEG_IN_RAD * a;
	q[0] = cosf (rad / 2.0f);
	q[1] = sinf (rad / 2.0f) * x;
	q[2] = sinf (rad / 2.0f) * y;
	q[3] = sinf (rad / 2.0f) * z;
}

/* convert a unit quaternion q to a 4x4 matrix m */
void quat_to_mat4 (float* m, float* q)
{
	float w = q[0];
	float x = q[1];
	float y = q[2];
	float z = q[3];
	m[0] = 1.0f - 2.0f * y * y - 2.0f * z * z;
	m[1] = 2.0f * x * y + 2.0f * w * z;
	m[2] = 2.0f * x * z - 2.0f * w * y;
	m[3] = 0.0f;
	m[4] = 2.0f * x * y - 2.0f * w * z;
	m[5] = 1.0f - 2.0f * x * x - 2.0f * z * z;
	m[6] = 2.0f * y * z + 2.0f * w * x;
	m[7] = 0.0f;
	m[8] = 2.0f * x * z + 2.0f * w * y;
	m[9] = 2.0f * y * z - 2.0f * w * x;
	m[10] = 1.0f - 2.0f * x * x - 2.0f * y * y;
	m[11] = 0.0f;
	m[12] = 0.0f;
	m[13] = 0.0f;
	m[14] = 0.0f;
	m[15] = 1.0f;
}

/* normalise a quaternion in case it got a bit mangled */
void normalise_quat (float* q)
{
	// norm(q) = q / magnitude (q)
	// magnitude (q) = sqrt (w*w + x*x...)
	// only compute sqrt if interior sum != 1.0
	float sum = q[0] * q[0] + q[1] * q[1] + q[2] * q[2] + q[3] * q[3];
	// NB: floats have min 6 digits of precision
	const float thresh = 0.0001f;
	if (fabs (1.0f - sum) < thresh) {
		return;
	}
	float mag = sqrt (sum);
	for (int i = 0; i < 4; i++) {
		q[i] = q[i] / mag;
	}
}

/* multiply quaternions to get another one. result=R*S */
void mult_quat_quat (float* result, float* r, float* s)
{
	result[0] = s[0] * r[0] - s[1] * r[1] -
		s[2] * r[2] - s[3] * r[3];
	result[1] = s[0] * r[1] + s[1] * r[0] -
		s[2] * r[3] + s[3] * r[2];
	result[2] = s[0] * r[2] + s[1] * r[3] +
		s[2] * r[0] - s[3] * r[1];
	result[3] = s[0] * r[3] - s[1] * r[2] +
		s[2] * r[1] + s[3] * r[0];
	// re-normalise in case of mangling
	normalise_quat (result);
}

/// Rotate 2D point around origon
vec2 Rotate_2f(float angle, vec2 point)
{
    vec2 newpos;
    float cosTheta = cos(angle);
    float sinTheta = sin(angle);

    newpos.v[0] = point.v[0]*cosTheta - point.v[1]*sinTheta;
    newpos.v[1] = point.v[0]*sinTheta + point.v[1]*cosTheta;
    return newpos;
}

/// Rotate 2D point around origon
vec2d Rotate_2d(double angle, vec2d point)
{
    vec2d newpos;
    double cosTheta = cos(angle);
    double sinTheta = sin(angle);

    newpos.v[0] = point.v[0]*cosTheta - point.v[1]*sinTheta;
    newpos.v[1] = point.v[0]*sinTheta + point.v[1]*cosTheta;
    return newpos;
}

double Linear_interpolate(double in, vec2d first, vec2d last)
{
    return (((in - first.v[0]) * (last.v[1] - last.v[0])) / (first.v[1] - first.v[0])) + last.v[0];
}

/// *** Angle between two 2D vectors ***
float angle_of_twoVecsf(vec2 a, vec2 b)
{
    return acos(dot2(normalise2(a), normalise2(b)));
}

/// *********** Angle between two 3D vectors *************
double angle_of_twoVecs(vec3d vec1, vec3d vec2)
{
    return acos(dot_d(normalised(vec1), normalised(vec2)));
}

/// *********** Normal vector of two 3D vectors ***********
vec3d normal_of_twoVecs(vec3d vec1, vec3d vec2)
{
    return crossd(normalised(vec1), normalised(vec2));
}

/// *********** Normal vector of two 3D vectors ***********
vec3 normal_of_twoVecsf(vec3 vec1, vec3 vec2)
{
    return cross(normalise(vec1), normalise(vec2));
}

/// Get equation of plane (input: normal and point on plane)
vec4d Plane_equation2(vec3d normal, vec3d p1)
{
    double D;

    D = -(normal.v[0]*p1.v[0] + normal.v[1]*p1.v[1] + normal.v[2]*p1.v[2]);
    return vec4d(normal, D);
}

/// Area of 2D triangle
float Triangle_area_2d(vec2 t1, vec2 t2, vec2 t3)
{
    return abs(0.5 * (t1.v[0]*(t2.v[1]-t3.v[1]) + t2.v[0]*(t3.v[1]-t1.v[1]) + t3.v[0]*(t1.v[1]-t2.v[1])));
}

/// Circle Circle intersection points 2D
void Circle_circle_intersect(circle_circle *cc)
{
    double a;
    double xf, yf;
    double xb, yb;
    double dist;

    dist = sqrt(((cc->cx1 - cc->cx2)*(cc->cx1 - cc->cx2)) + ((cc->cy1 - cc->cy2)*(cc->cy1 - cc->cy2)));
    a = 0.25 * sqrt((dist + cc->r1 + cc->r2)*(dist + cc->r1 - cc->r2)*(dist - cc->r1 + cc->r2)*(-dist + cc->r1 + cc->r2));
    xf = ((cc->cx1 + cc->cx2)*0.5) + (((cc->cx2 - cc->cx1)*(cc->r2*cc->r2 - cc->r1*cc->r1)) / (2.0 * dist*dist));
    yf = ((cc->cy1 + cc->cy2)*0.5) + (((cc->cy2 - cc->cy1)*(cc->r2*cc->r2 - cc->r1*cc->r1)) / (2.0 * dist*dist));
    xb = (2 * ((cc->cy1 - cc->cy2) / (dist*dist))) * a;
    yb = (2 * ((cc->cx1 - cc->cx2) / (dist*dist))) * a;
    cc->px1 = xf + xb;        // intersection point 1
    cc->py1 = yf - yb;
    cc->px2 = xf - xb;        // intersection point 2
    cc->py2 = yf + yb;
}

/// Circle line intersection points 2D
bool Circle_line_intersect(circle_line *cl)
{
    double lab, lec, dt;
    double dx, dy;
    double t;
    double ex, ey;

    lab = sqrt((cl->px2 - cl->px1)*(cl->px2 - cl->px1) + (cl->py2 - cl->py1)*(cl->py2 - cl->py1));
    dx = (cl->px2 - cl->px1) / lab;
    dy = (cl->py2 - cl->py1) / lab;
    t = dx*(cl->cx - cl->px1) + dy*(cl->cy - cl->py1);
    ex = t*dx + cl->px1;
    ey = t*dy + cl->py1;
    lec = sqrt((ex - cl->cx)*(ex - cl->cx) + (ey - cl->cy)*(ey - cl->cy));
    if (lec <= cl->r)
    {
        dt = sqrt((cl->r * cl->r) - (lec * lec));
        cl->fx = (t-dt) * dx + cl->px1;  // first intersection point.
        cl->fy = (t-dt) * dy + cl->py1;
        cl->gx = (t+dt) * dx + cl->px1;  // second intersetion point.
        cl->gy = (t+dt) * dy + cl->py1;
        return true;
    }
    return false;
}

/// Law of Cosines (find angle with lengths of three sides known
double Angle_A_LawOfCosines(double a, double b, double c)
{
    return acos((-(a*a) + b*b + c*c) / (2*b*c));
}
double Angle_B_LawOfCosines(double a, double b, double c)
{
    return acos((a*a - b*b + c*c) / (2*a*c));
}
double Angle_C_LawOfCosines(double a, double b, double c)
{
    return acos((a*a + b*b - c*c) / (2*a*b));
}

/// Law of Cosines (find length of third side, opposite angle known and two known sides)
double Side_A_LawOfCosines(double A, double b, double c)
{
    return sqrt(b*b + c*c - 2*b*c*cos(A));
}
double Side_B_LawOfCosines(double B, double a, double c)
{
    return sqrt(a*a + c*c - 2*a*c*cos(B));
}
double Side_C_LawOfCosines(double C, double a, double b)
{
    return sqrt(a*a + b*b - 2*a*b*cos(C));
}

/// line segment intersection 2D.
vec2 Line_line_intersect(vec2 p1, vec2 p2, vec2 p3, vec2 p4, bool *flag)
{
    double Ua;
    double Ub;
    double denom;
    double x, y;

    denom = (p4.v[1] - p3.v[1]) * (p2.v[0] - p1.v[0]) - (p4.v[0] - p3.v[0]) * (p2.v[1] - p1.v[1]);
    if (denom>=0.0 && denom<0.0001) denom = 0.0001;
    else if (denom>-0.0001 && denom<=0.0) denom = -0.0001;
    Ua = ((p4.v[0] - p3.v[0]) * (p1.v[1] - p3.v[1]) - (p4.v[1] - p3.v[1]) * (p1.v[0] - p3.v[0])) / denom;
    Ub = ((p2.v[0] - p1.v[0]) * (p1.v[1] - p3.v[1]) - (p2.v[1] - p1.v[1]) * (p1.v[0] - p3.v[0])) / denom;
    x = p1.v[0] + Ua*(p2.v[0] - p1.v[0]);
    y = p1.v[1] + Ua*(p2.v[1] - p1.v[1]);
    if ((Ua>=0 && Ua<=1.0) && (Ub>=0 && Ub<=1.0)) *flag = true;
    else *flag = false;
    return vec2(x, y);
}

/// line segment intersection 2D
bool Line_line_intersect2(vec2 p1, vec2 p2, vec2 p3, vec2 p4)
{
    float denom = (p4.v[1] - p3.v[1]) * (p2.v[0] - p1.v[0]) - (p4.v[0] - p3.v[0]) * (p2.v[1] - p1.v[1]);
    if (denom>=0.0 && denom<0.0001) denom = 0.0001;
    else if (denom>-0.0001 && denom<=0.0) denom = -0.0001;
    float Ua = ((p4.v[0] - p3.v[0]) * (p1.v[1] - p3.v[1]) - (p4.v[1] - p3.v[1]) * (p1.v[0] - p3.v[0])) / denom;
    float Ub = ((p2.v[0] - p1.v[0]) * (p1.v[1] - p3.v[1]) - (p2.v[1] - p1.v[1]) * (p1.v[0] - p3.v[0])) / denom;
    if ((Ua>=0 && Ua<=1.0) && (Ub>=0 && Ub<=1.0)) return  true;
    return false;
}

/// Infinite lines intersection 2D
vec2 Line_line_intersect3(vec2 p1, vec2 p2, vec2 p3, vec2 p4)
{
    double Ua;
    double Ub;
    double denom;
    double x, y;

    denom = (p4.v[1] - p3.v[1]) * (p2.v[0] - p1.v[0]) - (p4.v[0] - p3.v[0]) * (p2.v[1] - p1.v[1]);
    if (denom>=0.0 && denom<0.0001) denom = 0.0001;
    else if (denom>-0.0001 && denom<=0.0) denom = -0.0001;
    Ua = ((p4.v[0] - p3.v[0]) * (p1.v[1] - p3.v[1]) - (p4.v[1] - p3.v[1]) * (p1.v[0] - p3.v[0])) / denom;
    Ub = ((p2.v[0] - p1.v[0]) * (p1.v[1] - p3.v[1]) - (p2.v[1] - p1.v[1]) * (p1.v[0] - p3.v[0])) / denom;
    x = p1.v[0] + Ua*(p2.v[0] - p1.v[0]);
    y = p1.v[1] + Ua*(p2.v[1] - p1.v[1]);
    return vec2(x, y);
}


