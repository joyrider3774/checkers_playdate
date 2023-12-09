#include <string.h>
#include <pd_api.h>
#include "SDL_HelperTypes.h"
#include "pd_helperfuncs.h"


PlaydateAPI* pd;
LCDBitmap *Buffer = NULL;
static volatile char pdDelayDummy;

typedef struct DrawTextColorBitmapCacheItem DrawTextColorBitmapCacheItem;
struct DrawTextColorBitmapCacheItem
{
	char Text[DrawTextColorBitmapCacheMaxTextSize+1];
	LCDBitmap* Bitmap;
	LCDColor color;
	LCDFont *font;
};

DrawTextColorBitmapCacheItem DrawTextColorBitmapCache[DrawTextColorBitmapCacheCount];
int DrawTextColorBitmapCacheItems = 0;
long int DrawTextColorBitmapCacheMisses = 0;

LCDPattern kColorGrey = {
	// Bitmap
	0b10101010,
	0b01010101,
	0b10101010,
	0b01010101,
	0b10101010,
	0b01010101,
	0b10101010,
	0b01010101,

	// Mask
	0b11111111,
	0b11111111,
	0b11111111,
	0b11111111,
	0b11111111,
	0b11111111,
	0b11111111,
	0b11111111,
};

//START: from discord by rezich
const uint8_t PATTERN_WHITE[8] = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };
const uint8_t PATTERN_BLACK[8] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

const uint8_t BAYER[8][8] = {
    { 0, 32,  8, 40,  2, 34, 10, 42},
    {48, 16, 56, 24, 50, 18, 58, 26},
    {12, 44,  4, 36, 14, 46,  6, 38},
    {60, 28, 52, 20, 62, 30, 54, 22},
    { 3, 35, 11, 43,  1, 33,  9, 41},
    {51, 19, 59, 27, 49, 17, 57, 25},
    {15, 47,  7, 39, 13, 45,  5, 37},
    {63, 31, 55, 23, 61, 29, 53, 21}
};

//from discord by rezich
void pattern_set_alpha(LCDPattern* pattern, float alpha) { // alpha goes from 0 (invisible) to 1 (fully opaque)
    const uint8_t threshold = (uint8_t)((1.f - alpha) * 64.f);
    for (int row = 0; row < 8; ++row) 
		for (int col = 0; col < 8; ++col)
        	if (BAYER[row][col] >= threshold) 
				(*pattern)[8 + row] |=  (1 << col); // set
        	else                              
				(*pattern)[8 + row] &= ~(1 << col); // clear
}

void pattern_set_black(LCDPattern* pattern) { memcpy(*pattern, PATTERN_BLACK, sizeof(PATTERN_BLACK)); }
void pattern_set_white(LCDPattern* pattern) { memcpy(*pattern, PATTERN_WHITE, sizeof(PATTERN_WHITE)); }

//END: from discord by rezich

void bitmap_set_alpha_rect(LCDBitmap *Bitmap, int rx, int ry, int rw, int rh, float alpha)
{
	if (!Bitmap)
		return;

	LCDPattern pattern;
	pattern_set_black(&pattern);
	pattern_set_alpha(&pattern, 1.0f - alpha);
	
	LCDBitmap* mask = pd->graphics->getBitmapMask(Bitmap);
	pd->graphics->pushContext(mask);
	pd->graphics->fillRect(rx,ry,rw,rh, (LCDColor)pattern);
	pd->graphics->popContext();
	//docs does not metion it but seems we need to free the mask returned ?
	//otherwise get 32 bytes of leaks per call to getbitmapmask
	pd->graphics->freeBitmap(mask);
}

void bitmap_set_alpha(LCDBitmap* Bitmap, float alpha) 
{
	if (!Bitmap)
		return;
	int w, h;
	pd->graphics->getBitmapData(Bitmap, &w, &h, NULL, NULL, NULL);
	bitmap_set_alpha_rect(Bitmap, 0, 0, w, h, alpha);
}
	
    

float fadeAlpha = 1.0f;
bool fadeUseWhite = true;;
eFadeType fadeFadeType = fadeNone;
float fadeStep = 0.01f;
eFadeType prevFadeType = fadeNone;

void startFade(eFadeType fadeType, bool useWhite, float step)
{
	prevFadeType = fadeType;
	fadeFadeType = fadeType;
	if (fadeFadeType == fadeIn)
	{
		fadeAlpha = 1.0f;
		fadeStep = -step;
	}

	if (fadeFadeType == fadeOut)
	{
		fadeAlpha = 0.0f;
		fadeStep = step;
	}
	fadeUseWhite = useWhite;
}

