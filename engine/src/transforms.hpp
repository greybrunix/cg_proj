#ifndef __TRANSFORMS__
#define __TRANSFORMS__
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glew.h>
#include <GL/glut.h>
#include <GL/glxew.h>
#endif
#define _USE_MATH_DEFINES
#include <math.h>
#include <vector>
#include "vectors.hpp"
#include <cstdio>
/* Classes */

#define TRANS_ROT 0
#define TRANS_SCA 1
#define TRANS_TRA 2
typedef float* point;
extern bool draw;
extern float tesselation;
class transform {
private:
	float matrix[16];
	int type,time;
	bool align;
	float a,x,y,z;
public:
	transform(int t, float xx, float yy, float zz)
	{this->a=0.f,this->type = t,this->x=xx,this->y=yy,this->z=zz;}
	transform(int t, float aa, float xx, float yy, float zz)
	{this->a=aa,this->time=0.f,this->type = t,this->x=xx,this->y=yy,this->z=zz;}
	transform(int t, int ti, float xx, float yy, float zz)
	{this->time=ti,this->type=t,this->x=xx,this->y=yy,this->z=zz,this->a=0.f;}
	transform(int t, int ti, bool al)
	{this->type=t, this->time=ti, this->align=al;}
	virtual void do_transformation(){}
	void updateMatrix(float newMatrix[16]){
		mult_mat_vec(newMatrix, matrix, matrix);
	}
	int get_type(){return this->type;}
	int get_time(){return this->time;}
	bool is_align(){return this->align;}
	float get_angle(){return this->a;}
	float get_x(){return this->x;}
	float get_y(){return this->y;}
	float get_z(){return this->z;}
	void set_angle(float aa) { this->a = aa; }
	void get_matrix(float* m){
		m = matrix;
	}
	virtual void add_point(triple p) {}
};

class rotate : public transform{
private:
	int previous_elapsed, init_time;
	int get_PE() { return this->previous_elapsed; }
	void set_PE(float elapsed) { this->previous_elapsed = elapsed; }
	int get_IT() { return this->init_time; }
	void set_IT(float elapsed) { this->init_time = elapsed; }
public:
	rotate(float a, float xx, float yy, float zz)
		: transform(TRANS_ROT, a, xx, yy, zz)
	{
		this->previous_elapsed = this->init_time = 0.f;
	}

	void do_transformation() override
	{
		float rot_a = this->get_angle();
		int t = this->get_time();
		float current_time = glutGet(GLUT_ELAPSED_TIME) / 1000.f;
		int i_ti = this->get_IT();
		float elapsed = current_time - i_ti;

		if (t) {
			rot_a = (360.f * elapsed) / t;
			glRotatef(rot_a, this->get_x(),
				this->get_y(), this->get_z());
			this->set_PE(current_time);
			if (elapsed >= t) {
				rot_a = 0.f;
				this->set_IT(current_time);
			}
		}
		else {
			float matrix[16];
			// Convert angle from degrees to radians
			float radians = rot_a* M_PI / 180.0f;

			// Normalize the rotation axis
			float x = get_x(), y = get_y(),
						z = get_z();
			float length = sqrt(x * x + y * y + z * z);
			if (length == 0) return; // Avoid division by zero
			x /= length;
			y /= length;
			z /= length;

			// Calculate trigonometric values
			float c = cos(radians);
			float s = sin(radians);
			float t = 1 - c;

			// Compute the rotation matrix components
			matrix[0] = t * x * x + c;
			matrix[1] = t * x * y - s * z;
			matrix[2] = t * x * z + s * y;
			matrix[3] = 0;

			matrix[4] = t * x * y + s * z;
			matrix[5] = t * y * y + c;
			matrix[6] = t * y * z - s * x;
			matrix[7] = 0;

			matrix[8] = t * x * z - s * y;
			matrix[9] = t * y * z + s * x;
			matrix[10] = t * z * z + c;
			matrix[11] = 0;

			matrix[12] = 0;
			matrix[13] = 0;
			matrix[14] = 0;
			matrix[15] = 1;
			updateMatrix(matrix);
			glRotatef(rot_a, this->get_x(),
				this->get_y(), this->get_z());
		this->set_angle(rot_a);
		}
	}
	rotate(int ti, float xx, float yy, float zz)
		: transform(TRANS_ROT, ti, xx, yy, zz)
	{
		this->previous_elapsed = this->init_time = 0.f;
	}
};


