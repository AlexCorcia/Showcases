//---------------------------------------------------------------------------
/**
* @file		Object Manager
* @author	Alexandre Corcia Aguilera
* @par		Login: alexandre.corcia
* @brief 	This is the object manager, meaning it has the objects deals with the components and adds objects to the scene .
**/
//---------------------------------------------------------------------------
#include "ObjectManager.h"
#include "Systems.h"
#include "object.h"
#include "component.h"
#include "geometry.h"
#include "mesh.h"
#include "raycast.h"

#include "GLFW/glfw3.h"
#include "camera_ai.h"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

/* ****************************************
brief: Initializer for the Object Manager
**************************************** */
void OM::enter(GLFWwindow * c_window)
{
	m_system_manager = SM::instance();
	m_all_meshes = m_system_manager->m_renderer->m_all_meshes;
	m_window = c_window;

	m_camera = new Object(nullptr, Transform(vec3(0.0f, 1.0f, 5.0f),vec3(1.0f,1.0f,1.0f),vec3(0.0f,0.0f,0.0f)));
	cam_comp = m_camera->add_component<Camera>();
	m_camera->add_component<Camera_AI>();

	Object * floor = create_obj(m_all_meshes[0], Transform(vec3(0.0f, -1.0f, 0.0f), vec3(6.0f, 0.2f, 6.0f), vec3(0.0f, 0.0f, 0.0f)), vec4(0.5f, 0.5f, 0.5f, 1.f), false);

	Object * player = create_obj(m_all_meshes[1], Transform(vec3(0.0f, 0.0f, 0.0f), vec3(1.0f,1.0f,1.0f), vec3(0.0f, 0.0f, 0.0f)), vec4(.1f, 0.1f, 0.8f, 1.f));
	player->add_component<Target>();

	m_selected_object = nullptr;
}

/* ****************************************
brief: Loop of the Object manager
**************************************** */
void OM::loop()
{

	update_aabbs();
	clicker_selector();

	const float dt = 0.016f;
	if(m_selected_object)
	{

		mat4 inverse_matcam = glm::inverse(cam_comp->get_cam_mat());

		if (glfwGetKey(m_window, GLFW_KEY_W))
		{
			vec3 direction = vec3(inverse_matcam * vec4(0.0f, 0.0f, 1.0f, 0.0f));
			m_selected_object->m_transform.m_position -= glm::normalize(vec3(direction.x,0.0, direction.z)) * bunny_speed * dt;
		}

		if (glfwGetKey(m_window, GLFW_KEY_S))
		{
			vec3 direction = vec3(inverse_matcam * vec4(0.0f, 0.0f, 1, 0.0f));
			m_selected_object->m_transform.m_position += glm::normalize(vec3(direction.x, 0.0, direction.z)) * bunny_speed * dt;
		}

		if (glfwGetKey(m_window, GLFW_KEY_A))
		{
			vec3 direction = vec3(inverse_matcam * vec4(1.0f, 0.0f, 0.0f, 0.0f));
			m_selected_object->m_transform.m_position -= glm::normalize(vec3(direction.x, 0.0, direction.z)) * bunny_speed * dt;
		}

		if (glfwGetKey(m_window, GLFW_KEY_D))
		{
			vec3 direction = vec3(inverse_matcam * vec4(1, 0.0f, 0.0f, 0.0f));
			m_selected_object->m_transform.m_position += glm::normalize(vec3(direction.x, 0.0, direction.z)) * bunny_speed * dt;
		}

	}

	m_camera->o_update();

	for (auto & obj : m_all_objects)
	{
		obj->o_update();
	}

	

}

/* ****************************************
brief: Exit of the object Manager
**************************************** */
void OM::exit()
{
}

/* ****************************************
brief: Adds an obstacle to the scene
**************************************** */
void OM::add_obstacle_scene()
{
	create_obj(m_all_meshes[0], Transform(vec3(0.0f, 1.5f, 0.0f), vec3(0.3f, 2.0f, 0.3f), vec3(0.0f, 0.0f, 0.0f)), vec4(1.0f, 1.0f, 1.0f, 1.f));
}

/* ****************************************
brief: Adds an entity with taget component to the scene
**************************************** */
void OM::add_entity_scene()
{
	Object * new_obj = create_obj(m_all_meshes[5], Transform(vec3(0.0f, 0.0f, 0.0f), vec3(.5f, .5f, .5f), vec3(0.0f, 0.0f, 0.0f)), vec4(.8f, 0.1f, 0.1f, 1.f));

	new_obj->add_component<Target>();


}

