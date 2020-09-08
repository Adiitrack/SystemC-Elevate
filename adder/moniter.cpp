#include "systemc.h"

SC_MODULE(moniter){
  sc_out<sc_uint<32>>   A;  					//output port to change A
  sc_out<sc_uint<32>>   B;						//output port to change B 
  sc_in<sc_uint<32>>   OUT; 					//input port to detect changes
  
  SC_CTOR(moniter){								
    SC_THREAD(test);
  	SC_THREAD(mnitr);
    sensitive<<OUT;
  }
 
  // Moniter to detect changes 
  void mnitr(){
  	while(true){
    	wait();
  		cout << "@" << sc_time_stamp() <<
          " output port = "<<OUT.read()<< endl;
  	 }
   }
  
  // Test functanality by changing A and B at intervals
  void test(){
    A = 0;
    B = 0;
  	wait(5,SC_NS);
    A=5;
    wait(3,SC_NS);
    B=10;
  	wait(7,SC_NS);
    A=7;B=8;
  	wait(4,SC_NS);
  	A=15;B=15;
  	wait(4,SC_NS);
  	cout<<"TEST_DONE"<<endl;
    return ;
  }
  
};
