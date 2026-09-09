#ifndef BGDISPLAYFACESIMPLEDARK_H
#define BGDISPLAYFACESIMPLEDARK_H

#include "BGDisplayFaceTextBase.h"
#include "BGSource.h"

// A night rendering of the Simple face: same value and arrow placement, different assignment
// of colour, and no age blocks.
//
// At the brightness night mode caps to, a channel is either on or off - there is no dim - so
// the only way to take light away is to light fewer channels. The value therefore drops the
// glucose band colour for one chosen colour, and the band moves to the trend arrow, which is
// a fraction of the pixels.
class BGDisplayFaceSimpleDark : public BGDisplayFaceTextBase {
public:
    void showReadings(const std::list<GlucoseReading>& readings, bool dataIsOld = false) const override;
    void showNoData() const override;

private:
    uint16_t getValueColor() const;
};

#endif
