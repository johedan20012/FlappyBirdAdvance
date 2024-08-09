#include "bn_core.h"
#include "bn_log.h"
#include "bn_vector.h"
#include "bn_optional.h"

#include "bn_regular_bg_ptr.h"
#include "bn_regular_bg_items_bg_day.h"
#include "bn_regular_bg_items_bg_floor.h"

#include "bird.h"
#include "pipes.h"
#include "global_stuff.h"

int main(){
    bn::core::init();

    bn::regular_bg_ptr background = bn::regular_bg_items::bg_day.create_bg(8,48);
    bn::regular_bg_ptr floor = bn::regular_bg_items::bg_floor.create_bg(8,48);
    floor.set_priority(1);
    Bird bird;
    bn::optional<Pipes> pipes;
    GlobalStuff global;

    while(true){
        if(bn::keypad::a_pressed() && bird.is_idle()){
            bird.set_idle(false);
            pipes.emplace(Pipes(global));
        }

        floor.set_x(floor.x() - 1);
        if(floor.x() < -11){
            floor.set_x(0);
        }
        
        bird.update();
        if(pipes.has_value()){ 
            pipes.value().update();
            if(pipes.value().check_collision(bird.hitbox())){
                pipes.reset();
                bird.set_idle(true);
            }
        }
        bn::core::update();
    }
}
