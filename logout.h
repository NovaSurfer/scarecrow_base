//
// Created by max on 6/6/23.
//

#ifndef SC_BASE_LOGOUT_H
#define SC_BASE_LOGOUT_H

#define log_info_cmd(...)                                                                          \
    sc::logging::log(sc::logging::CONSOLE, sc::logging::INFO, __FILE__, __LINE__, __VA_ARGS__)
#define log_info_file(...)                                                                         \
    sc::logging::log(sc::logging::FILE_S, sc::logging::INFO, __FILE__, __LINE__, __VA_ARGS__)
#define log_warn_cmd(...)                                                                          \
    sc::logging::log(sc::logging::CONSOLE, sc::logging::WARN, __FILE__, __LINE__, __VA_ARGS__)
#define log_warn_file(...)                                                                         \
    sc::logging::log(sc::logging::FILE_S, sc::logging::WARN, __FILE__, __LINE__, __VA_ARGS__)
#define log_err_cmd(...)                                                                           \
    sc::logging::log(sc::logging::CONSOLE, sc::logging::ERR, __FILE__, __LINE__, __VA_ARGS__)
#define log_err_file(...)                                                                          \
    sc::logging::log(sc::logging::FILE_S, sc::logging::ERR, __FILE__, __LINE__, __VA_ARGS__)

namespace sc::logging
{
    enum LogLevel
    {
        INFO,
        WARN,
        ERR
    };

    enum LogType
    {
        CONSOLE,
        FILE_S
    };

    static constexpr const char* lvl_names[] {"INFO", "WARNING", "ERROR"};
    static constexpr const char* lvl_colors[] {
        "\x1b[32m", // INFO - green
        "\x1b[33m", // WARNING - yellow
        "\x1b[31m" // ERROR - red
    };

    void log(LogType log_type, LogLevel log_level, const char* file, int line, const char* fmt...);

} // sc

#endif //SC_BASE_LOGOUT_H
