#include "BGDisplayManager.h"

#include <algorithm>
#include <list>

#include "BGSource.h"
#include "BGSourceManager.h"
#include "DisplayManager.h"
#include "ServerManager.h"
#include "SettingsManager.h"
#include "globals.h"

// The getter for the instantiated singleton instance
BGDisplayManager_& BGDisplayManager_::getInstance() {
    static BGDisplayManager_ instance;
    return instance;
}

// Initialize the global shared instance
BGDisplayManager_& bgDisplayManager = bgDisplayManager.getInstance();

void BGDisplayManager_::setup() {
    configureGlucoseIntervals();

    faces.push_back(new BGDisplayFaceSimple());
    facesNames[0] = "Simple";
    faces.push_back(new BGDisplayFaceGraph());
    facesNames[1] = "Full graph";
    faces.push_back(new BGDisplayFaceGraphAndBG());
    facesNames[2] = "Graph and BG";
    faces.push_back(new BGDisplayFaceBigText());
    facesNames[3] = "Big text";
    faces.push_back(new BGDisplayFaceValueAndDiff());
    facesNames[4] = "Value and diff";
    faces.push_back(new BGDisplayFaceClock());
    facesNames[5] = "Clock and value";
    faces.push_back(new BGDisplayFaceUnicorn());
    facesNames[6] = "Unicorn";

    if (faces.size() != CLOCK_FACE_COUNT) {
        DEBUG_PRINTF(
            "Face count mismatch: %u registered, CLOCK_FACE_COUNT is %d",
            static_cast<unsigned int>(faces.size()), CLOCK_FACE_COUNT);
    }

    configureFaceCycle();

    currentFaceIndex = configuredFaceId();
    currentFace = (faces[currentFaceIndex]);
}

// The face to start on: the first face of the cycle, otherwise the default face.
int BGDisplayManager_::configuredFaceId() const {
    int faceId = faceCycleActive ? faceCycleFaces.front() : SettingsManager.settings.default_clockface;
    return faceId >= 0 && static_cast<size_t>(faceId) < faces.size() ? faceId : 0;
}

void BGDisplayManager_::configureGlucoseIntervals() {
    glucoseIntervals = GlucoseIntervals();
    /// TODO: Add urgent values to settings

    glucoseIntervals.addInterval(1, SettingsManager.settings.bg_low_urgent_limit, BG_LEVEL::URGENT_LOW);
    glucoseIntervals.addInterval(
        SettingsManager.settings.bg_low_urgent_limit + 1, SettingsManager.settings.bg_low_warn_limit - 1,
        BG_LEVEL::WARNING_LOW);
    glucoseIntervals.addInterval(
        SettingsManager.settings.bg_low_warn_limit, SettingsManager.settings.bg_high_warn_limit,
        BG_LEVEL::NORMAL);
    glucoseIntervals.addInterval(
        SettingsManager.settings.bg_high_warn_limit, SettingsManager.settings.bg_high_urgent_limit - 1,
        BG_LEVEL::WARNING_HIGH);
    glucoseIntervals.addInterval(
        SettingsManager.settings.bg_high_urgent_limit, 401, BG_LEVEL::URGENT_HIGH);
}

// The face moves only when the face settings changed; the redraw shows new colours, units and limits.
void BGDisplayManager_::reloadSettings(const Settings& previous) {
    configureGlucoseIntervals();
    configureFaceCycle();

    bool faceSettingsChanged =
        previous.default_clockface != SettingsManager.settings.default_clockface ||
        previous.face_cycle_enabled != SettingsManager.settings.face_cycle_enabled ||
        previous.face_cycle_faces != SettingsManager.settings.face_cycle_faces;
    setFace(faceSettingsChanged ? configuredFaceId() : currentFaceIndex);
}

void BGDisplayManager_::configureFaceCycle() {
    faceCycleFaces.clear();
    faceCycleActive = false;
    faceCycleTimerStarted = false;

    for (int faceId : SettingsManager.settings.face_cycle_faces) {
        if (faceId < 0 || static_cast<size_t>(faceId) >= faces.size()) {
            continue;
        }

        if (std::find(faceCycleFaces.begin(), faceCycleFaces.end(), faceId) == faceCycleFaces.end()) {
            faceCycleFaces.push_back(faceId);
        }
    }

    if (!SettingsManager.settings.face_cycle_enabled) {
        return;
    }

    if (faceCycleFaces.size() < 2) {
        DEBUG_PRINTF(
            "Clock face cycling disabled: at least two valid unique faces are required, found %u\n",
            static_cast<unsigned int>(faceCycleFaces.size()));
        return;
    }

    faceCycleActive = true;
}

std::map<int, String> BGDisplayManager_::getFaces() { return facesNames; }

int BGDisplayManager_::getCurrentFaceId() { return currentFaceIndex; }

GlucoseIntervals BGDisplayManager_::getGlucoseIntervals() { return glucoseIntervals; }

void BGDisplayManager_::setFace(int id) {
    if (id < 0 || static_cast<size_t>(id) >= faces.size()) {
        return;
    }

    currentFaceIndex = id;
    currentFace = (faces[currentFaceIndex]);
    lastRefreshEpoch = 0;
    resetFaceCycleTimer();
    runRenderCycle(RenderReason::FACE_CHANGE, ServerManager.getTimezonedTime());
}

