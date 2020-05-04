//---------------------------------------------------------------------------
/**
* @file		interpolations
* @author	Alexandre Corcia Aguilera
* @par		Login: alexandre.corcia
* @brief 	File witha a couple of helper functions for interpolations
**/
//---------------------------------------------------------------------------
#include "interpolations.h"

/* ****************************************
brief: Lerping function with floats (linear)
**************************************** */
float lerp(float start, float end, float t)
{
	return start + (end - start) * t;
}
/* ****************************************
brief: Lerping function with vectors (linear)
**************************************** */
vec3 lerp(vec3 start, vec3 end, float t)
{
	return start + (end - start) * t;
}
