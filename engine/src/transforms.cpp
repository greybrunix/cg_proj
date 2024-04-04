#include "transforms.cpp.h"

transform::transform(struct sum_trans t, union args a){this->type=t,this->args=a;}
void transform::do_transformation()
{
	switch (this->type.type) {
	case 0:
		this->type.function.rot(this->args.rot.angle,this->args.rot.x,
			   this->args.rot.y,this->args.rot.z);
		glRotatef(this->args.rot.angle, this->args.rot.x,
			  this->args.rot.y,this->args.rot.z);
		break;
	case 1:
		this->type.function.trans_scal(this->args.trans_scal.x,this->args.rot.y,
			      this->args.trans_scal.z);
		glScalef(this->args.trans_scal.x, this->args.trans_scal.y, this->args.trans_scal.z);
		break;
	case 2:
		this->type.function.trans_scal(this->args.trans_scal.x,this->args.rot.y,
			      this->args.trans_scal.z);
		glTranslatef(this->args.trans_scal.x, this->args.trans_scal.y, this->args.trans_scal.z);
	}
}
float transform::get_angle()
{
	return (!this->type.type? this->args.rot.angle :
		0.f);
}
float transform::get_x()
{
	return (!this->type.type? this->args.rot.x :
		this->args.trans_scal.x);
}
float transform::get_y()
{
	return (!this->type.type? this->args.rot.y :
		this->args.trans_scal.y);
}
float transform::get_z()
{
	return (!this->type.type? this->args.rot.z :
		this->args.trans_scal.z);
}

rotate::rotate(float a,float xx, float yy, float zz)
	:transform({.type=0, .function={.rot=glRotatef}},
		   {.rot={.angle=a,.x=xx,.y=yy,.z=zz}}) {}
scale::scale(float xx, float yy, float zz)
	:transform({.type=1, .function={.trans_scal=glScalef}},
		   {.trans_scal={.x=xx,.y=yy,.z=zz}}) {}
translate::translate(float xx, float yy, float zz)
	:transform({.type=2, .function={.trans_scal=glTranslatef}},
		   {.trans_scal={.x=xx,.y=yy,.z=zz}}) {}
