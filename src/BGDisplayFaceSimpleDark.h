#ifndef BGDISPLAYFACESIMPLEDARK_H
#define BGDISPLAYFACESIMPLEDARK_H

#include "BGDisplayFaceTextBase.h"
#include "BGSource.h"

// A night rendering of the Simple face: value in one chosen colour, glucose band on the
// trend arrow, no age blocks.
class BGDisplayFaceSimpleDark : public BGDisplayFaceTextBase {
public:
    void showReadings(const std::list<GlucoseReading>& readings, bool dataIsOld = false) const override;
    void showNoData() const override;

protected:
    uint16_t getValueColor() const;
};

#endif
