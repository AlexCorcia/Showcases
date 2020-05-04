//---------------------------------------------------------------------------
/**
* @file		component
* @author	Alexandre Corcia Aguilera
* @par		Login: alexandre.corcia
* @brief 	File that has the logic for the component based and the camera.
**/
//---------------------------------------------------------------------------
#pragma once
#include "pch.h"
#include "Systems.h"
#include "shapes.h"
#include "GLFW/glfw3.h"

class Object;
class Component
{

public:
	Object * m_owner = nullptr;
	
	virtual void initialize() {}
	virtual void update() {}
	
	virtual ~Component() {}


private:
};

class Camera : public Component
{
public: // Variables
	float m_fov = 45.0f;

	float m_far = 200.0f;
	float m_near = 0.5f;

	vec3 m_target_pos;
	vec2 cursor_pos;
	vec3 m_up;

	frustum m_camera_frustum;

public: // Functions

	void initialize() override;
	void update() override;

	mat4 get_cam_mat() { return m_mat; }
	mat4 get_projection_mat() { return m_projection_matrix; }


private: // Variables

	mat4 m_mat;
	mat4 m_projection_matrix;
	mat4 m_view_matrix;

	GLFWwindow * m_window{ nullptr };


private: // Functions

};