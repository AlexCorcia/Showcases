//---------------------------------------------------------------------------
/**
* @file		camera_ai
* @author	Alexandre Corcia Aguilera
* @par		Login: alexandre.corcia
* @brief 	File that has logic for multitargeting camera and obstacle avoidance.
**/
//---------------------------------------------------------------------------
#include "camera_ai.h"
#include "GLFW/glfw3.h"
#include "object.h"
#include "raycast.h"
#include "interpolations.h"


/* ****************************************
brief: Initialization of the component
**************************************** */
void Camera_AI::initialize()
{
	m_system_manager = SM::instance();
	m_object_manager = m_system_manager->m_object_manager;
	m_render_manager = m_system_manager->m_renderer;

	m_window = m_system_manager->m_window_manager->m_window;
	m_cam_obj = m_system_manager->m_object_manager->m_camera;
	m_camera = m_system_manager->m_object_manager->cam_comp;
}


/* ****************************************
brief: Updating the component
**************************************** */
void Camera_AI::update()
{
	switch (m_targeting)
	{
	case Camera_AI::none:
	{
		input_camera();
		break;
	}
	case Camera_AI::single:
	{
		targeting(m_object_manager->m_selected_object);

		if (m_object_manager->m_selected_object)
			vision_check();
		else
			input_camera();

		break;
	}
	case Camera_AI::multi:
	{
		multi_targeting(m_object_manager->m_all_objects);
		break;
	}
	default:
		break;
	}
}


/* ****************************************
brief: Manual control of the camera
**************************************** */
void Camera_AI::input_camera()
{

	double cursor_x = 0.0;
	double cursor_y = 0.0;
	glfwGetCursorPos(m_window, &cursor_x, &cursor_y);

	//Input for the camera
	if (glfwGetMouseButton(m_window, GLFW_MOUSE_BUTTON_2))
	{
		const float dt = 0.016f;

		auto  side = glm::normalize(glm::cross(m_camera->m_target_pos, { 0, 1, 0 }));
		auto  up = glm::normalize(glm::cross(m_camera->m_target_pos, side));

		if (glfwGetKey(m_window, GLFW_KEY_W))
			m_owner->m_transform.m_position += glm::normalize(m_camera->m_target_pos) * dt * cam_speed;

		if (glfwGetKey(m_window, GLFW_KEY_S))
			m_owner->m_transform.m_position -= glm::normalize(m_camera->m_target_pos) * dt * cam_speed;

		if (glfwGetKey(m_window, GLFW_KEY_A))
			m_owner->m_transform.m_position -= glm::normalize(side) * dt * cam_speed;

		if (glfwGetKey(m_window, GLFW_KEY_D))
			m_owner->m_transform.m_position += glm::normalize(side) * dt * cam_speed;

		if (glfwGetKey(m_window, GLFW_KEY_E))
			m_owner->m_transform.m_position += up * dt * cam_speed;

		if (glfwGetKey(m_window, GLFW_KEY_Q))
			m_owner->m_transform.m_position -= up * dt * cam_speed;

		vec2 cursor_delta = { (float)cursor_x - m_camera->cursor_pos.x, (float)cursor_y - m_camera->cursor_pos.y };
		m_camera->m_target_pos = vec3(vec4(m_camera->m_target_pos, 0) * glm::rotate(glm::radians(15.0f) * dt * cursor_delta.y, side));
		m_camera->m_target_pos = vec3(vec4(m_camera->m_target_pos, 0) * glm::rotate(glm::radians(15.0f) * dt * cursor_delta.x, vec3(0, 1, 0)));

	}

	m_camera->cursor_pos = { (float)cursor_x, (float)cursor_y };
}

/* ****************************************
brief: Simply setting the direction of the 
camera to look at the target passed.
**************************************** */
void Camera_AI::targeting(Object * m_target)
{
	if (m_target == nullptr)
		return;

	m_targeting_position = m_target->m_transform.m_position;

	m_camera->m_target_pos = glm::normalize(m_targeting_position - m_owner->m_transform.m_position);

}

