
#include "InterpreterError.hpp"

#include <utility>

static std::string buildWhatText(const std::string& file, const std::string& errorType, const std::string& shortMessage, const std::string& longMessage)
{
    std::string str;

    if (!file.empty())
    {
        str += "[" + errorType + "]";
    }

    str += "[" + errorType + "] " + shortMessage;

    if (!longMessage.empty())
    {
        str += " (" + longMessage + ")";
    }

    return str;
}

InterpreterError::InterpreterError(std::string errorType, std::string shortMessage, std::string longMessage, std::string file)
: m_errorType{std::move(errorType)}
, m_shortMessage{std::move(shortMessage)}
, m_longMessage{std::move(longMessage)}
, m_file{std::move(file)}
, m_whatText{buildWhatText(m_file, m_errorType, m_shortMessage, m_longMessage)}
{
}

const char* InterpreterError::what() const
{
    return m_whatText.c_str();
}
