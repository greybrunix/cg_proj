#ifndef __VFC__
#define __VFC__
#include "vectors.hpp"
#include <cmath>
class Transform {
    int x;
};
typedef struct camera {
	float dist;
	float alfa;
	float beta;
	float ratio;
	triple pos;
	triple lookAt;
	triple up; /* 0 1 0 */
	triple proj; /* 60 1 1000*/
} camera;

class Plane  {
public:
	triple normal,point;
	float d;
	Plane( triple& v1,  triple& v2,  triple& v3)
	{
		set3Points(v1,v2,v3);
	}
	Plane(void){}
	~Plane(){}

	void set3Points( triple& v1,  triple& v2,  triple& v3)
	{
		triple aux1, aux2;

		sub(v1, v2, aux1);
		sub(v3,v2, aux2);

		cross(aux2,aux1,normal);

		normalize(normal);
		point.copy(v2);
		d = -dot(normal,point);
	}
	void setNormalAndPoint(triple& normal, triple& point)
	{
		this->normal.copy(normal);
		normalize(this->normal);
		d = -(dot(this->normal, point));

	}
	void setCoefficients(float a, float b, float c, float d)
	{
		// set the normal vector
		normal.x=a,normal.y=b,normal.z=c;
		//compute the lenght of the vector
		float l = len(normal);
		// normalize the vector
		normalize(this->normal);
		// and divide d by th length as well
		this->d = d/l;
	}
	float distance(triple& p)
	{
		return (d + dot(normal, p));
	}

};
class AABox {
public:
	triple corner;
	float x,y,z;
	AABox( triple&corner, float x, float y, float z)
	{
		setBox(corner,x,y,z);
	}
	AABox(void)
	{
		corner.x = 0; corner.y = 0; corner.z = 0;

		x = 1.0f;
		y = 1.0f;
		z = 1.0f;
	}
	~AABox(){}

	void setBox( triple&corner, float x, float y, float z)
	{
		this->corner.copy(corner);

		if (x < 0.F) {
			x = -x;
			this->corner.x -= x;
		}
		if (y < 0.F) {
			y = -y;
			this->corner.y -= y;
			}
		if (z < 0.F) {
			z = -z;
			this->corner.z -= z;
		}
		this->x = x;
		this->y = y;
		this->z = z;

	}

	// for use in frustum computations
	triple getVertexP(triple&normal)
	{
		triple res = corner;
		if (normal.x > 0)
			res.x += x;

		if (normal.y > 0)
			res.y += y;

		if (normal.z > 0)
			res.z += z;

		return(res);
	}
	triple getVertexN(triple&normal)
	{
		triple res = corner;

		if (normal.x < 0)
			res.x += x;

		if (normal.y < 0)
			res.y += y;

		if (normal.z < 0)
			res.z += z;

		return(res);
	}
};


struct Frustum {
public:
	enum {
		top = 0,
		bot,
		left,
		right,
		near,
		far
	};
	enum {OUTSIDE, INTERSECT, INSIDE};

	Plane pl[6];

	Frustum(camera cam) {
		const float Hfar = 2.F * tanf((cam.proj.x*.5F*M_PI)/180.F) * cam.proj.z,
				Wfar = Hfar * cam.ratio,
				Hnear = 2.F * tanf((cam.proj.x*.5F*M_PI)/180.F) * cam.proj.y,
				Wnear = Hnear * cam.ratio;
		triple HNear_up, WNear_r;
		triple d, up = cam.up, r;
		triple HFar_up,	WFar_r;
		triple fvc, nvc;
		triple fc, nc;
		triple v1,v2,v3;
		triple fbr, ftr, fbl,ftl,
			nbr, ntr, nbl, ntl;


		/* d, r and real up*/
		sub(cam.lookAt, cam.pos, d);
		normalize(d);
		cross(up, d, r);
		normalize(r);
		cross(d,r, up);

		scalar(up, Hfar*0.5F, HFar_up), scalar(r,Wfar*0.5F, WFar_r);
		scalar(d,cam.proj.z, fvc);		
		/* Far Frustum Points*/
		add(cam.pos, fvc, fc);
		sub(fc,HFar_up, fbr), add(fbr,WFar_r, fbr);
		add(fc,HFar_up,ftr),add(ftr,WFar_r,ftr);
		sub(fc, HFar_up, fbl),sub(fbl,WFar_r,fbl);
		add(fc,HFar_up,ftl),sub(ftl,WFar_r,ftl);

		scalar(up, Hnear*0.5F, HNear_up), scalar(r,Wnear*0.5F, WNear_r);
		scalar(d,cam.proj.y, nvc);
		/* Near Frustum Points*/
		add(cam.pos, nvc, nc);
		sub(nc,HNear_up, nbr), add(nbr,WNear_r, nbr);
		add(nc,HNear_up,ntr),add(ntr,WNear_r,ntr);
		sub(nc, HNear_up, nbl),sub(nbl,WNear_r,nbl);
		add(nc,HNear_up,ntl),sub(ntl,WNear_r,ntl);

		pl[far] = Plane(ftr,ftl,fbl);
		pl[near] = Plane(ntl,ntr,nbr);
		pl[top] = Plane(ntr,ntl,ftl);
		pl[bot] = Plane(nbl,nbr,fbr);
		pl[left] = Plane(ntl,nbl,fbl);
		pl[right] = Plane(nbr,ntr,fbr);
	}
	bool BoxInFrustum(AABox& b)
	{

		triple tmp1, tmp2;
		int result = INSIDE;
		for(int i=0; i < 6; i++) {
			tmp1 = b.getVertexP(pl[i].normal);
			tmp2 = b.getVertexN(pl[i].normal);
			if (pl[i].distance(tmp1) < 0)
				return OUTSIDE;
			else if (pl[i].distance(tmp2) < 0)
				result =  INTERSECT;
		}
		return(result);

 	}

};

#endif
