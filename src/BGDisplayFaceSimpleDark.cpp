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

void BGDisplayFaceSimpleDark::showReadings(
    const std::list<GlucoseReading>& readings, bool dataIsOld) const {
    auto lastReading = readings.back();

    // Stale takes the whole face - value, arrow and bars. Any cyan on this face means the
    // reading has stopped updating.
    showReadingInColor(
        lastReading, 0, 6, TEXT_ALIGNMENT::CENTER, FONT_TYPE::MEDIUM,
        dataIsOld ? COLOR_CYAN : getValueColor());

    // The inversion that distinguishes this from Simple: the value never carries the band, so
    // it never lights red or green, and the arrow carries it instead at a fraction of the cost.
    showTrendArrowInColor(
        lastReading, MATRIX_WIDTH - 5, 1, dataIsOld ? COLOR_CYAN : getColorByBGValue(lastReading));

    drawTimerBlocks(lastReading, MATRIX_WIDTH, 0, 7);
}

// Blue is the cheapest colour on the panel, so keeping the age readout costs very little once
// the blocks are not green. Deleting them was never buying much.
uint16_t BGDisplayFaceSimpleDark::getTimerBlockColor(const GlucoseReading& lastReading) const {
    const int secondsAgo = lastReading.getSecondsAgo();
    if (secondsAgo >= 60 * SettingsManager.settings.bg_data_too_old_threshold_minutes) {
        return COLOR_CYAN;
    }
    return COLOR_BLUE;
}

void BGDisplayFaceSimpleDark::showNoData() const {
    DisplayManager.clearMatrix();
    // The font has to be set rather than inherited: it is left behind by whichever face drew
    // last, and the base implementation draws in a gray this face cannot show.
    DisplayManager.setFont(FONT_TYPE::MEDIUM);
    DisplayManager.setTextColor(getValueColor());
    DisplayManager.printText(0, 6, "No data", TEXT_ALIGNMENT::CENTER, 0);
}

// The age blocks advance once a minute. Repainting the whole face for that would redraw the
// value and the arrow every minute all night, so only row 7 is cleared and redrawn - the same
// trick BGDisplayFaceClock uses for its clock region. The stale transition still repaints in
// full, because BGDisplayFaceWithAge::getRenderDecision returns FULL when dataIsOld flips.
RenderDecision BGDisplayFaceSimpleDark::getAgeTickRenderDecision() const {
    return RenderDecision::PARTIAL;
}

void BGDisplayFaceSimpleDark::renderPartial(const RenderContext& ctx) const {
    if (ctx.readings.empty()) {
        return;
    }

    DisplayManager.clearMatrixPart(0, 7, MATRIX_WIDTH, 1);
    drawTimerBlocks(ctx.readings.back(), MATRIX_WIDTH, 0, 7);
}

uint16_t BGDisplayFaceSimpleDark::getValueColor() const {
    return static_cast<uint16_t>(SettingsManager.settings.night_value_color);
}
