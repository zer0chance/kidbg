#include <sys/personality.h>
#include "debugger.hpp"

auto main(int argc, char** argv) -> int {
  if (argc < 2) {
    std::cerr << "Debugee has not been specified\n";
    return -1;
  }

  auto prog = argv[1];

  auto pid = fork();

  // pid == 0 - child process (debuggee)
  if (pid == 0) {
    personality(ADDR_NO_RANDOMIZE);
    ptrace(PTRACE_TRACEME, 0, nullptr, nullptr);
    execl(prog, prog, nullptr);
  }

  // pid > 0 - parent process (debugger)
  if (pid > 0) {
    std::cout << "Running debugger on pid " << pid << "...\n\n";
    Debugger dbg(prog, pid);
    dbg.run();
  }

  return 0;   
}
