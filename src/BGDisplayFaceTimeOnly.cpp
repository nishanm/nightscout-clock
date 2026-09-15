#include "BGDisplayFaceTimeOnly.h"

#include "BGAlarmManager.h"
#include "ServerManager.h"
#include "globals.h"

void BGDisplayFaceTimeOnly::showReadings(
    const std::list<GlucoseReading>& readings, bool dataIsOld) const {
    showingAlarm = bgAlarmManager.isAlarmActive();
    if (showingAlarm) {
        BGDisplayFaceClock::showReadings(readings, dataIsOld);
        return;
    }

    showTime();
}

// Without readings no alarm can be active, so only the time is shown.
void BGDisplayFaceTimeOnly::showNoData() const {
    showingAlarm = false;
    DisplayManager.clearMatrix();
    showTime();
}

RenderDecision BGDisplayFaceTimeOnly::getRenderDecision(const RenderContext& ctx) const {
    if (ctx.reason != RenderReason::TIME_TICK || bgAlarmManager.isAlarmActive() != showingAlarm) {
        return RenderDecision::FULL;
    }

    if (showingAlarm) {
        return BGDisplayFaceClock::getRenderDecision(ctx);
    }

    return RenderDecision::PARTIAL;
}

void BGDisplayFaceTimeOnly::renderPartial(const RenderContext& ctx) const {
    if (showingAlarm) {
        BGDisplayFaceClock::renderPartial(ctx);
        return;
    }

    DisplayManager.clearMatrixPart(0, 0, MATRIX_WIDTH, 7);
    showTime();
}

bool BGDisplayFaceTimeOnly::ticksEverySecond() const { return true; }

// 24-hour format shows HH:MM:SS; 12-hour format has no room for seconds beside AM/PM.
void BGDisplayFaceTimeOnly::showTime() const {
    tm timeinfo = ServerManager.getTimezonedTime();

    char text[16];
    if (SettingsManager.settings.time_format == TIME_FORMAT::HOURS_12) {
        int hour = timeinfo.tm_hour % 12 == 0 ? 12 : timeinfo.tm_hour % 12;
        snprintf(
            text, sizeof(text), "%d:%02d %s", hour, timeinfo.tm_min,
            timeinfo.tm_hour < 12 ? "AM" : "PM");
    } else {
        snprintf(
            text, sizeof(text), "%02d:%02d:%02d", timeinfo.tm_hour, timeinfo.tm_min, timeinfo.tm_sec);
    }

    DisplayManager.setTextColor(COLOR_WHITE);
    DisplayManager.setFont(FONT_TYPE::MEDIUM);
    DisplayManager.printText(0, 6, text, TEXT_ALIGNMENT::CENTER, 2);
}
