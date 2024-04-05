#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glew.h>
#include <GL/glut.h>
#endif

#include <cstdio>
#define _USE_MATH_DEFINES
#include <math.h>
/* Classes */

#define TRANS_ROT 0
#define TRANS_SCA 1
#define TRANS_TRA 2

class transform {
	private:
		int type;
	public:
		transform(int t);
		virtual void do_transformation();
		int get_type();
};

class rotate : public transform{
	private:
		float x,y,z,angle;
	public: 
		rotate(float a, float xx,
		       float yy,float zz);
		void do_transformation() override;
		float get_angle();
		float get_x();
		float get_y();
		float get_z();
};

class scale : public transform{
	private:
		float x,y,z;
	public:
		scale(float xx, float yy, float zz);
		void do_transformation() override;
		float get_angle();
		float get_x();
		float get_y();
		float get_z();

};

class translate : public transform{
	private:
		float x,y,z;
	public:
		translate(float xx, float yy, float zz);
		void do_transformation() override;
		float get_angle();
		float get_x();
		float get_y();
		float get_z();
};
