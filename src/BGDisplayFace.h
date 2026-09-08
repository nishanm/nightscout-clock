#ifndef BGDISPLAYFACE_H
#define BGDISPLAYFACE_H

#include <list>

#include "BGSource.h"
#include "DisplayManager.h"
#include "SettingsManager.h"
#include "enums.h"

enum class RenderReason {
    NEW_DATA,
    TIME_TICK,
    FACE_CHANGE,
    FORCED,
};

enum class RenderDecision {
    NONE,
    PARTIAL,
    FULL,
};

struct RenderContext {
    RenderReason reason;
    tm currentTime;
    bool dataIsOld;
    bool wasDataOld;
    bool dataIsEarlyStale;
    bool wasDataEarlyStale;
    const std::list<GlucoseReading>& readings;
};

// True while a reading is old enough to warn about but not yet old enough to be called stale.
// Free rather than a member because the render loop needs the same answer the faces do: this
// state changes as time passes rather than when data arrives, so the screen has to be redrawn
// when it changes.
bool isReadingEarlyStale(const GlucoseReading& reading);

class BGDisplayFace {
public:
    virtual void showReadings(
        const std::list<GlucoseReading>& readings, bool dataIsOld = false) const = 0;
    virtual void showNoData() const;
    virtual RenderDecision getRenderDecision(const RenderContext& ctx) const;
    virtual void renderPartial(const RenderContext& ctx) const;

protected:
    // Configurable color for old readings and no-data screens;
    // gray can be invisible at minimum brightness.
    uint16_t getDataOldColor() const;

    // True once a reading is older than the early-stale threshold. Only the lower bound is
    // checked here; callers test the fully-old state first, so that keeps precedence.
    bool isEarlyStale(const GlucoseReading& reading) const;
    uint16_t getEarlyStaleColor() const;
};

#endif
