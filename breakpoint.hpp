#if !defined(BREAKPOINT_HPP)
#define BREAKPOINT_HPP

#include <cstdint>
#include <sys/types.h>

class Breakpoint {
public:
  Breakpoint() = default;
  Breakpoint(pid_t pid, std::intptr_t addr) :
    m_pid(pid), m_addr(addr), m_enabled(false) {}

  void enable();
  void disable();

  bool is_enabled() { return m_enabled; }
  std::intptr_t address() { return m_addr; }

private:
  pid_t m_pid;
  std::intptr_t m_addr;
  bool m_enabled;
  // data at breakpoint address
  uint8_t m_data;
};

#endif

