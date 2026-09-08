#include "BGDisplayFace.h"

void BGDisplayFace::showNoData() const {
    DisplayManager.clearMatrix();
    // Reset the font because the previous face may have left LARGE selected,
    // which makes "No data" too wide for the panel.
    DisplayManager.setFont(FONT_TYPE::MEDIUM);
    DisplayManager.setTextColor(getDataOldColor());
    DisplayManager.printText(0, 6, "No data", TEXT_ALIGNMENT::CENTER, 0);
}

uint16_t BGDisplayFace::getDataOldColor() const {
    return static_cast<uint16_t>(SettingsManager.settings.data_old_color);
}

// True while a reading is old enough to warn about but not yet old enough to be called stale.
// Both bounds are checked here rather than left to the caller: every face would otherwise have to
// remember to test the fully-old state first, and one that forgot would show the early warning
// color on a reading that is actually stale -- the milder state winning over the worse one.
bool isReadingEarlyStale(const GlucoseReading& reading) {
    if (!SettingsManager.settings.stale_early_enable) {
        return false;
    }

    const int secondsAgo = reading.getSecondsAgo();
    return secondsAgo >= 60 * SettingsManager.settings.stale_early_minutes &&
           secondsAgo < 60 * SettingsManager.settings.bg_data_too_old_threshold_minutes;
}

bool BGDisplayFace::isEarlyStale(const GlucoseReading& reading) const {
    return isReadingEarlyStale(reading);
}

uint16_t BGDisplayFace::getEarlyStaleColor() const {
    return static_cast<uint16_t>(SettingsManager.settings.stale_early_color);
}

RenderDecision BGDisplayFace::getRenderDecision(const RenderContext& ctx) const {
    if (ctx.reason == RenderReason::TIME_TICK) {
        // Both of these change with the clock rather than with new data, so nothing else asks for
        // a redraw when a reading crosses one of them while sitting on screen. Waking only for the
        // fully-old transition left the early stale color unreachable on exactly the faces this
        // indicator exists for: a value drawn green when it arrived stayed green, then jumped
        // straight to the data-is-old color at the later threshold. Faces that inherit
        // BGDisplayFaceWithAge redraw every tick anyway, which is why they never showed it.
        if (ctx.dataIsOld != ctx.wasDataOld || ctx.dataIsEarlyStale != ctx.wasDataEarlyStale) {
            return RenderDecision::FULL;
        }
        return RenderDecision::NONE;
    }

    return RenderDecision::FULL;
}

void BGDisplayFace::renderPartial(const RenderContext& ctx) const {}
