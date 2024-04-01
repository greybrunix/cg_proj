#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glew.h>
#include <GL/glut.h>
#endif

#define _USE_MATH_DEFINES
#include <math.h>
/* Classes */
union trans_type {
	void (*trans_scal)(GLfloat x,GLfloat y, GLfloat z);
	void (*rot)(GLfloat ang, GLfloat x,
		     GLfloat y, GLfloat z);
};
union args {
	struct {
		float x,y,z;
	} trans_scal;
	struct {
		float angle,x,y,z;
	}rot;
};
class transform {
	private:
		union trans_type type;
		union args args;
	public:
		transform(union trans_type type, union args args);
		void do_transformation();
};

class rotate : public transform {
	public: 
		rotate(float a, float xx,
		       float yy,float zz);
};

class scale : public transform {
	public:
		scale(float xx, float yy, float zz);
};

class translate : public transform {
	public:
		translate(float xx, float yy, float zz);
};
