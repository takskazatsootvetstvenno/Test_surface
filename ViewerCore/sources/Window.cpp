#include <GL/glew.h>

#include <memory>
#include "Window.h"
#include "ResourceManager.h"
#include "ShaderProgram.h"
#include "VertexArray.h"

#include "IndexBuffer.h"
#include "LogManager.h"
#include "UniformBuffer.h"

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <imgui.h>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_glfw.h>

namespace TestEngine
{
	void APIENTRY glDebugOutput(GLenum source, GLenum type, unsigned int id, GLenum severity, GLsizei length,
		const char* message, const void* userParam) {
		if (id == 131169 ||
			id == 131185 ||
			id == 131204 ||
			id == 131218)return;
			
		LogManager::Error() << "---------------\n";
		LogManager::Error() << "OpenGL debug message (" << id << "): " << message << LogManager::Show();

		switch (source) {
		case GL_DEBUG_SOURCE_API:
			LogManager::Error() << "Source: API";
			break;
		case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
			LogManager::Error() << "Source: Window System";
			break;
		case GL_DEBUG_SOURCE_SHADER_COMPILER:
			LogManager::Error() << "Source: Shader Compiler";
			break;
		case GL_DEBUG_SOURCE_THIRD_PARTY:
			LogManager::Error() << "Source: Third Party";
			break;
		case GL_DEBUG_SOURCE_APPLICATION:
			LogManager::Error() << "Source: Application";
			break;
		case GL_DEBUG_SOURCE_OTHER:
			LogManager::Error() << "Source: Other";
			break;
		}
		LogManager::Error() << "\n" << LogManager::Show();

		switch (type) {
		case GL_DEBUG_TYPE_ERROR:
			LogManager::Error() << "Type: Error";
			break;
		case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
			LogManager::Error() << "Type: Deprecated Behaviour";
			break;
		case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
			LogManager::Error() << "Type: Undefined Behaviour";
			break;
		case GL_DEBUG_TYPE_PORTABILITY:
			LogManager::Error() << "Type: Portability";
			break;
		case GL_DEBUG_TYPE_PERFORMANCE:
			LogManager::Error() << "Type: Performance";
			break;
		case GL_DEBUG_TYPE_MARKER:
			LogManager::Error() << "Type: Marker";
			break;
		case GL_DEBUG_TYPE_PUSH_GROUP:
			LogManager::Error() << "Type: Push Group";
			break;
		case GL_DEBUG_TYPE_POP_GROUP:
			LogManager::Error() << "Type: Pop Group";
			break;
		case GL_DEBUG_TYPE_OTHER:
			LogManager::Error() << "Type: Other";
			break;
		}
		LogManager::Error() << "\n"<< LogManager::Show();

		switch (severity) {
		case GL_DEBUG_SEVERITY_HIGH:
			LogManager::Error() << "Severity: high";
			break;
		case GL_DEBUG_SEVERITY_MEDIUM:
			LogManager::Error() << "Severity: medium";
			break;
		case GL_DEBUG_SEVERITY_LOW:
			LogManager::Error() << "Severity: low";
			break;
		case GL_DEBUG_SEVERITY_NOTIFICATION:
			LogManager::Error() << "Severity: notification";
			break;
		}
		LogManager::Error() << "\n\n" << LogManager::Show();
	}
		
	Window::Window(const unsigned int width, const unsigned int height, std::string title)
		:m_data({ width, height, std::move(title)})
	{
		init();
	
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGui_ImplOpenGL3_Init("#version 460");
		ImGui_ImplGlfw_InitForOpenGL(m_pWindow, true);
		glDisable(GL_CULL_FACE);
	}

	void Window::init()
	{
		if (!glfwInit()) LogManager::Error() << "Can't initialize GLFW!!" << LogManager::Show(); 

		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	#ifdef CMAKE_BUILD_TYPE_DEBUG
        glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);  // DEBUG
	#endif

