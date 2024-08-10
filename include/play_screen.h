#ifndef PLAY_SCREEN_H
#define PLAY_SCREEN_H

#include "bn_bg_palettes.h"
#include "bn_bg_palettes_actions.h"
#include "bn_regular_bg_items_bg_gameover.h"

#include "bn_sprite_palettes.h"
#include "bn_sprite_items_medals.h"
#include "bn_sprite_items_gameover.h"
#include "bn_sprite_items_new_label.h"
#include "bn_sprite_items_get_ready.h"
#include "bn_sprite_palettes_actions.h"
#include "bn_sprite_items_how_to_jump.h"

#include "bird.h"
#include "pipes.h"
#include "sparks.h"
#include "screen.h"
#include "background.h"
#include "global_stuff.h"

enum class STATE {
    GET_READY,
    PLAYING,
    GAME_OVER
};

class PlayScreen : public Screen { 
private:
    static constexpr bn::fixed_point SCORE_POSITION = bn::fixed_point(0,-56);
    static constexpr bn::fixed_point FINAL_SCORE_POSITION = bn::fixed_point(44,-18);
    static constexpr bn::fixed_point BEST_SCORE_POSITION = bn::fixed_point(44,2);

    GlobalStuff& global;
    STATE state;

    Background background;
    Bird bird;
    bn::optional<Pipes> pipes;

    bool fade_in_ready;
    bn::sprite_ptr get_ready_label[3];
    bn::sprite_ptr how_2_jump_label;
    bool show_gameover;

    bn::fixed global_brightness;
    int wait_frames;
    bn::regular_bg_ptr gameover_bg;
    bn::sprite_ptr gameover_label[3];
    bn::sprite_ptr medal;
    Sparks medal_sparks;
    bn::sprite_ptr new_label;
    bn::optional<bn::bg_palettes_fade_to_action> bg_fade_action;
    bn::optional<bn::sprite_palettes_fade_to_action> sprite_fade_action;
    bn::vector<bn::sprite_ptr, 10> final_score_label;
    bn::vector<bn::sprite_ptr, 10> best_score_label;

    bn::vector<bn::sprite_ptr, 10> score_label;
public:
    PlayScreen(GlobalStuff& _global);
    ~PlayScreen() = default;

    bn::optional<SCREEN_TYPE> update() final;

private:
    void update_get_ready();
    void update_playing();
    void update_game_over();

    void toggle_get_ready_things(bool show);
    void toggle_game_over_things(bool show);
};

#endif