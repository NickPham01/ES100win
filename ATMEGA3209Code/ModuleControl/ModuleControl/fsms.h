/* fsms.h */
#ifndef FSMs
#define FSMs



/***************************************************************
	DEBOUNCE
****************************************************************/

struct debounceBTN {
	uint8_t pin;
	uint8_t curButtonState;
	uint8_t prevButtonState;
	unsigned long debounceStartTime;
	unsigned long currentTime;
	unsigned long debounceTime;
	uint8_t isDone;
	
	uint8_t state;

	enum btnOutputs output;
	};
struct debounceBTN initDebounce(uint8_t button_pin);
void updateDebounce(struct debounceBTN *btn);

/***************************************************************
	UI/Routing
	
	The routing is implemented with two instantiations of an FSM which each control
	a single output.  The state is saved as an enum defined just before the struct.
****************************************************************/

struct singleOutputUI {
		/* input pins */
		uint8_t straightButton;
		uint8_t crossButton;
		
		/* output pins/relays */
		uint8_t straightLED;
		uint8_t crossLED;
		uint8_t sumSltRly;
		uint8_t abSltRly;
		
		/* current state variable */		
		enum stateUI state;
		
		
		/* input button structs */
		struct debounceBTN straightBtn;
		struct debounceBTN crossBtn;
	};
struct singleOutputUI initUI(uint8_t straight_button,
 								uint8_t cross_button,
								uint8_t straight_LED,
								uint8_t cross_LED,
								uint8_t sum_rly,
								uint8_t ab_rly);
void setUIFSMOutputs(struct singleOutputUI *fsm);
void updateUIFSM(struct singleOutputUI *fsm);

/***************************************************************
	Receiver
****************************************************************/
struct receiverFSM {
	/* Input pins */
	uint8_t detect0;
	uint8_t detect1;
	uint8_t vcc_slt0;
	uint8_t vcc_slt1;
	
	/* Output pins/relays */
	uint8_t vcc9;
	uint8_t vcc12;
	uint8_t rly;
	
	/* Detect debounce FSM structs */
	struct debounceBTN detect0BTN;
	struct debounceBTN detect1BTN;
	
	/* current "button" states */
	uint8_t detect0State;
	uint8_t detect1State;
	
	enum receiverState prev_state;
	enum receiverState state;
	};
	
struct receiverFSM initReceiver(uint8_t detect0_pin,
								uint8_t detect1_pin,
								uint8_t vcc_slt0_pin,
								uint8_t vcc_slt1_pin,
								uint8_t vcc9_pin,
								uint8_t vcc12_pin,
								uint8_t rly_num);
void updateReceiver(struct receiverFSM *receiver);
void setReceiverOutputs(struct receiverFSM *receiver);

#endif