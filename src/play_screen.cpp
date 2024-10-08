#include "play_screen.h"

#include "bn_core.h"

PlayScreen::PlayScreen(GlobalStuff& _global) :
    global(_global),
    portrait_mode(global.is_portrait_mode()),
    state(STATE::GET_READY),
    background(_global),
    bird(global,0,INITIAL_BIRD_POS[portrait_mode].x(),INITIAL_BIRD_POS[portrait_mode].y()),
    pipes(),
    fade_in_ready(true),
    get_ready_label{
        bn::sprite_items::get_ready.create_sprite(GET_READY_LABEL_POSITIONS[portrait_mode][0]),
        bn::sprite_items::get_ready.create_sprite(GET_READY_LABEL_POSITIONS[portrait_mode][1],1),
        bn::sprite_items::get_ready.create_sprite(GET_READY_LABEL_POSITIONS[portrait_mode][2],2)
    },
    how_2_jump_label((portrait_mode) ? bn::sprite_items::how_to_jump_portrait.create_sprite(8,0) : bn::sprite_items::how_to_jump.create_sprite(0, 8)),
    show_gameover(false),
    global_brightness(0),
    gameover_bg((portrait_mode)? bn::regular_bg_items::bg_gameover_portrait.create_bg(8, 48) :bn::regular_bg_items::bg_gameover.create_bg(8, 48)),
    gameover_label{
        bn::sprite_items::gameover.create_sprite(GAMEOVER_LABEL_POSITIONS[portrait_mode][0],0),
        bn::sprite_items::gameover.create_sprite(GAMEOVER_LABEL_POSITIONS[portrait_mode][1],1),
        bn::sprite_items::gameover.create_sprite(GAMEOVER_LABEL_POSITIONS[portrait_mode][2],2)
    },
    medal(bn::sprite_items::medals.create_sprite(MEDAL_POSITION[portrait_mode])),
    medal_sparks(bn::fixed_rect(MEDAL_POSITION[portrait_mode].x(),MEDAL_POSITION[portrait_mode].y(), 24,24), global),
    new_label(bn::sprite_items::new_label.create_sprite(NEW_LABEL_POSITION[portrait_mode])),
    buttons{
        bn::sprite_items::gameover_buttons.create_sprite(BUTTONS_POSITIONS[portrait_mode][0],0),
        bn::sprite_items::gameover_buttons.create_sprite(BUTTONS_POSITIONS[portrait_mode][1],1)
    },
    button_over(bn::sprite_items::button_over.create_sprite(BUTTONS_POSITIONS[portrait_mode][0])),
    selected_button(false)
    { 
    global.reset_score();   
    gameover_bg.set_visible(false);
    for(auto& label : gameover_label) label.set_visible(false);
    medal.set_visible(false);
    new_label.set_visible(false);
    for(auto& button : buttons) button.set_visible(false);
    button_over.set_visible(false);
    
    for(auto& label : gameover_label) label.set_bg_priority(0);
    gameover_bg.set_priority(0);
    medal.set_bg_priority(0);
    new_label.set_bg_priority(0);
    for(auto& button : buttons) button.set_bg_priority(0);
    button_over.set_bg_priority(0);
    global.huge_gen().generate(0, SCORE_POSITION[portrait_mode], score_label,portrait_mode);

    if(portrait_mode){
        for(auto& label : get_ready_label) label.set_rotation_angle(270);
        for(auto& label : gameover_label) label.set_rotation_angle(270);
        medal.set_rotation_angle(270);
        new_label.set_rotation_angle(270);
        for(auto& button : buttons) button.set_rotation_angle(270);
        button_over.set_rotation_angle(270);
    }
}

bn::optional<SCREEN_TYPE> PlayScreen::update() {
    switch(state){
        case STATE::GET_READY:
            update_get_ready();
            break;
        case STATE::PLAYING:
            update_playing();
            break;
        default: // STATE::GAME_OVER:
            return update_game_over();
            break;
    }
    return bn::nullopt;
}

void PlayScreen::update_get_ready(){
    if(fade_in_ready){
        bg_fade_action = bn::bg_palettes_fade_to_action(16, 0);
        sprite_fade_action = bn::sprite_palettes_fade_to_action(16, 0);

        while(!bg_fade_action.value().done()){
            bg_fade_action.value().update();
            sprite_fade_action.value().update();
            bn::core::update();
        }

        bg_fade_action.reset();
        sprite_fade_action.reset();
        fade_in_ready = false;
    }

    if(bn::keypad::a_pressed() || (portrait_mode && bn::keypad::right_pressed())){
        toggle_get_ready_things(false);

        state = STATE::PLAYING;
        bird.activate();
        pipes.emplace(Pipes(global));
    }

    background.update();
    bird.update();
}

