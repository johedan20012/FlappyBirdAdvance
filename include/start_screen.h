#ifndef START_SCREEN_H
#define START_SCREEN_H

#include "bn_bg_palettes_actions.h"

#include "bn_sprite_ptr.h"
#include "bn_sprite_items_logo.h"
#include "bn_sprite_items_port_by.h"
#include "bn_sprite_items_copyright.h"
#include "bn_sprite_palettes_actions.h"
#include "bn_sprite_items_play_button.h"

#include "bn_sound_items.h"

#include "screen.h"
#include "background.h"
#include "bird.h"

class StartScreen : public Screen {
private:
    Background background;
    Bird bird;
    bn::sprite_ptr logo[3];
    bn::sprite_ptr play_button;
    bn::sprite_ptr copy_right;
    bn::sprite_ptr port_by[3];

    bn::optional<bn::bg_palettes_fade_to_action> bg_fade_action;
    bn::optional<bn::sprite_palettes_fade_to_action> sprite_fade_action;
public:
    StartScreen();
    ~StartScreen() = default;

    bn::optional<SCREEN_TYPE> update() final;
};

#endif