#include "play_screen.h"

#include "bn_core.h"

PlayScreen::PlayScreen(GlobalStuff& _global) :
    global(_global),
    state(STATE::GET_READY),
    background(),
    bird(),
    pipes(),
    fade_in_ready(true),
    get_ready_label{
        bn::sprite_items::get_ready.create_sprite(-32, -24),
        bn::sprite_items::get_ready.create_sprite(0,   -24,1),
        bn::sprite_items::get_ready.create_sprite(32,  -24,2)
    },
    how_2_jump_label(bn::sprite_items::how_to_jump.create_sprite(0, 8)),
    show_gameover(false),
    global_brightness(0),
    gameover_bg(bn::regular_bg_items::bg_gameover.create_bg(8, 48)),
    gameover_label{
        bn::sprite_items::gameover.create_sprite(-32, -48),
        bn::sprite_items::gameover.create_sprite(0, -48,1),
        bn::sprite_items::gameover.create_sprite(32, -48,2)
    },
    medal(bn::sprite_items::medals.create_sprite(-32, -8)),
    medal_sparks(bn::fixed_rect(-32, -8, 24, 24), global),
    new_label(bn::sprite_items::new_label.create_sprite(20, -8))
    {    
    gameover_bg.set_visible(false);
    for(auto& label : gameover_label) label.set_visible(false);
    medal.set_visible(false);
    new_label.set_visible(false);
    
    for(auto& label : gameover_label) label.set_bg_priority(0);
    gameover_bg.set_priority(0);
    medal.set_bg_priority(0);
    new_label.set_bg_priority(0);
    global.huge_gen().generate(0, SCORE_POSITION, score_label);
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
            update_game_over();
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

    if(bn::keypad::a_pressed()){
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

    if(bn::keypad::b_held()) global.add_point();

    if(pipes.value().check_passed(bird.hitbox())) {
        global.add_point();
        score_label.clear();
        global.huge_gen().generate(global.score(), SCORE_POSITION, score_label);
        bn::sound_items::sfx_point.play();
    }

    if(!bn::keypad::b_held()){
        if(pipes.value().check_collision(bird.hitbox())) bird.kill(true);
    }
    
    if (bird.is_dead()){
        bn::bg_palettes::set_brightness(1);
        bn::sprite_palettes::set_brightness(1);
        global_brightness = 1;
        wait_frames = 60;
        state = STATE::GAME_OVER;
    }
}

void PlayScreen::update_game_over(){
    bird.update();

    if(global_brightness > 0){
        global_brightness -= 0.0625;
        bn::bg_palettes::set_brightness(global_brightness);
        bn::sprite_palettes::set_brightness(global_brightness);
        return;
    }

    if(wait_frames > 0){
        wait_frames--;
        if(wait_frames == 0) bn::sound_items::sfx_swooshing.play();
        return;
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
        global.normal_gen().generate(global.score(), FINAL_SCORE_POSITION, final_score_label);
        if(global.save_data().is_high_score(global.score())) {
            new_label.set_visible(true);
            global.save_data().set_high_score(global.score());
            global.save_data().save_to_sram();
        }else{
            new_label.set_visible(false);
        } 
        global.normal_gen().generate(global.save_data().high_score(), BEST_SCORE_POSITION, best_score_label);

        show_gameover = true;
    }

    if(show_gameover && medal.visible()) medal_sparks.update();

    if(bird.is_on_floor() && bn::keypad::a_pressed()){
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

        bird = Bird();
        pipes.reset();

        global.reset_score();
        score_label.clear();
        global.huge_gen().generate(global.score(), SCORE_POSITION, score_label);
        final_score_label.clear();
        best_score_label.clear();

        toggle_game_over_things(false);
        toggle_get_ready_things(true);
        medal_sparks.clear();
        fade_in_ready = true;
        state = STATE::GET_READY;
    }
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
}