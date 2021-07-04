#include "breakpoint.hpp"

#include <sys/ptrace.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

void Breakpoint::enable() {
  auto data = ptrace(PTRACE_PEEKDATA, m_pid, m_addr, nullptr);
  m_data = static_cast<uint8_t>(data & 0xff);

  // 0xcc -- encoded "int 3" instruction that we want to insert
  // into debuggee. Data on that place will be stored in m_data
  uint8_t int3 = 0xcc;
  uint8_t modified_data = ((data & ~0xff) | int3);

  ptrace(PTRACE_POKEDATA, m_pid, m_addr, modified_data);
  m_enabled = true;
}

void Breakpoint::disable() {
  auto data = ptrace(PTRACE_PEEKDATA, m_pid, m_addr, nullptr);

  // Change inserted interrupt on the original instruction back
  uint8_t restored_data = ((data & ~0xff) | m_data);

  ptrace(PTRACE_POKEDATA, m_pid, m_addr, restored_data);
  m_enabled = false;
}

