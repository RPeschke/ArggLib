#ifndef ArggLib_signal_slot_test
#define ArggLib_signal_slot_test
#include "RQ_OBJECT.h"
#include <iostream>
#include "C:/ArggsComputerEcke/ArggLib/Include/ArggLib/root/RQSignals.hh"
class signal_test {
  RQ_OBJECT("signal_test");
  std::ostream& m_out;
public:
  signal_test(std::ostream& out) : m_out(out) {}
  void send_signal() {
    m_out << "<signal_test>\n";
    Emit("send_signal()");
    m_out << "</signal_test>\n";
  }
  void send_signal_int(Int_t i) {
    m_out << "<send_signal_int>\n";
    Emit("send_signal_int(Int_t)",i);
    m_out << "</send_signal_int>\n";
  }
  void send_signal_char_star(const char* i) {
    m_out << "<send_signal_char_star>\n";
    Emit("send_signal_char_star(const char*)", i);
    m_out << "</send_signal_char_star>\n";
  }
};

START__DEFINE_RQ_SIGNAL_BASE_CLASS(signal_test)
  __DEFINE_RQ_SIGNAL__(send_signal,"send_signal()")
  __DEFINE_RQ_SIGNAL__(send_signal_int, "send_signal_int(Int_t)")
  __DEFINE_RQ_SIGNAL__(send_signal_char_star, "send_signal_char_star(const char*)")
END__DEFINE_RQ_SIGNAL_CLASS(signal_test)


class slot_test {
  RQ_OBJECT("slot_test");
  std::ostream& m_out;
public:
  slot_test(std::ostream& out) :m_out(out) {}
  void recieve_signal() {
    m_out << "<slot_test/>\n";

  }
  void recieve_signal_int(Int_t i) {
    m_out << "<recieve_signal_int>\n";
    m_out << i << "\n";
    m_out << "</recieve_signal_int>\n";
  }
  void recieve_signal_char_star(const char* i) {
    m_out << "<recieve_signal_char_star>\n";
    m_out << i << "\n";
    m_out << "</recieve_signal_char_star>\n";
  }
};

START__DEFINE_RQ_SLOTS_BASE_CLASS(slot_test)
__DEFINE_RQ_SLOT__(recieve_signal, "recieve_signal()")
__DEFINE_RQ_SLOT__(recieve_signal_int, "recieve_signal_int(Int_t)")
__DEFINE_RQ_SLOT__(recieve_signal_char_star, "recieve_signal_char_star(const char*)")
END__DEFINE_RQ_SLOTS_CLASS(slot_test)

#endif //ArggLib_signal_slot_test