#include "sparks.h"

Spark::Spark(bn::fixed x, bn::fixed y,int wait_frames) :
    spr(bn::sprite_items::sparks.create_sprite(x, y,3)),
    anim(bn::create_sprite_animate_action_once(spr, wait_frames, bn::sprite_items::sparks.tiles_item(), 3, 2, 1, 0, 1, 2, 3))
{}

bool Spark::update(){
    anim.update();
    if(anim.done()) return true; // done
    return false;
}

Sparks::Sparks(bn::fixed_rect _spawneable_area,GlobalStuff& _global) :
    global(_global),
    spawneable_area(_spawneable_area)
{}

void Sparks::update(){
    for (auto it = sparks.begin(); it != sparks.end();){
        if ((*it).update())
            it = sparks.erase(it);
        else
            ++it;
    }

    if(sparks.size() < MAX_SPARKS && global.rng().get_unbiased_int(10) == 0){
        bn::fixed x = ((global.rng().get_unbiased_int(100)*spawneable_area.width())/100) + spawneable_area.left();
        bn::fixed y = ((global.rng().get_unbiased_int(100)*spawneable_area.height())/100) + spawneable_area.top();
        sparks.push_back(Spark(x,y,SPARK_WAIT_FRAMES));
        sparks.back().spr.set_bg_priority(0);
    }
}

void Sparks::clear(){
    sparks.clear();
}