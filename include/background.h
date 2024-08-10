#ifndef BACKGROUND_H
#define BACKGROUND_H

#include "bn_regular_bg_ptr.h"
#include "bn_regular_bg_items_bg_day.h"
#include "bn_regular_bg_items_bg_night.h"
#include "bn_regular_bg_items_bg_floor.h"

class Background {
private:
    bn::regular_bg_ptr bg_day;
    bn::regular_bg_ptr bg_floor;

public:
    Background();
    ~Background() = default;

    void update();

    void change_time(bool night);
};

#endif