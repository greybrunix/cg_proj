#include "transforms.cpp.h"

static float Q_rsqrt(float number)
{
	long i;
	float x2, y;
	const float threehalfs = 1.5F;
	x2 = number * 0.5F;
	y  = number;
	i  = * ( long * ) &y;                       // evil floating point bit level hacking
	i  = 0x5f3759df - ( i >> 1 );               // what the fuck?
	y  = * ( float * ) &i;
	y  = y * ( threehalfs - ( x2 * y * y ) );   // 1st iteration
	// y  = y * ( threehalfs - ( x2 * y * y ) );   // 2nd iteration, this can be removed
	return y;
}



transform::transform(int t,float xx, float yy, float zz)
{this->a=0.f,this->type = t,this->x=xx,this->y=yy,this->z=zz;}
transform::transform(int t,float aa,float xx, float yy, float zz)
{this->a=aa,this->time=0.f,this->type = t,this->x=xx,this->y=yy,this->z=zz;}
transform::transform(int t, int ti, float xx, float yy, float zz)
{this->time=ti,this->type=t,this->x=xx,this->y=yy,this->z=zz,this->a=0.f;}
transform::transform(int t, int ti, bool al,std::vector<struct point> ps)
  {this->type=t,this->a=0.f,this->align=al, this->ps=ps;}
void transform::do_transformation(){}
int transform::get_type(){return this->type;}
int transform::get_time(){return this->time;}
bool transform::is_align(){return this->align;}
float transform::get_angle(){return this->a;}
float transform::get_x(){return this->x;}
float transform::get_y(){return this->y;}
float transform::get_z(){return this->z;}
std::vector<struct point> transform::get_points(){return this->ps;}
void transform::set_angle(float aa) { this->a = aa; }

rotate::rotate(float a, float xx, float yy, float zz)
	: transform(TRANS_ROT, a, xx, yy, zz)
{
	this->previous_elapsed = this->init_time = 0.f;
}
float rotate::get_PE() { return this->previous_elapsed; }
void rotate::set_PE(float elapsed) { this->previous_elapsed = elapsed; }
float rotate::get_IT() { return this->init_time; }
void rotate::set_IT(float elapsed) { this->init_time = elapsed; }
void rotate::do_transformation()
{
	float rot_a = this->get_angle();
	int t = this->get_time();
	float p_el = this->get_PE(); /* previous time */
	float i_ti = this->get_IT(); /* initial time */
	float elapsed;
	if (t) {
		elapsed = (glutGet(GLUT_ELAPSED_TIME) - p_el) / 1000.f;
		rot_a = (360.f * (elapsed-i_ti)) / (t);
		this->set_PE(elapsed);
		if (rot_a >= 360.f) {
			rot_a = 0.f;
			this->set_IT(elapsed);
		}
	}
	glRotatef(rot_a, this->get_x(),
		  this->get_y(), this->get_z());
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


translate_catmull_rom::translate_catmull_rom(int time, bool align,
					     std::vector<struct point> points)
	: transform(TRANS_TRA, time, align, points){}
void translate_catmull_rom::do_transformation()
{glTranslatef(this->get_x(),this->get_y(),this->get_z());}
void translate_catmull_rom::get_catmull_rom_point(float t,
						  float *p0, float *p1,
						  float *p2, float *p3,
						  float *pos, float *der)
{}
void translate_catmull_rom::get_catmull_rom_global_point(float gt,
							 float *pos,
							 float *der)
{}
void translate_catmull_rom::mult_vec_mat(float *m, float *v, float *r)
{}
void translate_catmull_rom::normalize(float *v)
{
	float l = Q_rsqrt(v[0]*v[0] + v[1] * v[1] + v[2] * v[2]);
	v[0] = v[0]*l;
	v[1] = v[1]*l;
	v[2] = v[2]*l;
}
void translate_catmull_rom::build_rot_matrix(float *x, float *y, float *z,
                                             float *r)
{
	r[0] = x[0]; r[1] = x[1]; r[2] = x[2]; r[3] = 0;
	r[4] = y[0]; r[5] = y[1]; r[6] = y[2]; r[7] = 0;
	r[8] = z[0]; r[9] = z[1]; r[10] = z[2]; r[11] = 0;
	r[12] = 0; r[13] = 0; r[14] = 0; r[15] = 1;
}
void translate_catmull_rom::cross(float *v, float *u, float *r)
{
	r[0] = v[1]*u[2] - v[2]*u[1];
	r[1] = v[2]*u[0] - v[0]*u[2];
	r[2] = v[0]*u[1] - v[1]*u[0];
}
float translate_catmull_rom::len(float *v)
{
	float res = sqrt(v[0]*v[0] + v[1]*v[1] + v[2]*v[2]);
	return res;
}

