//---------------------------------------------------------------------------
/**
* @file		transform
* @author	Alexandre Corcia Aguilera
* @par		Login: alexandre.corcia
* @brief 	all objs have one, its also has some computatiosn like mvp and model.
**/
//---------------------------------------------------------------------------
#pragma once
#include "pch.h"


class Camera;
class Transform
{
public:
	Transform();
	Transform(vec3 pos , vec3 sca, vec3 rot);

	glm::mat4 get_mvp(Camera * cam);
	glm::mat4 get_model();

public:
	glm::vec3 m_position;
	glm::vec3 m_scale;
	glm::vec3 m_rotation;
};