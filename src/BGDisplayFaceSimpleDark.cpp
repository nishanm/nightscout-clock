#include "BGDisplayFaceSimpleDark.h"

#include "SettingsManager.h"
#include "globals.h"

// At night brightness a pixel is either lit or not, so this face saves light by lighting
// fewer channels: one value colour, the glucose band on the arrow only, cyan when stale.

void BGDisplayFaceSimpleDark::showReadings(
    const std::list<GlucoseReading>& readings, bool dataIsOld) const {
    auto lastReading = readings.back();

    // Stale takes the whole face. Any cyan here means the reading has stopped updating.
    showReadingInColor(
        lastReading, 0, 6, TEXT_ALIGNMENT::CENTER, FONT_TYPE::MEDIUM,
        dataIsOld ? COLOR_CYAN : getValueColor());

    // The inversion that distinguishes this from Simple: the value never carries the band, so
    // it never lights red or green, and the arrow carries it instead at a fraction of the cost.
    if (dataIsOld) {
        showTrendArrow(lastReading, MATRIX_WIDTH - 5, 1, true);
    } else {
        showTrendArrowInColor(lastReading, MATRIX_WIDTH - 5, 1, getColorByBGValue(lastReading));
    }
}

void BGDisplayFaceSimpleDark::showNoData() const {
    DisplayManager.clearMatrix();
    // The font has to be set rather than inherited: it is left behind by whichever face drew
    // last, and the base implementation draws in a gray this face cannot show.
    DisplayManager.setFont(FONT_TYPE::MEDIUM);
    DisplayManager.setTextColor(getValueColor());
    DisplayManager.printText(0, 6, "No data", TEXT_ALIGNMENT::CENTER, 0);
}

uint16_t BGDisplayFaceSimpleDark::getValueColor() const {
    return static_cast<uint16_t>(SettingsManager.settings.night_value_color);
}
