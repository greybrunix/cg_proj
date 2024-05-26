#define _USE_MATH_DEFINES
#include <math.h>
static float Q_rsqrt(float number)
{
	long i;
	float x2, y;
	const float threehalfs = 1.5F;
	x2 = number * 0.5F;
	y  = number;
	i  = * ( long * ) &y;
	i  = 0x5f3759df - ( i >> 1 );
	y  = * ( float * ) &i;
	y  = y * ( threehalfs - ( x2 * y * y ) );
	return y;
}
void normalize(float* v)
{
	float l = Q_rsqrt(v[0]*v[0] + v[1]*v[1] + v[2]*v[2]);
	v[0] = v[0]*l;
	v[1] = v[1]*l;
	v[2] = v[2]*l;
}
void cross(const float* v, const float* u, float* r)
{
	r[0] = v[1]*u[2] - v[2]*u[1];
	r[1] = v[2]*u[0] - v[0]*u[2];
	r[2] = v[0]*u[1] - v[1]*u[0];
}
float len(const float* v)
{
	return sqrtf(v[0]*v[0] + v[1]*v[1] + v[2]*v[2]);
}
float dot(const float*v, const float*u)
{
	return (v[0]*u[0] + v[1]*u[1] + v[2]*u[2]);
}

void scalar(const float* v, const float scalar, float* r)
{
	r[0] = v[0]*scalar;
	r[1] = v[1]*scalar;
	r[2] = v[2]*scalar;
}
