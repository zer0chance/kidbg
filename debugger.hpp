#if !defined(DEBUGGER_HPP)
#define DEBUGGER_HPP

#include <iostream>
#include <sys/types.h>
#include <sys/ptrace.h>
#include <unistd.h>

class Debugger {
public:
  Debugger(std::string name, pid_t pid) : 
    m_prog_name(name), m_pid(pid) {}

  void run();

private:
  std::string m_prog_name;
  pid_t m_pid;
};

#endif
