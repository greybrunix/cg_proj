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

struct point {
	float x, y ,z;
};

class transform {
private:
	int type,time;
	bool align;
	float a,x,y,z;
	std::vector<struct point> ps;
public:
	transform(int t, float xx, float yy, float zz);
	transform(int t, float aa, float xx, float yy, float zz);
	transform(int t, int ti, float xx, float yy, float zz);
	transform(int t, int ti, bool al, std::vector<struct point>ps);
	virtual void do_transformation();
	int get_type();
	virtual float get_angle();
	virtual int get_time();
	virtual bool is_align();
	virtual float get_x();
	virtual float get_y();
	virtual float get_z();
	virtual void set_angle(float a);
	virtual void get_points(std::vector<struct point>* ps);
};

class rotate : public transform{
private:
	float previous_elapsed;
	float init_time;
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
	void get_catmull_rom_point(float t,
				   float *p0, float *p1, float *p2,
				   float *p3, float *pos, float *der);
	void get_catmull_rom_global_point(float gt, float *pos,
					  float *der);
	void mult_vec_mat(float *m, float *v, float *r);
	void normalize(float *v);
	void cross(float *v, float *u, float *r);
	float len(float *v);
	void build_rot_matrix(float *x, float *y, float *z, float *r);
public:
	translate_catmull_rom(int time, bool align,
			      std::vector<struct point>ps);
	void do_transformation() override;
};

