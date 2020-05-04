//---------------------------------------------------------------------------
/**
* @file		geometry.cpp
* @date 	1/18/2019
* @author	Alexandre Corcia Aguilera
* @par		Login: alexandre.corcia
* @par		Course: CS350
* @par		Assignment #1
* @brief 	Some functions that will compute the intersections between some
*			geometry.	
*
* Hours spent on this assignment: 8h
*
* Specific portions that gave you the most trouble:
**/
//---------------------------------------------------------------------------

#include "pch.h"
#include "geometry.h"
/**
* @brief Compute the intersection type between a point a plane
* @param point
* @param plane
* @param epsilon
*		 The value for the intersections
*
* @return intersection_type
*		  The type of intersection.
*/
intersection_type intersection_point_plane(const vec3 &point, const plane &plane, float epsilon) 
{
	float distance = (glm::dot(plane.normal, point) - plane.dot_result);
	
	if (glm::abs(distance) <= epsilon)
		return intersection_type::OVERLAPS;
	else if (distance > 0.0f)
		return intersection_type::INSIDE;

	return intersection_type::OUTSIDE;
}

/**
* @brief Compute the intersection type between a point a sphere
* @param point
* @param sphere
* @return intersection_type
*		  The type of intersection.
*/
intersection_type intersection_point_sphere(const vec3 &point, const sphere &sphere) 
{
	float distance = glm::length(point - sphere.center);

	if (distance > sphere.radius)
		return intersection_type::OUTSIDE;

	return intersection_type::INSIDE;
}

/**
* @brief Compute the intersection type between a point an aabb
* @param point
* @param aabb
* @return intersection_type
*		  The type of intersection.
*/
intersection_type intersection_point_aabb(const vec3 &point, const aabb &aabb) 
{
	if (point.x > aabb.max_point.x || point.x < aabb.min_point.x)
		return intersection_type::OUTSIDE;
	else if (point.y > aabb.max_point.y || point.y < aabb.min_point.y)
		return intersection_type::OUTSIDE;
	else if (point.z > aabb.max_point.z || point.z < aabb.min_point.z)
		return intersection_type::OUTSIDE;

    return intersection_type::INSIDE;
}


/**
* @brief Computes the intersection between the ray and plane.
* @param ray
* @param plane
* @return float
*		  The value of the ray where it intersects, if it returns -1
*		  there is no intersection.
*/
float intersection_ray_plane(const ray &ray, const plane &plane) 
{
	//getting a point.
	vec3 c = plane.get_point();

	float dividand = glm::dot(plane.normal, ray.dir);

	//check for parallel posibilities
	if (glm::abs(dividand) < cEpsilon)
		return -1;

	//Calculating the value of t in order to know if the ray intersects
	float t = (glm::dot(plane.normal, c) - glm::dot(plane.normal, ray.start)) / dividand;

	//Check if the direction is correct
	if (t < 0.0f)
		return -1;

	return t;
}

/**
* @brief Computes the intersection between the ray and sphere.
* @param ray
* @param sphere
* @return float
*		  The value of the ray where it intersects, if it returns -1
*		  there is no intersection.
*/
float intersection_ray_sphere(const ray &ray, const sphere &sphere) 
{
	vec3 m = ray.start - sphere.center;
	float b = glm::dot(m, glm::normalize(ray.dir));
	float c = glm::dot(m, m) - glm::pow(sphere.radius, 2);

	//Check if it is in the same direction as the sphere.
	if (c > 0.0f && b > 0.0f)
		return -1;

	float discr = b*b - c;

	//Check if it collides
	if (discr < 0.0f) return -1;

	float t = -b - glm::sqrt(discr);

	//check direction
	if (t < 0.0f)
		return 0.0f;

    return t;
}


