#define _USE_MATH_DEFINES
#include <math.h>
#include "vectors.hpp"
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
	float l = sqrt(v.x*v.x + v.y*v.y + v.z*v.z);
	scalar(v,1.F/l,v);
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
