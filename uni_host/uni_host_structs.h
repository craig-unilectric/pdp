//---------------------------------------------------------------------------------
//---------------------------------------------------------------------------------
/*
uni_host_structs.h
user structs
*/
//---------------------------------------------------------------------------------
//---------------------------------------------------------------------------------
//---------------------------------------------------------------------------------
//---------------------------------------------------------------------------------
#ifndef _uni_structs_H
#define _uni_structs_H
#ifdef __cplusplus
 extern "C" {
#endif
	 
//---------------------------------------------------------------------------------
//---------------------------------------------------------------------------------
//


typedef struct
{
		// add dummy values for 32 bit alignment
		//
		uint8 			channel_number; // relay channel 0-15
		uint8       dummy_1;
    uint8       dummy_2;
    uint8       dummy_3;
		//
		float    		trip_level[16]; // peak amps
		float    		ical[16]; // sensor amps/volt
		float    		vcal; // sensor (input volts)/(output volts), or (line volts)/(ADC input volts)
		uint32    	vphase; // number of ADC samples that voltage input lags current input, resistive load
		//uint16    	phase_polarity[16]; // n samples to adjust for two-phase, 0xfnnn = invert
		//
	
}PDP_Command_Struct;


//
//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
//
// Status struct
//


typedef struct
{
		// add dummy values for 32 bit alignment
		//
		float    		dc[16]; // volts
    float    		pk[16]; // volts, amps
    float     	rms[16]; // volts, amps
	
    float     	real[16]; // Watts
    float     	apparent[16]; // VA
    float     	power_factor[16]; // fraction 0-1, (real/apparent)
		//
		uint8      	relay_state[16]; // save last state
		//
		float     	voltage_freq; // Hz
		//
    uint8      	voltage_cycle_timer_error; // flag - checks AC cycle length
    uint8      	voltage_cycle_length_error; // flag - checks AC cycle length
		uint8      	dummy_1;
		uint8      	dummy_2;
		//
		uint16     	voltage_cycle_time; // 10uS
		uint16     	voltage_cycle_samples; // ADC samples per cycle
		//
		// debug - use for whatever
    uint32      debug1;
    uint32      debug2;
		//
		// debug - read back the values sent from the Site Controller
		float    		trip_level[16]; // to read back trip_level[]
		float    		ical[16]; // to read back ical[]
		float    		vcal; // to read back vcal
		uint32    	vphase; // to read back vphase
		//uint16      phase_polarity[16]; // to read back phase_polarity[]
		//
		
}PDP_Status_Struct;
	 

//---------------------------------------------------------------------------------
//---------------------------------------------------------------------------------
//
// 
//

	 

	 
	 
	 
	 
	 

//---------------------------------------------------------------------------------
//---------------------------------------------------------------------------------
	 
 






//---------------------------------------------------------------------------------
//---------------------------------------------------------------------------------








//---------------------------------------------------------------------------------
//---------------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif
#endif 
//---------------------------------------------------------------------------------
//---------------------------------------------------------------------------------


