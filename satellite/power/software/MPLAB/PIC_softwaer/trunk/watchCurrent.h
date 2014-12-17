#define MAINCURRENTLIMIT (uint8_t)100
#define MISSIONCURRENTLIMIT (uint8_t)50
#define BATTEMPNUM	3

#define MAINError 1
#define MISSIONError 2
#define RXError 3

uint8_t mainOCtimer;
uint8_t mainOCcount;

void watchCurrent(void);
void writeError(uint8_t Location);
void WCInit();
void WCerrCheck();