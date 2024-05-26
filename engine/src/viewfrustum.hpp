#include "vectors.hpp"

typedef struct camera {
	float dist;
	float alfa;
	float beta;
	float ratio;
	struct triple pos;
	struct triple lookAt;
	struct triple up; /* 0 1 0 */
	struct triple proj; /* 60 1 1000*/
} camera;


struct Plane {
	float normal[3] = {0.F,0.F,0.F};
	float distance = 0.F;

	Plane() = default;
	Plane(const float* p1, const float* norm)
	{
		normalize((float*)norm);
		this->normal[0] = norm[0];
		this->normal[0] = norm[1];
		this->normal[0] = norm[2];
		this->distance = dot(normal,p1);
	}
	float getSignedDistanceToPlane(const float* point) const
	{
		return dot(normal, point) - this->distance;
	}
};

struct Frustum {
	Plane top,bot;
	Plane left,right;
	Plane near,far;
};

Frustum fromCamera(const camera cam);
bool isInFrustum(const Frustum frus, const float box[8]);