eFadeType handleFade() {
	eFadeType ret = fadeFadeType;
	if (ret == fadeNone)
		return ret;
	LCDPattern pattern;
	if(fadeUseWhite)
		pattern_set_white(&pattern);
	else
		pattern_set_black(&pattern);
	
	fadeAlpha += fadeStep;

	if (fadeFadeType == fadeIn)
	{
		if (fadeAlpha < 0.0f)
		{
			fadeAlpha = 0.0f;
			fadeFadeType = fadeNone;
		}
	}
	
	if(fadeFadeType == fadeOut)
	{
		if (fadeAlpha > 1.0f)
		{
			fadeAlpha = 1.0f;
			fadeFadeType = fadeNone;
		}		
	}
	pattern_set_alpha(&pattern, fadeAlpha);
	
	if(ret != fadeNone)
		pd->graphics->fillRect(0,0, LCD_COLUMNS, LCD_ROWS, (LCDColor) pattern);
	
	
	return fadeFadeType;
}

void drawTextColor(bool IgnoreBitmapContext, LCDBitmap* BitmapContext, LCDFont* font, const char* text, size_t len, PDStringEncoding encoding, int x, int y, LCDColor color, bool inverted)
{
	//grab width & height of our to be rendered text
	if (!IgnoreBitmapContext)
		pd->graphics->pushContext(BitmapContext);
	
	//check if we previously drawn this text
	int bcacheditemindex = -1;
	for (int i = 0; i < DrawTextColorBitmapCacheItems; i++)
	{
		if ((strcmp(DrawTextColorBitmapCache[i].Text, text) == 0) && 
			(DrawTextColorBitmapCache[i].color == color) &&
			(DrawTextColorBitmapCache[i].font == font))
		{
			bcacheditemindex = i;
			break;
		}
	}
	
	//text & bitmap was found in cache draw from cache
	if (bcacheditemindex != -1)
	{
		pd->graphics->drawBitmap(DrawTextColorBitmapCache[bcacheditemindex].Bitmap, x, y, kBitmapUnflipped);
	}
	else
	{		
		//not found in check, if we have max items erase the 1st item we had used
		//and move the others down
		if (DrawTextColorBitmapCacheItems == DrawTextColorBitmapCacheCount)
		{
			pd->graphics->freeBitmap(DrawTextColorBitmapCache[0].Bitmap);
			for (int i = 1 ; i < DrawTextColorBitmapCacheCount; i++)
			{
				DrawTextColorBitmapCache[i - 1] = DrawTextColorBitmapCache[i];
			}
			DrawTextColorBitmapCacheItems--;
		}
		int Lines = 1;
		size_t chars = 0;
		const char* p = text;
		while ((*p != '\0') && (chars < len))
		{
			if (*p == '\n')
				Lines++;
			p++;
			chars++;
		}

		int h = Lines * pd->graphics->getFontHeight(font);
		pd->graphics->setFont(font);
		int w = pd->graphics->getTextWidth(font, text, len, encoding, 0);
		//create new bitmap and fillrect with our color / pattern
		DrawTextColorBitmapCache[DrawTextColorBitmapCacheItems].Bitmap = pd->graphics->newBitmap(w, h, kColorClear);
		if (inverted)
			pd->graphics->setDrawMode(kDrawModeInverted);
		pd->graphics->pushContext(DrawTextColorBitmapCache[DrawTextColorBitmapCacheItems].Bitmap);
		pd->graphics->fillRect(0, 0, w, h, color);
		pd->graphics->popContext();

		//create mask with black background and draw text in white on the mask 
		LCDBitmap* bitmapmask = pd->graphics->newBitmap(w,h, kColorBlack);
		pd->graphics->pushContext(bitmapmask);
		pd->graphics->setDrawMode(kDrawModeFillWhite);
		pd->graphics->setFont(font);
		pd->graphics->drawText(text, len, encoding, 0, 0);
		pd->graphics->popContext();

		//set the mask to the bitmap with our pattern, this will make sure only the text
		//part (white in mask is drawn from the bitmap)
		pd->graphics->setBitmapMask(DrawTextColorBitmapCache[DrawTextColorBitmapCacheItems].Bitmap, bitmapmask);
		pd->graphics->freeBitmap(bitmapmask);
		//now draw the bitmap containing our text to the x & y position
		pd->graphics->drawBitmap(DrawTextColorBitmapCache[DrawTextColorBitmapCacheItems].Bitmap, x, y, kBitmapUnflipped);
		//ignore debug text for the cache
		if (strstr(text, "DTC Misses") == NULL)
		{
			if (DrawTextColorBitmapCacheItems > 0)
				DrawTextColorBitmapCacheMisses++;
			DrawTextColorBitmapCache[DrawTextColorBitmapCacheItems].color = color;
			DrawTextColorBitmapCache[DrawTextColorBitmapCacheItems].font = font;
			size_t textlen = (len < DrawTextColorBitmapCacheMaxTextSize ? len : DrawTextColorBitmapCacheMaxTextSize);
			strncpy(DrawTextColorBitmapCache[DrawTextColorBitmapCacheItems].Text, text, textlen);
			DrawTextColorBitmapCache[DrawTextColorBitmapCacheItems].Text[textlen] = '\0';
			DrawTextColorBitmapCacheItems++;
			if (textlen < len)
				pd->system->logToConsole("Warning DrawTextColorBitmapCache text truncated given size:%d maxsize:%d", len, textlen);
		}
		else
			pd->graphics->freeBitmap(DrawTextColorBitmapCache[DrawTextColorBitmapCacheItems].Bitmap);
	}

	if (!IgnoreBitmapContext)
		pd->graphics->popContext();
}


