#include <wiringPi.h>
#include <wiringShift.h>

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define DEBUG 1

#define DEBUG_LOG(...) \
            do { if (DEBUG) fprintf(stderr, __VA_ARGS__); \
                 if (DEBUG) fprintf(stderr, "\n"); } while (0)

#define NUM_STATIONS 16

#define SR_CLK_PIN 7
#define SR_NOE_PIN 0
#define SR_DAT_PIN 2
#define SR_LAT_PIN 3

int sr[NUM_STATIONS];

void pushShiftRegister()
{
    digitalWrite(SR_CLK_PIN, 0);
    digitalWrite(SR_LAT_PIN, 0);

    for (int i=0; i < NUM_STATIONS; ++i) {
        digitalWrite(SR_CLK_PIN, 0);
        digitalWrite(SR_DAT_PIN, sr[(NUM_STATIONS - 1) - i]);
        digitalWrite(SR_CLK_PIN, 1);
    }

    digitalWrite(SR_LAT_PIN, 1);
}

int set(int station)
{
    DEBUG_LOG("Switching station %d on", station);

    if (station < 0 || station > (NUM_STATIONS - 1)) {
        DEBUG_LOG("Error, invalid station index: %d", station);
        return -1;
    }

    sr[station] = 1;
    pushShiftRegister();

    return 0;
}

int reset(int station)
{
    DEBUG_LOG("Switching station %d off", station);

    if (station < 0 || station > (NUM_STATIONS - 1)) {
        DEBUG_LOG("Error, invalid station index: %d", station);
        return -1;
    }

    sr[station] = 0;
    pushShiftRegister();

    return 0;
}

int resetAll()
{
    DEBUG_LOG("Switching all stations off");

    for (int i=0; i < NUM_STATIONS; ++i) {
        sr[i] = 0;
    }

    pushShiftRegister();

    return 0;
}

int isOn(int station)
{
    if (station < 0 || station > (NUM_STATIONS - 1)) {
        DEBUG_LOG("Error, invalid station index: %d", station);
        return -1;
    }

    int status = sr[station];

    DEBUG_LOG("Station %d is currently set to %d", station, status);

    return status == 1;
}

int isOff(int station)
{
    if (station < 0 || station > (NUM_STATIONS - 1)) {
        DEBUG_LOG("Error, invalid station index: %d", station);
        return -1;
    }

    int status = sr[station];

    DEBUG_LOG("Station %d is currently set to %d", station, status);

    return status == 0;
}

int init()
{
    DEBUG_LOG("Initializing wiringPi");

    int status = wiringPiSetup();

    if (status != 0) {
        DEBUG_LOG("Error, wiringPiSetup() failed! (%d)", status);
        return status;
    }

    DEBUG_LOG("wiringPi initialized");
    DEBUG_LOG("Initializing shift register");

    pinMode(SR_CLK_PIN, OUTPUT); // set CLK to digital out
    pinMode(SR_NOE_PIN, OUTPUT); // set NOE to digital out
    digitalWrite(SR_NOE_PIN, 1); // disable shift register out
    pinMode(SR_DAT_PIN, OUTPUT); // set DATA to digital out
    pinMode(SR_LAT_PIN, OUTPUT); // set LATCH to digital out

    resetAll(); // all stations off initially
    
    digitalWrite(SR_NOE_PIN, 0); // enable shift register out

    DEBUG_LOG("Shift register initialized");

    return 0;
}
