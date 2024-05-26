#define _USE_MATH_DEFINES
#include <math.h>
#include "vectors.hpp"
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
void cross(const triple& v, const triple& u, triple& r)
{
	r.x = v.y*u.z - v.z*u.y;
	r.y = v.z*u.x - v.x*u.z;
	r.z = v.x*u.y - v.y*u.x;
}
float len(const triple& v)
{
	return sqrtf(v.x*v.x + v.y*v.y + v.z*v.z);
}
float dot(const triple&v, const triple&u)
{
	return (v.x*u.x + v.y*u.y + v.z*u.z);
}

void scalar(const triple& v, const float scalar, triple& r)
{
	r.x = v.x*scalar;
	r.y = v.y*scalar;
	r.z = v.z*scalar;
}
void add(const triple& v, const triple& u, triple& r)
{
	r.x = v.x+u.x;
	r.y = v.y+u.y;
	r.z = v.z+u.z;
}
void sub(const triple& v, const triple& u, triple& r)
{
	r.x = v.x-u.x;
	r.y = v.y-u.y;
	r.z = v.z-u.z;
}
void normalize(triple& v)
{
	float l = Q_rsqrt(v.x*v.x + v.y*v.y + v.z*v.z);
	scalar(v,l,v);
}

void mult_mat_vec(float* m, float* v, float* r)
{
	int j,k;
	for (j=0; j<4;++j) {
		r[j] = 0;
		for (k=0;k<4;++k) {
			r[j] += v[k] * m[j*4+k];
		}
	}
}