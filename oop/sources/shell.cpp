
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
    ("a,aiuto","Ottieni informazioni sui comandi.")
    ("p,proporzionale", "Imposta il guadagno proporzionale", cxxopts::value<float>()->default_value("1.0"))
  ;

  RegisterCMD<float>("proporzionale", &ShellCommands::setProportionalGain);
}


void Shell::ProcessParameters(int argc, char* argv[]) {
  init();

  auto result = options.parse(argc, argv);
  auto iter = result.begin();

  for (; iter != result.end(); ++iter) {
    auto callback = callbacks.find(iter->key());
    if (callback != callbacks.end()) {
      callback->second(iter);
    } else {
      std::cerr << "Comando scacolato: " << iter->key() << std::endl;
    }
  }
}