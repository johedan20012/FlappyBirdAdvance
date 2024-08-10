#ifndef SPARKS_H
#define SPARKS_H

#include "bn_vector.h"
#include "bn_fixed_rect.h"

#include "bn_sprite_ptr.h"
#include "bn_sprite_animate_actions.h"
#include "bn_sprite_items_sparks.h"

#include "global_stuff.h"

struct Spark{
    bn::sprite_ptr spr;
    bn::sprite_animate_action<7> anim;

    Spark(bn::fixed x, bn::fixed y,int wait_frames);
    ~Spark() = default;

    bool update();
};

class Sparks {
private:
    GlobalStuff& global;

    static constexpr int MAX_SPARKS = 4;
    static constexpr int SPARK_WAIT_FRAMES = 16;

    bn::fixed_rect spawneable_area;
    bn::vector<Spark,MAX_SPARKS> sparks;
public:
    Sparks(bn::fixed_rect _spawneable_area, GlobalStuff& _global);
    ~Sparks() = default;

    void update();
    void clear();
};

#endif