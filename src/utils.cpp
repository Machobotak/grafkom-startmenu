#include "../include/utils.h"
#include <time.h>

bool startMenuOpen = false;
bool needsRedraw = true; // Awalnya true untuk initial draw

static int lastHour = -1;
static int lastMinute = -1;
static bool lastMenuState = false;
static float lastAnimProgress = 0.0f;

bool hasTimeChanged()
{
    time_t now = time(NULL);
    tm *t = localtime(&now);
    
    if (t->tm_hour != lastHour || t->tm_min != lastMinute) {
        lastHour = t->tm_hour;
        lastMinute = t->tm_min;
        return true;
    }
    return false;
}

bool hasMenuStateChanged()
{
    return startMenuOpen != lastMenuState;
}

void updateLastState()
{
    lastMenuState = startMenuOpen;
}