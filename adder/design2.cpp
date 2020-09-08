#include "systemc.h"

//module name :: "adder"
SC_MODULE (adder) {
    sc_in<sc_uint<32>> A,B;		// input 32 bit
    sc_out<sc_uint<32>> OUT;	//output 32 bit
  	sc_uint<32> local;			//local variable 
  	
  	sc_event e1;
  
    SC_CTOR (adder){
      SC_METHOD(add);
      sensitive<<A<<B;
      
      SC_METHOD(dely);
      sensitive<<e1;
    }
  
  	void dely(){
    	 OUT.write(local);			//assign to output
 	}
  
    void add(){
      		local=A.read()^B.read();	//save to local variable	
      		e1.notify(2,SC_NS);			//notify associated process after 2ns
  		
	}   
  	
};

