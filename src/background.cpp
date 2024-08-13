#include "background.h"

Background::Background(GlobalStuff& _global) :
    portrait_mode(_global.is_portrait_mode()),
    bg_day(bn::regular_bg_items::bg_day.create_bg(8, 48)),
    bg_floor(bn::regular_bg_items::bg_floor.create_bg(8, 48))
    {
    if(portrait_mode){
        bg_day.set_item(bn::regular_bg_items::bg_day_portrait);
        bg_floor.set_item(bn::regular_bg_items::bg_floor_portrait);
    }
    bg_floor.set_priority(1);
}

void Background::update(){
    if(portrait_mode){
        bg_floor.set_y(bg_floor.y() - 1);
        if(bg_floor.y() < -11) bg_floor.set_y(0);
    }else{
        bg_floor.set_x(bg_floor.x() - 1);
        if(bg_floor.x() < -11) bg_floor.set_x(0);
    }
}

void Background::change_time(bool night){
    if(portrait_mode){
        if(night)bg_day.set_item(bn::regular_bg_items::bg_night_portrait);
        else bg_day.set_item(bn::regular_bg_items::bg_day_portrait);
    }else{
        if(night)bg_day.set_item(bn::regular_bg_items::bg_night);
        else bg_day.set_item(bn::regular_bg_items::bg_day);
    }
}