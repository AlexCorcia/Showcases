//---------------------------------------------------------------------------
/**
* @file		mesh
* @author	Alexandre Corcia Aguilera
* @par		Login: alexandre.corcia
* @brief 	Mesh component is here, loades .obj files
**/
//---------------------------------------------------------------------------
#pragma once
#include "pch.h"
#include <glad/glad.h>

class Mesh
{
public: //PUBLIC FUNCTIONS
	Mesh();
	Mesh(std::string filename);

public: //PUBLIC VARIABLES
	GLuint VAO; //Vertex Array Object
	GLuint VBO;	//Vertex Buffer Object
	GLuint IBO; //Index buffer Object

	std::vector<glm::vec3> vertices;
	std::vector<unsigned short> indices;

private: //PRIVATE FUNCTIONS
	bool obj_loader(std::string filename);
	void center_and_normalize();
	bool set_up_mesh();

private: //PRIVATE VARIABLES
	

};