/* 
 * Things related to initializing the program: setting pin I/O types, setting up timers
 */
#ifndef INITIALIZE_H
#define INITIALIZE_H

void setPinInitializationRegisters(const uint8_t pinArray[], uint8_t num_pins, enum pinConfig configuration);
void initializePins(void);

void setupMillis(void);



#endif