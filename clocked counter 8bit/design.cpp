#include "systemc.h"
#include<stdio.h>
#include <iostream>
using namespace std;
using namespace sc_dt;   

SC_MODULE(timer){
	sc_in<bool>			clk;
  	sc_in<bool>	 		reset;
  	sc_in<sc_uint<8>*> 	Addr;
	sc_in<sc_uint<8>> 	Data_in;
  	sc_out<sc_uint<8>> 	Data_out;
  	sc_in<bool>			read_en;
  	sc_in<bool>			write_en;
  	sc_out<bool>	    intr0;
  	sc_out<bool>		intr1;
  
     sc_uint<8> cntrl;
     sc_uint<8> val;
     sc_uint<8> cmp;
     sc_uint<8> intr_status;
	 
    SC_CTOR(timer){		
    	SC_THREAD(reset_all_reg);
      	sensitive<<reset;
    	SC_METHOD(inc_timr);
     	sensitive<<clk.pos(); 
    	SC_METHOD(data_get_set);
      	sensitive<<read_en<<write_en<<Addr;
  	}
  
  	void data_get_set(){
    	sc_uint<8>  *location;
    	location=Addr.read();
      	//cout<<location<<" "<<&cntrl<<endl;  
    	if(write_en.read()==1){
      		sc_uint<8> da_ta=Data_in.read();
    		if(location==&cntrl)cntrl=da_ta;
        	else if(location==&cmp)cmp=da_ta;
    	}
    	else if(read_en.read()==1){
          	if(location==&cntrl)Data_out.write(cntrl);
        	else if(location==&cmp)Data_out.write(cmp);
        	else if(location==&val)Data_out.write(val);
    	}
  	}
 
   	void reset_all_reg(){
  		cntrl=0;
    	val=0;
    	cmp=0;
    	intr_status=0;
  	}
  
  	void inc_timr(){
  		int en_val=cntrl[0];
    	int cmp_val=cntrl[1];
    	int ov_val=cntrl[2];
    	if(en_val==1){
      		if(val==255){
        		if(ov_val){
          		intr1.write(1);
        		}
      			val=0;
      		}
      		else{
      			val++;
        		intr1.write(0);
      		}
      		if(cmp_val && val==cmp){
      			intr0.write(1);
      		}
      		else{
      			intr0.write(0);
      		}
    	}
  }
  
};