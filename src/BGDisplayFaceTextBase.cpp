#include "BGDisplayFaceTextBase.h"

#include <map>

#include "BGDisplayManager.h"
#include "globals.h"

void BGDisplayFaceTextBase::showReading(
    const GlucoseReading reading, int16_t x, int16_t y, TEXT_ALIGNMENT alignment, FONT_TYPE font,
    bool isOld) const {
    String readingToDisplay = getPrintableReading(reading.sgv);
    if (!isOld) {
        SetDisplayColorByBGValue(reading);
    } else {
        DisplayManager.setTextColor(getDataOldColor());
    }

    DisplayManager.setFont(font);

    DisplayManager.printText(x, y, readingToDisplay.c_str(), alignment, 2);
}

void BGDisplayFaceTextBase::SetDisplayColorByBGValue(const GlucoseReading& reading) const {
    auto bgLevel = bgDisplayManager.getGlucoseIntervals().getBGLevel(reading.sgv);
    auto textColor = COLOR_GRAY;

    switch (bgLevel) {
        case BG_LEVEL::URGENT_LOW:
        case BG_LEVEL::URGENT_HIGH:
            textColor = BG_COLOR_URGENT;
            break;
        case BG_LEVEL::WARNING_LOW:
        case BG_LEVEL::WARNING_HIGH:
            textColor = BG_COLOR_WARNING;
            break;
        case BG_LEVEL::NORMAL:
            textColor = BG_COLOR_NORMAL;
            break;
    }

    DisplayManager.setTextColor(textColor);
}

String BGDisplayFaceTextBase::getPrintableReading(const int sgv) const {
    String readingToDisplay = "";
    if (SettingsManager.settings.bg_units == BG_UNIT::MGDL) {
        readingToDisplay += String(sgv);
    } else {
        char buffer[10];
        sprintf(buffer, "%.1f", round((float)sgv / 1.8) / 10);
        readingToDisplay += String(buffer);
    }
    return readingToDisplay;
}

#pragma region Show arrow

// Glucose trends
const uint8_t symbol_doubleUp[] PROGMEM = {
    0x50, 0xF8, 0x50, 0x50, 0x50,
};
const uint8_t symbol_singleUp[] PROGMEM = {
    0x20, 0x70, 0xA8, 0x20, 0x20,
};
const uint8_t symbol_fortyFiveUp[] PROGMEM = {
    0x38, 0x18, 0x28, 0x40, 0x80,
};
const uint8_t symbol_flat[] PROGMEM = {
    0x20, 0x10, 0xF8, 0x10, 0x20,
};
const uint8_t symbol_fortyFiveDown[] PROGMEM = {
    0x80, 0x40, 0x28, 0x18, 0x38,
};
const uint8_t symbol_singleDown[] PROGMEM = {
    0x20, 0x20, 0xA8, 0x70, 0x20,
};
const uint8_t symbol_doubleDown[] PROGMEM = {
    0x50, 0x50, 0x50, 0xF8, 0x50,
};

const uint8_t symbol_empty[] PROGMEM = {
    0x00, 0x00, 0x00, 0x00, 0x00,
};

// Drawn in the arrow's place once the reading is too old to act on. A cross rather than a
// dimmed arrow because the two states differ in kind, not in degree: the panel is not showing
// a weaker trend, it has no idea what the trend is. Nine lit pixels, the same as the arrow it
// replaces, which matters on a panel read in the dark where lit channels are the budget.
const uint8_t symbol_noData[] PROGMEM = {
    0x88, 0x50, 0x20, 0x50, 0x88,
};