class scale : public transform{
public:
	scale(float xx, float yy, float zz)
		: transform(TRANS_SCA, xx, yy, zz){}
	void do_transformation() override
	{
		float matrix[4][4] =  {{get_x(),0.F,0.F,0.F},
			{0.F,get_y(),0.F,0.F},
			{0.F,0.F,get_z(),0.F},
			{0.F,0.F,0.F,1.F}};
			updateMatrix(&(matrix[0][0]));
			glScalef(this->get_x(),this->get_y(),this->get_z());}
};

class translate_static : public transform{
public:
	translate_static(float xx, float yy, float zz)
		: transform(TRANS_TRA, xx, yy, zz){}
	void do_transformation() override
	{
		float matrix[4][4] =  {{0.F,0.F,0.F,get_x()},
			{0.F,0.F,0.F,get_y()},
			{0.F,0.F,0.F,get_z()},
			{0.F,0.F,0.F,1.F}};
		updateMatrix(&(matrix[0][0]));
		glTranslatef(this->get_x(),this->get_y(),this->get_z());
	}
};
class translate_catmull_rom : public transform{
private:
	std::vector<quad> *ps;
	int previous_elapsed, init_time;
	int get_PE() { return this->previous_elapsed; }
	void set_PE(float elapsed) { this->previous_elapsed = elapsed; }
	int get_IT() { return this->init_time; }
	void set_IT(float elapsed) { this->init_time = elapsed; }


	void get_catmull_rom_point(float t,
				quad p0,
				quad p1,
				quad p2,
				quad p3,
				quad& pos,
				quad& der)
	{
		float P[4] = {0.F}, A[4] = {0.F}; int i;
		float m[4][4] = {{-0.5f,  1.5f, -1.5f,  0.5f},
				{1.0f, -2.5f,  2.0f, -0.5f},
				{-0.5f,  0.0f,  0.5f,  0.0f},
				{0.0f,  1.0f,  0.0f,  0.0f}};
		//float h = FLT_EPSILON*1e3F;

		P[0] = p0.xyz.x; P[1] = p1.xyz.x; P[2] = p2.xyz.x; P[3] = p3.xyz.x;
		mult_mat_vec(&m[0][0], P, A);
		pos.xyz.x = (t*t*t*A[0] + t*t*A[1] + t*A[2] + A[3]);
		der.xyz.x = (3*t*t*A[0] + 2*t*A[1] + A[2]);

		P[0] = p0.xyz.y; P[1] = p1.xyz.y; P[2] = p2.xyz.y; P[3] = p3.xyz.y;
		mult_mat_vec(&m[0][0], P, A);
		pos.xyz.y = (t*t*t*A[0] + t*t*A[1] + t*A[2] + A[3]);
		der.xyz.y = (3*t*t*A[0] + 2*t*A[1] + A[2]);

		P[0] = p0.xyz.z; P[1] = p1.xyz.z; P[2] = p2.xyz.z; P[3] = p3.xyz.z;
		mult_mat_vec(&m[0][0], P, A);
		pos.xyz.z = (t*t*t*A[0] + t*t*A[1] + t*A[2] + A[3]);
		der.xyz.z = (3*t*t*A[0] + 2*t*A[1] + A[2]);

		P[0] = p0.w; P[1] = p1.w; P[2] = p2.w; P[3] = p3.w;
		mult_mat_vec(&m[0][0], P, A);
		pos.w = (t*t*t*A[0] + t*t*A[1] + t*A[2] + A[3]);
		der.w = (3*t*t*A[0] + 2*t*A[1] + A[2]);

	}

	void get_catmull_rom_global_point(float gt,
					quad& pos,
					quad& der)
	{
		quad p0,p1,p2,p3;
		int point_c = this->ps->size();
		float t = gt * (point_c); // this is the real global t
		int index = floor(t);  // which segment
		t = t - index; // where within  the segment

		// indices store the points
		int indices[4];
		indices[0] = (index + point_c-1)%point_c;
		indices[1] = (indices[0]+1)%point_c;
		indices[2] = (indices[1]+1)%point_c;
		indices[3] = (indices[2]+1)%point_c;

		this->get_point(p0,indices[0]), this->get_point(p1,indices[1]);
		this->get_point(p2,indices[2]), this->get_point(p3,indices[3]);
		this->get_catmull_rom_point(t, p0, p1,
					p2, p3,
					pos, der);
	}