/**
* @brief Computes the intersection between the ray and aabb.
* @param ray
* @param aabb
* @return float
*		  The value of the ray where it intersects, if it returns -1
*		  there is no intersection.
*/
float intersection_ray_aabb(const ray &ray, const aabb &aabb) 
{
	float t_min = 0.0f;
	float t_max = FLT_MAX;

	for (unsigned i = 0; i < 3; i++)
	{
		if (glm::abs(ray.dir[i]) < cEpsilon)
		{
			// checking if the ray is parralel to the stab
			if (ray.start[i] < aabb.min_point[i] || ray.start[i] > aabb.max_point[i])
				return -1;
		}
		else
		{
			//COmputing the intersection wich the planes of the slab.
			float dividand = 1.0f / ray.dir[i];

			float min = (aabb.min_point[i] - ray.start[i]) * dividand;
			float max = (aabb.max_point[i] - ray.start[i]) * dividand;

			//checking that the intersection are with the correct plane
			if (min > max)
			{
				float temp = min;
				min = max;
				max = temp;
			}

			// Check the minimum and maximums with t
			if (min > t_min) 
				t_min = min;
			if (max < t_max)
				t_max = max;

			//no collision.
			if (t_min > t_max)
				return -1;
		}

	}

    return t_min;

}

/**
* @brief Computes the intersection between the ray and triangle.
* @param ray
* @param triangle
* @return float
*		  The value of the ray where it intersects, if it returns -1
*		  there is no intersection.
*/
float intersection_ray_triangle(const ray &ray, const triangle &triangle) 
{
	//Check for the plane first
	float t = intersection_ray_plane(ray, plane(triangle));

	if (t != -1.0f)
	{
		const vec3 plane_point = ray.start + ray.dir * t;

		vec3 * result = nullptr;

		//check if it collides with the triangle.
		if (get_barycentric_coordinates(triangle, plane_point, result))
			return t;

		return -1;
	}

    return -1;
}

/**
* @brief Computes the intersection between the plane and triangle.
* @param plane
* @param triangle
* @param epsilon
*		 The value for the intersections
*
* @return intersection_type
*		  The type of intersection.
*/
intersection_type intersection_plane_triangle(const plane &plane, const triangle &triangle, float epsilon)
{
	intersection_type all_intersections[3];

	//Check all points of the triangle
	all_intersections[0] = intersection_point_plane(triangle.points[0], plane, epsilon);
	all_intersections[1] = intersection_point_plane(triangle.points[1], plane, epsilon);
	all_intersections[2] = intersection_point_plane(triangle.points[2], plane, epsilon);

	//Checks the kind of collision 
	unsigned overlap_count = 0;
	bool any_in = false;
	bool any_out = false;

	for (unsigned i = 0; i < 3; i++)
	{
		if (all_intersections[i] == intersection_type::OVERLAPS)
		{
			overlap_count++;
			continue;
		}

		if (all_intersections[i] == intersection_type::INSIDE)
		{
			any_in = true;
			continue;
		}

		if (all_intersections[i] == intersection_type::OUTSIDE)
		{
			any_out = true;
			continue;
		}
	}

	if (any_in && any_out)
		return intersection_type::OVERLAPS;

	if (overlap_count == 3)
		return intersection_type::COPLANAR;

	if (any_in)
		return intersection_type::INSIDE;
	
	return intersection_type::OUTSIDE;

}

/**
* @brief Computes the intersection between the plane and sphere.
* @param plane
* @param sphere
* @return intersection_type
*		  The type of intersection.
*/

intersection_type intersection_plane_sphere(const plane &plane, const sphere &sphere) 
{
    return intersection_point_plane(sphere.center,plane,sphere.radius + cEpsilon);
}

/**
* @brief Computes the intersection between the plane and aabb.
* @param plane
* @param aabb
* @return intersection_type
*		  The type of intersection.
*/
intersection_type intersection_plane_aabb(const plane &plane, const aabb &aabb) 
{
	//finding the middle point.
	vec3 c = (aabb.max_point + aabb.min_point) / 2.0f;
	vec3 e = aabb.max_point - c;

	//COmputing the projection interval
	float r = e.x * glm::abs(plane.normal.x) + e.y * glm::abs(plane.normal.y)
			+ e.z * glm::abs(plane.normal.z);

	//distance of box center to plane.
	float s = glm::dot(plane.normal, c) - plane.dot_result;

	 if(glm::abs(s) <= r)
	    return intersection_type::OVERLAPS;

	 if (s <= -r)
		 return intersection_type::OUTSIDE;
	 else
		 return intersection_type::INSIDE;
}

