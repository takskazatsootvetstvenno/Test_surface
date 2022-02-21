#pragma once
#include<functional>
#include<array>
namespace TestEngine
{
	enum class EventType
	{
		WindowResize = 0,
		WindowClose,
		KeyPressed,
		KeyReleased,
		MouseButtonPressed,
		MouseButtonReleased,
		MouseMoved,
		CommandInput,
		Scroll,

		EventsCount
	};
	class BaseEvent 
	{
	public:
		virtual ~BaseEvent() = default;
		virtual EventType get_type() const = 0;
	};
	class EventDispatcher
	{
	public:
		template<typename EventType>
		void add_event_listener(std::function<void(EventType&)> callBack)
		{
			auto baseCallBack = [func = std::move(callBack)](BaseEvent& e)
			{
				func(static_cast<EventType&>(e));
			};
			m_eventCallBacks[static_cast<size_t>(EventType::type)] = std::move(baseCallBack);
		}
		void dispatch(BaseEvent& event)
		{
			auto& callBack = m_eventCallBacks[static_cast<size_t>(event.get_type())];
			callBack(event);
		}
	private:
		std::array<std::function<void(BaseEvent&)>, static_cast<size_t>(EventType::EventsCount)> m_eventCallBacks;
	};
	class EventMouseMoved : public BaseEvent
	{
	public:
		EventMouseMoved(const double new_x, const double new_y)
			:x(new_x), y(new_y)
		{}
		virtual EventType get_type() const override
		{
			return EventType::MouseMoved;
		}
		static const EventType type = EventType::MouseMoved;
		double x, y;
	};
	class EventWindowResize : public BaseEvent
	{
	public:
		EventWindowResize(const unsigned int new_width, const unsigned int new_height)
			:width(new_width), height(new_height)
		{}
		virtual EventType get_type() const override
		{
			return EventType::WindowResize;
		}
		static const EventType type = EventType::WindowResize;
		double width, height;
	};

	class EventKeyPressed : public BaseEvent
	{
	public:
		EventKeyPressed(int key, int scancode, int action, int mods)
			:key(key), scancode(scancode), action(action), mods(mods)
		{}
		virtual EventType get_type() const override
		{
			return EventType::KeyPressed;
		}
		static const EventType type = EventType::KeyPressed;
		int key, scancode, action, mods;
	};

	class EventWindowClose : public BaseEvent
	{
	public:
		virtual EventType get_type() const override
		{
			return EventType::WindowClose;
		}
		static const EventType type = EventType::WindowClose;
	};
	class EventCommandInput : public BaseEvent
	{
	public:
		EventCommandInput(std::string& input_command)
			:command(std::move(input_command)) {}
		EventCommandInput(std::string&& input_command)
			:command(std::move(input_command)) {}
		virtual EventType get_type() const override
		{
			return EventType::CommandInput;
		}
		static const EventType type = EventType::CommandInput;
		std::string command;
	};
	class EventScroll : public BaseEvent
	{
	public:
		EventScroll(const double new_x, const double new_y)
			:x(new_x), y(new_y)
		{}
		virtual EventType get_type() const override
		{
			return EventType::Scroll;
		}
		static const EventType type = EventType::Scroll;
		double x, y;
	};
}