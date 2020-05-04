//---------------------------------------------------------------------------
/**
* @file		System Manager
* @author	Alexandre Corcia Aguilera
* @par		Login: alexandre.corcia
* @brief 	its god
**/
//---------------------------------------------------------------------------
#include "Systems.h"

/****
*brief: Function to create check input.
****/
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	(void)mods;
	(void)scancode;

	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GLFW_TRUE);
}


SM* SM::m_SMInstance = nullptr;
/****
*brief: The instance for the System Manager
****/
SM * SM::instance()
{
	if (!m_SMInstance)   // Only allow one instance of class to be generated.
		m_SMInstance = new SM;

	return m_SMInstance;
}

/****
*brief: The starting function for the System Manager, this function will start the window manager too.
****/
void SM::enter()
{
	//Starting the GLFW and the window.
	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//Starting the window manager
	m_window_manager = new WindowManager();
	m_window = m_window_manager->enter();

	//Initiating Glad library
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return;
	}
	//Setting the Key CallBack
	glfwSetKeyCallback(m_window, key_callback);
	glfwMakeContextCurrent(m_window);

	glfwSwapInterval(1);

	//Initializing the Object Manager
	m_renderer = new Renderer();
	m_renderer->enter(m_window);

	m_object_manager = new OM();
	m_object_manager->enter(m_window);
}

/****
*brief: The System Manager is the main loop of the program, this will start all of the rendering and others.
****/
void SM::loop()
{
	while (!glfwWindowShouldClose(m_window))
	{
		glfwSwapBuffers(m_window);
		m_object_manager->loop();
		m_renderer->update();
		glfwPollEvents();
	}

	exit();
}

/****
*brief: Exits all of the SM and the window manager.
****/
void SM::exit()
{
	glfwTerminate();
}