/**
* @brief Computes the intersection between three planes
* @param plane_a
* @param plane_b
* @param plane_c
* @param intersect
* @return bool
*		  if there is a collision or not.
*/
bool intersection_plane_plane_plane(const plane &plane_a, const plane &plane_b, const plane &plane_c, vec3 *intersect) 
{
	vec3 v1 = vec3(plane_a.normal.x, plane_b.normal.x, plane_c.normal.x);
	vec3 v2 = vec3(plane_a.normal.y, plane_b.normal.y, plane_c.normal.y);
	vec3 v3 = vec3(plane_a.normal.z, plane_b.normal.z, plane_c.normal.z);

	vec3 u = glm::cross(v2, v3);
	float dividand = glm::dot(v1, u);
	
	if (glm::abs(dividand) < cEpsilon)
		return false;

	vec3 d(plane_a.dot_result, plane_b.dot_result, plane_c.dot_result);
	vec3 v(glm::cross(v1, d));

	float dividand_2 = 1.0f / dividand;

	intersect->x = glm::dot(d, u) * dividand_2;
	intersect->y = glm::dot(v3, v) * dividand_2;
	intersect->z = -glm::dot(v2, v) * dividand_2;

	return true;
}

/**
* @brief Computes the intersection between a frustrum and a triangle
* @param frustum
* @param triangle
* @return intersection_type
*		  The type of intersection.
*/
intersection_type intersection_frustum_triangle(const frustum &frustum, const triangle &triangle) 
{
	bool overlaps = false;
	
	//Go through all of the planes and check plane triangle
	for (unsigned i = 0; i < 6; i++)
	{
		intersection_type inter = intersection_plane_triangle(frustum.planes[i], triangle);

		if (inter == intersection_type::OUTSIDE)
			return intersection_type::OUTSIDE;
		else if (inter == intersection_type::OVERLAPS)
			overlaps = true;
	}
	if (overlaps)
		return intersection_type::OVERLAPS;

	return intersection_type::INSIDE;
}

/**
* @brief Computes the intersection between a frustrum and a sphere
* @param frustum
* @param sphere
* @return intersection_type
*		  The type of intersection.
*/
intersection_type intersection_frustum_sphere(const frustum &frustum, const sphere &sphere) 
{
	bool overlaps = false;

	//Go through all of the planes and check plane sphere
	for (unsigned i = 0; i < 6; i++)
	{
		intersection_type inter = intersection_plane_sphere(frustum.planes[i], sphere);

		if (inter == intersection_type::OUTSIDE)
			return intersection_type::OUTSIDE;
		else if (inter == intersection_type::OVERLAPS)
			overlaps = true;
	}
	if(overlaps)
		return intersection_type::OVERLAPS;

	return intersection_type::INSIDE;
}

/**
* @brief Computes the intersection between a frustrum and an aabb
* @param frustum
* @param aabb
* @return intersection_type
*		  The type of intersection.
*/
intersection_type intersection_frustum_aabb(const frustum &frustum, const aabb &aabb) 
{
	bool overlaps = false;

	//Go through all of the planes and check plane aabb
	for (unsigned i = 0; i < 6; i++)
	{
		intersection_type inter = intersection_plane_aabb(frustum.planes[i], aabb);

		if (inter == intersection_type::OUTSIDE)
			return intersection_type::OUTSIDE;
		else if (inter == intersection_type::OVERLAPS)
			overlaps = true;
	}

	if (overlaps)
		return intersection_type::OVERLAPS;

	return intersection_type::INSIDE;
}

/**
* @brief Computes the intersection between two spheres
* @param sphere_a
* @param sphere_b
* @return bool
*		  If they collide
*/
bool intersection_sphere_sphere(const sphere &sphere_a, const sphere &sphere_b) 
{
    return glm::length(sphere_b.center - sphere_a.center) <= sphere_a.radius + sphere_b.radius;
}

/**
* @brief Computes the intersection between two aabb
* @param aabb_a
* @param aabb_b
* @return bool
*		  If they collide
*/
bool intersection_aabb_aabb(const aabb &aabb_a, const aabb &aabb_b)
{
	for (unsigned i = 0; i < 3; i++)
	{
		if (aabb_a.min_point[i] > aabb_b.max_point[i])
			return false;

		if (aabb_b.min_point[i] > aabb_a.max_point[i])
			return false;
	}

    return true;
}

