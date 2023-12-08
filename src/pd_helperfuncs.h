#ifndef PD_HELPERFUNCS_H
#define PD_HELPERFUNCS_H

#include <pd_api.h>

#ifndef DrawTextColorBitmapCacheCount
#define DrawTextColorBitmapCacheCount 10
#endif

#ifndef DrawTextColorBitmapCacheMaxTextSize
#define DrawTextColorBitmapCacheMaxTextSize 850
#endif

// Determine pixel at x, y is black or white.
#define samplepixel(data, x, y, rowbytes) (((data[(y)*rowbytes+(x)/8] & (1 << (uint8_t)(7 - ((x) % 8)))) != 0) ? kColorWhite : kColorBlack)

// Set the pixel at x, y to black.
#define setpixel(data, x, y, rowbytes) (data[(y)*rowbytes+(x)/8] &= ~(1 << (uint8_t)(7 - ((x) % 8))))

// Set the pixel at x, y to white.
#define clearpixel(data, x, y, rowbytes) (data[(y)*rowbytes+(x)/8] |= (1 << (uint8_t)(7 - ((x) % 8))))

// Set the pixel at x, y to the specified color.
#define drawpixel(data, x, y, rowbytes, color) (((color) == kColorBlack) ? setpixel((data), (x), (y), (rowbytes)) : clearpixel((data), (x), (y), (rowbytes)))

void pattern_set_black(LCDPattern* pattern);
void pattern_set_white(LCDPattern* pattern);
void pattern_set_alpha(LCDPattern* pattern, float alpha);
void bitmap_set_alpha(LCDBitmap* Bitmap, float alpha);
void bitmap_set_alpha_rect(LCDBitmap *Bitmap, int rx, int ry, int rw, int rh, float alpha);

enum eFadeType {fadeNone, fadeIn, fadeOut};
typedef enum eFadeType eFadeType;

extern eFadeType prevFadeType;

eFadeType handleFade();
void startFade(eFadeType fadeType, bool useWhite, float step);

extern LCDPattern kColorGrey;
extern PlaydateAPI* pd;
extern long int DrawTextColorBitmapCacheMisses;
void drawTextColor(bool IgnoreBitmapContext, LCDBitmap* BitmapContext, LCDFont* font, const char* text, size_t len, PDStringEncoding encoding, int x, int y, LCDColor color, bool inverted);
void DrawBitmapScaledSrcRec(LCDBitmap* Bitmap, float scalex, float scaley, int dstX, int dstY, int srcX, int srcY, int srcW, int srcH);
void DrawBitmapSrcRec(LCDBitmap* Bitmap, int dstX, int dstY, int srcX, int srcY, int srcW, int srcH, LCDBitmapFlip FlipMode);
LCDFont* loadFontAtPath(const char* path);
LCDBitmap* loadImageAtPath(const char* path);
LCDBitmapTable* loadBitmapTableAtPath(const char* path);
unsigned int logPower(const char* filename, unsigned int logIntervalSeconds, unsigned int prevLogTime);
void setPDPtr(PlaydateAPI* playdate);
void pdDelay(unsigned int milliseconds);

#endif