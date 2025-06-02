
#pragma once

#include "../../libraries/cxxopts.hpp"
#include <unordered_map>
#include <functional>

class Shell {
  private:
    static cxxopts::Options options;

    static void init();

    Shell() {}
    ~Shell() {}

  public:

    typedef std::function<void(cxxopts::ParseResult::Iterator)> CallbackFunction;
    static std::unordered_map<std::string, CallbackFunction> callbacks;

    static void ProcessParameters(int argc, char* argv[]);

};

namespace ShellCommands {
  void setProportionalGain(float gain);
  void stopMotors(bool kill);
  void calibrate(bool calibrate);
}