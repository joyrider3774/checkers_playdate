#ifndef crank_h
#define crank_h

#include <stdlib.h>
#include <stdio.h>
#include "pd_api.h"

#define CRANKDOCKED 0
#define CRANKMOVENONE 1
#define CRANKMOVELEFT 2
#define CRANKMOVERIGHT 3

unsigned int crankUpdate();
void setCrankMoveThreshold(float degrees);

#endif