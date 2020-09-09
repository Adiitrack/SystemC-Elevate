#include "systemc.h"

//module name :: "adder"
SC_MODULE (adder) {
    sc_in<sc_uint<32>> A,B;		// input 32 bit
    sc_out<sc_uint<32>> OUT;	//output 32 bit
  	sc_uint<32> local;			//local variable 
  	
  	int t=0;
  
    SC_CTOR (adder){
      SC_METHOD(add);
      sensitive<<A<<B;
    }
  
  
    void add(){
      local=A.read()^B.read();	//save to local variable 	
      
      //if sensitive event occur increment t and 
      //make kernel notify to execute it after 2ns
      //if kernel already notify and time passed than write to output port and 
      //initilize t again;
      
      if(t==1){
        OUT.write(local);
      	t=0;
      }
      else{
        t++;
        next_trigger(2,SC_NS);  
      }
      
	}   
  	
};

