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


------------------------------

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

The PDP writes the same values back to the Status Struct to verify settings:
trip_level[16];
ical[16];
vcal;
vphase;


------------------------------

Site Controller

example writing command struct

Command Struct has to be completely filled in with default or correct values

I recommend that you read the status struct to get the default settings

trip_level[16];
ical[16];
vcal;
vphase;

Then fill in the command settings from the status values

    pc.vcal = ps.vcal;

etc.
then:
    pdp_write_command_struct();



------------------------------


example reading status

    pdp_read_status_struct();


    Log("");
    Log("Status Struct");
    uint32 size = sizeof(PDP_Status_Struct);
    Log("size: " + IntToStr(size));
    Log("");

    int i = channel;




    Log("");
    Log("     pk[" + IntToStr(i) + "]: " + IntToStr(ps.pk[i]));
    Log("     rms[" + IntToStr(i) + "]: " + IntToStr(ps.rms[i]));
    Log("");
    Log("     real[" + IntToStr(i) + "]: " + IntToStr(ps.real[i]));
    Log("     apparent[" + IntToStr(i) + "]: " + IntToStr(ps.apparent[i]));
    Log("     power factor[" + IntToStr(i) + "]: " + IntToStr(ps.power_factor[i]));
    Log("");
    Log("     dc[" + IntToStr(i) + "]: " + IntToStr(ps.dc[i]));
    Log("");
    Log("     cycle freq (Hz): " + IntToStr(ps.voltage_freq)); // cycle freq from device
    Log("");
    Log("     samples per cycle: " + IntToStr(ps.voltage_cycle_samples));
    Log("");
    Log("     debug1: " + IntToStr(ps.debug1));
    Log("     debug2: " + IntToStr(ps.debug2));
    Log("");
    if(ps.voltage_cycle_length_error) Log("*** voltage_cycle_length_error");
    Log("");
    Log("---- Command check ----");
    Log("     trip_level[" + IntToStr(i) + "]: " + IntToStr(ps.trip_level[i]));
    Log("     ical[" + IntToStr(i) + "]: " + IntToStr(ps.ical[i]));
    Log("     vcal: " + IntToStr(ps.vcal));
    Log("     vphase: " + IntToStr(ps.vphase));
    Log("----");
    Log("");


-------------------




-------------------



https://github.com/craig-unilectric/pdp







