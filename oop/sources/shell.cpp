
#include "../headers/shell.hpp"
#include <iostream>

cxxopts::Options Shell::options("bbot", "Programma di controllo e gestione di BB8-V2");
std::unordered_map<std::string, Shell::CallbackFunction> Shell::callbacks = {};

template <typename T>
void RegisterCMD(const std::string& name, void (*func)(const T)) {
  Shell::callbacks[name] = [func](cxxopts::ParseResult::Iterator iter) {
    func(iter->as<T>());
  };
}


void Shell::init() {
  options.add_options()
    ("k,kill","Ferma i motori istantaneamente.", cxxopts::value<bool>()->default_value("false"))
    ("c,calibrate","Calibra il dispositivo prima di partire.", cxxopts::value<bool>()->default_value("false"))
    ("p,prop", "Imposta il guadagno proporzionale", cxxopts::value<float>()->default_value("1.0"))
  ;

  RegisterCMD<bool>("calibrate", &ShellCommands::calibrate);
  RegisterCMD<bool>("kill", &ShellCommands::stopMotors);
  RegisterCMD<float>("prop", &ShellCommands::setProportionalGain);
}


void Shell::ProcessParameters(int argc, char* argv[]) {
  init();
  try {
    auto result = options.parse(argc, argv);
    auto iter = result.begin();

    for (; iter != result.end(); ++iter) {
      auto callback = callbacks.find(iter->key());
      if (callback != callbacks.end()) {
        callback->second(iter);
      } else {
        std::cerr << "Comando non associato ad un'azione: " << iter->key() << std::endl;
      }
    }
  } catch (const cxxopts::exceptions::exception &e) {
    throw Returner(e.what(), true, true);
  }
}