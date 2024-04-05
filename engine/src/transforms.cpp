#include "transforms.cpp.h"

transform::transform(int t){this->type = t;}
void transform::do_transformation(){}
int transform::get_type(){return this->type;}

rotate::rotate(float a,float xx, float yy, float zz)
	: transform(TRANS_ROT)
{this->angle=a,this->x=xx,this->y=yy,this->z=zz;}
void rotate::do_transformation()
{glRotatef(this->angle,this->x,this->y,this->z);}

scale::scale(float xx, float yy, float zz)
	:transform(TRANS_SCA)
{this->x=xx,this->y=yy,this->z=zz;}
void scale::do_transformation()
{glScalef(this->x,this->y,this->z);}

translate::translate(float xx, float yy, float zz)
	: transform(TRANS_TRA)
{this->x=xx,this->y=yy,this->z=zz;}
void translate::do_transformation()
{glTranslatef(this->x,this->y,this->z);}
