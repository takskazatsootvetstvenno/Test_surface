#include "Application.h"
#include "Window.h"
#include "LogManager.h"
#include "ResourceManager.h"

namespace TestEngine
{
Application::Application()
{
	LogManager::Info() << "App has been created!\n"<< LogManager::Show();
}

Application::~Application()
{
	LogManager::Info() << "App has been destroed!\n" << LogManager::Show();
}
int Application::start(const unsigned int width, const unsigned int height, std::string title)
{
	m_pWindow = std::make_unique<Window>(width, height, std::move(title));

	m_event_dispather.add_event_listener <EventMouseMoved>(
		[](EventMouseMoved& event)
		{
		//	LogManager::Info() << "[MouseMoved] Mouse moved to " << event.x << " x " << event.y << LogManager::Show();
		}
	);

	m_event_dispather.add_event_listener <EventScroll>(
		[&](EventScroll& event)
		{
			//LogManager::Info() << "[Scroll] y = " << event.y << "\n"<< LogManager::Show();
			m_pWindow->addCameraDistance(static_cast<float>(event.y));
		}
	);

	m_event_dispather.add_event_listener <EventKeyPressed>(
		[&](EventKeyPressed& event)
		{
			switch (event.key)
			{
			case 69:	//E
				m_pWindow->addCameraRotation(static_cast<float>(0.05));
				break;
			case 83:	//S
				m_pWindow->addCameraDisplacement(static_cast<float>(-0.5));
				break;
			case 87:	//W
				m_pWindow->addCameraDisplacement(static_cast<float>(0.5));
				break;
			case 81:	//Q
				m_pWindow->addCameraRotation(static_cast<float>(-0.05));
				break;
			case 256:	//GLFW_KEY_ESCAPE
				m_bCloseWindow = true;
				break;
			}
		}
	);

	m_event_dispather.add_event_listener <EventWindowResize>(
		[](EventWindowResize& event)
		{
			LogManager::Info() << "[Resized] Changing size to " << event.width << " x " << event.height << LogManager::Show();
		}
	);
	m_event_dispather.add_event_listener <EventWindowClose>(
		[&](EventWindowClose& event)
		{
			LogManager::Info() << "[WindowClose]\n" << LogManager::Show();
			m_bCloseWindow = true;
		}
	);
	m_pWindow->set_event_callback(
		[&](BaseEvent& event)
		{
			m_event_dispather.dispatch(event);
		}
	);
	while (!m_bCloseWindow)
	{
		m_pWindow->onUpdate();
	}
	
	m_pWindow = nullptr;
	return 0;
}
}
