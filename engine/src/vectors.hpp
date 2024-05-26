#ifndef __VEC__
#define __VEC__

struct triple {
	float x, y, z;
};

float dot(const triple& v, const triple& u);
float len(const triple& v);
void normalize(triple& v);
void scalar(const triple& v, const float scalar, triple& r);
void cross(const triple& v, const triple& u, triple& r);
void add(const triple& v, const triple& u, triple& r);
void sub(const triple& v, const triple& u, triple& r);
#endif