void PlayScreen::update_playing(){
    background.update();
    bird.update();
    pipes.value().update();

    if(pipes.value().check_passed(bird.hitbox())) {
        global.add_point();
        score_label.clear();
        global.huge_gen().generate(global.score(), SCORE_POSITION[portrait_mode], score_label,portrait_mode);
        bn::sound_items::sfx_point.play();
    }
    
    if(pipes.value().check_collision(bird.hitbox())) bird.kill(true);
    
    if (bird.is_dead()){
        bn::bg_palettes::set_brightness(1);
        bn::sprite_palettes::set_brightness(1);
        global_brightness = 1;
        wait_frames = 60;
        state = STATE::GAME_OVER;
    }
}

bn::optional<SCREEN_TYPE> PlayScreen::update_game_over(){
    bird.update();

    if(global_brightness > 0){
        global_brightness -= 0.0625;
        bn::bg_palettes::set_brightness(global_brightness);
        bn::sprite_palettes::set_brightness(global_brightness);
        return bn::nullopt;
    }

    if(wait_frames > 0){
        wait_frames--;
        if(wait_frames == 0) bn::sound_items::sfx_swooshing.play();
        return bn::nullopt;
    }

    if(bird.is_on_floor() && !show_gameover){
        toggle_game_over_things(true);
        if(global.score() >= 10){
            medal.set_tiles(bn::sprite_items::medals.tiles_item(), 0);
            if(global.score() >= 20) medal.set_tiles(bn::sprite_items::medals.tiles_item(), 1);
            if(global.score() >= 30) medal.set_tiles(bn::sprite_items::medals.tiles_item(), 2);
            if(global.score() >= 40) medal.set_tiles(bn::sprite_items::medals.tiles_item(), 3);
        }else{
            medal.set_visible(false);
        }
        score_label.clear();
        global.normal_gen().generate(global.score(), FINAL_SCORE_POSITION[portrait_mode], final_score_label,portrait_mode);
        if(global.save_data().is_high_score(global.score())) {
            new_label.set_visible(true);
            global.save_data().set_high_score(global.score());
            global.save_data().save_to_sram();
        }else{
            new_label.set_visible(false);
        } 
        global.normal_gen().generate(global.save_data().high_score(), BEST_SCORE_POSITION[portrait_mode], best_score_label,portrait_mode);

        show_gameover = true;
    }

    if(show_gameover && medal.visible()) medal_sparks.update();

    if(bird.is_on_floor()){
        if(((bn::keypad::left_pressed() && !portrait_mode) || (bn::keypad::up_pressed() && portrait_mode)) && selected_button){
            selected_button = false;
            if(portrait_mode) button_over.set_y(BUTTONS_POSITIONS[portrait_mode][0].y());
            else button_over.set_x(BUTTONS_POSITIONS[portrait_mode][0].x());
        }else if(((bn::keypad::right_pressed() && !portrait_mode) || (bn::keypad::down_pressed() && portrait_mode)) && !selected_button){
            selected_button = true;
            if(portrait_mode) button_over.set_y(BUTTONS_POSITIONS[portrait_mode][1].y());
            else button_over.set_x(BUTTONS_POSITIONS[portrait_mode][1].x());
        }

        if(bn::keypad::a_pressed() || (bn::keypad::right_pressed() && portrait_mode)){
            if(!selected_button){ // ok button
                show_gameover = false;

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

                bird = Bird(global,global.rng().get_unbiased_int(3),INITIAL_BIRD_POS[portrait_mode].x(),INITIAL_BIRD_POS[portrait_mode].y());
                background.change_time(global.rng().get_unbiased_int(2));
                pipes.reset();

                global.reset_score();
                score_label.clear();
                global.huge_gen().generate(global.score(), SCORE_POSITION[portrait_mode], score_label,portrait_mode);
                final_score_label.clear();
                best_score_label.clear();

                toggle_game_over_things(false);
                toggle_get_ready_things(true);
                medal_sparks.clear();
                fade_in_ready = true;
                state = STATE::GET_READY;
            }else{ // menu button
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

                return SCREEN_TYPE::TITLE;
            }
        }
    }

    return bn::nullopt;
}

void PlayScreen::toggle_get_ready_things(bool show){
    for(auto& label : get_ready_label) label.set_visible(show);
    how_2_jump_label.set_visible(show);
}

void PlayScreen::toggle_game_over_things(bool show){
    gameover_bg.set_visible(show);
    for(auto& label : gameover_label) label.set_visible(show);
    medal.set_visible(show);
    new_label.set_visible(show);
    buttons[0].set_visible(show);
    buttons[1].set_visible(show);
    button_over.set_visible(show);
}