void BGDisplayManager_::showNextFace() {
    if (!faceCycleActive) {
        int nextFaceIndex = currentFaceIndex + 1;
        if (static_cast<size_t>(nextFaceIndex) >= faces.size()) {
            nextFaceIndex = 0;
        }
        setFace(nextFaceIndex);
        return;
    }

    auto current = std::find(faceCycleFaces.begin(), faceCycleFaces.end(), currentFaceIndex);
    if (current == faceCycleFaces.end()) {
        setFace(faceCycleFaces.front());
        return;
    }

    current++;
    setFace(current == faceCycleFaces.end() ? faceCycleFaces.front() : *current);
}

void BGDisplayManager_::showPreviousFace() {
    if (!faceCycleActive) {
        int previousFaceIndex = currentFaceIndex - 1;
        if (previousFaceIndex < 0) {
            previousFaceIndex = static_cast<int>(faces.size()) - 1;
        }
        setFace(previousFaceIndex);
        return;
    }

    auto current = std::find(faceCycleFaces.begin(), faceCycleFaces.end(), currentFaceIndex);
    if (current == faceCycleFaces.end() || current == faceCycleFaces.begin()) {
        setFace(faceCycleFaces.back());
    } else {
        setFace(*--current);
    }
}

void BGDisplayManager_::resetFaceCycleTimer() {
    lastFaceCycleMillis = millis();
    faceCycleTimerStarted = true;
}

void BGDisplayManager_::updateFaceCycle() {
    if (!faceCycleActive) {
        return;
    }

    if (MATRIX_OFF) {
        faceCycleTimerStarted = false;
        return;
    }

    unsigned long currentMillis = millis();
    if (!faceCycleTimerStarted) {
        lastFaceCycleMillis = currentMillis;
        faceCycleTimerStarted = true;
        return;
    }

    unsigned long intervalMillis =
        static_cast<unsigned long>(SettingsManager.settings.face_cycle_interval_seconds) * 1000UL;
    if (currentMillis - lastFaceCycleMillis >= intervalMillis) {
        showNextFace();
    }
}

void BGDisplayManager_::tick() {
    updateFaceCycle();
    maybeRrefreshScreen();
}

void BGDisplayManager_::commitRenderedState(bool dataIsOld) {
    lastRenderedDataWasOld = dataIsOld;
    lastRefreshEpoch = ServerManager.getUtcEpoch();
}

void BGDisplayManager_::runRenderCycle(RenderReason reason, const tm& timeInfo) {
    bool dataIsOld = displayedReadings.size() > 0 &&
                     displayedReadings.back().getSecondsAgo() >
                         60 * SettingsManager.settings.bg_data_too_old_threshold_minutes;
    RenderContext ctx{reason, timeInfo, dataIsOld, lastRenderedDataWasOld, displayedReadings};

    switch (currentFace->getRenderDecision(ctx)) {
        case RenderDecision::NONE:
            return;
        case RenderDecision::PARTIAL:
            currentFace->renderPartial(ctx);
            DisplayManager.update();
            commitRenderedState(dataIsOld);
            return;
        case RenderDecision::FULL:
            DisplayManager.clearMatrix();
            if (displayedReadings.size() > 0) {
                currentFace->showReadings(displayedReadings, dataIsOld);
            } else {
                currentFace->showNoData();
            }
            DisplayManager.update();
            commitRenderedState(dataIsOld);
            return;
    }
}

void BGDisplayManager_::maybeRrefreshScreen(bool force) {
    auto currentEpoch = ServerManager.getUtcEpoch();
    tm timeInfo = ServerManager.getTimezonedTime();

    auto lastReading = bgDisplayManager.getLastDisplayedGlucoseReading();

    if (bgSourceManager.hasNewData(lastReading == NULL ? 0 : lastReading->epoch)) {
        DEBUG_PRINTLN("We have new data");
        bgDisplayManager.showData(bgSourceManager.getInstance().getGlucoseData());
    } else {
        // We refresh the display every minue trying to match the exact :00 second
        if (force) {
            runRenderCycle(RenderReason::FORCED, timeInfo);
        } else if (
            timeInfo.tm_sec == 0 && currentEpoch > lastRefreshEpoch ||
            currentEpoch - lastRefreshEpoch > 60) {
            runRenderCycle(RenderReason::TIME_TICK, timeInfo);
        }
    }
}

void BGDisplayManager_::showData(std::list<GlucoseReading> glucoseReadings) {
    if (glucoseReadings.size() == 0) {
        displayedReadings.clear();
        runRenderCycle(RenderReason::NEW_DATA, ServerManager.getTimezonedTime());
        return;
    }

    displayedReadings = glucoseReadings;
    runRenderCycle(RenderReason::NEW_DATA, ServerManager.getTimezonedTime());
}

GlucoseReading* BGDisplayManager_::getLastDisplayedGlucoseReading() {
    if (displayedReadings.size() > 0) {
        return &displayedReadings.back();
    } else {
        return NULL;
    }
}
