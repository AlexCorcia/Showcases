//---------------------------------------------------------------------------
/**
* @file		interpolations
* @author	Alexandre Corcia Aguilera
* @par		Login: alexandre.corcia
* @brief 	File witha a couple of helper functions for interpolations
**/
//---------------------------------------------------------------------------
#pragma once
#include "pch.h"

float lerp(float start, float end, float t);

vec3 lerp(vec3 start, vec3 end, float t);