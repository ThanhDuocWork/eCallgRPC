#include "Logger.h"

// Add log state
const std::map<LogState, std::string> m_logState =
{
    {LogState::ALERT,   "[ALERT]"   },
    {LogState::CRIT,    "[CRIT]"    },
    {LogState::DEBUG,   "[DEBUG]"   },
    {LogState::EMERG,   "[EMERG]"   },
    {LogState::ERR,     "[ERR]"     },
    {LogState::INFO,    "[INFO]"    },
    {LogState::NOTICE,  "[NOTICE]"  },
    {LogState::WARNING, "[WARNING]" }
};

void printL(LogState logState, std::string pathSource, std::string funcName, int line, std::string text, ...)
{
    std::string state;
    std::string messagePatter;

    // Use vsnprintf()
    char buffer[1000];
    va_list args;
    va_start(args, text);
    int numChars = vsnprintf(buffer, sizeof(buffer), text.c_str(), args);
    va_end(args);
    if (numChars > 0 && numChars < sizeof(buffer))
    {
        messagePatter = buffer;
    }
    else
    {
        messagePatter = "Error formatting string";
    }

    // Check state log
    auto it = m_logState.find(logState);
    state = it->second;

    // Find and Cut name file
    std::string fileName;
    size_t pos = pathSource.find_last_of("/");
    if(pos == std::string::npos)
    {
        fileName = pathSource;
    }
    else
    {
        fileName = pathSource.substr(pos + 1);
    }
    // Handle format a message 
    std::string messages = state + "[" + fileName + ":" + funcName + "()" + ":" + std::to_string(line) + "] " + messagePatter;
    #ifdef _STD_LOG_
    std::cout << messages << std::endl;
    #else
    // Create log file with ID name is eCall 
    openlog("eCall", LOG_CONS | LOG_PID | LOG_NDELAY, LOG_LOCAL0);
    // Sent log file to system log
    syslog(logState, "%s", messages.c_str());
    // Close log file
    closelog();
    #endif
}