//---------------------------------------------------------------------------
/**
* @file		shapes.cpp
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
#include "pch.h"
#include "geometry.h"
#include "shapes.h"

/**
* @brief Creation of a Ray
* @param start
* @param dir
* @return
*/
ray::ray(const vec3 &start, const vec3 &dir) :
        start(start), dir(dir) {
}

/**
* @brief Creation of a plane from a triangle
* @param triangle
* @return
*/
plane::plane(const triangle &triangle) 
{
	set(triangle);
}

/**
* @brief Creation of a plane from a point and a normal
* @param normal
* @param point
* @return
*/
plane::plane(const vec3 &normal, const vec3 &point) 
{
	this->normal = glm::normalize(normal);
	this->dot_result = glm::dot(normal, point);
}

/**
* @brief Setting a plane with a triangle.
* @param triangle
* @return
*/
void plane::set(const triangle &triangle) 
{
	vec3 vec_1 = triangle.points[0] - triangle.points[1];
	vec3 vec_2 = triangle.points[1] - triangle.points[2];

	this->normal = glm::normalize(glm::cross(vec_1, vec_2));
	this->dot_result = glm::dot(this->normal, triangle.points[0]);
}

/**
* @brief Setting a plane from a point and a normal
* @param normal
* @param point
* @return
*/
void plane::set(const vec3 &norm, const vec3 &point) 
{
	this->normal = glm::normalize(norm);
	this->dot_result = glm::dot(normal, point);
}

/**
* @brief Gets a point from a plane
* @return vec3
*		  point from the plane.
*/
vec3 plane::get_point() const 
{
    return normal * dot_result;
}


/**
* @brief Creation of a sphere from a point and radius
* @param center
* @param radius
* @return
*/
sphere::sphere(const vec3 &center, float radius) :
        center(center),
        radius(radius) {
}

/**
* @brief Creation of a aabb from a 2 vectors min and max
* @param min_point
* @param max_point
* @return
*/
aabb::aabb(const vec3 &min_point, const vec3 &max_point) :
        min_point(min_point),
        max_point(max_point) {
}

/**
* @brief returns the center of the AABB
* @return vec3
*		  returns the center of the aabb
*/
vec3 aabb::get_center() const {
    return (min_point + max_point) * 0.5f;
}

/**
* @brief Creation of a frustum with 6 planes
* @param planes
* @return 
*/
frustum::frustum(std::array<plane, 6> planes) :
        planes(planes) {
}

/**
* @brief Creation of a frustrum with a matrix
* @param vp
* @return
*/
frustum::frustum(const mat4 &vp) 
{
	// In this order: left, right, top, bottom, near, far

	mat4 inverse = glm::inverse(vp);

	vec4 n_b_l = inverse * vec4(-1.0, -1.0, -1.0, 1.0); n_b_l /= n_b_l.w; 
	vec4 n_b_r = inverse * vec4(1.0, -1.0, -1.0, 1.0);  n_b_r /= n_b_r.w;
	vec4 n_t_l = inverse * vec4(-1.0, 1.0, -1.0, 1.0);  n_t_l /= n_t_l.w;
	vec4 n_t_r = inverse * vec4(1.0, 1.0, -1.0, 1.0);	n_t_r /= n_t_r.w;

	vec4 f_b_l = inverse * vec4(-1.0, -1.0, 1.0, 1.0);	f_b_l /= f_b_l.w;
	vec4 f_b_r = inverse * vec4(1.0, -1.0, 1.0, 1.0);	f_b_r /= f_b_r.w;
	vec4 f_t_l = inverse * vec4(-1.0, 1.0, 1.0, 1.0);	f_t_l /= f_t_l.w;
	vec4 f_t_r = inverse * vec4(1.0, 1.0, 1.0, 1.0);	f_t_r /= f_t_r.w;

	//Left plane
	planes[0] = plane(triangle{ vec3(f_t_l), vec3(n_t_l), vec3(n_b_l)});

	//Right plane
	planes[1] = plane(triangle{ vec3(n_b_r), vec3(n_t_r), vec3(f_t_r)});

	//top plane
	planes[2] = plane(triangle{ vec3(n_t_r), vec3(n_t_l), vec3(f_t_l) });

	//bottom plane
	planes[3] = plane(triangle{ vec3(f_b_l) , vec3(n_b_r), vec3(f_b_r) });

	//near plane
	planes[4] = plane(triangle{ vec3(n_t_l) , vec3(n_t_r), vec3(n_b_r) });

	//far plane
	planes[5] = plane(triangle{ vec3(f_b_r) , vec3(f_t_r), vec3(f_t_l) });

}


/**
* @brief Finds the points from the frustum;
* @param center
* @param radius
* @return std::array<vec3,8>
*		  The 8 points.
*/
std::array<vec3, 8> frustum::get_points() const 
{
	std::array<vec3, 8> result;

	// In this order: left = 1		right	= 2, 
	//				  top  = 1		bottom	= 2,
	//				  near = 1		far		= 2
	
	//111
	intersection_plane_plane_plane(planes[0], planes[2], planes[4], &result[0]);
	//112
	intersection_plane_plane_plane(planes[0], planes[2], planes[5], &result[1]);
	//121
	intersection_plane_plane_plane(planes[0], planes[3], planes[4], &result[2]);
	//122
	intersection_plane_plane_plane(planes[0], planes[3], planes[5], &result[3]);
	//211
	intersection_plane_plane_plane(planes[1], planes[2], planes[4], &result[4]);
	//212
	intersection_plane_plane_plane(planes[1], planes[2], planes[5], &result[5]);
	//221
	intersection_plane_plane_plane(planes[1], planes[3], planes[4], &result[6]);
	//222
	intersection_plane_plane_plane(planes[1], planes[3], planes[5], &result[7]);


    return result;
}
