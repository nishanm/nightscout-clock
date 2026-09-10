#ifndef BGDISPLAYFACESIMPLEDARK_H
#define BGDISPLAYFACESIMPLEDARK_H

#include "BGDisplayFaceDarkBase.h"
#include "BGSource.h"

// A night rendering of the Simple face: same value and arrow placement, and the colour and age
// treatment every dark face shares. See BGDisplayFaceDarkBase for why.
class BGDisplayFaceSimpleDark : public BGDisplayFaceDarkBase {
public:
    void showReadings(const std::list<GlucoseReading>& readings, bool dataIsOld = false) const override;
};

#endif
