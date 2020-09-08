#include "systemc.h"
#include "design.cpp"
#include "moniter.cpp"

int sc_main (int argc, char* argv[]) {
  
  //local signal decleration
  sc_signal<sc_uint<32>>   A;	//signal A
  sc_signal<sc_uint<32>>   B;	//signal B
  sc_signal<sc_uint<32>>   OUT; //signal OUT
  
  //module instance port connection
  adder adr("ADDER");
  adr.A(A);
  adr.B(B);
  adr.OUT(OUT);
  
  // module instance and port connection
  moniter mnitr("MONITER");
  mnitr.A(A);
  mnitr.B(B);
  mnitr.OUT(OUT);
  
  // Open VCD file
  sc_trace_file *wf = sc_create_vcd_trace_file("adder");
  // Dump the desired signals
  sc_trace(wf, A, "A");
  sc_trace(wf, B, "B");
  sc_trace(wf, OUT, "OUT");
  sc_start();
  
  mnitr.test();
  
  cout << "@" << sc_time_stamp() <<" Terminating simulation\n" << endl;
  sc_close_vcd_trace_file(wf);
  return 0;// Terminate simulation

}
