#ifndef __VEC__
#define __VEC__

struct triple {
	float x, y, z;
	void copy(triple c){
		x = c.x, y = c.y, z = c.z; 
	}
};
struct quad {
	triple xyz;
	float w;
};

float dot(const triple& v, const triple& u);
float len(const triple& v);
void normalize(triple& v);
void scalar(const triple& v, const float scalar, triple& r);
void cross(const triple& v, const triple& u, triple& r);
void add(const triple& v, const triple& u, triple& r);
void sub(const triple& v, const triple& u, triple& r);
void mult_mat_vec(float* m, float* v, float* r);
#endif
