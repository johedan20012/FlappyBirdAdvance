#include "background.h"

Background::Background() :
    bg_day(bn::regular_bg_items::bg_day.create_bg(8, 48)),
    bg_floor(bn::regular_bg_items::bg_floor.create_bg(8, 48))
    {
    bg_floor.set_priority(1);
}

void Background::update(){
    bg_floor.set_x(bg_floor.x() - 1);
    if(bg_floor.x() < -11) bg_floor.set_x(0);
}

void Background::change_time(bool night){
    if(night)bg_day.set_item(bn::regular_bg_items::bg_night);
    else bg_day.set_item(bn::regular_bg_items::bg_day);
}