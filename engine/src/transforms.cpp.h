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
	std::vector<struct point> points;
public:
	transform(int t, float xx, float yy, float zz);
	transform(int t, float aa, float xx, float yy, float zz);
	transform(int t, int ti, float xx, float yy, float zz);
	transform(int t, int ti, bool al, std::vector<struct point> ps);
	virtual void do_transformation();
	virtual void do_transformation(int elapsed);
	int get_type();
	virtual float get_angle();
	virtual int get_time();
	virtual bool is_align();
	virtual float get_x();
	virtual float get_y();
	virtual float get_z();
	virtual void set_angle(float aa);
};

class rotate : public transform{
public:
	rotate(float a, float xx,
	       float yy,float zz);
	rotate(int ti, float xx,
	       float yy, float zz);
	void do_transformation(int elapsed) override;
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
public:
	translate_catmull_rom(int time, bool align,
			      std::vector<struct point> points);
	void do_transformation() override;
};

