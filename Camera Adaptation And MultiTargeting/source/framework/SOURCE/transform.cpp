//---------------------------------------------------------------------------
/**
* @file		transform
* @author	Alexandre Corcia Aguilera
* @par		Login: alexandre.corcia
* @brief 	all objs have one, its also has some computatiosn like mvp and model.
**/
//---------------------------------------------------------------------------
#include "transform.h"
#include "component.h"

/* ****************************************
brief: COnstructor for the transform class
**************************************** */
Transform::Transform()
{
	this->m_position = glm::vec3(0.0, 0.0, 0.0);
	this->m_scale = glm::vec3(1.0, 1.0, 1.0);
	this->m_rotation = glm::vec3(0.0, 0.0, 0.0);
}

/* ****************************************
brief: COnstructor for the transform class
**************************************** */
Transform::Transform(vec3 pos, vec3 sca, vec3 rot)
{
	this->m_position = pos;
	this->m_scale = sca;
	this->m_rotation = rot;
}

/* ****************************************
brief: This will return the mvp of the obj depending on the camera passed
**************************************** */
glm::mat4 Transform::get_mvp(Camera * cam)
{
	glm::mat4 proj = cam->get_projection_mat();
	glm::mat4 trans = glm::translate(glm::mat4(1.0f), this->m_position);

	glm::mat4 rot_x = glm::rotate(trans, this->m_rotation.x, glm::vec3(1.0, 0.0, 0.0));
	glm::mat4 rot_y = glm::rotate(rot_x, this->m_rotation.y, glm::vec3(0.0, 1.0, 0.0));
	glm::mat4 rot_z = glm::rotate(rot_y, this->m_rotation.z, glm::vec3(0.0, 0.0, 1.0));

	glm::mat4 model = glm::scale(glm::mat4(1.0f), this->m_scale);

	if (cam->get_cam_mat() == mat4(1))
		return (proj * rot_z * model);
	else
		return (proj * cam->get_cam_mat() * rot_z * model);
}

/* ****************************************
brief: This will return the model matrix of the obj
**************************************** */
glm::mat4 Transform::get_model()
{
	glm::mat4 trans = glm::translate(glm::mat4(1.0f), this->m_position);

	glm::mat4 rot_x = glm::rotate(trans, this->m_rotation.x, glm::vec3(1.0, 0.0, 0.0));
	glm::mat4 rot_y = glm::rotate(rot_x, this->m_rotation.y, glm::vec3(0.0, 1.0, 0.0));
	glm::mat4 rot_z = glm::rotate(rot_y, this->m_rotation.z, glm::vec3(0.0, 0.0, 1.0));

	glm::mat4 model = glm::scale(glm::mat4(1.0f), this->m_scale);

	return(rot_z * model);
}