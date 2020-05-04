#pragma once
#include "pch.h"
#include "shapes.h"

// assignment #1

/**
 * The intersection types
 */
enum class intersection_type {
	COPLANAR,
	INSIDE,
	OVERLAPS,
	OUTSIDE,
};

// Points vs {}
intersection_type intersection_point_plane(const vec3& point, const plane& plane, float epsilon=cEpsilon);
intersection_type intersection_point_sphere(const vec3& point, const sphere& sphere);
intersection_type intersection_point_aabb(const vec3& point, const aabb& aabb);

// Ray vs {}
float intersection_ray_plane(const ray& ray, const plane& plane);
float intersection_ray_sphere(const ray& ray, const sphere& sphere);
float intersection_ray_aabb(const ray& ray, const aabb& aabb);
float intersection_ray_triangle(const ray& ray, const triangle& triangle);

// Planes vs {}
intersection_type intersection_plane_triangle(const plane& plane, const triangle& triangle, float epsilon=cEpsilon);
intersection_type intersection_plane_sphere(const plane& plane, const sphere& sphere);
intersection_type intersection_plane_aabb(const plane& plane, const aabb& aabb);
bool intersection_plane_plane_plane(const plane& plane_a, const plane& plane_b, const plane& plane_c, vec3* intersect);

// Frustum vs {}
intersection_type intersection_frustum_triangle(const frustum& frustum, const triangle& triangle);
intersection_type intersection_frustum_sphere(const frustum& frustum, const sphere& sphere);
intersection_type intersection_frustum_aabb(const frustum& frustum, const aabb& aabb);

// Sphere vs {}
bool intersection_sphere_sphere(const sphere& sphere_a, const sphere& sphere_b);

// Aabb vs {}
bool intersection_aabb_aabb(const aabb& aabb_a, const aabb& aabb_b);

// Projections
vec3 project_point_plane(const vec3& point, const plane& plane);

// Barycentric coordinates
bool get_barycentric_coordinates(const vec3& segment_a, const vec3& segment_b, const vec3& point, vec2* result);
bool get_barycentric_coordinates(const triangle& triangle, const vec3& point, vec3* result);

//MY OWN FUNCTIONS
std::vector<int> seperated_points_aabb(const std::vector<vec3>& points);
sphere sphere_distant_points(const std::vector<vec3>& points);
void sphere_from_sphere_and_points(sphere & s, vec3 point);

// Geometry construction
aabb aabb_from_points(const std::vector<vec3>& points);
aabb aabb_from_aabb_and_transform(const aabb& aabb_1, mat4 m2w);
sphere sphere_from_points_centroid(const std::vector<vec3>& points);
sphere sphere_from_points_ritter(const std::vector<vec3>& points);
sphere sphere_from_points_pca(const std::vector<vec3>& points);
sphere sphere_from_points_iterative(const std::vector<vec3>& points, int iteration_count, float shrink_ratio);