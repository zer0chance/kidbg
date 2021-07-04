#include <vector>
#include <sstream>
#include <cstdint>

#include "linenoise.h"
#include "debugger.hpp"

void Debugger::set_breakpoint_at_address(std::intptr_t addr) {
  std::cout << "Set breakpoint at address Ox" << std::hex << addr << '\n';
  Breakpoint bp{m_pid, addr};
  bp.enable();
  m_breakpoints[addr] = bp;
}

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
  } else if (is_prefix(command, "break")) {
    // TODO: Add check that address in format 0xXXXX
    std::string addr{args[1], 2};
    set_breakpoint_at_address(std::stol(addr, 0, 16));
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
