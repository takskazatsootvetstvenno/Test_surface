#pragma once
#include <string>
namespace TestEngine {
    enum class MessageCommand { Show = 0, Endl };
    enum class MessageType { Debug = 0, Info, Error, Command };
    enum class ConsoleType { Cout = 0, Console};
    class LogManager {
    public:
        static LogManager& Instance() {
            static LogManager manager;
            return manager;
        }
        template<typename T, std::enable_if_t<std::is_arithmetic_v<T>, bool> = true>
        LogManager& operator<<(const T& value) {
            m_buffer_string += std::to_string(value);
            return *this;
        }
        LogManager& operator<<(const std::string& s);
        LogManager& operator<<(const MessageCommand& command);
        LogManager& operator<<(const MessageType& type);

        static LogManager& Error();
        static LogManager& Info();
        static LogManager& Debug();
        static LogManager& Command();
        static MessageCommand Show();
        static MessageCommand Endl();

        LogManager(const LogManager&) = delete;
        LogManager(LogManager&&) = delete;
        LogManager& operator=(const LogManager&) = delete;
        LogManager& operator=(LogManager&&) = delete;

        std::string& get_console_string() noexcept{
            return m_console_string;
        }
        ConsoleType ManagerConsoleOutput = ConsoleType::Console;
        bool m_console_string_changed = false;
    private:
        std::string m_buffer_string;
        std::string m_console_string;
        LogManager(){};
        ~LogManager(){};
    };
}  // namespace TestEngine
