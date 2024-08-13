#ifndef START_SCREEN_H
#define START_SCREEN_H

#include "bn_unique_ptr.h"

#include "bn_bg_palettes_actions.h"

#include "bn_sprite_ptr.h"
#include "bn_sprite_actions.h"
#include "bn_sprite_items_logo.h"
#include "bn_sprite_items_port_by.h"
#include "bn_sprite_items_copyright.h"
#include "bn_sprite_palettes_actions.h"
#include "bn_sprite_items_play_button.h"
#include "bn_sprite_items_portrait_label.h"
#include "bn_sprite_items_landscape_label.h"

#include "bn_sound_items.h"

#include "screen.h"
#include "background.h"
#include "bird.h"

class StartScreen : public Screen {
private:
    GlobalStuff& global;
    bn::unique_ptr<Background> background;
    bn::unique_ptr<Bird> bird;
    bn::sprite_ptr logo[3];
    bn::sprite_ptr play_button;
    bn::sprite_ptr copy_right;
    bn::sprite_ptr port_by[4];
    bn::sprite_ptr orientation_label;

    bn::optional<bn::bg_palettes_fade_to_action> bg_fade_action;
    bn::optional<bn::sprite_palettes_fade_to_action> sprite_fade_action;
public:
    StartScreen(GlobalStuff& _global);
    ~StartScreen() = default;

    bn::optional<SCREEN_TYPE> update() final;

    void update_to_orientation();

private:
    void fade_in();
    void fade_out();
};

#endif