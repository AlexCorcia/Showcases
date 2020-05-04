//---------------------------------------------------------------------------
/**
* @file		Window Manager
* @author	Alexandre Corcia Aguilera
* @par		Login: alexandre.corcia
* @brief 	It contains the window .
**/
//---------------------------------------------------------------------------
#pragma once
#include "pch.h"


struct GLFWwindow;
class WindowManager
{
public: //PUBLIC FUNCTIONS
	GLFWwindow * enter();
	void exit();

public:	//PUBLIC VARIABLES

	GLFWwindow * m_window;

private: //PRIVATE FUNCTIONS

private: //PRIVATE VARIABLES


};