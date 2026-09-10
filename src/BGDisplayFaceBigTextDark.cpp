#include "BGDisplayFaceBigTextDark.h"

#include "globals.h"

void BGDisplayFaceBigTextDark::showReadings(
    const std::list<GlucoseReading>& readings, bool dataIsOld) const {
    // Big text's placement: the value left-aligned in the large font, which leaves the right of
    // the panel for the arrow the base draws.
    showDarkReadings(readings, 0, 7, TEXT_ALIGNMENT::LEFT, FONT_TYPE::LARGE, dataIsOld);
}
