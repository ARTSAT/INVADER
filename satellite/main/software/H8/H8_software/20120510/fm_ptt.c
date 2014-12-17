#ifndef _FM_PTT_C_
#define _FM_PTT_C_

#include "iodefine.h"
#include "fm_ptt.h"
#include "fm_pindef.h"

// PTTÇÃON/OFFêßå‰-----------------------------------------------------

void fmptt_on(void) {
    FMPTT = 0;
}

void fmptt_off(void) {
    FMPTT = 1;
}

#endif