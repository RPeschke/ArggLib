#include "ArggLib/root/TGSignals.hh"
#include "signal_slot_test.hh"
#include "ArggLib/complete_lib.hh"

#include <iostream>

ARGGLIB__DEFINE_TEST(signal_slot_1) {
  std::stringstream out;
  signal_test s(out);
  slot_test slot(out);
  _RQ_signals(s).send_signal() >> _RQ_slots(slot).recieve_signal();

  s.send_signal();
  ___ARGGLIB_TEST("stream out", out.str(), "<signal_test>\n<slot_test/>\n</signal_test>\n");
  out.str("");
  out.clear();

  _RQ_signals(s).send_signal_int() >> _RQ_slots(slot).recieve_signal_int();
  s.send_signal_int(1);
  ___ARGGLIB_TEST("stream out", out.str(), "<send_signal_int>\n<recieve_signal_int>\n1\n</recieve_signal_int>\n</send_signal_int>\n");
  out.str("");
  out.clear();

  _RQ_signals(s).send_signal_char_star() >> _RQ_slots(slot).recieve_signal_char_star();
  s.send_signal_char_star("test");
  ___ARGGLIB_TEST("stream out", out.str(), "<send_signal_char_star>\n<recieve_signal_char_star>\ntest\n</recieve_signal_char_star>\n</send_signal_char_star>\n");
  out.str("");
  out.clear();
  std::cout << out.str();

}