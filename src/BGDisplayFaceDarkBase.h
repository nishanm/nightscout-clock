#ifndef BGDISPLAYFACEDARKBASE_H
#define BGDISPLAYFACEDARKBASE_H

#include "BGDisplayFaceTextBase.h"
#include "BGSource.h"

// Shared behaviour of the faces built to be read in a dark room. Each one is a night rendering
// of an existing daytime face: same value and arrow placement, different assignment of colour,
// and no age blocks.
//
// At the brightness night mode caps to, a channel is either on or off - there is no dim - so the
// only way to take light away is to light fewer channels. The value therefore drops the glucose
// band colour for one chosen colour, and the band moves to the trend arrow, which is a fraction
// of the pixels. Everything below that line is the same for every dark face, so a new one only
// has to say where its value goes and in which font.
class BGDisplayFaceDarkBase : public BGDisplayFaceTextBase {
public:
    virtual void showReadings(
        const std::list<GlucoseReading>& readings, bool dataIsOld = false) const = 0;
    void showNoData() const override;

protected:
    // The one colour the reading is drawn in, whatever the glucose band is.
    uint16_t getValueColor() const;
    // Draws a whole dark face: the value at the given position and font, and the trend arrow in
    // its usual corner. The arrow position is not a parameter because every text face in the
    // repo, dark or not, already puts it in the same place.
    void showDarkReadings(
        const std::list<GlucoseReading>& readings, int16_t x, int16_t y, TEXT_ALIGNMENT alignment,
        FONT_TYPE fontType, bool dataIsOld) const;
};

#endif  // BGDISPLAYFACEDARKBASE_H