/**
* @brief Computes the projection between a point a plane.
* @param point
* @param plane
* @return vec3
*/
vec3 project_point_plane(const vec3 &point, const plane &plane) 
{
	float distance = (glm::dot(plane.normal, point) - plane.dot_result) / glm::dot(plane.normal, plane.normal);

    return point + plane.normal * -distance;
}

/**
* @brief Computes the position of a point depending on a segment.
* @param segment_a
* @param segment_b
* @param point
* @param result
* @return bool
*		  If the point is in the segment.
*/
bool get_barycentric_coordinates(const vec3 &segment_a, const vec3 &segment_b, const vec3 &point, vec2 *result) 
{
	unsigned counter = 0;
	float t = 0.0f;

	for (unsigned i = 0; i < 3; i++)
	{
		float q1 = segment_a[i];
		float q2 = segment_b[i];
		float p = point[i];

		if (q1 == q2)
			continue;

		t += (p - q2) / (q1 - q2);
		counter++;
	}

	if (counter == 0)
		return false;

	t /= counter;

	if (t > 1.0f || t < 0.0f)
		return false;

	result->x = t;
	result->y = 1 - t;

    return true;
}

/**
* @brief Computes the position of a point depending on a triangle.
* @param triangle
* @param point
* @param result
* @return bool
*		  If the point is in the triangle.
*/
bool get_barycentric_coordinates(const triangle &triangle, const vec3 &point, vec3 *result) 
{
	vec3 q0 = triangle.points[0];
	vec3 q1 = triangle.points[1];
	vec3 q2 = triangle.points[2];

	vec3 v0 = q1 - q0;
	vec3 v1 = q2 - q0;

	vec3 v2 = point - q0;

	float d00 = glm::dot(v0, v0);
	float d01 = glm::dot(v0, v1);
	float d11 = glm::dot(v1, v1);
	float d20 = glm::dot(v2, v0);
	float d21 = glm::dot(v2, v1);

	float dividand = d00 * d11 - d01 * d01;

	if (dividand == 0)
		return false;

	float s = (d11 * d20 - d01 * d21) / dividand;
	float t = (d00 * d21 - d01 * d20) / dividand;
	float r = 1.0f - s - t;

	if (t > 1.0f || t < 0.0f || 
		s > 1.0f || s < 0.0f || 
		r > 1.0f || r < 0.0f)
		return false;

	if(result != nullptr)
	
	{
		result->x = r;
		result->y = s;
		result->z = t;
	}

    return true;
}


/**
* @brief Computes an tight aabb from a vector of points
* @param poitns
* @return aabb
*		  The final aabb
*/
aabb aabb_from_points(const std::vector<vec3>& points)
{
	if (points.empty())
		return aabb();

	vec3 min{FLT_MAX};
	vec3 max{-FLT_MAX};

	for (unsigned i = 0; i < points.size(); i++)
	{
		for (unsigned j = 0; j < 3; j++)
		{
			if (points[i][j] < min[j])
				min[j] = points[i][j];

			if (points[i][j] > max[j])
				max[j] = points[i][j];
		}
	}
	return aabb(min, max);
}

/**
* @brief Transform the aabb with the model_2_world matrix we have
* @param aabb_1
* @param m2w 
* @return aabb
*		  The final aabb with the transformation
*/
aabb aabb_from_aabb_and_transform(const aabb & aabb_1, mat4 m2w)
{

	vec3 min{ FLT_MAX };
	vec3 max{ FLT_MIN };

	for (unsigned i = 0; i < 3; i++)
	{
		min[i] = max[i] = m2w[3][i];

		for (unsigned j = 0; j < 3; j++)
		{
			float e = m2w[j][i] * aabb_1.min_point[j];
			float f = m2w[j][i] * aabb_1.max_point[j];

			if (e < f)
			{
				min[i] += e;
				max[i] += f;
			}
			else
			{
				min[i] += f;
				max[i] += e;
			}
		}
	}

	return aabb(min,max);
}

