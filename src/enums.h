#ifndef Enums_h

#define Enums_h

#include <Arduino.h>


enum class BG_UNIT : uint8_t {
    MGDL = 0,
    MMOLL = 1,
};

enum class BG_TREND : uint8_t {
    NONE = 0,
    DOUBLE_UP = 1,
    SINGLE_UP = 2,
    FORTY_FIVE_UP = 3,
    FLAT = 4,
    FORTY_FIVE_DOWN = 5,
    SINGLE_DOWN = 6,
    DOUBLE_DOWN = 7,
    NOT_COMPUTABLE = 8,
    RATE_OUT_OF_RANGE = 9,
};

enum class TEXT_ALIGNMENT : uint8_t {
    LEFT = 0,
    CENTER = 1,
    RIGHT = 2,
};

enum class FONT_TYPE : uint8_t {
    SMALL = 0,
    MEDIUM = 1,
    LARGE = 2,
};

enum class BG_LEVEL : uint8_t {
    INVALID = 0,
    URGENT_LOW = 1,
    WARNING_LOW = 2,
    NORMAL = 3,
    WARNING_HIGH = 4,
    URGENT_HIGH = 5,
};

enum class BG_SOURCE : uint8_t {
    NO_SOURCE = 0,
    NIGHTSCOUT = 1,
    DEXCOM = 2,
    MEDTRONIC = 3,
    API = 4,
    LIBRELINKUP = 5,
    MEDTRUM = 6,
};

enum class DEXCOM_SERVER : uint8_t {
    INVALID = 0,
    US = 1,
    NON_US = 2,
    JAPAN = 3,
};

enum class TIME_FORMAT : uint8_t {
    INVALID = 0,
    HOURS_12 = 1,
    HOURS_24 = 2,
};

enum class BRIGHTNES_MODE : uint8_t {
    MANUAL = 0,
    AUTO_LINEAR = 100,
    AUTO_DIMMED = 101,
};

// Every color the firmware draws with. The enum value IS the RGB565 code, so a DISPLAY_COLOR can
// be used wherever a color is expected without a lookup table, and the codes live in one place
// rather than two.
//
// A note on GRAY, since it is why the data-is-old color is configurable at all: 0xA514 is
// (165, 162, 165) and no channel is at its maximum, so at MIN_BRIGHTNESS the panel cannot render
// it and a stale reading disappears entirely. Every other color here keeps at least one channel
// at maximum. See BGDisplayFace::getDataOldColor().
enum class DISPLAY_COLOR : uint16_t {
    BLACK = 0x0000,
    BLUE = 0x001F,
    GREEN = 0x07E0,
    CYAN = 0x07FF,
    GRAY = 0xA514,
    RED = 0xF800,
    MAGENTA = 0xF81F,
    YELLOW = 0xFFE0,
    WHITE = 0xFFFF,
};

// The colours the night face will draw a reading in. Deliberately only these three: red,
// yellow and green are the glucose band colours and would collide, and gray is invisible at
// night brightness for the reason given above.
enum class NIGHT_VALUE_COLOR : uint16_t {
    WHITE = static_cast<uint16_t>(DISPLAY_COLOR::WHITE),
    MAGENTA = static_cast<uint16_t>(DISPLAY_COLOR::MAGENTA),
    BLUE = static_cast<uint16_t>(DISPLAY_COLOR::BLUE),
};

inline String toString(BG_TREND trend) {
    switch (trend) {
        case BG_TREND::NONE:
            return "NONE";
        case BG_TREND::DOUBLE_UP:
            return "DOUBLE_UP";
        case BG_TREND::SINGLE_UP:
            return "SINGLE_UP";
        case BG_TREND::FORTY_FIVE_UP:
            return "FORTY_FIVE_UP";
        case BG_TREND::FLAT:
            return "FLAT";
        case BG_TREND::FORTY_FIVE_DOWN:
            return "FORTY_FIVE_DOWN";
        case BG_TREND::SINGLE_DOWN:
            return "SINGLE_DOWN";
        case BG_TREND::DOUBLE_DOWN:
            return "DOUBLE_DOWN";
        case BG_TREND::NOT_COMPUTABLE:
            return "NOT_COMPUTABLE";
        case BG_TREND::RATE_OUT_OF_RANGE:
            return "RATE_OUT_OF_RANGE";
        default:
            return "unknown";
    }
}

