#include "BGDisplayFaceBigTextDark.h"

#include "globals.h"

void BGDisplayFaceBigTextDark::showReadings(
    const std::list<GlucoseReading>& readings, bool dataIsOld) const {
    auto lastReading = readings.back();

    showReadingInColor(
        lastReading, 0, 7, TEXT_ALIGNMENT::LEFT, FONT_TYPE::LARGE,
        dataIsOld ? COLOR_CYAN : getValueColor());
    if (dataIsOld) {
        showTrendArrow(lastReading, MATRIX_WIDTH - 5, 1, true);
    } else {
        showTrendArrowInColor(lastReading, MATRIX_WIDTH - 5, 1, getColorByBGValue(lastReading));
    }
}
