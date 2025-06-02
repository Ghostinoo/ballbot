
#pragma once

#include <stdexcept>

class Returner : public std::exception {
  private:
    std::string msg;
    bool error, terminate;
  public:
    explicit Returner(const std::string& message, bool terminate = false, bool throwError = false)
      : msg(message), error(throwError), terminate(terminate) {}
    const char* what() const noexcept override { return msg.c_str(); }
    bool isError() const noexcept { return error; }
    bool shouldTerminate() const noexcept { return terminate; }
};

void AssignPriority();

void InitializeShell(int argc, char* argv[]);