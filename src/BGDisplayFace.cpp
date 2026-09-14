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

uint16_t BGDisplayFace::getBandColor(BG_LEVEL level) const {
    const auto& settings = SettingsManager.settings;
    DISPLAY_COLOR color = DISPLAY_COLOR::GRAY;
    switch (level) {
        case BG_LEVEL::URGENT_LOW:
            color = settings.bg_color_urgent_low;
            break;
        case BG_LEVEL::WARNING_LOW:
            color = settings.bg_color_low;
            break;
        case BG_LEVEL::NORMAL:
            color = settings.bg_color_normal;
            break;
        case BG_LEVEL::WARNING_HIGH:
            color = settings.bg_color_high;
            break;
        case BG_LEVEL::URGENT_HIGH:
            color = settings.bg_color_urgent_high;
            break;
        default:
            break;
    }
    return static_cast<uint16_t>(color);
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
