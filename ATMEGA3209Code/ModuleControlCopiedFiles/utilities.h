/* utilities.h */
#ifndef UTILITIES
#define UTILITIES

unsigned long millis(void);

uint8_t	readVccSwitch(uint8_t vcc_slt_0, uint8_t vcc_slt_1);
void	setVcc(uint8_t vcc_9, uint8_t vcc_12, uint8_t voltage);
void	activateRelay(uint8_t rly_p, uint8_t rly_n, uint8_t relayState);
void	setRelay(uint8_t relayNum, uint8_t relayState);
uint8_t readPlateDetect(uint8_t receiver);
uint8_t readUISW(void);
void	setLED(uint8_t led, uint8_t state);
#endif