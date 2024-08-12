#include "start_screen.h"

#include "bn_core.h"

StartScreen::StartScreen() :
    background(),
    bird(0,0,-16),
    logo{
        bn::sprite_items::logo.create_sprite(-28, -48),
        bn::sprite_items::logo.create_sprite(4, -48,1),
        bn::sprite_items::logo.create_sprite(36, -48,2)
    },
    play_button(bn::sprite_items::play_button.create_sprite(0, 24)),
    copy_right(bn::sprite_items::copyright.create_sprite(0, 52)),
    port_by{
        bn::sprite_items::port_by.create_sprite(-44, 66),
        bn::sprite_items::port_by.create_sprite(-12, 66,1),
        bn::sprite_items::port_by.create_sprite(20, 66,2),
        bn::sprite_items::port_by.create_sprite(52, 66,3)
    }
    {
    bg_fade_action = bn::bg_palettes_fade_to_action(16, 0);
    sprite_fade_action = bn::sprite_palettes_fade_to_action(16, 0);

    while(!bg_fade_action.value().done()){
        bg_fade_action.value().update();
        sprite_fade_action.value().update();
        bird.update();
        background.update();
        bn::core::update();
    }

    bg_fade_action.reset();
    sprite_fade_action.reset();
}

bn::optional<SCREEN_TYPE> StartScreen::update() {
    if(bn::keypad::a_pressed() || bn::keypad::start_pressed()) play_button.set_y(26);

    if(bn::keypad::a_released() || bn::keypad::start_released()){
        play_button.set_y(24);

        bn::bg_palettes::set_fade(bn::color(0,0,0),0);
        bg_fade_action = bn::bg_palettes_fade_to_action(16, 1);
        bn::sprite_palettes::set_fade(bn::color(0,0,0),0);
        sprite_fade_action = bn::sprite_palettes_fade_to_action(16, 1);

        bn::sound_items::sfx_swooshing.play();
        while(!bg_fade_action.value().done()){
            bg_fade_action.value().update();
            sprite_fade_action.value().update();
            bn::core::update();
        }

        bg_fade_action.reset();
        sprite_fade_action.reset();
        return SCREEN_TYPE::GAME;
    }

    bird.update();
    background.update();
    return bn::nullopt;
}