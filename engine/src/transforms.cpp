#include "transforms.cpp.h"
#include <tinyxml2.h>

transform::transform(int t){this->type = t;}
void transform::do_transformation(){}

rotate::rotate(float a,float xx, float yy, float zz)
	: transform(0)
{this->angle=a,this->x=xx,this->y=yy,this->z=zz;}
void rotate::do_transformation()
{glRotatef(this->angle,this->x,this->y,this->z);}

scale::scale(float xx, float yy, float zz)
	:transform(1)
{this->x=xx,this->y=yy,this->z=zz;}
void scale::do_transformation()
{glScalef(this->x,this->y,this->z);}

translate::translate(float xx, float yy, float zz)
	: transform(2)
{this->x=xx,this->y=yy,this->z=zz;}
void translate::do_transformation()
{glTranslatef(this->x,this->y,this->z);}
