#include "BGDisplayFaceSimpleDark.h"

#include "globals.h"

void BGDisplayFaceSimpleDark::showReadings(
    const std::list<GlucoseReading>& readings, bool dataIsOld) const {
    // Simple's placement: the value centred in the medium font.
    showDarkReadings(readings, 0, 6, TEXT_ALIGNMENT::CENTER, FONT_TYPE::MEDIUM, dataIsOld);
}
