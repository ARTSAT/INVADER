#define ANTENNAWAITTIME		4000
#define ANTENNAEXPANDTIME	2400 //Œ»İ–ñ2•ª
#define ANTENNAINTERVALTIME 4 //–ñ4ŠÔ
#define EXPANDANTENNAON		175//–{”Ô‚Í-5“x(175)
#define SUNSHINECUR			20

int16_t antennacount;
int8_t  antennaminute;
int8_t	antennahour;

short antennaExpandflag;//short‚©‚ç•ÏX

void wait_AntennaExpand(void);
void antennaInit(void);
void interval_AntennaExpand(void);
void antennaExpander(void);
void AEerrCheck(void);