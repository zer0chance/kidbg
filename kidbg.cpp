#include "debugger.hpp"
#include "linenoise.h"

#include <vector>
#include <sstream>

std::vector<std::string> split(const std::string& s, char delim) {
  std::vector<std::string> out;
  std::stringstream ss{s};
  std::string item;

  while (std::getline(ss, item, delim)) {
    out.push_back(item);
  }

  return out;
}

bool is_prefix(const std::string& s, const std::string& of) {
  if (s.size() > of.size()) return false;
  return std::equal(s.begin(), s.end(), of.begin());
}

void Debugger::continue_execution() {
  ptrace(PTRACE_CONT, m_pid, nullptr, nullptr);
  int wait_status;
  auto options = 0;
  waitpid(m_pid, &wait_status, options);
}

void Debugger::handle_command(const std::string& line) {
  auto args = split(line, ' ');
  auto command = args[0];

  if (is_prefix(command, "continue")) {
    continue_execution();
  } else if (is_prefix(command, "quit")) {
    kill(m_pid, 0);
    exit(0);
  } else {
    std::cerr << "Unknows debugging command\n";
  }
}

void Debugger::run() {
  int wait_status;
  auto options = 0;
  waitpid(m_pid, &wait_status, options);

  char* line = nullptr;
  while ((line = linenoise("kidbg> ")) != nullptr) {
    handle_command(line);
    linenoiseHistoryAdd(line);
    linenoiseFree(line);
  }
}

auto main(int argc, char** argv) -> int {
  if (argc < 2) {
    std::cerr << "Debugee has not been specified\n";
    return -1;
  }

  auto prog = argv[1];

  auto pid = fork();

  // pid == 0 - child process (debuggee)
  if (pid == 0) {
    ptrace(PTRACE_TRACEME, 0, nullptr, nullptr);
    execl(prog, prog, nullptr);
  }

  // pid > 0 - parent process (debugger)
  if (pid > 0) {
    std::cout << "Running debugger on pid " << pid << '\n';
    Debugger dbg(prog, pid);
    dbg.run();
  }

  return 0;   
}
