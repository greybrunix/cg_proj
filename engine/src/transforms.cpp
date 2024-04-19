#include "transforms.cpp.h"

transform::transform(int t,float xx, float yy, float zz)
{this->a=0.f,this->type = t,this->x=xx,this->y=yy,this->z=zz;}
transform::transform(int t,float aa,float xx, float yy, float zz)
{this->a=aa,this->type = t,this->x=xx,this->y=yy,this->z=zz;}
transform::transform(int t, int ti, float xx, float yy, float zz)
{this->time=ti,this->type=t,this->x=xx,this->y=yy,this->z=zz,this->a=0.f;}
transform::transform(int t, int ti, bool al,std::vector<struct point> ps)
{this->type=t,this->a=0.f,this->align=al, this->points=ps;}
void transform::do_transformation(){}
int transform::get_type(){return this->type;}
int transform::get_time(){return this->time;}
bool transform::is_align(){return this->align;}
float transform::get_angle(){return this->a;}
float transform::get_x(){return this->x;}
float transform::get_y(){return this->y;}
float transform::get_z(){return this->z;}

rotate_angle::rotate_angle(float a,float xx, float yy, float zz)
	: transform(TRANS_ROT, a, xx, yy, zz){}
void rotate_angle::do_transformation()
{glRotatef(this->get_angle(),this->get_x(),this->get_y(),this->get_z());}
rotate_time::rotate_time(int ti, float xx, float yy, float zz)
	: transform(TRANS_ROT, ti, xx, yy, zz){}
void rotate_time::do_transformation()
{
	int elapsed = 0;
	int t_time = this->get_time()*1000000;
	float rps  = 360.f/t_time;
	(void)glutGet(GLUT_ELAPSED_TIME);
	while (elapsed < t_time) {
		glRotatef(rps, this->get_x(), this->get_y(), this->get_z());
		elapsed = glutGet(GLUT_ELAPSED_TIME);
	}
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
