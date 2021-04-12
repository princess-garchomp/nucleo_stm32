#include "button_state_eval.h"

void buton_stae_eval_fsm(switch_state* g_button_state, int* g_debounce_tiemr, _Bool button_pushed, int debounce_constant){
		
	switch(*g_button_state){
		case(WAIT_FOR_PUSH):
			if(button_pushed==1){
				*g_debounce_tiemr=*g_debounce_tiemr + 1;
				if(*g_debounce_tiemr>debounce_constant){
					*g_debounce_tiemr=0;
					*g_button_state=PUSHED;
				}
				else{}
			}
			else{*g_debounce_tiemr=0;}
		break;
			
		case(PUSHED):
			*g_button_state=WAIT_FOR_RELEASE;
		break;
			
		case(WAIT_FOR_RELEASE):
			if(button_pushed==0){
				*g_debounce_tiemr=*g_debounce_tiemr + 1;
				if(*g_debounce_tiemr>debounce_constant){
					*g_debounce_tiemr=0;
					*g_button_state=RELEASED;
				}
				else{}
			}
			else{*g_debounce_tiemr=0;}
		break;
		
		case(RELEASED):
			*g_button_state=WAIT_FOR_PUSH;
		break;
	}
}