/**
* @brief Computes an tight sphere from a vector of points with the centroid method
* @param poitns
* @return sphere
*		  The final sphere
*/
sphere sphere_from_points_centroid(const std::vector<vec3>& points)
{
	aabb seperates_points = aabb_from_points(points);

	sphere new_sphere;

	new_sphere.center = ((seperates_points.min_point + seperates_points.max_point) * 0.5f);
	new_sphere.radius = 0;

	for (unsigned i = 0; i < points.size(); i++)
	{
		float check_value = glm::length(points[i] - new_sphere.center);

		if (new_sphere.radius < check_value)
			new_sphere.radius = check_value;
	}

	return new_sphere;
}

//---------------------------------------------------------------------------------//
/**
* @brief finds the min and max from a group of points
* @param points
* @return std::vector<int>
*		  the vector will always be size 2 so min = 0, max = 1. 
*/
std::vector<int> seperated_points_aabb(const std::vector<vec3>& points)
{
	int min, max;

	unsigned min_x = 0, max_x = 0,
			 min_y = 0, max_y = 0,
			 min_z = 0, max_z = 0;

	for (unsigned i = 1; i < points.size(); i++)
	{
		if (points[i].x < points[min_x].x) min_x = i;
		if (points[i].x > points[max_x].x) max_x = i;
		if (points[i].y < points[min_y].y) min_y = i;
		if (points[i].y > points[max_y].y) max_y = i;
		if (points[i].z < points[min_z].z) min_z = i;
		if (points[i].z > points[max_z].z) max_z = i;
	}

	float dist_x = glm::dot(points[max_x] - points[min_x], points[max_x] - points[min_x]);
	float dist_y = glm::dot(points[max_y] - points[min_y], points[max_y] - points[min_y]);
	float dist_z = glm::dot(points[max_z] - points[min_z], points[max_z] - points[min_z]);

	min = min_x;
	max = max_x;

	if (dist_y >= dist_x && dist_y >= dist_z)
	{
		min = min_y;
		max = max_y; 
	}

	if (dist_z >= dist_x && dist_z >= dist_y)
	{
		max = max_z; 
		min = min_z;
	}

	std::vector<int> result;

	result.push_back(min);
	result.push_back(max);

	return result;
}

/**
* @brief generates a sphere points.
* @param points
* @return sphere
*		  The sphere
*/
sphere sphere_distant_points(const std::vector<vec3>& points)
{
	std::vector<int> indexes = seperated_points_aabb(points);

	sphere new_sphere;

	new_sphere.center = (points[indexes[0]] + points[indexes[1]]) * 0.5f;
	new_sphere.radius = glm::length(points[indexes[1]] - new_sphere.center);

	return new_sphere;
}

/**
* @brief checks if the point is inside the sphere if not we grow the sphere.
* @param sphere
* @param point
* @return
*/
void sphere_from_sphere_and_points(sphere & s, vec3 point)
{
	const vec3 distance = point - s.center;
	float dist2 = glm::dot(distance, distance);

	if (dist2 >= (s.radius * s.radius))
	{
		float dist = glm::sqrt(dist2);
		float n_radius = (s.radius + dist) * 0.5f;
		float k = (n_radius - s.radius) / dist;

		s.radius = n_radius;
		s.center += distance * k;
	}
}
//---------------------------------------------------------------------------------//

/**
* @brief Computes an tight sphere from a vector of points with the ritter method
* @param poitns
* @return sphere
*		  The final sphere
*/
sphere sphere_from_points_ritter(const std::vector<vec3>& points)
{
	if (points.empty())
		return sphere();

	sphere result = sphere_distant_points(points);


	for (unsigned i = 0; i < points.size(); i++)
		sphere_from_sphere_and_points(result, points[i]);


	return result;
}

/**
* @brief Computes an tight sphere from a vector of points with the iterative method
* @param poitns
* @return sphere
*		  The final sphere
*/
sphere sphere_from_points_iterative(const std::vector<vec3>& points, int iteration_count, float shrink_ratio)
{
	sphere result = sphere_from_points_ritter(points);
	sphere test = result;

	for (int i = 0; i < iteration_count; i++)
	{
		test.radius = test.radius * shrink_ratio;

		for(unsigned j = 0; j < points.size(); j++)
			sphere_from_sphere_and_points(test, points[j]);

		if (test.radius < result.radius)
			result = test;
	}

	return result;
}



sphere sphere_from_points_pca(const std::vector<vec3>& points)
{
	(void)points;
	return sphere();
}
