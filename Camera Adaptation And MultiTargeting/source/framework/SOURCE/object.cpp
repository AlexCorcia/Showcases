//---------------------------------------------------------------------------
/**
* @file		object
* @author	Alexandre Corcia Aguilera
* @par		Login: alexandre.corcia
* @brief 	The entities of the engine, witha  component based, a transform and a mesh.
**/
//---------------------------------------------------------------------------
#include "object.h"
#include "component.h"
/* ****************************************
brief: Object Constructor
**************************************** */
Object::Object()
{
	this->m_transform = Transform();
	this->m_mesh = nullptr;
}

/* ****************************************
brief: Object Constructor
**************************************** */
Object::Object(Mesh * new_mesh, Transform owner)
{
	this->m_transform = owner;
	this->m_mesh = new_mesh;
}

/* ****************************************
brief: Updates the componenents of the Object
**************************************** */
void Object::o_update()
{
	for (auto & i : m_components) i->update();
}
