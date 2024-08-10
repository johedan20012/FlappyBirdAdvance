#include "bird.h"

Bird::Bird(int color) : 
    spr(bn::sprite_items::bird.create_sprite(-64,0)),
    flap_action(bn::create_sprite_animate_action_forever(spr, 3, bn::sprite_items::bird.tiles_item(), 0, 1, 2, 1)),
    y_velocity(0),
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

    spr.set_bg_priority(2);
    spr.set_z_order(-1);
    spr.set_rotation_angle(0);
}

void Bird::update(){
    if(is_on_floor()) return;

    flap_action.update();

    if(idle){
        // Up and down
        if(dir > 0 && spr.y() > 4) dir = -0.35;
        if(dir < 0 && spr.y() < -4) dir = 0.35;
        spr.set_y(spr.y() + dir);
        return;
    } 

    y_velocity += GRAVITY;
    if(bn::keypad::a_pressed() & !dead) {
        y_velocity = JUMP_VELOCITY;
        bn::sound_items::sfx_wing.play();
    }

    spr.set_y(spr.y() + y_velocity);
    if(spr.y() > FLOOR_Y){
        spr.set_y(FLOOR_Y); 
        if(!dead) kill();
    }
    if(spr.y() < -180) spr.set_y(-180); // Can't cheat the game
    
    // Calculate angle of rotation based on velocity,
    // 22.5 degrees max with velocity > 0
    // -90 degrees max with velocity in range [0,4]
    if(y_velocity < 0) angle = 22.5;
    else angle = ((((3-bn::min(y_velocity,bn::fixed(3)))*112.5) / 3 )+270) % 360; // 360-112.5 
    if(is_on_floor()) angle = 270;
    spr.set_rotation_angle(angle);
}

bool Bird::is_idle() const{
    return idle;
}

void Bird::activate(){
    idle = false;
    spr.set_y(0);
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
    return spr.y() == FLOOR_Y;
}

bn::fixed_rect Bird::hitbox() const{
    return bn::fixed_rect(spr.x(), spr.y(), BIRD_WIDTH, BIRD_HEIGHT);
}