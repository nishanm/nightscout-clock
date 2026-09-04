#include "BGDisplayFace.h"

void BGDisplayFace::showNoData() const {
    DisplayManager.clearMatrix();
    DisplayManager.setTextColor(COLOR_GRAY);
    DisplayManager.printText(0, 6, "No data", TEXT_ALIGNMENT::CENTER, 0);
}

static uint16_t colorFor(STALE_COLOR color) {
    switch (color) {
        case STALE_COLOR::CYAN:
            return COLOR_CYAN;
        case STALE_COLOR::MAGENTA:
            return COLOR_MAGENTA;
        case STALE_COLOR::BLUE:
            return COLOR_BLUE;
        case STALE_COLOR::GRAY:
        default:
            return COLOR_GRAY;
    }
}

uint16_t BGDisplayFace::getOldColor() const {
    return colorFor(SettingsManager.settings.stale_old_color);
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
