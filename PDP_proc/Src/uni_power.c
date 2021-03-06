//---------------------------------------------------------------------------------
//---------------------------------------------------------------------------------
/*
uni_power.c
user power routines
*/
//---------------------------------------------------------------------------------
//---------------------------------------------------------------------------------
 
#include "stm32f4xx_hal.h"
#include "stm32f4xx.h"
#include "stm32f4xx_it.h"
#include <stdio.h>

#include "uni_defines.h" // pins, ports, types, structs, commands, memory

#include "uni_power.h" 
#include "uni_uart.h" // for log() 

#include "uni_command.h"
extern IoTcam_Command_Struct ic; // IoTcam Host command
extern IoTcam_Status_Struct is; // IoTcam System status

//#include "..\..\uni_host\uni_host_structs.h" // PDP structs
extern PDP_Command_Struct pc;
extern PDP_Status_Struct ps;


extern PDP_Status_Struct ps_temp;


#include "float.h"
#include "math.h"






//---------------------------------------------------------------------------------
//---------------------------------------------------------------------------------
//
// memory
//


uint16 adc_channel_buf[ADC_DMA_SAMPLES_PER_CHANNEL];
int16 adc_channel_buf_filtered[ADC_DMA_SAMPLES_PER_CHANNEL];


uint16* dma_buf_filtered;

float offset_prev[ADC_DMA_CHANNELS];



float phase_shifted_voltage[CYCLE_SAMPLES_MAX];



//
//---------------------------------------------------------------------------------
//---------------------------------------------------------------------------------
//
// Buffer moves
//
//
// move temp status struct to output status struct
//
void transfer_metrics_to_status(void)
{
	uint32 i;
	uint8* src = (uint8*)&ps_temp;
	uint8* dest = (uint8*)&ps;
	for(i = 0;i < sizeof(PDP_Status_Struct);i++){
		dest[i] = src[i];
	}	
}
//
void transfer_buf(uint8* bufin, uint8* bufout, uint32 size)
{
	uint32 i;
	for(i = 0;i < size;i++){
		bufout[i] = bufin[i];	
	}
}
//
void transfer_bufin_to_channel_bufout(uint16* bufin, uint16 bufin_size, uint8 channel, uint32 transferCount, uint16* bufout, uint16 bufout_size)
{
	uint32 i, index;
	for(i = 0;i < transferCount;i++){
		index = channel + (i * ADC_DMA_CHANNELS);
		bufout[i] = bufin[index];	
	}
}
//
//
//---------------------------------------------------------------------------------
//---------------------------------------------------------------------------------
//
// Power math
//

void power_init_math(void)
{
	uint32 i;
	// init power calculations
	for(i = 0;i < ADC_DMA_CHANNELS;i++){
		//i_prev[i] = 0;
    //i_filtered_prev[i] = 0;
		offset_prev[i] = 0;
	}
}	
//


//static uint32 samplesPerCycle = CYCLE_SAMPLES_DEFAULT;

