#ifndef BGDISPLAYFACEBIGTEXTNIGHT_H
#define BGDISPLAYFACEBIGTEXTNIGHT_H

#include "BGDisplayFaceTextBase.h"
#include "BGSource.h"

class BGDisplayFaceBigTextNight : public BGDisplayFaceTextBase {
public:
    void showReadings(const std::list<GlucoseReading>& readings, bool dataIsOld = false) const override;
    void showNoData() const override;
};

#endif
