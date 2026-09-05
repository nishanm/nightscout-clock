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

bool BGDisplayFace::isEarlyStale(const GlucoseReading& reading) const {
    if (!SettingsManager.settings.stale_early_enable) {
        return false;
    }

    return reading.getSecondsAgo() >= 60 * SettingsManager.settings.stale_early_minutes;
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
