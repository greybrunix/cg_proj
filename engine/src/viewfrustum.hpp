#ifndef __VFC__
#define __VFC__
#include <GL/gl.h>
#include <GL/glut.h>
#include "vectors.hpp"
#include <cstdio>
#include <cmath>
#define TO_RADIANS(x) x*M_PI/180.F
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
		triple aux1, aux2;

		sub(v1, v2, aux1);
		sub(v3,v2, aux2);

		cross(aux2,aux1,normal);

		normalize(normal);
		point.copy(v2);
		d = -dot(normal,point);
	}
	Plane(void){}
	~Plane(){}

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
	triple center;
	triple extents;
	AABox( triple&center, float x, float y, float z)
	{
		setBox(center,x,y,z);
	}
	AABox(void)
	{
		center.x = 0; center.y = 0; center.z = 0;

		extents.x = 1.0f;
		extents.y = 1.0f;
		extents.z = 1.0f;
	}
	~AABox(){}

	void setBox( triple&center,float x, float y, float z)
	{
		this->center.copy(center);

		extents.x = x;
		extents.y = y;
		extents.z = z;

	}

	triple getVertexP(triple&normal)
	{
		triple res;
		res.x = (normal.x < 0) ? (center.x - extents.x) : (center.x + extents.x);
		res.y = (normal.y < 0) ? (center.y - extents.y) : (center.y + extents.y);
		res.z = (normal.z < 0) ? (center.z - extents.z) : (center.z + extents.z);

		return(res);
	}
	triple getVertexN(triple&normal)
	{
		triple res = center;
		res.x = (normal.x > 0) ? (center.x - extents.x) : (center.x + extents.x);
		res.y = (normal.y > 0) ? (center.y - extents.y) : (center.y + extents.y);
		res.z = (normal.z > 0) ? (center.z - extents.z) : (center.z + extents.z);

		return(res);
	}
	void applyMVP(float *matrix) {
    // Transform center
    triple newCenter;
		float center_v[4] = {center.x,center.y,center.z, 1.F};
		float new_center[4];
		center.x = matrix[0] * center.x + matrix[1] * center.y +
			matrix[2] * center.z + matrix[3];
		center.y = matrix[4] * center.x + matrix[5] * center.y +
			matrix[6] * center.z + matrix[7];
		center.z = matrix[8] * center.x + matrix[9] * center.y +
			matrix[10] * center.z + matrix[11];

    
    // Transform extents
    triple newExtents;
		float extent_v[4] = {extents.x, extents.y, extents.z, 0.F};
		float new_extents[4];
		extents.x = matrix[0] * extents.x + matrix[1] * extents.y +
			matrix[2] * extents.z;
		extents.y = matrix[4] * extents.x + matrix[5] * extents.y +
			matrix[6] * extents.z;
		extents.z = matrix[8] * extents.x + matrix[9] * extents.y +
			matrix[10] * extents.z;
    
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

	Frustum() {}
	Frustum(camera cam) {
		const float Hfar = 2.F * tanf(TO_RADIANS(cam.proj.x*.5F)) * cam.proj.z,
				Wfar = Hfar * cam.ratio,
				Hnear = 2.F * tanf(TO_RADIANS(cam.proj.x*.5F)) * cam.proj.y,
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
		add(fc,HFar_up,ftl),sub(ftl,WFar_r,ftl);
		add(fc,HFar_up,ftr),add(ftr,WFar_r,ftr);
		sub(fc, HFar_up, fbl),sub(fbl,WFar_r,fbl);
		sub(fc,HFar_up, fbr), add(fbr,WFar_r, fbr);

		scalar(up, Hnear*0.5F, HNear_up), scalar(r,Wnear*0.5F, WNear_r);
		scalar(d,cam.proj.y, nvc);
		/* Near Frustum Points*/
		add(cam.pos, nvc, nc);
		add(nc,HNear_up,ntl),sub(ntl,WNear_r,ntl);
		add(nc,HNear_up,ntr),add(ntr,WNear_r,ntr);
		sub(nc, HNear_up, nbl),sub(nbl,WNear_r,nbl);
		sub(nc,HNear_up, nbr), add(nbr,WNear_r, nbr);

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
		bool res= INSIDE;
		for(int i=0; i < 6; i++) {
			tmp1 = b.getVertexP(pl[i].normal);
			tmp2 = b.getVertexN(pl[i].normal);
			if (pl[i].distance(tmp1) > 0)
				return  OUTSIDE;
			else if (pl[i].distance(tmp2) < 0)
				return INTERSECT;
		}
		return res;

 	}

};

#endif
