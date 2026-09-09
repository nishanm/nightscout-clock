#include "BGDisplayFaceBigTextNight.h"

#include "BGDisplayManager.h"
#include "globals.h"

// Night variant of the "Big text" face. The value is always drawn in the dimmest neutral color so
// the display does not light up a dark room, and the trend arrow carries the band color, so the
// face still tells low/in-range/high at a glance
void BGDisplayFaceBigTextNight::showReadings(
    const std::list<GlucoseReading>& readings, bool dataIsOld) const {
    auto lastReading = readings.back();

    showReadingInColor(lastReading, 0, 7, TEXT_ALIGNMENT::LEFT, FONT_TYPE::LARGE, BG_COLOR_NIGHT);

    // show arrow in the right part of the screen. A dim, colorless arrow means the data is too old
    auto arrowColor = dataIsOld ? BG_COLOR_NIGHT : getColorByBGValue(lastReading);
    showTrendArrowInColor(lastReading, MATRIX_WIDTH - 5, 1, arrowColor);
}

void BGDisplayFaceBigTextNight::showNoData() const {
    DisplayManager.clearMatrix();
    DisplayManager.setTextColor(BG_COLOR_NIGHT);
    DisplayManager.printText(0, 6, "No data", TEXT_ALIGNMENT::CENTER, 0);
}
