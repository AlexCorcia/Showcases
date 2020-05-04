//---------------------------------------------------------------------------
/**
* @file		shapes.h
* @date 	1/18/2019
* @author	Alexandre Corcia Aguilera
* @par		Login: alexandre.corcia
* @par		Course: CS350
* @par		Assignment #1
* @brief 	All shapes logic
*
* Hours spent on this assignment: 8h
*
* Specific portions that gave you the most trouble:
**/
//---------------------------------------------------------------------------
#pragma once
#include "pch.h"

/**
 *
 */
struct triangle {
	std::array<vec3, 3> points;
};

static_assert(std::is_pod<triangle>(), "NOT POD");

/**
 *
 */
struct ray {
	vec3 start;
	vec3 dir;

	ray() = default;
	ray(const vec3& start, const vec3& dir);
};

static_assert(std::is_pod<ray>(), "NOT POD");

/**
 *
 */
struct plane {
	vec3  normal;
	float dot_result;

	plane() = default;
	plane(const triangle& triangle);
	plane(const vec3& normal, const vec3& point);

	void set(const triangle& triangle);
	void set(const vec3& norm, const vec3& point);
	vec3 get_point() const;
};

static_assert(std::is_pod<plane>(), "NOT POD");

/**
 *
 */
struct sphere {
	vec3  center;
	float radius;

	sphere() = default;
	sphere(const vec3& center, float radius);
};

static_assert(std::is_pod<sphere>(), "NOT POD");

/**
 *
 */
struct aabb {
	vec3 min_point;
	vec3 max_point;

	aabb() = default;
	aabb(const vec3& min_point, const vec3& max_point);
	vec3 get_center() const;
};

static_assert(std::is_pod<aabb>(), "NOT POD");

/**
 *
 */
struct frustum {
	// In this order: left, right, top, bottom, near, far
	std::array<plane, 6> planes;

	frustum() = default;
	frustum(std::array<plane, 6> planes);
	frustum(const mat4& vp);
	const plane& get_left() const {return planes[0];}
	const plane& get_right() const {return planes[1];}
	const plane& get_top() const {return planes[2];}
	const plane& get_bottom() const {return planes[3];}
	const plane& get_near() const {return planes[4];}
	const plane& get_far() const {return planes[5];}
	std::array<vec3, 8> get_points() const;
};

static_assert(std::is_pod<frustum>(), "NOT POD");


