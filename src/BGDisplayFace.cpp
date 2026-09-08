#include "BGDisplayFace.h"

void BGDisplayFace::showNoData() const {
    DisplayManager.clearMatrix();
    // The font has to be set, not inherited. showReading() sets it per face, so without this the
    // no-data screen draws in whichever font the last reading used - and on Big text that is
    // FONT_TYPE::LARGE, which renders "No data" far wider than the 32px panel and clips it to
    // "O DAT". The coordinates below are the ones the Simple face uses with MEDIUM.
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
// colour on a reading that is actually stale -- the milder state winning over the worse one.
bool BGDisplayFace::isEarlyStale(const GlucoseReading& reading) const {
    if (!SettingsManager.settings.stale_early_enable) {
        return false;
    }

    const int secondsAgo = reading.getSecondsAgo();
    return secondsAgo >= 60 * SettingsManager.settings.stale_early_minutes &&
           secondsAgo < 60 * SettingsManager.settings.bg_data_too_old_threshold_minutes;
}

uint16_t BGDisplayFace::getEarlyStaleColor() const {
    return static_cast<uint16_t>(SettingsManager.settings.stale_early_color);
}

RenderDecision BGDisplayFace::getRenderDecision(const RenderContext& ctx) const {
    if (ctx.reason == RenderReason::TIME_TICK) {
        if (ctx.dataIsOld != ctx.wasDataOld) {
            return RenderDecision::FULL;
        }
        return RenderDecision::NONE;
    }

    return RenderDecision::FULL;
}

void BGDisplayFace::renderPartial(const RenderContext& ctx) const {}
