#include "start_screen.h"

#include "bn_core.h"

StartScreen::StartScreen(GlobalStuff& _global) :
    global(_global),
    background(bn::make_unique<Background>(global)),
    bird(bn::make_unique<Bird>(global,0,0,-16)),
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
    },
    orientation_label(bn::sprite_items::portrait_label.create_sprite(140, 60))
    {
    copy_right.set_bg_priority(0);
    for(auto& port_by_spr: port_by) port_by_spr.set_bg_priority(0);
    
    update_to_orientation();
}

bn::optional<SCREEN_TYPE> StartScreen::update() {
    if(bn::keypad::select_pressed()){
        fade_out();    
        global.toggle_portrait_mode();
        update_to_orientation();
    }

    if(bn::keypad::a_pressed() || bn::keypad::start_pressed() || (global.is_portrait_mode() && bn::keypad::right_pressed())) { 
        if(global.is_portrait_mode()) play_button.set_x(-18); else play_button.set_y(26);
    }

    if(bn::keypad::a_released() || bn::keypad::start_released() || (global.is_portrait_mode() && bn::keypad::right_released())){
        if(global.is_portrait_mode()) play_button.set_x(-16); else play_button.set_y(24);

        fade_out();
        return SCREEN_TYPE::GAME;
    }

    bird->update();
    background->update();
    return bn::nullopt;
}

void StartScreen::update_to_orientation(){
    // Change the orientation
    background.reset();
    background = bn::make_unique<Background>(global);
    bird.reset();
    bird = bn::make_unique<Bird>(global,0,0,(global.is_portrait_mode())? 37 :-16);

    if(global.is_portrait_mode()){
        logo[0].set_position(90, -28);
        logo[0].set_rotation_angle(270);
        logo[1].set_position(90, 4);
        logo[1].set_rotation_angle(270);
        logo[2].set_position(90, 36);
        logo[2].set_rotation_angle(270);

        play_button.set_position(-16, 0);
        play_button.set_rotation_angle(270);

        copy_right.set_position(-90, 0);
        copy_right.set_rotation_angle(270);

        port_by[0].set_position(-104, -44);
        port_by[0].set_rotation_angle(270);
        port_by[1].set_position(-104, -12);
        port_by[1].set_rotation_angle(270);
        port_by[2].set_position(-104, 20);
        port_by[2].set_rotation_angle(270);
        port_by[3].set_position(-104, 52);
        port_by[3].set_rotation_angle(270);

        orientation_label = bn::sprite_items::landscape_label.create_sprite(-53, 120);
    }else{
        logo[0].set_position(-28, -48);
        logo[0].remove_affine_mat();
        logo[1].set_position(4, -48);
        logo[1].remove_affine_mat();
        logo[2].set_position(36, -48);
        logo[2].remove_affine_mat();

        play_button.set_position(0, 24);
        play_button.remove_affine_mat();

        copy_right.set_position(0, 52);
        copy_right.remove_affine_mat();

        port_by[0].set_position(-44, 66);
        port_by[0].remove_affine_mat();
        port_by[1].set_position(-12, 66);
        port_by[1].remove_affine_mat();
        port_by[2].set_position(20, 66);
        port_by[2].remove_affine_mat();
        port_by[3].set_position(52, 66);
        port_by[3].remove_affine_mat();

        orientation_label = bn::sprite_items::portrait_label.create_sprite(160, 60);
    }
    
    fade_in();
}

void StartScreen::fade_in(){
    bg_fade_action = bn::bg_palettes_fade_to_action(16, 0);
    sprite_fade_action = bn::sprite_palettes_fade_to_action(16, 0);

    while(!bg_fade_action.value().done()){ 
        bg_fade_action.value().update();
        sprite_fade_action.value().update();
        
        bird->update();
        background->update();
        bn::core::update();
    }

    bn::sprite_move_to_action orient_label_action(orientation_label,30, 110, 60);
    if(global.is_portrait_mode()) orient_label_action = bn::sprite_move_to_action(orientation_label, 30, -53, 64);
    
    while(!orient_label_action.done()){
        orient_label_action.update();

        bird->update();
        background->update();
        bn::core::update();
    }

    bg_fade_action.reset();
    sprite_fade_action.reset();
}

void StartScreen::fade_out(){
    bn::bg_palettes::set_fade(bn::color(0,0,0),0);
    bg_fade_action = bn::bg_palettes_fade_to_action(16, 1);
    bn::sprite_palettes::set_fade(bn::color(0,0,0),0);
    sprite_fade_action = bn::sprite_palettes_fade_to_action(16, 1);

    bn::sound_items::sfx_swooshing.play();
    while(!bg_fade_action.value().done()){
        bg_fade_action.value().update();
        sprite_fade_action.value().update();
        
        if(global.is_portrait_mode()) orientation_label.set_y(orientation_label.y() + 4);
        else orientation_label.set_x(orientation_label.x() + 4);
        bn::core::update();
    }

    bg_fade_action.reset();
    sprite_fade_action.reset();
}