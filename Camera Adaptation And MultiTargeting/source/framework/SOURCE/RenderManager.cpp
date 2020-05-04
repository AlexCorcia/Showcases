//---------------------------------------------------------------------------
/**
* @file		Renderer Manager
* @author	Alexandre Corcia Aguilera
* @par		Login: alexandre.corcia
* @brief 	Renderer all of the objs, and imgui
**/
//---------------------------------------------------------------------------

#include "RenderManager.h"
#include "ObjectManager.h"
#include "Systems.h"
#include "mesh.h"
#include "object.h"
#include "camera_ai.h"
#include "GLFW/glfw3.h"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"




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
brief: Initializer for the Renderer class
**************************************** */
void Renderer::enter(GLFWwindow * c_window)
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); 
	(void)io;

	ImGui::StyleColorsDark();

	//Setup Platform/Renderer bindings
	const char* glsl_version = "#version 130";
	ImGui_ImplGlfw_InitForOpenGL(c_window, true);
	ImGui_ImplOpenGL3_Init(glsl_version);

	init_all_meshes();

	m_window = c_window;

	m_vertex_shader = glCreateShader(GL_VERTEX_SHADER);
	std::string content = read_file("./RESOURCES/SHADERS/basic_shader.vert");
	const char * vertex_shader = content.c_str();

	glShaderSource(m_vertex_shader, 1, &vertex_shader, NULL);
	glCompileShader(m_vertex_shader);
	checkGlError();

	m_fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
	content = read_file("./RESOURCES/SHADERS/basic_shader.frag");
	const char * fragment_shader = content.c_str();

	glShaderSource(m_fragment_shader, 1, &fragment_shader, NULL);
	glCompileShader(m_fragment_shader);
	checkGlError();

	m_program = glCreateProgram();
	glAttachShader(m_program, m_vertex_shader);
	glAttachShader(m_program, m_fragment_shader);
	glLinkProgram(m_program);

	m_mvp_location = glGetUniformLocation(m_program, "MVP");
	m_color_location = glGetUniformLocation(m_program, "COLOR");

	
	
}

/* ****************************************
brief: Will initialize all of the meshes that are added here
**************************************** */
void Renderer::init_all_meshes()
{
	m_all_meshes.push_back(new Mesh("./RESOURCES/MESHES/cube.obj"));         //0
	m_all_meshes.push_back(new Mesh("./RESOURCES/MESHES/bunny.obj"));        //1
	m_all_meshes.push_back(new Mesh("./RESOURCES/MESHES/cylinder.obj"));     //2
	m_all_meshes.push_back(new Mesh("./RESOURCES/MESHES/gourd.obj"));	     //3
	m_all_meshes.push_back(new Mesh("./RESOURCES/MESHES/icosahedron.obj"));  //4
	m_all_meshes.push_back(new Mesh("./RESOURCES/MESHES/octohedron.obj"));	 //5
	m_all_meshes.push_back(new Mesh("./RESOURCES/MESHES/quad.obj"));		 //6
	m_all_meshes.push_back(new Mesh("./RESOURCES/MESHES/segment.obj"));		 //7
	m_all_meshes.push_back(new Mesh("./RESOURCES/MESHES/sphere.obj"));	     //8
	m_all_meshes.push_back(new Mesh("./RESOURCES/MESHES/triangle.obj"));	 //9
	m_all_meshes.push_back(new Mesh("./RESOURCES/MESHES/teapot.obj"));	     //10
}

