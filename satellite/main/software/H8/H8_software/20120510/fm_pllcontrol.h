#ifndef _FM_PLLCONTROL_H_
#define _FM_PLLCONTROL_H_

/* PLL Control --------------------------------------------------------------*/


void out_bit_fm(unsigned char);
void strobe_fm(void);
void out_bits_fm(int, long);
void set_reference_divider_fm(long);
void set_option_register_fm(unsigned char, unsigned char, unsigned char, unsigned char, unsigned char);
void set_programmable_divider_fm(unsigned char, long, int);
void pll_init_fm(long, unsigned char, long, int, unsigned char, unsigned char, unsigned char, unsigned char, unsigned char);
void set_pll_fm(void);


#endif