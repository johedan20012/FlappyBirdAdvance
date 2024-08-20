#ifndef INIT_SCREEN_H
#define INIT_SCREEN_H

#include "bn_regular_bg_ptr.h"
#include "bn_bg_palettes_actions.h"
#include "bn_regular_bg_items_bg_gears_logo.h"
#include "screen.h"

class InitScreen : public Screen {
    private:
        bn::regular_bg_ptr logo_bg;
        bn::optional<bn::bg_palettes_fade_to_action> bg_fade_action;

    public:
        InitScreen();
        ~InitScreen() = default;

        bn::optional<SCREEN_TYPE> update() final;
};

#endif