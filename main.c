#include "include.h"

static void test1(void);

#define TX_RESET_FSM()         do{s_tState=START;}while(0)

#define MAX_DELAY_TICK         0x5ffff

int main(void)
{
    system_init();

    while(1) {
        breath_led();
        test1();
    }

}

static void test1(void)
{
    static uint8_t s_chCnt;
    static uint32_t s_wDelaytick;
    static enum {
        START=0,
        TX_H,
        TX_E,
        TX_L,
        TX_O,
        TX_R,
        TX_N,
        DELAY
    } s_tState=START;
    switch(s_tState) {

    case  START :
        s_chCnt=0;
        s_wDelaytick=0;
        s_tState=TX_H;
        //break;
        
    case  TX_H :
        if(serial_out('H')) {
            s_tState=TX_E;
        }
        break;

    case  TX_E :
        if(serial_out('E')) {
            s_tState=TX_L;
        }
        break;

    case  TX_L :
        if(serial_out('L')) {
            s_chCnt++;
            if(s_chCnt>=2) {
                s_chCnt=0;
                s_tState=TX_O;
            }
        }
        break;

    case  TX_O :
        if(serial_out('O')) {
            s_tState=TX_R;
        }
        break;

    case  TX_R :
        if(serial_out('\r')) {
            s_tState=TX_N;
        }
        break;

    case  TX_N :

        if(serial_out('\n')) {
            s_tState=DELAY;
        }
        break;

    case  DELAY :
        s_wDelaytick++;
        if(s_wDelaytick>=MAX_DELAY_TICK) {
            TX_RESET_FSM();
        }
        break;

    default  :
        break;
    }
}

