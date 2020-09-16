#include "systemc.h"
#include<stdio.h>
#include <iostream>
using namespace std;
using namespace sc_dt;   

SC_MODULE(timer){
  	sc_in<bool>	 		reset;
  	sc_in<sc_uint<8>> 	Addr;
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
	 double last_update=0,cur_cnt;
  	 sc_event evnt_i0, evnt_i1;
  	 sc_uint<8>	lastval;
  	 int clk_period;
    SC_CTOR(timer){
      	clk_period=20; // in SC_NS
    	SC_METHOD(reset_all_reg);
      	sensitive<<reset;
      	dont_initialize();
      
    	SC_METHOD(get_data);
      	sensitive<<read_en;
      
     	SC_METHOD(set_data);
      	sensitive<<write_en;
      
        SC_THREAD(int0_events);
      	sensitive<<evnt_i0;
      	dont_initialize();
      	evnt_i0.notify(val*clk_period,SC_NS);
      
      	SC_THREAD(int1_events);
      	sensitive<<evnt_i1;
      	dont_initialize();
      	evnt_i1.notify(256*clk_period,SC_NS);
  	}	
	
  void int0_events(){
    while(true){
    wait(evnt_i0);
  	int cmp_val=cntrl[1];
    if(cmp_val==1){intr0.write(1);}
    evnt_i0.notify((256)*clk_period,SC_NS);
    wait(1,SC_NS);
    intr0.write(0);
    }
  }
  
  void int1_events(){
    while(true){
  		int ov_val=cntrl[2];
    	if(ov_val==1){intr1.write(1);}
    	evnt_i1.notify(256*clk_period,SC_NS);
    	wait(1,SC_NS);
    	intr1.write(0);
        wait(evnt_i1);
    }
  }
  
  	void set_data(){
      if(write_en.read()==1){
      double NowTime = sc_simulation_time(); 
      evnt_i0.cancel();
      evnt_i1.cancel();
    	sc_uint<8>  location;
    	location=Addr.read();  
    	if(write_en.read()==1){
      		sc_uint<8> da_ta=Data_in.read();
          if(location==0x0){
            cntrl=da_ta;
            if(da_ta==6){lastval=(sc_simulation_time()-last_update)/20;}
            if(da_ta==1 || da_ta==7){
              if(lastval==0)evnt_i0.notify((cmp-sc_simulation_time())*clk_period,SC_NS);
              else if(lastval>cmp)evnt_i0.notify((cmp+256-lastval)*clk_period,SC_NS);
              else evnt_i0.notify((cmp-lastval)*clk_period,SC_NS);
              last_update=sc_simulation_time();
              evnt_i1.notify((256-lastval)*clk_period,SC_NS);
            }
          }
          else if(location==0x8){
            cmp=da_ta%256;
            sc_uint<8> ccnt=lastval;
           // cout<<last_update<<" "<<sc_simulation_time()<<endl;
            cur_cnt=(sc_simulation_time()-last_update)/20;
            if(last_update!=0)cur_cnt=ccnt;
            if(cmp>cur_cnt)evnt_i0.notify((cmp-cur_cnt)*clk_period,SC_NS);
            else evnt_i0.notify((cmp+256-cur_cnt)*clk_period,SC_NS);
            evnt_i1.notify((256-cur_cnt)*clk_period,SC_NS);
        	}
    	}
  		}
    }
  
  void get_data(){
    if(read_en.read()==1){
  		sc_uint<8>  location;
    	location=Addr.read(); 
    	int en_val=cntrl[0];
    	if(read_en.read()==1){
          if(location==0x4){
            if(en_val==1)Data_out.write((sc_simulation_time()-last_update)/clk_period);
            else Data_out.write(lastval);
          }
         else if(location==0x8)Data_out.write(cmp);
         else if(location==0x0)Data_out.write(cntrl);
    	}
    }
  }
 
   	void reset_all_reg(){
  		cntrl=0;
    	val=0;
    	cmp=0;
    	intr_status=0;
        evnt_i0.cancel();
        evnt_i1.cancel();
  	}
  
};
