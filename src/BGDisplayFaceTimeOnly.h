#ifndef BGDISPLAYFACETIMEONLY_H
#define BGDISPLAYFACETIMEONLY_H

#include "BGDisplayFaceClock.h"

// An ordinary clock that hides glucose. While a glucose alarm is active it shows the
// "Clock and value" layout instead, so anyone hearing the alarm can see why.
class BGDisplayFaceTimeOnly : public BGDisplayFaceClock {
public:
    void showReadings(const std::list<GlucoseReading>& readings, bool dataIsOld = false) const override;
    void showNoData() const override;
    RenderDecision getRenderDecision(const RenderContext& ctx) const override;
    void renderPartial(const RenderContext& ctx) const override;
    bool ticksEverySecond() const override;

private:
    // Whether the last full render showed the alarm layout.
    mutable bool showingAlarm = false;
    void showTime() const;
};

#endif  // BGDISPLAYFACETIMEONLY_H