        m_pWindow = glfwCreateWindow(m_data.m_width, m_data.m_height, m_data.m_title.c_str(), nullptr, nullptr);
		if (!m_pWindow) {
			LogManager::Error() << "\nCan't create Window!!" << LogManager::Show();
			glfwTerminate();
		}
		glfwMakeContextCurrent(m_pWindow);
		glfwSwapInterval(0);
		glewExperimental = GL_TRUE;
		if (glewInit() != GLEW_OK)  LogManager::Error() << "Can't initialize GLEW!" << LogManager::Show();

	#ifdef CMAKE_BUILD_TYPE_DEBUG
        glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);  // DEBUG
        int flags;
        glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
        if (flags & GL_CONTEXT_FLAG_DEBUG_BIT) {
            glEnable(GL_DEBUG_OUTPUT);
            glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
            glDebugMessageCallback(glDebugOutput, nullptr);
            glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
        }
	#endif

        glfwSetWindowUserPointer(m_pWindow, &m_data);
		glfwSetWindowSizeCallback(m_pWindow,
			[](GLFWwindow* pWindow, int width, int height)
			{
				WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(pWindow));
				data.m_width = width;
				data.m_height = height;
				EventWindowResize event(width, height);
				data.eventCallbackFn(event);
			});
		glfwSetCursorPosCallback(m_pWindow, [](GLFWwindow* pWindow, double x, double y) {
			WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(pWindow));

			EventMouseMoved event(x, y);
			data.eventCallbackFn(event);
			});
		glfwSetScrollCallback(m_pWindow, [](GLFWwindow* pWindow, double x, double y) {
			WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(pWindow));

			EventScroll event(x, y);
			data.eventCallbackFn(event);
			});
		glfwSetKeyCallback(m_pWindow, [](GLFWwindow* pWindow, int key, int scancode, int action, int mods) {
			WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(pWindow));

			EventKeyPressed event(key, scancode, action, mods);
			data.eventCallbackFn(event);
			});
		glfwSetWindowCloseCallback(m_pWindow, [](GLFWwindow* pWindow) {
			WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(pWindow));

			EventWindowClose event;
			data.eventCallbackFn(event);
			});

		glfwSetFramebufferSizeCallback(m_pWindow, [](GLFWwindow* pWindow, int width, int height) {
			glViewport(0, 0, width, height);
			});
		BufferLayout buffer_layout_2vec3
		{
			ShaderDataType::Float3,
			ShaderDataType::Float3,
		};
		
		ResourceManager::Instance().getUBO(ResourceManager::GLOBAL_UBO::GENERAL_MATRICES).updateElementData(
			[&](glm::mat4& m) {
				m = glm::rotate(m, glm::radians(90.0f),
					glm::vec3(1.0, 0.0, 0.0));
				return glm::value_ptr(m);
			}(m_CameraData.model), 2);

		glClearColor(0.f, 0.f, 0.f, 0.f);
		m_pSurface = std::make_unique<Surface>(100);
		m_pSurface->updateSurfaceVB(70, 0.9f);
		const auto& UBO = ResourceManager::Instance().getUBO(ResourceManager::GLOBAL_UBO::LIGHT_INFO);
		UBO.updateData(&m_surfData.new_light_pos);
		glEnable(GL_DEPTH_TEST);
	}
	void Window::onUpdate() noexcept
	{
		glfwPollEvents();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		if (m_surfData.old_useWireFrames != m_surfData.new_useWireFrames)
		{
			if(m_surfData.new_useWireFrames)
				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			else
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			m_surfData.old_useWireFrames = m_surfData.new_useWireFrames;
		}
		if (m_surfData.new_light_pos != m_surfData.old_light_pos || m_surfData.new_surface_color != m_surfData.old_surface_color)
		{
			const auto& UBO = ResourceManager::Instance().getUBO(ResourceManager::GLOBAL_UBO::LIGHT_INFO);
			Surface::SurfaceUBOfragmentData temp_data {
				glm::fvec4(m_surfData.new_light_pos, 1.f),
				glm::fvec4(m_surfData.new_surface_color, 1.f)
			};
			UBO.updateData(&temp_data);
			m_surfData.old_light_pos = m_surfData.new_light_pos;
			m_surfData.old_surface_color = m_surfData.new_surface_color;
		}
		if (m_surfData.isChanged())
		{
			m_pSurface->updateSurfaceVB(m_surfData.new_grid_size, m_surfData.new_const);
			m_surfData.update();
		}
		ImGuiIO& io = ImGui::GetIO();
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		ImGui::Begin("Surface options:");
		ImGui::SliderFloat("Camera height", &m_CameraData.displacement, -5.f, 5.f);
		ImGui::SliderFloat("Const", &m_surfData.new_const, 0.4f, 1.9f, "%0.5f");
		ImGui::SliderInt("Grid size", &m_surfData.new_grid_size, 5, 100, "%0.3d");
		ImGui::SliderFloat3("Light pos", (float*)&m_surfData.new_light_pos, -10.f, 10.f, "%0.3f");
		ImGui::SliderFloat3("Surface color", (float*)&m_surfData.new_surface_color, 0.f, 1.f, "%0.3f");
		ImGui::Checkbox("Use wireframe", &m_surfData.new_useWireFrames);
		const auto& UBO = ResourceManager::Instance().getUBO(ResourceManager::GLOBAL_UBO::GENERAL_MATRICES);
		UBO.updateElementData([&]() {
			m_CameraData.projection = glm::perspective(45.0,
				static_cast<double>((m_data.m_width * 1.f) / m_data.m_height),
				0.1, 50.0);
			return glm::value_ptr(m_CameraData.projection);
			}(), 0);
		UBO.updateElementData([&](float t, float r, float displacement) {//view
			m_CameraData.projection = glm::lookAt(glm::vec3(r * cos(t), r * sin(t), displacement),
					glm::vec3(0, 0, 0),
					glm::vec3(0, 0, 1));
				return glm::value_ptr(m_CameraData.projection);
				}(getCameraRotation(), getCameraDistance(), m_CameraData.displacement), 1);

		for (const auto& mesh : ResourceManager::Instance().getMeshes())
		{
			mesh.get_VAO().bind();
			ResourceManager::Instance().getShaderProgram(mesh.get_shaderName()).bind();
			if(mesh.get_shaderName()[0] == 'R')
				glDrawArrays(mesh.get_renderType(), 0, m_pSurface->count());
			else
				glDrawArrays(mesh.get_renderType(), 0, static_cast<GLsizei>(mesh.get_VAO().get_indices_count()));
		}
		ImGui::NewLine();
		ImGui::End();
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		glfwSwapBuffers(m_pWindow);
		
	}
	void Window::addCameraDistance(float newValue) noexcept
	{
		if (m_CameraData.distance - newValue <= 0 || m_CameraData.distance - newValue >= 10) return;
		m_CameraData.distance -= newValue;
	}
	void Window::addCameraRotation(float newValue) noexcept
	{
		m_CameraData.rotation += newValue;
	}
	void Window::addCameraDisplacement(float newValue) noexcept
	{
		if (m_CameraData.displacement + newValue < -5 || m_CameraData.displacement + newValue > 5) return;
		m_CameraData.displacement += newValue;
	}
	float Window::getCameraDistance() const noexcept
	{
		return m_CameraData.distance;
	}
	float Window::getCameraRotation() const noexcept
	{
		return m_CameraData.rotation;
	}

	float Window::getCameraDisplacement() const noexcept
	{
		return m_CameraData.displacement;
	}

	Window::~Window()
	{
		glfwDestroyWindow(m_pWindow);
		glfwTerminate();
	}

	size_t Window::getWidth() const noexcept
	{
		return m_data.m_width;
	}

	size_t Window::getHeight() const noexcept
	{
		return m_data.m_height;
	}
}


