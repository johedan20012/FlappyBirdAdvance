#include "bird.h"

Bird::Bird(GlobalStuff& _global,int color,bn::fixed _static_axis_pos, bn::fixed _moving_axis_offset) :
    portrait_mode(_global.is_portrait_mode()), 
    iddle_offset(_moving_axis_offset),
    spr(bn::sprite_items::bird.create_sprite(_static_axis_pos,_moving_axis_offset)),
    flap_action(bn::create_sprite_animate_action_forever(spr, 3, bn::sprite_items::bird.tiles_item(), 0, 1, 2, 1)),
    moving_axis_velocity(0),
    idle(true),
    dead(false){
    switch (color){
    case 0:
        break; // default
    case 1:
        spr.set_tiles(bn::sprite_items::bird.tiles_item(),3);
        flap_action = bn::create_sprite_animate_action_forever(spr, 3, bn::sprite_items::bird.tiles_item(), 3, 4, 5, 4);
        break;
    default: // 2 -> red
        spr.set_tiles(bn::sprite_items::bird.tiles_item(),6);
        flap_action = bn::create_sprite_animate_action_forever(spr, 3, bn::sprite_items::bird.tiles_item(), 6, 7, 8, 7);
        break;
    }

    spr.set_rotation_angle((portrait_mode)? 270 : 0);
    if(portrait_mode){
        spr.set_x(_moving_axis_offset);
        spr.set_y(_static_axis_pos);
    }

    spr.set_bg_priority(2);
    spr.set_z_order(-1);
}

void Bird::update(){
    if(is_on_floor()) return;

    flap_action.update();

    if(idle){
        if(portrait_mode){
        // Left and right
            if(dir > 0 && spr.x() > iddle_offset+4) dir = -0.35;
            if(dir < 0 && spr.x() < iddle_offset-4) dir = 0.35;
            spr.set_x(spr.x() + dir);
        }else{
        // Up and down
            if(dir > 0  && spr.y() > iddle_offset+4) dir = -0.35;
            if(dir < 0 && spr.y() < iddle_offset-4) dir = 0.35;
            spr.set_y(spr.y() + dir);
        }
        return;
    } 

    moving_axis_velocity += GRAVITY;
    if((bn::keypad::a_pressed() || (bn::keypad::right_pressed() && portrait_mode)) & !dead) {
        moving_axis_velocity = JUMP_VELOCITY;
        bn::sound_items::sfx_wing.play();
    }

    if(portrait_mode){
        spr.set_x(spr.x() - moving_axis_velocity);
        if(spr.x() < FLOOR_LEVEL_PORTRAIT){
            spr.set_x(FLOOR_LEVEL_PORTRAIT); 
            if(!dead) kill();
        }
        if(spr.x() > 140) spr.set_x(-140); // Can't cheat the game
    }else{
        spr.set_y(spr.y() + moving_axis_velocity);
        if(spr.y() > FLOOR_LEVEL_LANDSCAPE){
            spr.set_y(FLOOR_LEVEL_LANDSCAPE); 
            if(!dead) kill();
        }
        if(spr.y() < -180) spr.set_y(-180); // Can't cheat the game
    }
    // Calculate angle of rotation based on velocity,
    // 22.5 degrees max with velocity > 0
    // -90 degrees max with velocity in range [0,4]
    if(moving_axis_velocity < 0) angle = 22.5;
    else angle = (((3-bn::min(moving_axis_velocity,bn::fixed(3)))*112.5) / 3 )+270; // 360-112.5 
    if(portrait_mode) angle += 270;
    angle %= 360;
    if(is_on_floor()) angle = (portrait_mode)? 180 : 270;
    spr.set_rotation_angle(angle);
}

bool Bird::is_idle() const{
    return idle;
}

void Bird::activate(){
    idle = false;
}

void Bird::kill(bool hit_pipe){
    bn::sound_items::sfx_hit.play();
    dead = true;
    if(hit_pipe) bn::sound_items::sfx_die.play();
}

bool Bird::is_dead() const{
    return dead;
}

bool Bird::is_on_floor() const{
    if(portrait_mode) return spr.x() == FLOOR_LEVEL_PORTRAIT;
    return spr.y() == FLOOR_LEVEL_LANDSCAPE;
}

bn::fixed_rect Bird::hitbox() const{
    return bn::fixed_rect(spr.x(), spr.y(), BIRD_WIDTH, BIRD_HEIGHT);
}