const std::map<BG_TREND, const uint8_t*> glucoseTrendSymbols = {
    {BG_TREND::NONE, symbol_empty},
    {BG_TREND::DOUBLE_UP, symbol_doubleUp},
    {BG_TREND::SINGLE_UP, symbol_singleUp},
    {BG_TREND::FORTY_FIVE_UP, symbol_fortyFiveUp},
    {BG_TREND::FLAT, symbol_flat},
    {BG_TREND::FORTY_FIVE_DOWN, symbol_fortyFiveDown},
    {BG_TREND::SINGLE_DOWN, symbol_singleDown},
    {BG_TREND::DOUBLE_DOWN, symbol_doubleDown},
    {BG_TREND::NOT_COMPUTABLE, symbol_empty},
    {BG_TREND::RATE_OUT_OF_RANGE, symbol_doubleUp},
};

void BGDisplayFaceTextBase::showTrendArrow(
    const GlucoseReading reading, int16_t x, int16_t y, bool dataIsOld) const {
    // Past the threshold the arrow was only recoloured, so the face kept drawing a confident
    // direction for a reading it had already given up on - a claim about the present that the
    // clock cannot support. showTrendVerticalLine() below already refuses to do that, blanking
    // the trend outright when dataIsOld. This is the same rule for the faces that draw the 5x5
    // arrow, except that the slot is filled with a mark rather than left empty, so the state is
    // asserted rather than merely missing.
    //
    // The mark stays white while the value takes the data-is-old colour. That is deliberate:
    // the value's colour says "this number is stale", and it is the one thing on the panel that
    // is allowed to be hard to read. The mark says "there is no current reading at all", which
    // has to survive the colour and the brightness that the value does not.
    if (dataIsOld) {
        DisplayManager.drawBitmap(x, y, symbol_noData, 5, 5, COLOR_WHITE);
        return;
    }

    DisplayManager.drawBitmap(x, y, glucoseTrendSymbols.at(reading.trend), 5, 5, COLOR_WHITE);
}

#pragma endregion Show arrow

void BGDisplayFaceTextBase::showTrendVerticalLine(int x, BG_TREND trend, bool dataIsOld) const {
    if (dataIsOld) {
        trend = BG_TREND::NONE;
    }
    switch (trend) {
        case BG_TREND::DOUBLE_UP:
            DisplayManager.drawPixel(x, 0, COLOR_RED);
            DisplayManager.drawPixel(x, 1, COLOR_YELLOW);
            DisplayManager.drawPixel(x, 2, COLOR_GREEN);
            DisplayManager.drawPixel(x, 3, COLOR_WHITE);
            break;
        case BG_TREND::DOUBLE_DOWN:
            DisplayManager.drawPixel(x, 4, COLOR_WHITE);
            DisplayManager.drawPixel(x, 5, COLOR_GREEN);
            DisplayManager.drawPixel(x, 6, COLOR_YELLOW);
            DisplayManager.drawPixel(x, 7, COLOR_RED);
            break;
        case BG_TREND::SINGLE_UP:
            DisplayManager.drawPixel(x, 1, COLOR_YELLOW);
            DisplayManager.drawPixel(x, 2, COLOR_GREEN);
            DisplayManager.drawPixel(x, 3, COLOR_WHITE);
            break;
        case BG_TREND::SINGLE_DOWN:
            DisplayManager.drawPixel(x, 4, COLOR_WHITE);
            DisplayManager.drawPixel(x, 5, COLOR_GREEN);
            DisplayManager.drawPixel(x, 6, COLOR_YELLOW);
            break;
        case BG_TREND::FORTY_FIVE_UP:
            DisplayManager.drawPixel(x, 2, COLOR_GREEN);
            DisplayManager.drawPixel(x, 3, COLOR_WHITE);
            break;
        case BG_TREND::FORTY_FIVE_DOWN:
            DisplayManager.drawPixel(x, 4, COLOR_WHITE);
            DisplayManager.drawPixel(x, 5, COLOR_GREEN);
            break;
        case BG_TREND::FLAT:
            DisplayManager.drawPixel(x, 3, COLOR_WHITE);
            DisplayManager.drawPixel(x, 4, COLOR_WHITE);
            break;
        default:
            DisplayManager.setTextColor(COLOR_BLACK);
            break;
    }
}
