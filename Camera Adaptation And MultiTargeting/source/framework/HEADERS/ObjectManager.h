//---------------------------------------------------------------------------
/**
* @file		Object Manager
* @author	Alexandre Corcia Aguilera
* @par		Login: alexandre.corcia
* @brief 	This is the object manager, meaning it has the objects deals with the components and adds objects to the scene .
**/
//---------------------------------------------------------------------------
#pragma once
#include "pch.h"
#include "transform.h"



class Object;
class Mesh;
struct GLFWwindow;
class SM;
class OM
{
public: //PUBLIC FUNCTIONS

	void enter(GLFWwindow * c_window);
	void loop();
	void exit();

	void add_obstacle_scene();
	void add_entity_scene();


	void debug_render_line(const vec3 & point_1, const vec3 & point_2);

public: //PUBLIC VARIABLES
	std::vector<Object *> m_all_objects;
	std::vector<Object *> m_all_debug_objects;



	Object * m_camera;
	Camera * cam_comp;



	Object * m_selected_object{ nullptr };
	float bunny_speed = 2.0f;

private: //PRIVATE FUNCTIONS
	Object * create_obj(Mesh * mesh, Transform transform, vec4 color, bool debug = true);
	Object * create_debug_obj(Object * to_debug, vec4 color);

	void update_aabbs();
	void clicker_selector();

private: //PRIVATE VARIABLES
	std::vector<Mesh *> m_all_meshes;
	SM * m_system_manager = nullptr;
	GLFWwindow * m_window;

	//Color schems
	vec4 m_selected_color = vec4(.0f, 1.0f, 0.0f, 1.0f);
	vec4 m_hovered_color = vec4(.9f, 0.9f, .1f, 1.0f);
	
};