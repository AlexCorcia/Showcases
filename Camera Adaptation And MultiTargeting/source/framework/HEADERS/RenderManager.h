//---------------------------------------------------------------------------
/**
* @file		Renderer Manager
* @author	Alexandre Corcia Aguilera
* @par		Login: alexandre.corcia
* @brief 	Renderer all of the objs, and imgui
**/
//---------------------------------------------------------------------------
#pragma once
#include "pch.h"
#include "glad/glad.h"



class OM; 
class Mesh;
class Camera;
class Camera_AI;

struct GLFWwindow;
class Renderer
{

public: //PUBLIC FUNCTIONS
	void enter(GLFWwindow * c_window);
	void update();
	void exit();

	

public: //PUBLIC VARIABLES
	std::vector<Mesh * > m_all_meshes;

private: //PRIVATE FUNCTIONS
	void init_all_meshes();
	void update_imgui();
	std::string read_file(std::string filename);

private: //PRIVATE VARIABLES
	GLFWwindow * m_window;
	OM * m_om;

	Camera * m_camera = nullptr;
	Camera_AI * m_smart_camera = nullptr;

	//shaders and programs
	GLuint m_vertex_shader, m_fragment_shader;
	GLuint m_program;

	//Location
	GLint m_mvp_location;
	GLint m_color_location;

	//IMGUI and Modes
	bool m_debug_mode = false;



	
};