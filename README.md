# pdp
firmware for the pdp, software for the host

PDP github


Description:

"PDP_proc" - PDP firmware (using Keil comiler) for the STM32F407 processor

"uni_host" - Windows GUI (Qt5 IDE) for Windows

"PDP_api" - for the Site Controller (serial comms and commands for the Site Controller to talk to the PDP)

"power code" - Power calculation notes (from online sources)


"ST processor_Keil_Qt_setup.txt" - How to set up the Keil and Qt tools



Notes:

Programming the PDP - use the ST-Link/V2 jtag programmer

Windows/Linux VCP USB-serial bridge (SiLab CP2102) driver - may be included already in windows and linux


Input these values to init PDP

In the Command Struct:
// peak amps
float trip_level[16]; 
// sensor amps/volt
float ical[16]; 
// sensor (input volts)/(output volts), or (line volts)/(ADC input volts)
float vcal; 
// number of ADC samples that voltage input lags current input, resistive load
uint32 vphase; 

The same values are written back to the Status Struct to verify settings:
trip_level[16];
ical[16];
vcal; 
vphase;


https://github.com/craig-unilectric/pdp







