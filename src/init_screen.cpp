#include "init_screen.h"

#include "bn_core.h"

InitScreen::InitScreen():
    logo_bg(bn::regular_bg_items::bg_gears_logo.create_bg(8, 48))
    {
    bn::bg_palettes::set_fade(bn::color(0,0,0),1);
}

bn::optional<SCREEN_TYPE> InitScreen::update(){
    int wait_frames = 90;

    bg_fade_action = bn::bg_palettes_fade_to_action(32, 0);
    while(!bg_fade_action.value().done()){ 
        bg_fade_action.value().update();
        bn::core::update();
    }
    bg_fade_action.reset();

    while(wait_frames--) bn::core::update();

    bg_fade_action = bn::bg_palettes_fade_to_action(32, 1);
    while(!bg_fade_action.value().done()){ 
        bg_fade_action.value().update();
        bn::core::update();
    }
    bg_fade_action.reset();
    return SCREEN_TYPE::TITLE;
}

