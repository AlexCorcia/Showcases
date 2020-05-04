//---------------------------------------------------------------------------
/**
* @file		component
* @author	Alexandre Corcia Aguilera
* @par		Login: alexandre.corcia
* @brief 	File that has the logic for the component based and the camera.
**/
//---------------------------------------------------------------------------
#include "component.h"
#include "object.h"

/* ****************************************
brief: Initialization of the component
**************************************** */
void Camera::initialize()
{
	SM * system_manager = SM::instance();

	m_window = system_manager->m_window_manager->m_window;
	m_target_pos = glm::normalize(vec3(0.0f,0.0f,-1.0f));
	m_up = vec3(0.0f, 1.0f, 0.0f);

	m_mat = glm::lookAt(m_owner->m_transform.m_position,
		m_target_pos,
		m_up);

	m_projection_matrix = glm::perspective(glm::radians(m_fov), 16.0f / 9.0f, 0.01f, 1000.0f);

	m_camera_frustum = frustum(m_projection_matrix * m_mat);
}

/* ****************************************
brief: Updating the component
**************************************** */
void Camera::update()
{
	m_projection_matrix = glm::perspective(glm::radians(m_fov), 16.0f / 9.0f, 0.1f, 1000.0f);

	m_mat = glm::lookAt(m_owner->m_transform.m_position,
		m_target_pos + m_owner->m_transform.m_position,
		m_up);

	m_camera_frustum = frustum(m_projection_matrix * m_mat);
}