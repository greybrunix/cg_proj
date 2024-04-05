#include "transforms.cpp.h"

transform::transform(int t,float xx, float yy, float zz)
{this->a=0.f,this->type = t,this->x=xx,this->y=yy,this->z=zz;}
transform::transform(int t,float aa,float xx, float yy, float zz)
{this->a=aa,this->type = t,this->x=xx,this->y=yy,this->z=zz;}
void transform::do_transformation(){}
int transform::get_type(){return this->type;}
float transform::get_angle(){return this->a;}
float transform::get_x(){return this->x;}
float transform::get_y(){return this->y;}
float transform::get_z(){return this->z;}

rotate::rotate(float a,float xx, float yy, float zz)
	: transform(TRANS_ROT, a, xx, yy, zz){}
void rotate::do_transformation()
{glRotatef(this->get_angle(),this->get_x(),this->get_y(),this->get_z());}

scale::scale(float xx, float yy, float zz)
	:transform(TRANS_SCA, xx, yy, zz){}
void scale::do_transformation()
{glScalef(this->get_x(),this->get_y(),this->get_z());}

translate::translate(float xx, float yy, float zz)
	: transform(TRANS_TRA, xx, yy, zz){}
void translate::do_transformation()
{glTranslatef(this->get_x(),this->get_y(),this->get_z());}
