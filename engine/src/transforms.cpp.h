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
		float a,x,y,z;
	public:
		transform(int t, float xx, float yy, float zz);
		transform(int t, float aa, float xx, float yy, float zz);
		virtual void do_transformation();
		int get_type();
		virtual float get_angle();
		virtual float get_x();
		virtual float get_y();
		virtual float get_z();
};

class rotate : public transform{
	public: 
		rotate(float a, float xx,
		       float yy,float zz);
		void do_transformation() override;
};

class scale : public transform{
	public:
		scale(float xx, float yy, float zz);
		void do_transformation() override;
};

class translate : public transform{
	public:
		translate(float xx, float yy, float zz);
		void do_transformation() override;
};