/* ****************************************
brief: The loop for the renderer, this calles
Imgui , renders Normal and debug objs.
**************************************** */
void Renderer::update()
{
	if (m_om == nullptr)
	{
		SM * system_manager = SM::instance();
		m_om = system_manager->m_object_manager;
	}

	m_camera = m_om->cam_comp;

	int width, height;

	glfwGetFramebufferSize(m_window, &width, &height);

	glViewport(0, 0, width, height);
	glClearColor(0.2f, 0.3f, 0.4f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_DEPTH_TEST);


	//RENDERING ALL OBJS
	for (unsigned i = 0; i < m_om->m_all_objects.size(); i++)
	{

		glm::mat4 mvp = m_om->m_all_objects[i]->m_transform.get_mvp(m_camera);

		glUseProgram(m_program);
		glUniformMatrix4fv(m_mvp_location, 1, GL_FALSE, &mvp[0][0]);

		glBindVertexArray(m_om->m_all_objects[i]->m_mesh->VAO);

		glm::vec4 color = m_om->m_all_objects[i]->m_color;
		glUniform4fv(m_color_location, 1, &color[0]);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glDrawElements(GL_TRIANGLES, GLsizei(m_om->m_all_objects[i]->m_mesh->indices.size()), GL_UNSIGNED_SHORT, 0);

		color = vec4{0.0f, 0.0f, 0.0f, 1.0f };
		glUniform4fv(m_color_location, 1, &color[0]);
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glDrawElements(GL_TRIANGLES, GLsizei(m_om->m_all_objects[i]->m_mesh->indices.size()), GL_UNSIGNED_SHORT, 0);

		glBindVertexArray(0);
	}

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_DEPTH_TEST);


	//RENDERING ALL DEBUG OBJS
	if (m_debug_mode)
	{
		for (unsigned i = 0; i < m_om->m_all_debug_objects.size(); i++)
		{

			glm::mat4 mvp = m_om->m_all_debug_objects[i]->m_transform.get_mvp(m_camera);

			glUseProgram(m_program);
			glUniformMatrix4fv(m_mvp_location, 1, GL_FALSE, &mvp[0][0]);

			glBindVertexArray(m_om->m_all_debug_objects[i]->m_mesh->VAO);

			glm::vec4 color{ m_om->m_all_debug_objects[i]->m_color.x, m_om->m_all_debug_objects[i]->m_color.y, m_om->m_all_debug_objects[i]->m_color.z, 0.2f };
			glUniform4fv(m_color_location, 1, &color[0]);
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			glDrawElements(GL_TRIANGLES, GLsizei(m_om->m_all_debug_objects[i]->m_mesh->indices.size()), GL_UNSIGNED_SHORT, 0);

			color = vec4{ 0.0f, 0.0f, 0.0f, 1.0f };
			glUniform4fv(m_color_location, 1, &color[0]);
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			glDrawElements(GL_TRIANGLES, GLsizei(m_om->m_all_debug_objects[i]->m_mesh->indices.size()), GL_UNSIGNED_SHORT, 0);

			glBindVertexArray(0);
		}
	}


	update_imgui();
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());


}
/* ****************************************
brief: Exit for the Renderer class
**************************************** */
void Renderer::exit()
{

}

/* ****************************************
brief: Updates the Gui and puts the variables in
**************************************** */
void Renderer::update_imgui()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
	
	ImGui::Begin("Editor");
	
	if (ImGui::Button("Add Obstacle"))
		m_om->add_obstacle_scene();

	if (ImGui::Button("Add Entity"))
		m_om->add_entity_scene();

	ImGui::Text(" ");

	if (ImGui::CollapsingHeader("Object"))
	{
		if(m_om->m_selected_object == nullptr)
			ImGui::Text("Please select an object");
		else
		{
			ImGui::Text("Transform");
			ImGui::DragFloat3("Position", &(m_om->m_selected_object->m_transform.m_position.x), 0.1f);
			ImGui::DragFloat3("Scale", &(m_om->m_selected_object->m_transform.m_scale.x), 0.1f);
			ImGui::DragFloat3("Rotation", &(m_om->m_selected_object->m_transform.m_rotation.x), 0.1f);
		}

		ImGui::DragFloat("Target Speed", &m_om->bunny_speed, 0.1f);

	}
	
	if (ImGui::CollapsingHeader("Camera Variables"))
	{



		if (m_smart_camera == nullptr)
			m_smart_camera = m_camera->m_owner->do_i_have<Camera_AI>();


		ImGui::Text("Targeting");
		
		//TARGETING
		int target_type = static_cast<int>(m_smart_camera->m_targeting);
		ImGui::Combo("Targeting Type", &target_type, "No Target\0Single Target\0Multi Targeting\0");
		m_smart_camera->m_targeting = static_cast<Camera_AI::m_targeting_type>(target_type);
		ImGui::Text(" ");

		if (m_smart_camera->m_targeting == Camera_AI::m_targeting_type::single)
		{
			ImGui::Text("Obstacle Avoidance Variables");
			ImGui::DragInt("Checking Angle", &m_smart_camera->smart_camera_angle_check);
			ImGui::DragFloat("Camera Lerp", &m_smart_camera->cam_lerp, 0.01f);

			ImGui::DragFloat("Camera Radius", &m_smart_camera->radius, 0.1f);
			ImGui::DragFloat("Camera Height", &m_smart_camera->cam_height, 0.1f);

		}
		else if (m_smart_camera->m_targeting == Camera_AI::m_targeting_type::multi)
		{
			ImGui::Text("Multi Targeting Variables");
			ImGui::DragFloat("Camera Angle", &m_smart_camera->angle_1, 0.1f);
			ImGui::DragFloat("Camera Height", &m_smart_camera->cam_height, 0.1f);
			ImGui::DragFloat("Camera Lerp", &m_smart_camera->cam_lerp, 0.01f);
		}
	}
	
	if (ImGui::CollapsingHeader("Debugging"))
	{
		ImGui::Text(" ");
		ImGui::Text("FPS: %f", ImGui::GetIO().Framerate);
		ImGui::Text(" ");
		ImGui::Checkbox("Debug Mode", &m_debug_mode);
	}
	
	ImGui::End();
}

/* ****************************************
brief: File reader returns a string with the file's contents.
**************************************** */
std::string Renderer::read_file(std::string filename)
{
	std::ifstream inFile(filename, std::ios::in);

	if (!inFile)
		return std::string();

	std::string content((std::istreambuf_iterator<char>(inFile)),
		(std::istreambuf_iterator<char>()));

	inFile.close();

	return content;
}

