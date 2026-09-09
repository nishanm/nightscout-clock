#include "BGDisplayFaceSimpleDark.h"

#include "SettingsManager.h"
#include "globals.h"

// Why this face exists, and why it cannot just draw the usual colours darker:
//
// Colour reaches the panel as RGB565, through the Framebuffer_GFX gamma tables, into FastLED,
// which scales by (v * (1 + brightness)) >> 8. At the brightness night mode caps to, a channel
// needs to survive gamma to emit at all, and everything that survives lands on the same single
// PWM step. So there is no brightness axis left at the pixel level, only a channel axis: every
// pixel is off, or lit on some combination of red, green and blue.
//
// Two consequences drive the code below. Asking for a dimmer colour does nothing. And gray is
// dim white, so COLOR_GRAY renders as black - which is why the stale state here is signalled
// with cyan rather than the BG_COLOR_OLD the daytime faces use.
//
// There are no age blocks. Dropping them is the rest of the light saving, and it also means
// nothing on this face changes between readings, so it never repaints on a clock tick - the
// inherited render decision only asks for a redraw when the reading goes stale.

void BGDisplayFaceSimpleDark::showReadings(
    const std::list<GlucoseReading>& readings, bool dataIsOld) const {
    auto lastReading = readings.back();

    // Stale takes the whole face. Any cyan here means the reading has stopped updating.
    showReadingInColor(
        lastReading, 0, 6, TEXT_ALIGNMENT::CENTER, FONT_TYPE::MEDIUM,
        dataIsOld ? COLOR_CYAN : getValueColor());

    // The inversion that distinguishes this from Simple: the value never carries the band, so
    // it never lights red or green, and the arrow carries it instead at a fraction of the cost.
    showTrendArrowInColor(
        lastReading, MATRIX_WIDTH - 5, 1, dataIsOld ? COLOR_CYAN : getColorByBGValue(lastReading));
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
