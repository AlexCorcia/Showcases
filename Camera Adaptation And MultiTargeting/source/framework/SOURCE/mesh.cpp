//---------------------------------------------------------------------------
/**
* @file		mesh
* @author	Alexandre Corcia Aguilera
* @par		Login: alexandre.corcia
* @brief 	Mesh component is here, loades 
**/
//---------------------------------------------------------------------------
#include "mesh.h"

#define GL_CALL(what) {what; checkGlError();}

inline void checkGlError()
{
	const auto iErr = glGetError();
	if (iErr != GL_NO_ERROR) {
		const char* pszError;
		switch (iErr) {
		case GL_INVALID_ENUM: {
			pszError = "GL_INVALID_ENUM";
			break;
		}
		case GL_INVALID_VALUE: {
			pszError = "GL_INVALID_VALUE";
			break;
		};
		case GL_INVALID_OPERATION: {
			pszError = "GL_INVALID_OPERATION";
			break;
		};
		case GL_INVALID_FRAMEBUFFER_OPERATION: {
			pszError = "GL_INVALID_FRAMEBUFFER_OPERATION";
			break;
		};
		case GL_OUT_OF_MEMORY: {
			pszError = "GL_OUT_OF_MEMORY";
			break;
		};
#if defined(GL_STACK_UNDERFLOW) && defined(GL_STACK_OVERFLOW)
		case GL_STACK_UNDERFLOW: {
			pszError = "GL_STACK_UNDERFLOW";
			break;
		};
		case GL_STACK_OVERFLOW: {
			pszError = "GL_STACK_OVERFLOW";
			break;
		};
#endif
		default: {pszError = "Unknown"; }
		}
	}
}
/* ****************************************
brief: Mesh loader, if not path is passed creates a cube
**************************************** */
Mesh::Mesh()
{
	std::vector<glm::vec3> vertexPositions =
	{
		glm::vec3(-0.5f, 0.5f, -0.5f),
		glm::vec3(0.5f, 0.5f, -0.5f),
		glm::vec3(0.5f, -0.5f, -0.5f),
		glm::vec3(-0.5f, -0.5f, -0.5f),

		glm::vec3(-0.5f, 0.5f, 0.5f),
		glm::vec3(0.5f, 0.5f, 0.5f),
		glm::vec3(0.5f, -0.5f, 0.5f),
		glm::vec3(-0.5f, -0.5f, 0.5f),
	};
	vertices = vertexPositions;


	std::vector<unsigned short> vertexIndices =
	{
		0,1,2,		1,6,2,
		0,2,3,		1,5,6,

		0,3,7,		2,6,7,
		0,7,4,		2,7,3,

		0,4,1,		4,7,6,
		1,4,5,		4,6,5,
	};
	indices = vertexIndices;

	set_up_mesh();

}

/* ****************************************
brief: Load a mesh.
**************************************** */
Mesh::Mesh(std::string filename)
{
	if (!obj_loader(filename))
	{
		std::vector<glm::vec3> vertexPositions =
		{
			glm::vec3(-0.5f, 0.5f, -0.5f),
			glm::vec3(0.5f, 0.5f, -0.5f),
			glm::vec3(0.5f, -0.5f, -0.5f),
			glm::vec3(-0.5f, -0.5f, -0.5f),

			glm::vec3(-0.5f, 0.5f, 0.5f),
			glm::vec3(0.5f, 0.5f, 0.5f),
			glm::vec3(0.5f, -0.5f, 0.5f),
			glm::vec3(-0.5f, -0.5f, 0.5f),
		};
		vertices = vertexPositions;

		std::vector<unsigned short> vertexIndices =
		{
			0,1,2,		1,6,2,
			0,2,3,		1,5,6,

			0,3,7,		2,6,7,
			0,7,4,		2,7,3,

			0,4,1,		4,7,6,
			1,4,5,		4,6,5,
		};
		indices = vertexIndices;
	}

	set_up_mesh();

}

/* ****************************************
brief: Load a mesh with vertices and indexes
**************************************** */
bool Mesh::obj_loader(std::string filename)
{
	std::ifstream inFile(filename);
	if (!inFile.is_open())
		return false;


	while (!inFile.eof())
	{
		std::string line;
		inFile >> line;

		// Read lineHeader
		if (line == "v")
		{
			std::string value;
			glm::vec3 vertex;

			inFile >> value;
			vertex.x = static_cast<float>(std::atof(value.c_str()));
			inFile >> value;
			vertex.y = static_cast<float>(std::atof(value.c_str()));
			inFile >> value;
			vertex.z = static_cast<float>(std::atof(value.c_str()));

			vertices.push_back(vertex);
		}
		else if (line == "f")
		{
			std::string value;
			unsigned short new_indice;

			inFile >> value;
			new_indice = static_cast<unsigned short>(std::atoi(value.c_str()));
			indices.push_back(new_indice - 1);

			inFile >> value;
			new_indice = static_cast<unsigned short>(std::atoi(value.c_str()));
			indices.push_back(new_indice - 1);

			inFile >> value;
			new_indice = static_cast<unsigned short>(std::atoi(value.c_str()));
			indices.push_back(new_indice - 1);
		}
	}

	center_and_normalize();
	set_up_mesh();
	return true;
}

/* ****************************************
brief: centralizes the mesh and it normalizes it.
**************************************** */
void Mesh::center_and_normalize()
{
	vec3 general_center{ 0,0,0 };

	vec3 min{ FLT_MAX };
	vec3 max{ -FLT_MAX };

	for (unsigned i = 0; i < vertices.size(); i++)
	{
		general_center += vertices[i];

		for (unsigned j = 0; j < 3; j++)
		{
			if (vertices[i][j] < min[j])
				min[j] = vertices[i][j];

			if (vertices[i][j] > max[j])
				max[j] = vertices[i][j];
		}
	}
	general_center /= vertices.size();

	for (unsigned i = 0; i < vertices.size(); i++)
		vertices[i] -= general_center;


	float maximum = FLT_MIN;
	for (unsigned j = 0; j < 3; j++)
	{
		if (fabs(min[j]) > maximum)
			maximum = fabs(min[j]);

		if (fabs(max[j]) > maximum)
			maximum = fabs(max[j]);
	}

	for (unsigned i = 0; i < vertices.size(); i++)
		vertices[i] /= maximum;
}


/* ****************************************
brief: Sets up the mesh to be able tto use it in the renderer
**************************************** */
bool Mesh::set_up_mesh()
{
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &IBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0].x, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned short), &indices[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
	glBindVertexArray(0);

	checkGlError();

	return true;
}
