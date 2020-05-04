//---------------------------------------------------------------------------
/**
* @file		raycast.cpp
* @date 	3/20/2019
* @author	Alexandre Corcia Aguilera
* @par		Login: alexandre.corcia
* @par		Course: CS350
* @par		Assignment #4
* @brief 	Logic for the raycast for the mouse
*
* Hours spent on this assignment: 2h
*
* Specific portions that gave you the most trouble:
**/
//---------------------------------------------------------------------------

#include "raycast.h" 
#include "object.h"
#include "component.h"
#include "geometry.h"
#include <GLFW/glfw3.h>

#define WINDOW_WIDTH 1920
#define WINDOW_HEIGHT 1080

/**
* @brief Throwing a ray to the camera and if it does it returns an obj else nullptr
* @return Obj *
*		  The object we are colliding 
*/
Object * raycast_for_mouse(Camera * m_camera, std::vector<Object *> all_objs, GLFWwindow * m_window)
{
	if (all_objs.empty())
		return nullptr;

	double cursor_x = 0.0;
	double cursor_y = 0.0;
	glfwGetCursorPos(m_window, &cursor_x, &cursor_y);

	//Getting the position of the mouse in ndc
	float x = 2.0f * (static_cast<float>(cursor_x) / WINDOW_WIDTH) - 1.0f;
	float y = -(2.0f * (static_cast<float>(cursor_y) / WINDOW_HEIGHT) - 1.0f);


	//Getting all the way to wolrd pos
	vec4 mouse_v = { x,y,1.0f,1.0f };
	vec4 mouse_w = glm::inverse(m_camera->get_projection_mat() * m_camera->get_cam_mat()) * mouse_v;
	
	vec3 world_pos = vec3{ mouse_w / mouse_w.w };

	float distance = FLT_MAX;

	//Creating a ray to check if we are colliding with an object
	ray new_ray = ray(m_camera->m_owner->m_transform.m_position, world_pos - m_camera->m_owner->m_transform.m_position);

	Object * result = nullptr;
	//Checking which one is the closest
	for (unsigned i = 0; i < all_objs.size(); i++)
	{
		float d = intersection_ray_aabb(new_ray, all_objs[i]->m_aabb);

		if (d > 0 && d < distance)
		{
			result = all_objs[i];
			distance = d;
		}
	}

	return result;
}

Object * raycast(vec3 starting, vec3 direction, std::vector<Object *>all_objs)
{

	//Creating a ray to check if we are colliding with an object
	ray new_ray = ray(starting, direction);

	Object * result = nullptr;

	float distance = FLT_MAX;
	for (unsigned i = 0; i < all_objs.size(); i++)
	{
		float d = intersection_ray_aabb(new_ray, all_objs[i]->m_aabb);
		if (d > 0 && d < distance)
		{
			result = all_objs[i];
			distance = d;
		}
	}

	

	return result;

}
