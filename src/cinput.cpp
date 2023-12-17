#include "cinput.h"

#include "pd_api.h"
#include "crank.h"

CInput::CInput(PlaydateAPI *pd_api, int UpdateCounterDelay) {
    Enabled = true;
	Reset();
	pda = pd_api;
    PUpdateCounterDelay = UpdateCounterDelay;
    UpdateCounter = 0;
	setCrankMoveThreshold(45);
}

CInput::~CInput() {
}

void CInput::Disable()
{
	Enabled = false;
	Reset();
}

void CInput::Enable()
{
	Enabled = true;
}

void CInput::Update() {
	if(!Enabled)
		return;

 	int x;
	for (x=0;x<SDLK_LAST;x++)
	{
        KeyboardPushed[x] = false;
	}
	
	PDButtons buttons, buttonsPushed;
	pda->system->getButtonState(&buttons,&buttonsPushed, NULL);
	KeyboardPushed[SDLK_a] = (buttonsPushed & kButtonA);
	KeyboardPushed[SDLK_b] = (buttonsPushed & kButtonB);
	KeyboardPushed[SDLK_UP] = (buttonsPushed & kButtonUp);
	KeyboardPushed[SDLK_DOWN] = (buttonsPushed & kButtonDown);
	KeyboardPushed[SDLK_LEFT] = (buttonsPushed & kButtonLeft);
	KeyboardPushed[SDLK_RIGHT] = (buttonsPushed & kButtonRight);

	//need to only pushed ones if we are having a delay
	//held ones should wait until updateCounter Reaches 0
	if (UpdateCounter > 0)
	{
        UpdateCounter--;
		return;
	}

		

	int ret = crankUpdate();
    KeyboardHeld[SDLK_PAGEDOWN] = ret == CRANKMOVELEFT;
	KeyboardHeld[SDLK_PAGEUP] = ret == CRANKMOVERIGHT;


	KeyboardHeld[SDLK_a] = (buttons & kButtonA);
	KeyboardHeld[SDLK_b] = (buttons & kButtonB);
	KeyboardHeld[SDLK_UP] = (buttons & kButtonUp);
	KeyboardHeld[SDLK_DOWN] = (buttons & kButtonDown);
	KeyboardHeld[SDLK_LEFT] = (buttons & kButtonLeft);
	KeyboardHeld[SDLK_RIGHT] = (buttons & kButtonRight);



}

void CInput::Reset() 
{
    int x;
    for (x=0;x<SDLK_LAST;x++)
	{
        KeyboardHeld[x] = false;
		KeyboardPushed[x] = false;
	}

}