inline String toString(DISPLAY_COLOR color) {
    switch (color) {
        case DISPLAY_COLOR::BLACK:
            return "black";
        case DISPLAY_COLOR::BLUE:
            return "blue";
        case DISPLAY_COLOR::GREEN:
            return "green";
        case DISPLAY_COLOR::CYAN:
            return "cyan";
        case DISPLAY_COLOR::RED:
            return "red";
        case DISPLAY_COLOR::MAGENTA:
            return "magenta";
        case DISPLAY_COLOR::YELLOW:
            return "yellow";
        case DISPLAY_COLOR::WHITE:
            return "white";
        case DISPLAY_COLOR::GRAY:
        default:
            return "gray";
    }
}

// Parses the value written by toString(DISPLAY_COLOR). An unknown or missing value falls back
// rather than failing, so an older config file - or a hand-edited one - still loads.
inline DISPLAY_COLOR displayColorFromString(const String& value, DISPLAY_COLOR fallback) {
    if (value == "black") {
        return DISPLAY_COLOR::BLACK;
    }
    if (value == "blue") {
        return DISPLAY_COLOR::BLUE;
    }
    if (value == "green") {
        return DISPLAY_COLOR::GREEN;
    }
    if (value == "cyan") {
        return DISPLAY_COLOR::CYAN;
    }
    if (value == "red") {
        return DISPLAY_COLOR::RED;
    }
    if (value == "magenta") {
        return DISPLAY_COLOR::MAGENTA;
    }
    if (value == "yellow") {
        return DISPLAY_COLOR::YELLOW;
    }
    if (value == "white") {
        return DISPLAY_COLOR::WHITE;
    }
    if (value == "gray") {
        return DISPLAY_COLOR::GRAY;
    }
    return fallback;
}

// The colors a data-age setting may use. DISPLAY_COLOR lists every color the firmware draws
// with, which is deliberately wider than what these settings should accept: BLACK is invisible,
// red, green and yellow are what the glucose bands mean, and white is what a fresh trend arrow
// already uses, so any of them would make the age of a reading indistinguishable from its
// severity. The WebUI offers exactly this list, and enforcing it here as well keeps a hand
// written config from recreating the bug the setting exists to escape.
inline bool isDataAgeColor(DISPLAY_COLOR color) {
    return color == DISPLAY_COLOR::GRAY || color == DISPLAY_COLOR::BLUE ||
           color == DISPLAY_COLOR::CYAN || color == DISPLAY_COLOR::MAGENTA;
}

inline String toString(BRIGHTNES_MODE mode) {
    switch (mode) {
        case BRIGHTNES_MODE::MANUAL:
            return "MANUAL";
        case BRIGHTNES_MODE::AUTO_LINEAR:
            return "AUTO_LINEAR";
        case BRIGHTNES_MODE::AUTO_DIMMED:
            return "AUTO_DIMMED";
        default:
            return "unknown";
    }
}

inline String toString(BG_UNIT unit) {
    switch (unit) {
        case BG_UNIT::MGDL:
            return "MGDL";
        case BG_UNIT::MMOLL:
            return "MMOLL";
        default:
            return "unknown";
    }
}

inline String toString(BG_LEVEL level) {
    switch (level) {
        case BG_LEVEL::INVALID:
            return "INVALID";
        case BG_LEVEL::URGENT_LOW:
            return "URGENT_LOW";
        case BG_LEVEL::WARNING_LOW:
            return "WARNING_LOW";
        case BG_LEVEL::NORMAL:
            return "NORMAL";
        case BG_LEVEL::WARNING_HIGH:
            return "WARNING_HIGH";
        case BG_LEVEL::URGENT_HIGH:
            return "URGENT_HIGH";
        default:
            return "unknown";
    }
}

inline String toString(BG_SOURCE source) {
    switch (source) {
        case BG_SOURCE::NO_SOURCE:
            return "NO_SOURCE";
        case BG_SOURCE::NIGHTSCOUT:
            return "NIGHTSCOUT";
        case BG_SOURCE::DEXCOM:
            return "DEXCOM";
        case BG_SOURCE::MEDTRONIC:
            return "MEDTRONIC";
        case BG_SOURCE::API:
            return "API";
        case BG_SOURCE::LIBRELINKUP:
            return "LIBRELINKUP";
        case BG_SOURCE::MEDTRUM:
            return "MEDTRUM";
        default:
            return "unknown";
    }
}

inline String toString(NIGHT_VALUE_COLOR color) {
    switch (color) {
        case NIGHT_VALUE_COLOR::WHITE:
            return "white";
        case NIGHT_VALUE_COLOR::MAGENTA:
            return "magenta";
        case NIGHT_VALUE_COLOR::BLUE:
            return "blue";
        default:
            return "unknown";
    }
}

#endif