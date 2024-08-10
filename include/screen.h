#ifndef SCREEN_H
#define SCREEN_H

#include "bn_optional.h"

enum class SCREEN_TYPE {
    LOGOS,
    TITLE,
    GAME,
    SCOREBOARD
};

class Screen {
public:
    virtual ~Screen() = default;

    virtual bn::optional<SCREEN_TYPE> update() = 0;

protected:
    Screen() = default;
};

#endif