#include "transforms.cpp.h"

/** IMPORTANTE
 *  ver Fast inverse square root algorithm
 *      ou Quake III Arena Q_rsqrt para mais informacao
 */
static float Q_rsqrt(float number)
{
	long i;
	float x2, y;
	const float threehalfs = 1.5F;
	x2 = number * 0.5F;
	y  = number;
	i  = * ( long * ) &y;
	i  = 0x5f3759df - ( i >> 1 );
	y  = * ( float * ) &i;
	y  = y * ( threehalfs - ( x2 * y * y ) );
	return y;
}

transform::transform(int t,float xx, float yy, float zz)
{this->a=0.f,this->type = t,this->x=xx,this->y=yy,this->z=zz;}
transform::transform(int t,float aa,float xx, float yy, float zz)
{this->a=aa,this->time=0.f,this->type = t,this->x=xx,this->y=yy,this->z=zz;}
transform::transform(int t, int ti, float xx, float yy, float zz)
{this->time=ti,this->type=t,this->x=xx,this->y=yy,this->z=zz,this->a=0.f;}
transform::transform(int t, int ti, bool al)
{this->type=t, this->time=ti, this->align=al;}
void transform::do_transformation(){}
int transform::get_type(){return this->type;}
int transform::get_time(){return this->time;}
bool transform::is_align(){return this->align;}
float transform::get_angle(){return this->a;}
float transform::get_x(){return this->x;}
float transform::get_y(){return this->y;}
float transform::get_z(){return this->z;}
void transform::set_angle(float aa) { this->a = aa; }
void transform::add_point(float* p) {}

rotate::rotate(float a, float xx, float yy, float zz)
	: transform(TRANS_ROT, a, xx, yy, zz)
{
	this->previous_elapsed = this->init_time = 0.f;
}
int rotate::get_PE() { return this->previous_elapsed; }
void rotate::set_PE(float elapsed) { this->previous_elapsed = elapsed; }
int rotate::get_IT() { return this->init_time; }
void rotate::set_IT(float elapsed) { this->init_time = elapsed; }
void rotate::do_transformation()
{
	float rot_a = this->get_angle();
	int t = this->get_time();
	float current_time = glutGet(GLUT_ELAPSED_TIME) / 1000.f;
	int i_ti = this->get_IT();
	float elapsed = current_time - i_ti;

	if (t) {
		rot_a = (360.f * elapsed) / t;
		//printf("R:%f, %f\n", elapsed, rot_a);
		glRotatef(rot_a, this->get_x(),
			  this->get_y(), this->get_z());
		this->set_PE(current_time);
		if (elapsed >= t) {
			rot_a = 0.f;
			this->set_IT(current_time);
		}
	}
	else
		glRotatef(rot_a, this->get_x(),
			  this->get_y(), this->get_z());
	this->set_angle(rot_a);
}
rotate::rotate(int ti, float xx, float yy, float zz)
    : transform(TRANS_ROT, ti, xx, yy, zz)
{
	this->previous_elapsed = this->init_time = 0.f;
}

scale::scale(float xx, float yy, float zz)
	: transform(TRANS_SCA, xx, yy, zz){}
void scale::do_transformation()
{glScalef(this->get_x(),this->get_y(),this->get_z());}

translate_static::translate_static(float xx, float yy, float zz)
	: transform(TRANS_TRA, xx, yy, zz){}
void translate_static::do_transformation()
{glTranslatef(this->get_x(),this->get_y(),this->get_z());}


translate_catmull_rom::translate_catmull_rom(int time, bool align)
    : transform(TRANS_TRA, time, align)
{
	this->ps = new std::vector<float*>();
	this->previous_elapsed = this->init_time = 0.f;
}
int translate_catmull_rom::get_PE() { return this->previous_elapsed; }
void translate_catmull_rom::set_PE(float elapsed) { this->previous_elapsed = elapsed; }
int translate_catmull_rom::get_IT() { return this->init_time; }
void translate_catmull_rom::set_IT(float elapsed) { this->init_time = elapsed; }

