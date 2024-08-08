#include "bird.h"

Bird::Bird() : 
    spr(bn::sprite_items::bird.create_sprite(-64,0)),
    flap_action(bn::create_sprite_animate_action_forever(spr, 3, bn::sprite_items::bird.tiles_item(), 0, 1, 2, 1)),
    y_velocity(0),
    idle(true){
}

void Bird::update(){
    flap_action.update();

    if(idle) return;

    y_velocity += GRAVITY;
    if(bn::keypad::a_pressed()) y_velocity = -2.2;

    spr.set_y(spr.y() + y_velocity);
    if(spr.y() > 48) spr.set_y(48); // 80 -24 (floor) - 8 (bird)
}

bool Bird::is_idle() const{
    return idle;
}

void Bird::set_idle(bool _idle){
    idle = _idle;
}  

bn::fixed_rect Bird::hitbox() const{
    return bn::fixed_rect(spr.x(), spr.y(), 9,9);
}