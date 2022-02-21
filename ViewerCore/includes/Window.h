#pragma once
#include<string>
#include<functional>
#include "Surface.h"
#include"Event.h"
#include"glm/glm.hpp"
struct GLFWwindow;
namespace TestEngine {
	struct CameraData
	{
		float distance = 10.f;
		float rotation = 0.f;
		float displacement = 2.f;
		glm::mat4 projection{ 1.0f };
		glm::mat4 model{ 1.0f };
	};

	class Window
	{
	public:
		using EventCallbackFn = std::function<void(BaseEvent&)>;
		~Window();
		Window(const unsigned int width, const unsigned int height, std::string title);
		Window(const Window&) = delete;
		Window(Window&&) = delete;
		Window& operator=(const Window&) = delete;
		Window& operator=(Window&&) = delete;
		size_t getWidth() const noexcept;
		size_t getHeight() const noexcept;
		void onUpdate() noexcept;
		void addCameraDistance(float newValue) noexcept;
		void addCameraRotation(float newValue) noexcept;
		void addCameraDisplacement(float newValue) noexcept;
		float getCameraDistance() const noexcept;
		float getCameraRotation() const noexcept;
		float getCameraDisplacement() const noexcept;
		void set_event_callback(const EventCallbackFn& callback)
		{
			m_data.eventCallbackFn = callback;
		}

	private:
		struct WindowData
		{
			unsigned int m_width;
			unsigned int m_height;
			std::string m_title;
			EventCallbackFn eventCallbackFn;
		};
		void init();
		WindowData m_data;
		GLFWwindow* m_pWindow = nullptr;
		CameraData m_CameraData;
		std::unique_ptr<Surface> m_pSurface;
		SurfaceData m_surfData{};
	};
}
