#ifndef UTILITIES
#define UTILITIES


uint8_t	readVccSwitch(uint8_t receiver);
void	setVcc(uint8_t receiver, uint8_t voltage);
void	activateRelay(uint8_t rly_p, uint8_t rly_n, uint8_t relayState);
void	setRelay(uint8_t relayNum, uint8_t relayState);
uint8_t readPlateDetect(uint8_t receiver);
uint8_t readUISW(void);
void	setLED(uint8_t led, uint8_t state);
#endif