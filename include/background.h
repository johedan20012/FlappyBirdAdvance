#ifndef BACKGROUND_H
#define BACKGROUND_H

#include "bn_regular_bg_ptr.h"
#include "bn_regular_bg_items_bg_day.h"
#include "bn_regular_bg_items_bg_night.h"
#include "bn_regular_bg_items_bg_floor.h"
#include "bn_regular_bg_items_bg_day_portrait.h"
#include "bn_regular_bg_items_bg_night_portrait.h"
#include "bn_regular_bg_items_bg_floor_portrait.h"

#include "global_stuff.h"

class Background {
private:
    bool portrait_mode;

    bn::regular_bg_ptr bg_day;
    bn::regular_bg_ptr bg_floor;

public:
    Background(GlobalStuff& _global);
    ~Background() = default;

    void update();

    void change_time(bool night);
};

#endif