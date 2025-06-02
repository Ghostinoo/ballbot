
#include "helpers.hpp"
#include "../oop/headers/shell.hpp"
#include <sys/time.h>
#include <sys/resource.h>
#include <sched.h>
#include <iostream>
#include <cstring>
#include <csignal>
#include <atomic>

extern std::atomic<bool> running;

void AssignPriority() {
  sched_param sch_params;
  sch_params.sched_priority = 99;
  if (
    (setpriority(PRIO_PROCESS, 0, -20) < 0) ||
    (sched_setscheduler(0, SCHED_RR, &sch_params) < 0)
  ) throw Returner("Errore nell'assegnazione della priorità.", true, true);
  std::cout << "Priorità real-time assegnata con successo." << std::endl;
}

void InitializeShell(int argc, char* argv[]) {
  Shell::ProcessParameters(argc, argv);
  signal(SIGINT, [](int signum) {
    std::cout << "\n==================\nRicevuto segnale di interruzione ("<<signum<<"). Terminazione in corso... " << std::endl;
    running = false;
  });
}