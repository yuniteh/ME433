#include <xc.h>           // processor SFR definitions
#include <sys/attribs.h>  // __ISR macro
#include <math.h>

// DEVCFG0
#pragma config DEBUG = OFF // no debugging
#pragma config JTAGEN = OFF // no jtag
#pragma config ICESEL = ICS_PGx1 // use PGED1 and PGEC1
#pragma config PWP = OFF // no write protect
#pragma config BWP = OFF // no boot write protect
#pragma config CP = OFF // no code protect

// DEVCFG1
#pragma config FNOSC = PRIPLL // use primary oscillator with pll
#pragma config FSOSCEN = OFF // turn off secondary oscillator
#pragma config IESO = OFF // no switching clocks
#pragma config POSCMOD = HS // high speed crystal mode
#pragma config OSCIOFNC = OFF // disable secondary osc
#pragma config FPBDIV = DIV_1 // divide sysclk freq by 1 for peripheral bus clock
#pragma config FCKSM = CSDCMD // do not enable clock switch
#pragma config WDTPS = PS1 // use slowest wdt
#pragma config WINDIS = OFF // wdt no window mode
#pragma config FWDTEN = OFF // wdt disabled
#pragma config FWDTWINSZ = WINSZ_25 // wdt window at 25%

// DEVCFG2 - get the sysclk clock to 48MHz from the 8MHz crystal
#pragma config FPLLIDIV = DIV_2 // divide input clock to be in range 4-5MHz
#pragma config FPLLMUL = MUL_24 // multiply clock after FPLLIDIV
#pragma config FPLLODIV = DIV_2 // divide clock after FPLLMUL to get 48MHz
#pragma config UPLLIDIV = DIV_2 // divider for the 8MHz input clock, then multiplied by 12 to get 48MHz for USB
#pragma config UPLLEN = ON // USB clock on

// DEVCFG3
#pragma config USERID = 0 // some 16bit userid, doesn't matter what
#pragma config PMDL1WAY = OFF // allow multiple reconfigurations
#pragma config IOL1WAY = OFF // allow multiple reconfigurations
#pragma config FUSBIDIO = ON // USB pins controlled by USB module
#pragma config FVBUSONIO = ON // USB BUSON controlled by USB module

#define TRI_N 200
#define SIN_N 100

static volatile char triangle[TRI_N];
static volatile char sine[SIN_N];

void make_triangle();
void make_sine();

int main() {

    __builtin_disable_interrupts();

    // set the CP0 CONFIG register to indicate that kseg0 is cacheable (0x3)
    __builtin_mtc0(_CP0_CONFIG, _CP0_CONFIG_SELECT, 0xa4210583);

    // 0 data RAM access wait states
    BMXCONbits.BMXWSDRM = 0x0;

    // enable multi vector interrupts
    INTCONbits.MVEC = 0x1;

    // disable JTAG to get pins back
    DDPCONbits.JTAGEN = 0;

    // do your TRIS and LAT commands here
    TRISBbits.TRISB4 = 1;
    TRISAbits.TRISA4 = 0;
    init_spi();
    __builtin_enable_interrupts();

    set_voltage(1, 128);
    set_voltage(0, 0);

    int tri_i = 0;
    int sin_i = 0;
    make_triangle();
    make_sine();

    while (1) {
        _CP0_SET_COUNT(0);
        while (_CP0_GET_COUNT() < 24000) {
            set_voltage(1, triangle[tri_i]);
            set_voltage(0, sine[sin_i]);
        }
        ++tri_i;
        ++sin_i;
        if (tri_i > TRI_N) {
            tri_i = 0;
        }
        if (sin_i > SIN_N) {
            sin_i = 0;
        }

        // use _CP0_SET_COUNT(0) and _CP0_GET_COUNT() to test the PIC timing
        // remember the core timer runs at half the sysclk
    }
}

void make_triangle() {
    int i = 0;
    float temp_triangle[TRI_N];
    for (i = 0; i < TRI_N; ++i) {
        temp_triangle[i] = i * 1.275;
    }
    for (i = 0; i < TRI_N; ++i) {
        triangle[i] = (char) temp_triangle[i];
    }
}

void make_sine() {
    int i = 0;
    float temp_sine[SIN_N];
    for (i = 0; i < SIN_N; ++i) {
        temp_sine[i] = 127 * sin(2*3.1415*.01*i) + 128;
    }
    for (i = 0; i < SIN_N; ++i) {
        sine[i] = (char) temp_sine[i];
    }
}