	void build_rot_matrix(point x, point y, point z,
				point r)
	{
		r[0] = x[0]; r[1] = x[1]; r[2] = x[2]; r[3] = 0;
		r[4] = y[0]; r[5] = y[1]; r[6] = y[2]; r[7] = 0;
		r[8] = z[0]; r[9] = z[1]; r[10] = z[2]; r[11] = 0;
		r[12] = 0; r[13] = 0; r[14] = 0; r[15] = 1;
	}


public:
	translate_catmull_rom(int time, bool align)
		: transform(TRANS_TRA, time, align)
	{
		this->ps = new std::vector<quad>();
		this->previous_elapsed = this->init_time = 0.f;
	}
	void do_transformation() override
	{
		static quad P = {0.F, 0.F, 0.F, 1.F};
		static quad X = {0.F, 0.F, 0.F, 0.F};
		static quad Y = {0.F, 1.F, 0.F, 0.F};
		quad Z = {0.F, 0.F, 0.F, 0.F};
		float mat[16];
		int t_seconds = this->get_time();
		float current_time = glutGet(GLUT_ELAPSED_TIME) / 1000.f;
		int i_ti = this->get_IT();
		float elapsed = current_time - i_ti;
		float gt;
		if (elapsed < t_seconds) {
			gt = elapsed / t_seconds;
			this->get_catmull_rom_global_point(gt, P, X);
			P.xyz.x /= P.w;
			P.xyz.y /= P.w;
			P.xyz.z /= P.w;
			P.w = 1.F;
			if (draw) {
				glBegin(GL_LINE_STRIP);
				for (float i = 0.F; i < 1; i += 1.0 / tesselation) {
					quad P_ = {0.F,0.F,0.F,1.F},
						der = {0.F, 0.F, 0.F, 0.F};
					this->get_catmull_rom_global_point(i, P_, der);
					glVertex3f(P_.xyz.x/P_.w, P_.xyz.y/P_.w, P_.xyz.z/P_.w);
				}
				glEnd();
			}
			float matrix[4][4] =  {{0.F,0.F,0.F,get_x()},
				{0.F,0.F,0.F,get_y()},
				{0.F,0.F,0.F,get_z()},
				{0.F,0.F,0.F,1.F}};
			glTranslatef(P.xyz.x, P.xyz.y, P.xyz.z);
			if (this->is_align()) {
				normalize(X.xyz);
				cross(X.xyz, Y.xyz, Z.xyz);
				normalize(Z.xyz);
				cross(Z.xyz, X.xyz, Y.xyz);
				if (len(Y.xyz) == 1.F)
					normalize(Y.xyz);
				float x[4] = {X.xyz.x,X.xyz.y,X.xyz.z,X.w},
					y[4] = {Y.xyz.x,Y.xyz.y,Y.xyz.z,Z.w},
					z[4] = {Z.xyz.x,Z.xyz.y,Z.xyz.z,Z.w}; 
				this->build_rot_matrix(x,y,z,mat);
				glMultMatrixf(mat);
			}
			mult_mat_vec(mat, &matrix[0][0], &matrix[0][0]);
			updateMatrix(&(matrix[0][0]));
		}
		else {
			// Reset elapsed time and initial time
			elapsed = 0.0f;
			i_ti = current_time;
			this->set_IT(i_ti);
		}
	}
	void add_point(triple p) override
	{
		quad p_ = quad();
		p_.xyz.x = p.x;
		p_.xyz.y = p.y;
		p_.xyz.z = p.z;
		p_.w = 1.F;
		this->ps->push_back(p_);
	}
	void get_point(quad& p, int i)
	{
		quad p_ = (*ps)[i];
		p.xyz.x = p_.xyz.x;
		p.xyz.y = p_.xyz.y;
		p.xyz.z = p_.xyz.z;
		p.w = p_.w;
	}
};

struct trans {
	int group;
	transform* t;
};
#endif
