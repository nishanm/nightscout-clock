#ifndef BGDISPLAYFACEWITHAGE_H
#define BGDISPLAYFACEWITHAGE_H

#include "BGDisplayFace.h"

class BGDisplayFaceWithAge : virtual public BGDisplayFace {
public:
    RenderDecision getRenderDecision(const RenderContext& ctx) const override;

protected:
    void drawTimerBlocks(GlucoseReading lastReading, int width, int xPosition, int yPosition) const;
    virtual RenderDecision getAgeTickRenderDecision() const;
    // The colour of the age blocks. A hook rather than a constant so a face can draw them in
    // something else, mirroring getAgeTickRenderDecision() above. The default is today's
    // green / yellow / gray, so no existing face changes appearance.
    virtual uint16_t getTimerBlockColor(const GlucoseReading& lastReading) const;
};

#endif  // BGDISPLAYFACEWITHAGE_H
