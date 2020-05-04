//---------------------------------------------------------------------------
/**
* @file		object
* @author	Alexandre Corcia Aguilera
* @par		Login: alexandre.corcia
* @brief 	The entities of the engine, witha  component based, a transform and a mesh.
**/
//---------------------------------------------------------------------------
#pragma once
#include "pch.h"
#include "transform.h"
#include "shapes.h"

class Mesh;
class Component;
class Object
{

public:
	Object();
	Object(Mesh * new_mesh, Transform owner = Transform());

	void o_update();
	
	template <typename T> T* do_i_have() const
	{
		for (unsigned i = 0; i < m_components.size(); i++)
		{
			if (dynamic_cast<T*>(m_components[i]))
			{
				T * found = dynamic_cast<T*>(m_components[i]);
				return found;
			}
		}
		return nullptr;
	}
	
	template <typename T>  T* add_component()
	{
		T* check = do_i_have<T>();
		if (check != nullptr)
			return check;
		
		T* new_component(new T);
		
		new_component->m_owner = this;
	
		m_components.push_back(dynamic_cast<Component *>(new_component)	);
		new_component->initialize();
	
		return new_component;
	}

	~Object() {}

public:
	Transform m_transform;
	Mesh * m_mesh;

	vec4 m_color{ 1.0f,0.0f,1.0f,1.0f };
	vec4 m_orginal_color{ 1.0f,0.0f,1.0f,1.0f };

	Object * m_debug_obj{ nullptr };
	aabb m_aabb;

	bool m_input_enabled{ true };
	bool m_to_debug{ true };

private:
	std::vector<Component*> m_components;
};