/* ****************************************
brief: Creates an obejct with the values passed,
if debug is true it will also create a debug 
object of the created object giving it an aabb
**************************************** */
Object * OM::create_obj(Mesh * mesh, Transform transform, vec4 color, bool debug)	
{
	Object * new_obj = new Object(mesh, transform);
	
	aabb new_aabb = aabb_from_points(mesh->vertices);
	new_obj->m_aabb = aabb_from_aabb_and_transform(new_aabb, new_obj->m_transform.get_model());

	new_obj->m_color = color;
	new_obj->m_orginal_color = color;
		
	if (debug)
	{
		new_obj->m_debug_obj = create_debug_obj(new_obj, vec4(0.0f, 1.0f, 0.0f, 1.0f));
		new_obj->m_to_debug = true;
		new_obj->m_input_enabled = true;
	}
	else
	{
		new_obj->m_input_enabled = false;
		new_obj->m_to_debug = false;
	}

	m_all_objects.push_back(new_obj);

	return new_obj;
}

/* ****************************************
brief: Creates a Debug obj for the obejct passed
**************************************** */
Object * OM::create_debug_obj(Object * to_debug, vec4 color)
{
	vec3 size = (to_debug->m_aabb.max_point - to_debug-> m_aabb.min_point) / 2.0f;

	Transform new_transform(to_debug->m_aabb.get_center(), size, vec3(0.0f, 0.0f, 0.0f));

	Object * new_obj = new Object(m_all_meshes[0], new_transform);
	new_obj->m_color = color;
	m_all_debug_objects.push_back(new_obj);

	return new_obj;
}

/* ****************************************
brief: Draws a line inn the debug objects
**************************************** */
void OM::debug_render_line(const vec3 & point_1, const vec3 & point_2)
{
	vec3 segment = point_2 - point_1;
	vec3 position = point_1 + (segment / 2.0f);

	Transform new_transform(position, segment, vec3(0,0,0));

	Object * new_item = new Object(m_all_meshes[7], new_transform);

	m_all_debug_objects.push_back(new_item);
}

/* ****************************************
brief: Updates the aabbs depending on their objs
**************************************** */
void OM::update_aabbs()
{
	unsigned size = m_all_objects.size();

	if (!m_all_debug_objects.empty())
		m_all_debug_objects.clear();

	for (unsigned i = 0; i < size; i++)
	{
		aabb new_aabb = aabb_from_points(m_all_objects[i]->m_mesh->vertices);
		m_all_objects[i]->m_aabb = aabb_from_aabb_and_transform(new_aabb, m_all_objects[i]->m_transform.get_model());

		if (m_all_objects[i]->m_to_debug)
			m_all_objects[i]->m_debug_obj = create_debug_obj(m_all_objects[i], vec4(0.0f, 1.0f, 0.0f, 1.0f));
	}

}

/* ****************************************
brief: Select an object logic is here, colors are too/
**************************************** */
void OM::clicker_selector()
{
	Object * clicked = raycast_for_mouse(cam_comp, m_all_objects, m_window);

	for (unsigned i = 0; i < m_all_objects.size(); i++)
	{
		if (m_all_objects[i] != clicked && m_all_objects[i] != m_selected_object)
			m_all_objects[i]->m_color = m_all_objects[i]->m_orginal_color;
	}

	if (clicked != nullptr && !clicked->m_input_enabled)
		return;

	if (clicked != nullptr && clicked != m_selected_object)
		clicked->m_color = m_hovered_color;

	if (glfwGetMouseButton(m_window, GLFW_MOUSE_BUTTON_1) &&
		!ImGui::IsWindowHovered(ImGuiHoveredFlags_AllowWhenBlockedByActiveItem | ImGuiHoveredFlags_AnyWindow))
	{
		if (clicked == nullptr)
		{
			if (m_selected_object != nullptr)
				m_selected_object->m_color = m_selected_object->m_orginal_color;

			m_selected_object = nullptr;
		}
		else
		{
			if (m_selected_object == nullptr)
			{
				m_selected_object = clicked;
				m_selected_object->m_color = m_selected_color;
			}
			else
			{
				m_selected_object->m_color = m_selected_object->m_orginal_color;
				m_selected_object = clicked;
				m_selected_object->m_color = m_selected_color;
			}
		}
	}


}

