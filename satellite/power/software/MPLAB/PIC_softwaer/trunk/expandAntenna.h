#define ANTENNAWAITTIME		4000
#define ANTENNAEXPANDTIME	2400 //���ݖ�2��
#define ANTENNAINTERVALTIME 4 //��4����
#define EXPANDANTENNAON		175//�{�Ԃ�-5�x(175)
#define SUNSHINECUR			20

int16_t antennacount;
int8_t  antennaminute;
int8_t	antennahour;

short antennaExpandflag;//short����ύX

void wait_AntennaExpand(void);
void antennaInit(void);
void interval_AntennaExpand(void);
void antennaExpander(void);
void AEerrCheck(void);