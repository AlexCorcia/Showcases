//---------------------------------------------------------------------------
/**
* @file		Window Manager
* @author	Alexandre Corcia Aguilera
* @par		Login: alexandre.corcia
* @brief 	It contains the window .
**/
//---------------------------------------------------------------------------
#include "WindowManager.h"

#include "GLFW/glfw3.h"

#define WINDOW_WIDTH 1920
#define WINDOW_HEIGHT 1080

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	//glViewport(0, 0, width, height);
}

/* ****************************************
brief: Initializer for the Window Manager
**************************************** */
GLFWwindow * WindowManager::enter()
{
	m_window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Obstacle Avoidance & Multi-Targeting", NULL, NULL);

	if (m_window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return nullptr;
	}

	glfwMakeContextCurrent(m_window);
	glfwSetFramebufferSizeCallback(m_window, framebuffer_size_callback);

	return m_window;
}

void WindowManager::exit()
{
}