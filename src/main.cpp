#include "bn_core.h"
#include "bn_optional.h"
#include "global_stuff.h"

#include "play_screen.h"

int main(){
    bn::core::init();

    GlobalStuff global;
    PlayScreen play_screen(global);
   
    while(true){
        play_screen.update();
        bn::core::update();
    }
}
