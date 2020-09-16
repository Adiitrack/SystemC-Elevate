#include "systemc.h"
#include "design.cpp"

SC_MODULE(intrrupt){
    sc_in<bool> clk; 
	sc_in<bool> intr0;
  	sc_in<bool> intr1;
  	sc_in<sc_uint<8>>  Data_out;
  	sc_out<bool> reset;
  	sc_out<sc_uint<8>*> 	Addr;
	sc_out<sc_uint<8>> 	Data_in;
  	sc_out<bool>			read_en;
  	sc_out<bool>			write_en;  	
  
  	timer *ptr; 
  
  	SC_CTOR(intrrupt){
  		SC_METHOD(int0);
    	sensitive<<intr0;
    	SC_METHOD(int1);
    	sensitive<<intr1;
    	SC_METHOD(tmr_val);
    	sensitive<<Data_out;
    	SC_THREAD(test);
  	}
  
  	void set_ptr(timer *val ){
    	//cout<<&val->cntrl<<endl;
  		ptr=val;
  	}
  
  	void make_reg_write_req(sc_uint<8>  *val , int data){
    	//cout<<val<<endl;
  		Addr.write(val);  
  		Data_in.write(data);
    	write_en.write(1);
  		wait(1,SC_NS);
 		write_en.write(0);
 	   wait(1,SC_NS);
 	 }
  
 	 void make_TIMER_READ_req(sc_uint<8>  *val){
 	 	Addr.write(val);
	 	read_en.write(1);   
 	 	wait(1,SC_NS);
  		read_en.write(0);
 	    wait(1,SC_NS);
  	 }
  
  	 void test(){  
  		reset.write(1);    
  		make_reg_write_req(&ptr->cntrl,7);  
  		make_reg_write_req(&ptr->cmp,135);  
  		wait(18466,SC_NS);make_TIMER_READ_req(&ptr->val);
 	 	wait(1518,SC_NS); make_TIMER_READ_req(&ptr->val);
 	 	wait(10,SC_NS);make_reg_write_req(&ptr->cntrl,6);      
 	 	wait(496,SC_NS);make_TIMER_READ_req(&ptr->val);
 	 	wait(498,SC_NS);make_reg_write_req(&ptr->cntrl,7);
 	 	wait(8988,SC_NS);make_TIMER_READ_req(&ptr->val);
 	 	wait(10,SC_NS);make_reg_write_req(&ptr->cmp,500);
 	 	wait(9995,SC_NS);make_reg_write_req(&ptr->cntrl,1);
 	 	wait(9959998,SC_NS);make_reg_write_req(&ptr->cntrl,7);  
	  	wait(749,SC_NS);make_TIMER_READ_req(&ptr->val);  
	  	wait(9246,SC_NS);make_reg_write_req(&ptr->cntrl,1);
	  	wait(98990000,SC_NS);
	  }
  
	  void int0(){
	    if(intr0.read()==1)
  			cout<<"Interrupt asserted: Intr0 ( "<<sc_time_stamp()<<")"<<endl; 
  	  }
  
  	  void int1(){
    	if(intr1.read()==1)
  			cout<<"Interrupt asserted: Intr1 ( "<<sc_time_stamp()<<")"<<endl; 
  	   }
  
 	  void tmr_val(){
    	if(Data_out.read()!=0)
  			cout<<"Value = "<<Data_out.read()<<" "<<sc_time_stamp()<<endl;
  	   }
  
};


int sc_main (int argc, char* argv[]) {
  
  sc_clock  clk("CLK", 20, SC_NS) ;
  sc_signal<bool>   reset;
  sc_signal<sc_uint<8>*>   Addr;
  sc_signal<sc_uint<8>>   Data_in;
  sc_signal<bool>   read_en;
  sc_signal<bool>   write_en;
  sc_signal<bool>   intr0;
  sc_signal<bool>   intr1;
  sc_signal<sc_uint<8>>   Data_out;
  
  timer timr("TIMER");
  timr.clk(clk);
  timr.reset(reset);
  timr.Addr(Addr);
  timr.Data_in(Data_in);
  timr.Data_out(Data_out);
  timr.read_en(read_en);
  timr.write_en(write_en);
  timr.intr0(intr0);
  timr.intr1(intr1);
  
  intrrupt intrpt("INTRRUPT");
  intrpt.intr0(intr0);
  intrpt.intr1(intr1);
  intrpt.clk(clk);
  intrpt.reset(reset);
  intrpt.Addr(Addr);
  intrpt.Data_in(Data_in);
  intrpt.Data_out(Data_out);
  intrpt.read_en(read_en);
  intrpt.write_en(write_en);
  
  timer *ptr=&timr;
  intrpt.set_ptr(ptr);
  
  //cout<<&timr.cntrl<<endl;
  //cout<<&ptr->cntrl<<endl;
  
  sc_start();
  intrpt.test();
  cout << "@" << sc_time_stamp() <<" Terminating simulation\n" << endl;
  
  return 0;

}
