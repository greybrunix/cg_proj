#ifndef __VFC__
#define __VFC__
#include "vectors.hpp"
#include <array>
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

struct Plane {
	triple normal = {0.F,0.F,0.F};
	float distance = 0.F;

	Plane() = default;
	Plane(const triple& p1, const triple& norm)
	{
		triple tmp = (triple) norm;
		normalize(tmp);
		this->normal.x = tmp.x;
		this->normal.y = tmp.y;
		this->normal.z = tmp.z;
		this->distance = dot(this->normal,p1);
	}
	float getSignedDistanceToPlane(const triple& point) const
	{
		return dot(normal, point) - this->distance;
	}
};

struct Frustum {
	Plane top,bot;
	Plane left,right;
	Plane near,far;
};
struct BoundingVolume
{
	virtual bool isOnFrustum(const Frustum& camFrustum, const Transform& transform) const = 0;

	virtual bool isOnOrForwardPlane(const Plane& plane) const = 0;

	bool isOnFrustum(const Frustum& camFrustum) const
	{
		return (isOnOrForwardPlane(camFrustum.left) &&
			isOnOrForwardPlane(camFrustum.right) &&
			isOnOrForwardPlane(camFrustum.top) &&
			isOnOrForwardPlane(camFrustum.bot) &&
			isOnOrForwardPlane(camFrustum.near) &&
			isOnOrForwardPlane(camFrustum.far));
	};
};
struct AABB : public BoundingVolume
{
	triple center { 0.f, 0.f, 0.f };
	triple extents{ 0.f, 0.f, 0.f };

	AABB(const triple& min, const triple&  max)
		: BoundingVolume{}
	{
		add(max,min,this->center);
		scalar(this->center,0.5F,this->center);
		sub(max,this->center,this->extents);
	}

	AABB(const triple& inCenter, float iI, float iJ, float iK)
		: BoundingVolume{}, extents{ iI, iJ, iK }
	{
		this->center.x = inCenter.x;
		this->center.y = inCenter.y;
		this->center.z = inCenter.z;
	}

	std::array<triple, 8> getVertice() const
	{
		std::array<triple, 8> vertice;
		vertice[0] = { center.x - extents.x, center.y - extents.y, center.z - extents.z };
		vertice[1] = { center.x + extents.x, center.y - extents.y, center.z - extents.z };
		vertice[2] = { center.x - extents.x, center.y + extents.y, center.z - extents.z };
		vertice[3] = { center.x + extents.x, center.y + extents.y, center.z - extents.z };
		vertice[4] = { center.x - extents.x, center.y - extents.y, center.z + extents.z };
		vertice[5] = { center.x + extents.x, center.y - extents.y, center.z + extents.z };
		vertice[6] = { center.x - extents.x, center.y + extents.y, center.z + extents.z };
		vertice[7] = { center.x + extents.x, center.y + extents.y, center.z + extents.z };
		return vertice;
	}

	//see https://gdbooks.gitbooks.io/3dcollisions/content/Chapter2/static_aabb_plane.html
	bool isOnOrForwardPlane(const Plane& plane) const final
	{
		// Compute the projection interval radius of b onto L(t) = b.c + t * p.n
		const float r = extents.x * std::abs(plane.normal.x) +
			extents.y * std::abs(plane.normal.y) +
			extents.z * std::abs(plane.normal.z);

		return -r <= plane.getSignedDistanceToPlane(center);
	}

	bool isOnFrustum(const Frustum& camFrustum, const Transform& transform) const final
	{
		//Get global scale thanks to our transform
		const triple globalCenter{ transform.getModelMatrix() * glm::vec4(center, 1.f) };

		// Scaled orientation
		const glm::vec3 right = transform.getRight() * extents.x;
		const glm::vec3 up = transform.getUp() * extents.y;
		const glm::vec3 forward = transform.getForward() * extents.z;

		const float newIi = std::abs(glm::dot(glm::vec3{ 1.f, 0.f, 0.f }, right)) +
			std::abs(glm::dot(glm::vec3{ 1.f, 0.f, 0.f }, up)) +
			std::abs(glm::dot(glm::vec3{ 1.f, 0.f, 0.f }, forward));

		const float newIj = std::abs(glm::dot(glm::vec3{ 0.f, 1.f, 0.f }, right)) +
			std::abs(glm::dot(glm::vec3{ 0.f, 1.f, 0.f }, up)) +
			std::abs(glm::dot(glm::vec3{ 0.f, 1.f, 0.f }, forward));

		const float newIk = std::abs(glm::dot(glm::vec3{ 0.f, 0.f, 1.f }, right)) +
			std::abs(glm::dot(glm::vec3{ 0.f, 0.f, 1.f }, up)) +
			std::abs(glm::dot(glm::vec3{ 0.f, 0.f, 1.f }, forward));

		const AABB globalAABB(globalCenter, newIi, newIj, newIk);

		return (globalAABB.isOnOrForwardPlane(camFrustum.leftFace) &&
			globalAABB.isOnOrForwardPlane(camFrustum.rightFace) &&
			globalAABB.isOnOrForwardPlane(camFrustum.topFace) &&
			globalAABB.isOnOrForwardPlane(camFrustum.bottomFace) &&
			globalAABB.isOnOrForwardPlane(camFrustum.nearFace) &&
			globalAABB.isOnOrForwardPlane(camFrustum.farFace));
	};
};


#endif
