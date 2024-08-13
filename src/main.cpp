#include "bn_core.h"
#include "bn_optional.h"
#include "bn_unique_ptr.h"

#include "play_screen.h"
#include "start_screen.h"
#include "global_stuff.h"

#include "bn_bg_maps.h"

int main(){
    bn::core::init();

    GlobalStuff global;
    bn::unique_ptr<Screen> current_screen(bn::make_unique<StartScreen>(global));
    bn::optional<SCREEN_TYPE> next_screen;

    while(true){
        if(current_screen != nullptr) next_screen = current_screen->update();
        if(next_screen){
            switch(*next_screen){
                case SCREEN_TYPE::LOGOS:
                    // current_screen = InitScreen();
                    break;
                case SCREEN_TYPE::TITLE:
                    current_screen.reset();
                    current_screen = bn::make_unique<StartScreen>(global);
                    break;
                default : // SCREEN_TYPE::GAME:
                    current_screen.reset();
                    current_screen = bn::make_unique<PlayScreen>(global);
                    break;
            }
        }

        global.rng().update();
        bn::core::update();
    }
}
