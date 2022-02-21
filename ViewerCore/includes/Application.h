#pragma once
#include <string>
#include <memory>
#include "Event.h"

namespace TestEngine {
	class Window;

	class Application
	{
	public:
		Application();
		~Application();
		Application(const Application& app) = delete;
		Application(Application&&) = delete;
		Application& operator=(const Application) = delete;
		Application& operator=(Application&&) = delete;
		int start(const unsigned int width, const unsigned int height, std::string title);
	private:
		std::unique_ptr<class Window> m_pWindow;

		EventDispatcher m_event_dispather;
		bool m_bCloseWindow = false;
	};
}
