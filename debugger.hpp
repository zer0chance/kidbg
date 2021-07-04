#if !defined(DEBUGGER_HPP)
#define DEBUGGER_HPP

#include "breakpoint.hpp"

#include <unordered_map>
#include <iostream>

#include <sys/ptrace.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

class Debugger {
public:
  Debugger(std::string name, pid_t pid) : 
    m_prog_name(name), m_pid(pid) {}

  void run();
  void set_breakpoint_at_address(std::intptr_t addr);

private:
  void continue_execution();
  void handle_command(const std::string&);

  std::string m_prog_name;
  pid_t m_pid;
  std::unordered_map<std::intptr_t, Breakpoint> m_breakpoints;
};

#endif