void translate_catmull_rom::do_transformation()
{
	float pos[4] = {0.F};
	float vx[4] = {0.F};
	float vy[4] = {0.F};
	float vz[4] = {0.F};
	float mat[16];
	int t_seconds = this->get_time();
	float current_time = glutGet(GLUT_ELAPSED_TIME) / 1000.f;
	int i_ti = this->get_IT();
	float elapsed = current_time - i_ti;
	float gt;
	pos[3] = 1.F;
	vy[1] = 1.F;
	if (elapsed < t_seconds) {
		gt = elapsed / t_seconds;
		this->get_catmull_rom_global_point(gt, pos, vx);
		pos[0] /= pos[3];
		pos[1] /= pos[3];
		pos[2] /= pos[3];
		pos[3] /= pos[3]; /* w=1 projection */
		glBegin(GL_LINE_STRIP);
		for (int i = 0; i <= 20; ++i) {
			float t_ = i / 20.F;
			float pos_[4] = {1.F}, der[4] = {0.F};
			this->get_catmull_rom_global_point(t_, pos_, der);
			glVertex3f(pos_[0]/pos_[3], pos_[1]/pos_[3], pos_[2]/pos_[3]);
		}
		glEnd();
		glTranslatef(pos[0], pos[1], pos[2]);
		if (this->is_align()) {
			this->normalize(vx);
			this->cross(vx, vy, vz);
			this->normalize(vz);
			this->cross(vz, vx, vy);
			if (this->len(vy) == 1.F)
				this->normalize(vy);
			this->build_rot_matrix(vx,vy,vz,mat);
			glMultMatrixf(mat);
		}
	}
	else {
		// Reset elapsed time and initial time
		elapsed = 0.0f;
		i_ti = current_time;
		this->set_IT(i_ti);
	}
}
void translate_catmull_rom::get_catmull_rom_point(float t,
						  point p0,
						  point p1,
						  point p2,
						  point p3,
						  point pos,
						  point der)
{
	float P[4] = {0.F}, A[4] = {0.F}; int i;
	float m[4][4] = {{-0.5f,  1.5f, -1.5f,  0.5f},
			 {1.0f, -2.5f,  2.0f, -0.5f},
			 {-0.5f,  0.0f,  0.5f,  0.0f},
			 {0.0f,  1.0f,  0.0f,  0.0f}};
	float h = FLT_EPSILON*1e3F;

	for (i = 0; i < 4; i++) {
		P[0] = p0[i]; P[1] = p1[i]; P[2] = p2[i]; P[3] = p3[i];
		this->mult_mat_vec(&m[0][0], P, A);
		pos[i] = (t*t*t*A[0] + t*t*A[1] + t*A[2] + A[3]);
		der[i] = (((t+h)*(t+h)*(t+h)*A[0] + (t+h)*(t+h)*A[1] + (t+h)*A[2] + A[3]) -
			  ((t-h)*(t-h)*(t-h)*A[0] + (t-h)*(t-h)*A[1] + (t-h)*A[2] + A[3]))
			/ (2 * h);
		//der[i] = (3*t*t*A[0] + 2*t*t*A[1] + A[2]);
		/* While simple and a direct implementation of T * A (as learnt in lectures
		   ) was creating problems with interpolation, using the Central Difference Formula
		for approximating `dir' is preferable*/
	}
}

void translate_catmull_rom::get_catmull_rom_global_point(float gt,
							 point pos,
							 point der)
{
	float p0[4],p1[4],p2[4],p3[4];
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
void translate_catmull_rom::mult_mat_vec(point m, point v, point r)
{
	int j,k;
	for (j=0; j<4;++j) {
		r[j] = 0;
		for (k=0;k<4;++k) {
			r[j] += v[k] * m[j*4+k];
		}
	}
}
void translate_catmull_rom::normalize(point v)
{
	float l = Q_rsqrt(v[0]*v[0] + v[1] * v[1] + v[2] * v[2]);
	v[0] = v[0]*l;
	v[1] = v[1]*l;
	v[2] = v[2]*l;
}
void translate_catmull_rom::build_rot_matrix(point x, point y, point z,
                                             point r)
{
	r[0] = x[0]; r[1] = x[1]; r[2] = x[2]; r[3] = 0;
	r[4] = y[0]; r[5] = y[1]; r[6] = y[2]; r[7] = 0;
	r[8] = z[0]; r[9] = z[1]; r[10] = z[2]; r[11] = 0;
	r[12] = 0; r[13] = 0; r[14] = 0; r[15] = 1;
}
void translate_catmull_rom::cross(point v, point u, point r)
{
	r[0] = v[1]*u[2] - v[2]*u[1];
	r[1] = v[2]*u[0] - v[0]*u[2];
	r[2] = v[0]*u[1] - v[1]*u[0];
}
float translate_catmull_rom::len(point v)
{
	float res = sqrt(v[0]*v[0] + v[1]*v[1] + v[2]*v[2]);
	return res;
}
void translate_catmull_rom::add_point(float *p)
{
	float *p_ = new float[4];
	p_[0] = p[0];
	p_[1] = p[1];
	p_[2] = p[2];
	p_[3] = 1.F;
	this->ps->push_back(p_);
}
void translate_catmull_rom::get_point(float *p, int i)
{
	float *p_ = (*ps)[i];
	p[0] = p_[0];
	p[1] = p_[1];
	p[2] = p_[2];
	p[3] = p_[3];
}
