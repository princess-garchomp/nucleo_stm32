#ifndef BUTTON_STATE_EVAL
#define BUTTON_STATE_EVAL

typedef enum {WAIT_FOR_PUSH=0b00, PUSHED=0b01, WAIT_FOR_RELEASE=0b10, RELEASED=0b11}
switch_state;

void buton_stae_eval_fsm (switch_state* g_button_state, int* g_debounce_tiemr, _Bool button_pushed, int debounce_constant);

#endif 