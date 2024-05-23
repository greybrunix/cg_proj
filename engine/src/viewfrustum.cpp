#include "viewfrustum.hpp"

Frustum fromCamera(const camera cam)
{
	Frustum frustum;
	const float Hfar = 2.F * tanf((cam.proj[0]*.5F*M_PI)/180.F) * cam.proj[2];
	const float Wfar = Hfar * cam.ratio;
	const float Hnear = 2.F * tanf((cam.proj[0]*.5F*M_PI)/180.F) * cam.proj[1];
	const float Wnear = Hnear * cam.ratio;
	float d[3] = {cam.lookAt[0] - cam.pos[0],
		      cam.lookAt[1] - cam.pos[1],
		      cam.lookAt[2] - cam.pos[2]};
	normalize(d);
	float r[3];
	cross(cam.up, d, r);
	normalize(r);
	float up[3];
	cross(d,r, up);
	const float HFar_up[3] = {up[0]*Hfar/2.F, up[1]*Hfar/2.F, up[2]*Hfar/2.F},
		WFar_r[3] =  {r[0]*Wfar/2.F, r[1]*Wfar/2.F, r[2]*Wfar/2.F};
	const float fvc[3] = {d[0]*cam.proj[2], d[1]*cam.proj[2], d[2]*cam.proj[2]};
	const float nvc[3] = {d[0]*cam.proj[1], d[1]*cam.proj[1], d[2]*cam.proj[1]};
	const float fc[3] = {cam.pos[0] + fvc[0],
			     cam.pos[1] + fvc[1],
			     cam.pos[2] + fvc[2]},
		nc[3] = {cam.pos[0] + nvc[0],
			     cam.pos[1] + nvc[1],
			     cam.pos[2] + nvc[2]};
	const float fbr[3] = {fc[0] - HFar_up[0] + WFar_r[0],
			fc[1] - HFar_up[1] + WFar_r[1],
			fc[2] - HFar_up[2] + WFar_r[2]},
	      ftr[3] = {fc[0] + HFar_up[0] + WFar_r[0]},
			fc[1] + HFar_up[1] + WFar_r[1],
			fc[2] + HFar_up[2] + WFar_r[2]},
	      fbl[3] = {fc[0] - HFar_up[0] - WFar_r[0]},
			fc[1] - HFar_up[1] - WFar_r[1],
			fc[2] - HFar_up[2] - WFar_r[2]},
	      ftl[3] = {fc[0] + HFar_up[0] - WFar_r[0]};
			fc[1] + HFar_up[1] - WFar_r[1],
			fc[2] + HFar_up[2] - WFar_r[2]},
	const float HNear_up[3] = {up[0]*Hnear/2.F, up[1]*Hnear/2.F, up[2]*Hnear/2.F},
		WNear_r[3] =  {r[0]*Wnear/2.F, r[1]*Wnear/2.F, r[2]*Wnear/2.F};
	const float nbr[3] = {fc[0] - HNear_up[0] + WNear_r[0],
			fc[1] - HNear_up[1] + WNear_r[1],
			fc[2] - HNear_up[2] + WNear_r[2]},
	      ntr[3] = {fc[0] + HNear_up[0] + WNear_r[0]},
			fc[1] + HNear_up[1] + WNear_r[1],
			fc[2] + HNear_up[2] + WNear_r[2]},
	      nbl[3] = {fc[0] - HNear_up[0] - WNear_r[0]},
			fc[1] - HNear_up[1] - WNear_r[1],
			fc[2] - HNear_up[2] - WNear_r[2]},
	      ntl[3] = {fc[0] + HNear_up[0] - WNear_r[0]};
			fc[1] + HNear_up[1] - WNear_r[1],
			fc[2] + HNear_up[2] - WNear_r[2]},
	float v1[3], v2[3];
	v1[0] = ftr[0] - ftl[0]; v1[1] = ftr[1] - ftl[1]; v1[2] = ftr[2] - ftl[2];
	v2[0] = fbr[0] - fbl[0]; v2[1] = fbr[1] - fbl[1]; v2[2] = fbr[2] - fbl[2];
	cross(v1, v2, frustum.far);
	
	v1[0] = ntr[0] - ntl[0]; v1[1] = ntr[1] - ntl[1]; v1[2] = ntr[2] - ntl[2];
	v2[0] = nbr[0] - nbl[0]; v2[1] = nbr[1] - nbl[1]; v2[2] = nbr[2] - nbl[2];
	cross(v1, v2, frustum.near);
	
	v1[0] = ftl[0] - ntl[0]; v1[1] = ftl[1] - ntl[1]; v1[2] = ftl[2] - ntl[2];
	v2[0] = ftr[0] - ntr[0]; v2[1] = ftr[1] - ntr[1]; v2[2] = ftr[2] - ntr[2];
	cross(v1, v2, frustum.top);

	v1[0] = fbl[0] - nbl[0]; v1[1] = fbl[1] - nbl[1]; v1[2] = fbl[2] - nbl[2];
	v2[0] = fbr[0] - nbr[0]; v2[1] = fbr[1] - nbr[1]; v2[2] = fbr[2] - nbr[2];
	cross(v1, v2, frustum.bot);
	
	v1[0] = ftl[0] - ntl[0]; v1[1] = ftl[1] - ntl[1]; v1[2] = ftl[2] - ntl[2];
	v2[0] = fbl[0] - nbl[0]; v2[1] = fbl[1] - nbl[1]; v2[2] = fbl[2] - nbl[2];
	cross(v1, v2, frustum.left);
	
	v1[0] = ftr[0] - ntr[0]; v1[1] = ftr[1] - ntr[1]; v1[2] = ftr[2] - ntr[2];
	v2[0] = fbr[0] - nbr[0]; v2[1] = fbr[1] - nbr[1]; v2[2] = fbr[2] - nbr[2];
	cross(v1, v2, frustum.right);
	return frustum;
}