LCDBitmap* loadImageAtPath(const char* path)
{
	const char* outErr = NULL;
	LCDBitmap* img = pd->graphics->loadBitmap(path, &outErr);
	return img;
}

LCDBitmapTable* loadBitmapTableAtPath(const char* path)
{
	const char* outErr = NULL;
	LCDBitmapTable* table = pd->graphics->loadBitmapTable(path, &outErr);
	return table;
}

LCDFont* loadFontAtPath(const char* path)
{
	const char* outErr = NULL;
	LCDFont* fnt = pd->graphics->loadFont(path, &outErr);
	return fnt;
}

void DrawBitmapSrcRec(LCDBitmap* Bitmap, int dstX, int dstY, int srcX, int srcY, int srcW, int srcH, LCDBitmapFlip FlipMode)
{
	pd->graphics->setClipRect(dstX, dstY, srcW, srcH);
	pd->graphics->setDrawOffset((-srcX) + dstX, (-srcY) + dstY);
	pd->graphics->drawBitmap(Bitmap, 0, 0, FlipMode);
	pd->graphics->setDrawOffset(0, 0);
	pd->graphics->clearClipRect();
}

void DrawBitmapScaledSrcRec(LCDBitmap* Bitmap, float scalex, float scaley, int dstX, int dstY, int srcX, int srcY, int srcW, int srcH)
{
	pd->graphics->setClipRect(dstX, dstY, (int)(srcW * scalex), (int)(srcH * scaley));
	pd->graphics->setDrawOffset((int)(-srcX * scalex) + dstX, (int)(-srcY * scaley) + dstY);
	pd->graphics->drawScaledBitmap(Bitmap, 0, 0, scalex, scaley);
	pd->graphics->setDrawOffset(0, 0);
	pd->graphics->clearClipRect();
}

unsigned int logPower(const char* filename, unsigned int logIntervalSeconds, unsigned int prevLogTime)
{
	unsigned int s = pd->system->getSecondsSinceEpoch(NULL);
	if(s - prevLogTime >= logIntervalSeconds)
	{
		float p = pd->system->getBatteryPercentage();
		float v = pd->system->getBatteryVoltage();
		SDFile* file = pd->file->open(filename, kFileAppend);
		if (file)
		{
			char* line;
			pd->system->formatString(&line, "%d, %f, %f\n", s, p, v);
			pd->file->write(file, line, (unsigned int)strlen(line));
			pd->system->realloc(line, 0);
			//simulator crashes on windows when calling flush
#ifndef _WIN32
			pd->file->flush(file);
#endif
			pd->file->close(file);
		}
		return s;
	}
	return prevLogTime;
}


void setPDPtr(PlaydateAPI* playdate)
{
	pd = playdate;
}

void pdDelay(unsigned int milliseconds)
{
	unsigned int start = pd->system->getCurrentTimeMilliseconds();
	while (start + milliseconds > pd->system->getCurrentTimeMilliseconds())
	{
		(void)pdDelayDummy;
	}
}

bool pdFileExists(char* Path)
{
	bool result = false;
	SDFile* File = pd->file->open(Path, kFileRead);
	if (File)
	{
		result = true;
		pd->file->close(File);
	}

	return result;
}