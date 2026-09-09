#ifndef BGDISPLAYFACESIMPLEDARK_H
#define BGDISPLAYFACESIMPLEDARK_H

#include "BGDisplayFaceTextBase.h"
#include "BGDisplayFaceWithAge.h"
#include "BGSource.h"

// A night rendering of the Simple face: same layout, different assignment of colour.
//
// At the brightness night mode caps to, a channel is either on or off - there is no dim - so
// the only way to take light away is to light fewer channels. The value therefore drops the
// glucose band colour for one chosen colour, and the band moves to the trend arrow, which is
// a fraction of the pixels.
class BGDisplayFaceSimpleDark : public BGDisplayFaceTextBase, public BGDisplayFaceWithAge {
public:
    void showReadings(const std::list<GlucoseReading>& readings, bool dataIsOld = false) const override;
    void showNoData() const override;
    void renderPartial(const RenderContext& ctx) const override;

private:
    RenderDecision getAgeTickRenderDecision() const override;
    uint16_t getTimerBlockColor(const GlucoseReading& lastReading) const override;
    uint16_t getValueColor() const;
};

#endif
