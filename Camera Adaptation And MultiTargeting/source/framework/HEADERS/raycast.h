//---------------------------------------------------------------------------
/**
* @file		raycast.h
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
#pragma once
#include "geometry.h"
#include "object.h"

struct GLFWwindow;
class Camera;

Object * raycast_for_mouse(Camera * m_camera, std::vector<Object *> all_objs, GLFWwindow * m_window);
Object * raycast(vec3 starting, vec3 direction, std::vector<Object *>all_objs);