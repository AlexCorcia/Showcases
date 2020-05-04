//---------------------------------------------------------------------------
/**
* @file		System Manager
* @author	Alexandre Corcia Aguilera
* @par		Login: alexandre.corcia
* @brief 	its god
**/
//---------------------------------------------------------------------------
#pragma once
#include "WindowManager.h"
#include "RenderManager.h"
#include "ObjectManager.h"

#include "GLFW/glfw3.h"
#include "glad/glad.h"

class SM
{
public: //PUBLIC FUNCTIONS
	void enter();
	void loop();
	void exit();

	static SM * instance();

public: //PUBLIC VARIABLES

	WindowManager * m_window_manager;
	Renderer * m_renderer;
	OM * m_object_manager;

private: //PRIVATE FUNCTIONS
	SM() {};
	SM( SM const & ) {};
	SM& operator= (SM const&) {};
	static SM * m_SMInstance;

private: //PRIVATE VARIABLES

	GLFWwindow * m_window;
};
