/*********************************************************
 * †
 *Created by Skyhorse（岡田 空馬）,2012/00/00
 *Copyright (c) 2012~. All rights reserved.
 *
 *********************************************************/
/*************************************************
 *  Function :
*    pll_init_rx(long reference_divider, byte swallow_counter,
*      long programmable_counter, int channel, byte charge_pump_tx,
*      byte charge_pump_rx, byte lock_detector,
*      byte standby_control_tx, byte standby_control_rx)                
********  Pin Definition for PLL Control  *******************************/
#include"Rx_protcol.h"
/*
#include"rx_pin_as.h"
#include"timer.h"
*/

void out_bit(int dat)
{
    if (dat == 0) {
      RX_DAT= 0x00;
    } else {
      RX_DAT = 0x01;
    }   
    // tick the clock
	delay_us(25);
    RX_CLK=1;
	delay_us(25);
    RX_CLK=0;
//	debug_puthex(dat);

}

/********  Generate Strobe  **********************************************/
void strobe(void)
{
    RX_STB=1;
	delay_us(25);
    RX_STB=0;
	delay_us(25);
}

/********  Output 1Bit Control Data  *************************************/
void out_bits(int num_bits, long dat)
{
    int i;

    for (i = 0; i < num_bits; i++) {
      out_bit(dat & 0x0001);
      dat >>= 1;
    }
}

/********  Set Reference Divider  ****************************************/
void set_reference_divider(long count)
{
    // send 12-bit reference count
    out_bits(12, count);

    // specify reference divider
    out_bit(1);
    out_bit(1);

    // strobe data
    strobe();
}

/********  Set Option Control  *******************************************/
void set_option_register(int charge_pump_tx, int charge_pump_rx, int lock_detector, int standby_control_tx, int standby_control_rx)
{
out_bits(3, 0);   // test mode (always 0)
out_bits(2, charge_pump_tx);
out_bits(2, charge_pump_rx);
out_bits(2, lock_detector);
out_bit(standby_control_tx);
out_bit(standby_control_rx);

// specify option register
out_bit(0);
out_bit(0);

// strobe data
strobe();
}

/********  Set Programmable Divider  *************************************/
void set_programmable_divider(int swallow_counter, long programmable_counter)
{
    out_bits(5, swallow_counter);
    out_bits(12, programmable_counter);

    // specify programmable divider Rx
      out_bit(1);
      out_bit(0);

    // strobe data
    strobe();
}

/********  Initialize PLL Control parameters  ****************************/
void pll_init_rx(long reference_divider, int swallow_counter, long programmable_counter, int charge_pump_tx, int charge_pump_rx, int lock_detector, int standby_control_tx, int standby_control_rx)
{
    set_reference_divider(reference_divider);
    set_option_register(charge_pump_tx, charge_pump_rx, lock_detector,
    standby_control_tx, standby_control_rx);
    set_programmable_divider(swallow_counter, programmable_counter);
}

