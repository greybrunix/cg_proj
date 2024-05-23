#include "transforms.hpp"

typedef struct Frustum
{
	float top_face[3],bot_face[3];
	float left_face[3],right_face[3];
	float near[3],far[3];
} Frustum;

Frustum fromCamera(const camera cam);
bool isInFrustum(const Frustum frus, const float box[8]);
