//---------------------------------------------------------------------------
/**
* @file		camera_ai
* @author	Alexandre Corcia Aguilera
* @par		Login: alexandre.corcia
* @brief 	File that has logic for multitargeting camera and obstacle avoidance.
**/
//---------------------------------------------------------------------------
#pragma once
#include "pch.h"
#include "component.h"
#include "geometry.h"


class SM;
class OM;
class Renderer;
class Camera_AI : public Component
{
public: //Public Variables
	enum m_targeting_type { none, single, multi};

	m_targeting_type m_targeting = m_targeting_type::none;

	float cam_speed = 2.0f;

	float cam_lerp = 0.05f;

	float cam_height = 3.0f;

	float angle_1 = 10.0f;
	float angle_2 = 10.0f;

	float radius = 9.0f;

	int smart_camera_angle_check = 180;

private: //Private Variables

	Camera * m_camera{nullptr};
	Object * m_cam_obj{ nullptr };
	GLFWwindow * m_window{ nullptr };

	SM * m_system_manager{ nullptr };
	OM * m_object_manager{ nullptr };
	Renderer * m_render_manager{ nullptr };

	vec3 m_targeting_position{ 0.0f,0.0f,0.0f };


public: //Public Functions

	void initialize() override;
	void update() override;


private: //Private Functions

	void targeting(Object * m_target);
	void multi_targeting(std::vector<Object *> m_targets);

	void vision_check();
	float compute_vision_fraction(vec3 pos, aabb to_check);
	float compute_new_position(float v_fraction, aabb to_check);

	void check_best_radius(std::vector<Object *> m_targets, frustum checking);

	void input_camera();
};

class Target : public Component
{
public: //Public Variables
	float degree_of_importance = 0.5f;
private: //Private Variables

public: //Public Functions

	void initialize() override;

	void update() override;


private: //Private Functions
};