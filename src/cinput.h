#ifndef CINPUT_H
#define CINPUT_H

#include <pd_api.h>

#define SDLK_LAST 256

#define SDLK_UP kButtonUp
#define SDLK_DOWN kButtonDown
#define SDLK_LEFT kButtonLeft
#define SDLK_RIGHT kButtonRight
#define SDLK_a kButtonA
#define SDLK_b kButtonB
#define SDLK_PAGEDOWN (SDLK_LAST -1)
#define SDLK_PAGEUP (SDLK_LAST -2)


class CInput {
    public:
        bool KeyboardHeld[SDLK_LAST];
		bool KeyboardPushed[SDLK_LAST];
        CInput(PlaydateAPI *pd_api, int UpdateCounterDelay);
        ~CInput(void);
		void Disable();
		void Enable();
        void Update();
        void Reset();
        bool Ready(){ return (UpdateCounter == 0);};
        void Delay(){ UpdateCounter = PUpdateCounterDelay;};
		void SetUpdateDelay(int aDelay) { PUpdateCounterDelay = aDelay;};
    private:
       int UpdateCounter;
       int PUpdateCounterDelay;
	   bool Enabled;
	   PlaydateAPI *pda;

};

#endif
