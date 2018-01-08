#ifndef KEY_EVENT_HPP
#define KEY_EVENT_HPP

typedef struct KeyEvents{
	enum SpecialKey{
		F1, F2, F3, F4, F5, F6, F7, F8, BKS, TAB,
		F9, F10, F11, ENT, F12, HOM, INS, DEL, PUP, PDN,
		END, KU, KL, KR, KD, ESC = 27
	};
	
	unsigned char keys_pressed_[128];
} KeyEvents;

/* returns an instance of a statically allocated KeyEvents */

KeyEvents *get_key_events();

#endif