/* ****************************************
brief: Function that will compute the average
between all of the targets and sets the camera 
target to that point then check for the best radius and 
sets the new position for the camera in spherical coordinates
**************************************** */
void Camera_AI::multi_targeting(std::vector<Object *> m_targets)
{


	//Computing the new target
	unsigned size = m_targets.size();

	m_targeting_position = vec3{ 0.0f,0.0f,0.0f };

	std::vector<Object *> m_all_targets;

	for (unsigned i = 0; i < size; i++)
	{
		Target * targeted = m_targets[i]->do_i_have<Target>();

		if (targeted != nullptr)
		{
			m_all_targets.push_back(m_targets[i]);
			m_targeting_position += m_targets[i]->m_transform.m_position;
		}
	}
	
	m_targeting_position /= static_cast<float>(m_all_targets.size());
	
	//Check if the radius can be improved
	check_best_radius(m_all_targets, m_camera->m_camera_frustum);

	//Setting the new position of the camera with spherical coordinates
	float x = radius * glm::sin(glm::radians(angle_1)) * glm::cos(glm::radians(angle_2));
	float y = cam_height;
	float z = radius * glm::cos(glm::radians(angle_1));

	vec3 usual_position{ x,y,z };
	m_cam_obj->m_transform.m_position = lerp(m_cam_obj->m_transform.m_position, m_targeting_position + usual_position, cam_lerp);

	m_camera->m_target_pos = glm::normalize(m_targeting_position - m_owner->m_transform.m_position);

}

/* ****************************************
brief: Function that check if the object is 
visible if call compute new position and set 
that new angle to recompute the position with 
spherical coordinates
**************************************** */
void Camera_AI::vision_check()
{
	if (m_object_manager->m_selected_object == nullptr)
		return;

	//Checking if we nned to recompute
	aabb checking_aabb = m_object_manager->m_selected_object->m_aabb;
	float v_fraction = compute_vision_fraction(m_cam_obj->m_transform.m_position, checking_aabb);



	if (v_fraction != 1.0f)
	{
	// we need to recompute so the new angle is computed and then set
		float new_angle = compute_new_position(v_fraction, checking_aabb);

		angle_1 = lerp(angle_1, new_angle, cam_lerp);

		float x = radius * glm::sin(glm::radians(angle_1)) * glm::cos(glm::radians(angle_2));
		float y = cam_height;
		float z = radius * glm::cos(glm::radians(angle_1));

		vec3 new_try{ x,y,z };

		m_cam_obj->m_transform.m_position = lerp(m_cam_obj->m_transform.m_position ,m_object_manager->m_selected_object->m_transform.m_position + new_try, cam_lerp);
	}
	else
	{
		//recomputing new positon
		float x = radius * glm::sin(glm::radians(angle_1)) * glm::cos(glm::radians(angle_2));
		float y = cam_height;
		float z = radius * glm::cos(glm::radians(angle_1));
		
		vec3 usual_position{ x,y,z };
		m_cam_obj->m_transform.m_position = lerp(m_cam_obj->m_transform.m_position ,m_object_manager->m_selected_object->m_transform.m_position + usual_position, cam_lerp);
	}
}

/* ****************************************
brief: Check how much of the target is visible.
**************************************** */
float Camera_AI::compute_vision_fraction(vec3 pos, aabb to_check)
{

	//We need an epsylone if not the computation go crazy. 
	float epsylon = 0.02f;
	unsigned counter = 0;

	float x_value;
	float y_value;
	float z_value;


	//RayCasting to all corners
	for (unsigned i = 0; i < 2; i++)
	{
		if (i == 0)
			x_value = to_check.max_point.x - epsylon;
		else
			x_value = to_check.min_point.x + epsylon;
		for (unsigned j = 0; j < 2; j++)
		{
			if (j == 0)
				y_value = to_check.max_point.y - epsylon;
			else
				y_value = to_check.min_point.y + epsylon;
			for (unsigned k = 0; k < 2; k++)
			{
				if (k == 0)
					z_value = to_check.max_point.z - epsylon;
				else
					z_value = to_check.min_point.z + epsylon;

				vec3 new_point(x_value, y_value, z_value);

				vec3 direction = glm::normalize(new_point - pos);
				Object * rc_ck = raycast(pos, direction, m_object_manager->m_all_objects);

				//If the raycast hits the object then add one to the counter
				if (rc_ck == m_object_manager->m_selected_object)
					counter++;
			}
		}
	}

	//divide by the number of corner  = 8
	return static_cast<float>(counter) / 8.0f;
}


