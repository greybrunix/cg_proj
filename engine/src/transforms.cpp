#include "transforms.cpp.h"

transform::transform(union trans_type t, union args a)
{
	this->type = t;
	this->args = a;
}
void transform::do_transformation()
{
	switch (sizeof (this->args)) {
	case (4*sizeof(float)):
		this->type.rot(this->args.rot.angle,this->args.rot.x,
			   this->args.rot.y,this->args.rot.z);
		break;
	case (3*sizeof(float)):
		this->type.trans_scal(this->args.rot.x,this->args.rot.y,
				      this->args.rot.z);
		break;
	}
}

rotate::rotate(float a,float xx, float yy, float zz)
	:transform({.rot=glRotatef},
		   {.rot={.angle=a,.x=xx,.y=yy,.z=zz}})
{
}
scale::scale(float xx, float yy, float zz)
	:transform({.trans_scal=glScalef},
		   {.trans_scal={.x=xx,.y=yy,.z=zz}})
{
}
translate::translate(float xx, float yy, float zz)
	:transform({.trans_scal=glTranslatef},
		   {.trans_scal={.x=xx,.y=yy,.z=zz}})
{
}