void power_from_ADC(uint16* buf)
{
	

	
	SCOPE_2_HIGH();	
	

	
	uint32 i, channel, index;
	uint16 val16;
	uint32 val32;
		
	// overwrite dma buf with filtered values
	dma_buf_filtered = &buf[0];
		
	//---------------------------------------------------
	//
	// Remove DC offset
	// Filter the DC out of the offset AC samples
	//
	// ps_temp.dc[channel]
	//
	float adc_new;
	float filt_new;
	float offset_adjust;
	float offset_new;
	float offset_test;
	float min;
	float best_offset;
	//
	// Process all ADC_DMA_SAMPLES so that the filter is continuous
	//
	for(channel = 0;channel < ADC_DMA_CHANNELS;channel++){	
		min = 9999.0f;
		for(i = 0;i < ADC_DMA_SAMPLES_PER_CHANNEL;i++){
			index = channel + (i * ADC_DMA_CHANNELS);			
			adc_new = (float)((uint16)buf[index]);
			//-----------------------------
			// filtered adc value
			// remove dc offset
			offset_adjust = (adc_new - offset_prev[channel]) * 0.0001f; // settles in 1 second
			offset_new = offset_prev[channel] + offset_adjust;	
			filt_new = adc_new - offset_new;	
			// save for next calculation
			offset_prev[channel] = offset_new;
			//
			// another approach - find best offset
			// init value
			if(i == 0) best_offset = offset_new;
			//-----------------------------
			// write filtered back to dma buf
			dma_buf_filtered[index] = (int16)(filt_new); 
			//-----------------------------
			// rough DC offset
			// we don't know zero crossing yet 
			// so estimate at lowest offset error
			offset_adjust = fabsf(adc_new - offset_new);
			if(offset_adjust < min){
				min = offset_adjust;
				best_offset = offset_new;
			}
		}
		// save vaue from either approach
		//ps_temp.dc[channel] = (float)offset_new * ADC_VOLTS_PER_BIT;  // *** calibrate
		ps_temp.dc[channel] = (float)best_offset * ADC_VOLTS_PER_BIT;  // *** calibrate
			
	}
	//---------------------------------------------------
	//
	// Find voltage zero crossings 
	// to define one AC cycle
	//
	bool cycle_error = true;
	int16 val16i, prev;
	bool cross_up = false;
	bool cross_down = false;
	uint32 cycle_start, cycle_end, cycle_length;
	//
	// compare two sequential adc values to determine if sine signal is crossing zero
	// filter the sine signal by comparing distant values in sequence
	uint32 filtergap = 10; // distance between samples
	//
	// start buf index past 0 
	// so that there is room to shift voltage cycle 
	// to the left against current cycle
	// max buffer = 2000, cycle = 810, 2 * cycle = 1620
	// max extra room to shift = 2000 - 1620 = 380 = 1/2 cycle
	uint32 max_voltage_shift = 200;
	// first zero crossing
		channel = VOLTAGE_ADC_CHANNEL;
		//
		for(i = filtergap + max_voltage_shift;i < (ADC_DMA_SAMPLES_PER_CHANNEL - CYCLE_SAMPLES_MAX);i++){
			index = channel + ((i - filtergap) * ADC_DMA_CHANNELS);
			prev = (int16)dma_buf_filtered[index];
			index = channel + (i * ADC_DMA_CHANNELS);	
			val16i = (int16)dma_buf_filtered[index];
			
			if((prev < 0) && (val16i >= 0)){ 
				cross_up = true;
				break;
			}
			//if((prev > 0) && (val16i <= 0)){ 
			//	cross_down = true;
			//	break;			
			//}
		}

		cycle_start = i;
		
		// next zero crossing
		if(i < (ADC_DMA_SAMPLES_PER_CHANNEL - CYCLE_SAMPLES_MAX))
		{
			i += CYCLE_SAMPLES_MIN; 
			for(;i < ADC_DMA_SAMPLES_PER_CHANNEL;i++){
				index = channel + ((i - filtergap) * ADC_DMA_CHANNELS);
				prev = (int16)dma_buf_filtered[index];
				index = channel + (i * ADC_DMA_CHANNELS);	
				val16i = (int16)dma_buf_filtered[index];
			
				if(cross_up && ((prev < 0) && (val16i >= 0))) break;
				//if(cross_down && ((prev > 0) && (val16i <= 0))) break;
			
			}
		}
		
		cycle_end = i;
		val32 = (cycle_end - cycle_start);
		
		ps_temp.debug1 = cycle_start;
		ps_temp.debug2 = cycle_end;
					
		if((val32 > CYCLE_SAMPLES_MIN) && (val32 < CYCLE_SAMPLES_MAX)){
			cycle_length = val32;
			cycle_error = false;
		}
		//else cycle_length = 821; // dummy flag value for debug

		//
		// or use zero crossing input for more accurate cycle length
		//
		cycle_length = ps_temp.voltage_cycle_samples;
		cycle_end = cycle_start + cycle_length;
		
	//---------------------------------------------------
	//
	// Shift Voltage
	// shift voltage samples against current samples
	// good method for >300 samples
	//
		
	uint32 vcycle_start, vcycle_end;
	uint32 icycle_start, icycle_end;

	vcycle_start = cycle_start;
	vcycle_end = cycle_end;
			
	// must be less than max_voltage_shift
	uint32 v_phase_shift_samples_count = ps_temp.vphase; // *** input from Site Controller
	
	// shift i to right to match delayed v
	icycle_start = cycle_start - v_phase_shift_samples_count;
	icycle_end = icycle_start + cycle_length;
	//icycle_start = vcycle_start;
	//icycle_end = vcycle_end;
	
	
	//---------------------------------------------------
	//
	// Shift voltage, for sparse samples
	// algorithm from online sources
	// bad method for > 300 samples per cycle
	// shift the voltage phase
	//
	// phase_shifted_voltage[j]
	//
	/*
	#define PHASECAL	2.0
	uint32 j;
	int16 val16i_prev;
	// algorithm from online sources
	//phaseShiftedV = lastFilteredV + PHASECAL * (filteredV - lastFilteredV); 
		channel = VOLTAGE_ADC_CHANNEL; // 2
		j = 0;
		// previous voltage sample
		index = channel + ((cycle_start - 1) * ADC_DMA_CHANNELS);			
		val16i_prev = (int16)dma_buf_filtered[index];
		for(i = cycle_start;i < cycle_end;i++){
			index = channel + (i * ADC_DMA_CHANNELS);			
			val16i = (int16)dma_buf_filtered[index];
			// phase shift                            
			// bad method due to noise
			phase_shifted_voltage[j] = (float)val16i_prev + ((float)(val16i - val16i_prev) * PHASECAL);
			j++;
			// save previous
			val16i_prev = val16i;
		}
		// test phase shift, output to host
		j = 0;	
		for(i = cycle_start;i < cycle_end;i++){
			index = channel + (i * ADC_DMA_CHANNELS);
			// test                          
			dma_buf_filtered[index] = (int16)phase_shifted_voltage[j];
			j++;
		}
	*/
	//---------------------------------------------------
	//
	// Noise filter
	// 
	// Note: make sure there is room in the input buffer 
	// to sample values in a window that extends 
	// outside of the cycle range
	//
	#define NOISE_FILTER_SIZE	32
	uint32 filtersize = NOISE_FILTER_SIZE; // even number > 0
	int32 filtersum;
	float filteravg;
	int32 val[NOISE_FILTER_SIZE];
	uint32 fi, first, last;
	uint32 jj;
	uint32 index2;

	for(channel = 0;channel < ADC_DMA_CHANNELS;channel++){
		// v cycle or i cycle, using phase shifted voltage index
		if(channel == VOLTAGE_ADC_CHANNEL) cycle_start = vcycle_start;
		else cycle_start = icycle_start;
		cycle_end = cycle_start + cycle_length;
		//
		// init filter
		fi = 0;
		i = cycle_start - 1;
		filtersum = 0;
		for(jj = (i - (filtersize >> 1));jj < (i + (filtersize >> 1));jj++){
			index2 = channel + (jj * ADC_DMA_CHANNELS);			
			val16i = (int16)dma_buf_filtered[index2];
			val[fi] = val16i;
			fi++;
			filtersum += val16i;
		}
		first = 0;
		last = filtersize - 1;
		//
		i = cycle_start;
		jj = (i + (filtersize >> 1));
		for(i = cycle_start;i < cycle_end;i++){
			index = channel + (i * ADC_DMA_CHANNELS);	
			val16i = (int16)dma_buf_filtered[index];
			//-----------------------------
			
			// subtract old value
			filtersum -= val[first];
			
			// too slow
			// change to mod() 
			// or fmod() to use FPU?
			first++;
			if(first >= filtersize) first = 0;
			last++;
			if(last >= filtersize) last = 0;
			
			// sum new value
			index2 = channel + (jj * ADC_DMA_CHANNELS);
			jj++;
			val16i = (int16)dma_buf_filtered[index2];
			filtersum += val16i;
			// save new value
			val[last] = val16i;

			// slow 
			// change to FPU command
			filteravg = ((float)filtersum/(float)filtersize); 
			
			//
			//-----------------------------
			// write filtered back to dma buf
			dma_buf_filtered[index] = (int16)(filteravg); 		
		}
		
	}	
	//
	/*
	// simple slow version
	uint8 filtersize = 32; // even number > 0
	int32 filtersum;
	int32 filteravg;
	uint32 jj, index2;	
	for(channel = 0;channel < ADC_DMA_CHANNELS;channel++){
		// v cycle or i cycle, using phase shifted voltage index
		if(channel == VOLTAGE_ADC_CHANNEL) cycle_start = vcycle_start;
		else cycle_start = icycle_start;
		cycle_end = cycle_start + cycle_length;
		for(i = cycle_start;i < cycle_end;i++){
			index = channel + (i * ADC_DMA_CHANNELS);			
			val16i = (int16)dma_buf_filtered[index];
			//-----------------------------
			filtersum = 0;
			for(jj = (i - (filtersize >> 1));jj < (i + (filtersize >> 1));jj++){
				index2 = channel + (jj * ADC_DMA_CHANNELS);			
				val16i = (int16)dma_buf_filtered[index2];
				filtersum += val16i;
			}
			filteravg = filtersum/filtersize;
			//-----------------------------
			// write filtered back to dma buf
			dma_buf_filtered[index] = (int16)(filteravg); 		
		}
	}	
	*/
	//---------------------------------------------------
	//
	// RMS
	// rms from filtered data
	// rms is calculated over a single AC cycle
	//
	// ps_temp.pk[channel]
	// ps_temp.rms[channel]
	//
	uint16 max;
	uint32 sq;
	uint32 sq_sum;
	double ms;
	double rms;
	float ratio;
	float vratio = pc.vcal * ADC_VOLTS_PER_BIT;
	int32 sum, avg; // debugging offset
		
	for(channel = 0;channel < ADC_DMA_CHANNELS;channel++){
		max = 0;	
		sq_sum = 0;
		sum = 0; // debugging offset
		// v cycle or i cycle, using phase shifted voltage index
		if(channel == VOLTAGE_ADC_CHANNEL) cycle_start = vcycle_start;
		else cycle_start = icycle_start;
		cycle_end = cycle_start + cycle_length;
		for(i = cycle_start;i < cycle_end;i++){
			index = channel + (i * ADC_DMA_CHANNELS);			
			val16i = (int16)dma_buf_filtered[index];
			// abs()
			// i_pk[]
			val16 = (uint16)fabsf((float)val16i);
			if(val16 > max) max = val16;		
			// i_rms[]
			sq = val16 * val16;
			sq_sum += sq;	
			//
			// debugging offset
			sum += val16i;
		}
		// pk[]
		
		if(channel == VOLTAGE_ADC_CHANNEL) ratio = vratio;
		else ratio = pc.ical[channel] * ADC_VOLTS_PER_BIT;
		
		ps_temp.pk[channel] = (float)max * ratio; // *** calibrate
		
		// rms[]
		if(cycle_length > 0) ms = (double)sq_sum/(double)cycle_length;
		else ms = 0.0;
		// root()		
		// sqrtf() is fastest
		// sqrtf() - 30 cycles
		// sqrt() - 60 cycles
		rms = sqrtf(ms); 
		ps_temp.rms[channel] = (float)rms * ratio; // *** calibrate
		//
		/*
		// debugging offset
		if(channel == 2){
			sum = (int32)fabsf((float)sum);
			avg = sum/cycle_length;
			ps_temp.debug1 = (float)avg;
			ps_temp.debug2 = ps_temp.dc[channel];
		}
		*/
	}	
		
	//---------------------------------------------------
	//
	// Power
	// real, apparent, power factor
	//
	// P = real power = (Vinst * Iinst)  Watts
	// S = apparent power = (Vrms * Irms)  VA  
	// PF = power factor = P/S, range 0.0-1.0
	//
	// ps_temp.real[channel]
	// ps_temp.apparent[channel]	
	// ps_temp.pf[channel]
	//
	
	uint32 j;
	int16 vval, ival;
	int32 instp;
	int32 instp_sum;
	double realp;
	double apparentp;
	float iratio;
	

	for(channel = 0;channel < ADC_DMA_CHANNELS;channel++){
		j = vcycle_start; // phase shifted voltage index;
		instp_sum = 0;
		// v cycle or i cycle, using phase shifted voltage index
		if(channel == VOLTAGE_ADC_CHANNEL) cycle_start = vcycle_start;
		else cycle_start = icycle_start;
		cycle_end = cycle_start + cycle_length;
		for(i = cycle_start;i < cycle_end;i++){
			index = channel + (i * ADC_DMA_CHANNELS);			
			ival = (int16)dma_buf_filtered[index];
			index = VOLTAGE_ADC_CHANNEL + (j * ADC_DMA_CHANNELS);			
			vval = (int16)dma_buf_filtered[index];
			j++;
			// One exception, when calculating power on voltage channel against itself,
			// use unshifted values to get power factor = 1.0
			if(channel == VOLTAGE_ADC_CHANNEL) ival = vval;
			// Instantaneous Power
			instp = ival * vval;          
			instp_sum += instp;
		}
		
		

		iratio = pc.ical[channel] * ADC_VOLTS_PER_BIT;
	

		// real
		if(cycle_length > 0) realp = (double)instp_sum/(double)cycle_length;
		else realp = 0.0;
		
		realp = realp * iratio * vratio;
		ps_temp.real[channel] = (float)realp; // *** calibrate
			
		// apparent
		apparentp = ps_temp.rms[VOLTAGE_ADC_CHANNEL] * ps_temp.rms[channel];
		ps_temp.apparent[channel] = (float)apparentp; // *** calibrate
		
		
		if(channel == VOLTAGE_ADC_CHANNEL){ 
			apparentp = (float)apparentp * FAKE_VOLTAGE_POWER_ADJUST;
			ps_temp.apparent[channel] = apparentp;
		}
		
		
		// power factor
		ps_temp.power_factor[channel] = (float)realp/(float)apparentp;
		
		
	}	
		
	//---------------------------------------------------
	//---------------------------------------------------
	//
	// Transfer readings to output struct
	//
	if(ic.enable){
		
		//
		// TBD - how to combine cycle length errors ***
		// combine cycle error test above with voltage timer error
    //uint8      	voltage_cycle_timer_error; // checks AC cycle length
    //uint8      	voltage_cycle_length_error; // checks AC cycle length
		// ps_temp.voltage_cycle_timer_error
		ps_temp.voltage_cycle_length_error = cycle_error;

		
		ps_temp.voltage_freq = (float)(100000.0f/(float)ps_temp.voltage_cycle_time);
		
		
		// for debug, read back command settings
		for(i = 0;i < PDP_RELAY_CHANNELS;i++){
			ps_temp.trip_level[i] = pc.trip_level[i];
			ps_temp.ical[i] = pc.ical[i];
		}
		ps_temp.vcal = pc.vcal;
		ps_temp.vphase = pc.vphase;


		
		// output status struct
		// transfer ps_temp to ps
		// transfer status values to status output struct
		transfer_metrics_to_status();
		
		
		
		
		// test for zero crossing or cycle length error before grabbing
		if(!cycle_error){
		
			
			// capture data beginning at cycle start
			if(pc.channel_number == VOLTAGE_ADC_CHANNEL) cycle_start = vcycle_start;
			else cycle_start = icycle_start;
			index = (cycle_start * ADC_DMA_CHANNELS);

			
			// extract channel data from dma buf
			// NA because later versions overwrite buf with filtered data
			//transfer_bufin_to_channel_bufout(&buf[index], ADC_DMA_SAMPLES, pc.channel_number, cycle_length, adc_channel_buf, ADC_DMA_SAMPLES_PER_CHANNEL);
		
	
			// extract filtered channel data from filtered buf
			transfer_bufin_to_channel_bufout(&dma_buf_filtered[index], ADC_DMA_SAMPLES, pc.channel_number, cycle_length, (uint16*)adc_channel_buf_filtered, ADC_DMA_SAMPLES_PER_CHANNEL);
		
		}
		
	}
	
	
	
	SCOPE_2_LOW();	
	

	
}
//
//---------------------------------------------------------------------------------
//---------------------------------------------------------------------------------


//---------------------------------------------------------------------------------
//---------------------------------------------------------------------------------
//---------------------------------------------------------------------------------
//---------------------------------------------------------------------------------