/* ****************************************
brief: This function will compute the new 
angle that the camera need to adapt to see 
more of the player or find a better position
**************************************** */

float Camera_AI::compute_new_position(float v_fraction, aabb to_check)
{
	vec3 current_pos = m_object_manager->m_selected_object->m_transform.m_position;
	
	float current_best = v_fraction;
	vec3 best_pos = current_pos;
	int best_iteration = 0;
	float best_angle = angle_1;

	float new_angle_1 = angle_1;


	//we go from -angle check to angle check so that we cover left and right 
	for (int i = -smart_camera_angle_check; i < smart_camera_angle_check; i++)
	{
		new_angle_1 = angle_1 + i;

		//Checking if that new position has a better fraction
		float x = radius * glm::sin(glm::radians(new_angle_1)) * glm::cos(glm::radians(angle_2));
		float y = cam_height;
		float z = radius * glm::cos(glm::radians(new_angle_1));

		vec3 new_try{ x,y,z };

		float new_fraction = compute_vision_fraction(current_pos + new_try, to_check);

		//If the fraction is better
		if (current_best <= new_fraction)
		{

			//we check if they are the same 
			if (current_best == new_fraction)
			{
				//because if so we need to check the closest one
				if (abs(i) < best_iteration)
				{
					current_best = new_fraction;
					best_pos = current_pos + new_try;
					best_iteration = abs(i);
					best_angle = new_angle_1;
				}
			}
			else // if not,simply change it
			{
				current_best = new_fraction;
				best_pos = current_pos + new_try;
				best_iteration = abs(i);
				best_angle = new_angle_1;
			}
		}
			
	}
	return best_angle;
}

/* ****************************************
brief: Checking what is the best distance 
for the multitargetting.
**************************************** */
void Camera_AI::check_best_radius(std::vector<Object*> m_targets, frustum checking)
{
	unsigned size = m_targets.size();

	//Compute new camera position with smaller radius
	float new_radius = radius - 0.1f;


	bool all_in = false;
	// while we havent found a solution
	while (!all_in)
	{

		//Compute a new frustum with the new radius
		all_in = true;
		float x = new_radius * glm::sin(glm::radians(angle_1)) * glm::cos(glm::radians(angle_2));
		float y = cam_height;
		float z = new_radius * glm::cos(glm::radians(angle_1));

		vec3 cam_sph_pos{ x,y,z };

		mat4 m_projection_matrix = glm::perspective(glm::radians(m_camera->m_fov), 16.0f / 9.0f, 0.1f, 1000.0f);

		mat4 m_mat = glm::lookAt(cam_sph_pos + m_targeting_position,
								 m_targeting_position,
								 vec3(0.0, 1.0, 0.0));

		frustum new_frustrum = frustum(m_projection_matrix * m_mat);

		//then check if they are all in
		for (unsigned i = 0; i < size; i++)
		{
			intersection_type check = intersection_frustum_aabb(new_frustrum, m_targets[i]->m_aabb);

			//if one is overlaping or outside increase the radius and restart
			if (check == intersection_type::OUTSIDE || check == intersection_type::OVERLAPS)
			{
				new_radius += 0.1f;
				all_in = false;
				break;
			}
		}
	}
	
	//Limit if not it goes crazy
	if (new_radius <= 6.0f)
		return;

	radius = new_radius;
}


/* ****************************************
brief: Initialization of the component
**************************************** */
void Target::initialize()
{

}


/* ****************************************
brief: Updating the component
**************************************** */
void Target::update()
{
}
