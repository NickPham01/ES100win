/* 
 * Things related to initializing the program: setting pin I/O types, setting up timers
 */
#ifndef INITIALIZE_H
#define INITIALIZE_H

void setPinInitializationRegisters(pin_t *pinArray[], enum pinConfig configuration);
void initializePins(void);

void setupMillis(void);



#endif