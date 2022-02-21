
#include "LogManager.h"
#include <ctime>
#include <iostream>
namespace TestEngine {
	LogManager& LogManager::operator<<(const std::string& s)
	{
		m_buffer_string += s;
		return *this;
	}
	LogManager& LogManager::operator<<(const MessageCommand& command)
	{
		switch (command)
		{
		case MessageCommand::Show:
			switch (ManagerConsoleOutput)
			{
			case TestEngine::ConsoleType::Cout:
				std::cout << m_buffer_string;
				m_buffer_string.clear();
				break;
			case TestEngine::ConsoleType::Console:
				std::cout << m_buffer_string;
				m_console_string += m_buffer_string;
				m_buffer_string.clear();
				break;
			}
			m_console_string_changed = true;
			break;
		case MessageCommand::Endl:
			m_buffer_string += '\n';
			break;
		}
		return *this;
	}
	LogManager& LogManager::operator<<(const MessageType& type)
	{
		switch (type)
		{
		case MessageType::Debug:
			m_buffer_string += "time";
			break;
		case MessageType::Error:
			m_buffer_string += "Error: ";
			break;
		case MessageType::Info:
			m_buffer_string += "Info: ";
			break;
		}
		return *this;
	}
	LogManager& LogManager::Error()
	{
		return LogManager::Instance() << MessageType::Error;
	}
	LogManager& LogManager::Info()
	{
		return LogManager::Instance() << MessageType::Info;
	}
	LogManager& LogManager::Debug()
	{
		return LogManager::Instance() << MessageType::Debug;
	}
	LogManager& LogManager::Command()
	{
		return LogManager::Instance();
	}
	MessageCommand LogManager::Show()
	{
		return MessageCommand::Show;
	}
	MessageCommand LogManager::Endl()
	{
		return MessageCommand::Endl;
	}
}