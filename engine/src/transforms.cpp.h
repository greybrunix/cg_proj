#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glew.h>
#include <GL/glut.h>
#endif

#include <cstdio>
#include <vector>
#define _USE_MATH_DEFINES
#include <math.h>
/* Classes */

#define TRANS_ROT 0
#define TRANS_SCA 1
#define TRANS_TRA 2
typedef float* point;

class transform {
private:
	int type,time;
	bool align;
	float a,x,y,z;
public:
	transform(int t, float xx, float yy, float zz);
	transform(int t, float aa, float xx, float yy, float zz);
	transform(int t, int ti, float xx, float yy, float zz);
	transform(int t, int ti, bool al);
	virtual void do_transformation();
	int get_type();
	virtual float get_angle();
	virtual int get_time();
	virtual bool is_align();
	virtual float get_x();
	virtual float get_y();
	virtual float get_z();
	virtual void set_angle(float a);
	virtual void add_point(float *p);
};

class rotate : public transform{
private:
	float previous_elapsed, init_time;
	float get_PE();
	void set_PE(float elapsed);
	float get_IT();
	void set_IT(float elapsed);
public:
	rotate(float a, float xx,
	       float yy,float zz);
	rotate(int ti, float xx,
	       float yy, float zz);
	void do_transformation() override;
};


class scale : public transform{
public:
	scale(float xx, float yy, float zz);
	void do_transformation() override;
};

class translate_static : public transform{
public:
	translate_static(float xx, float yy, float zz);
	void do_transformation() override;
};
class translate_catmull_rom : public transform{
private:
	std::vector<float*> *ps;
	float previous_elapsed, init_time;
	void get_catmull_rom_point(float t,
				   point p0, point p1, point p2,
				   point p3, point pos, point der);
	void get_catmull_rom_global_point(float gt, point pos,
					  point der);
	void mult_vec_mat(float *m, point v, point r);
	void normalize(point v);
	void cross(point v, point u, point r);
	float len(point v);
	void build_rot_matrix(point x, point y, point z, point r);
	void set_PE(float elapsed);
	float get_PE();
	void set_IT(float elapsed);
	float get_IT();
public:
	translate_catmull_rom(int time, bool align);
	void do_transformation() override;
	void add_point(float *p) override;
	void get_point(float *p, int i);
};

