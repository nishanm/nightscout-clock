#ifndef BGDISPLAYFACEBIGTEXTDARK_H
#define BGDISPLAYFACEBIGTEXTDARK_H

#include "BGDisplayFaceDarkBase.h"
#include "BGSource.h"

// A night rendering of the Big text face: same value and arrow placement, and the colour and age
// treatment every dark face shares. See BGDisplayFaceDarkBase for why.
//
// It exists because the medium font stops being legible at a distance well before it stops being
// visible: the digits stay lit, but 5, 6 and 8 become hard to tell apart, and a glucose value
// read wrong is worse than one not read at all. The large font costs more lit pixels than the
// medium one, which is the trade this face makes and Simple (dark) does not.
class BGDisplayFaceBigTextDark : public BGDisplayFaceDarkBase {
public:
    void showReadings(const std::list<GlucoseReading>& readings, bool dataIsOld = false) const override;
};

#endif
