#if !defined(DEBUGGER_HPP)
#define DEBUGGER_HPP

#include <iostream>
#include <string>

#include <sys/ptrace.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

class Debugger {
public:
  Debugger(std::string name, pid_t pid) : 
    m_prog_name(name), m_pid(pid) {}

  void run();

private:
  void continue_execution();
  void handle_command(const std::string&);

  std::string m_prog_name;
  pid_t m_pid;
};

#endif
