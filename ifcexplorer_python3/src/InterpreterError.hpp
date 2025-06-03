#pragma once

#include <exception>
#include <string>

class InterpreterError : public std::exception
{
  public:
    InterpreterError(std::string errorType, std::string shortMessage, std::string longMessage = {}, std::string file = {});
    ~InterpreterError() override = default;
    const char* what() const override;

  private:
    std::string m_errorType;
    std::string m_shortMessage;
    std::string m_longMessage;
    std::string m_file;
    std::string m_whatText;
};
