#include "systemc.h"

//module name :: "adder"
SC_MODULE (adder) {
    sc_in<sc_uint<32>> A,B;		// input 32 bit
    sc_out<sc_uint<32>> OUT;	//output 32 bit
  	sc_uint<32> local;			//local variable 
  
    SC_CTOR (adder){
      SC_THREAD(add);
      sensitive<<A<<B;
    }
  
    void add(){
		while(true){
    		wait();						//wait for A and B 
      		local=A.read()^B.read();	//save to local variable	
      		wait(2,SC_NS);				// wiat for 2ns before assign
      		OUT.write(local);			//assign to output
  		}
	}   
  	
};

