#ifndef BGDISPLAYFACEBIGTEXTDARK_H
#define BGDISPLAYFACEBIGTEXTDARK_H

#include "BGDisplayFaceSimpleDark.h"

// A night rendering of the Big text face: Simple (dark) with Big text's placement and font,
// for reading from across the room.
class BGDisplayFaceBigTextDark : public BGDisplayFaceSimpleDark {
public:
    void showReadings(const std::list<GlucoseReading>& readings, bool dataIsOld = false) const override;
};

#endif
