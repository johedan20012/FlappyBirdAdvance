#include "pipes.h"

PipePair::PipePair(bn::fixed_point position, bool _portrait_mode, bn::fixed _gap) :
    portrait_mode(_portrait_mode),
    half_gap(_gap/2),
    top_sprs{
        (portrait_mode)? bn::sprite_items::pipe_portrait.create_sprite(position.x()+half_gap+32,position.y()) : bn::sprite_items::pipe.create_sprite(position.x(), position.y()-half_gap-32), 
        (portrait_mode)? bn::sprite_items::pipe_portrait.create_sprite(position.x()+half_gap+96,position.y(),1) : bn::sprite_items::pipe.create_sprite(position.x(), position.y()-half_gap-96,1)},
    bottom_sprs{
        (portrait_mode)? bn::sprite_items::pipe_portrait.create_sprite(position.x()-half_gap-32,position.y()) : bn::sprite_items::pipe.create_sprite(position.x(), position.y()+half_gap+32), 
        (portrait_mode)? bn::sprite_items::pipe_portrait.create_sprite(position.x()-half_gap-96,position.y(),1) : bn::sprite_items::pipe.create_sprite(position.x(), position.y()+half_gap+96,1)}
    {
    if(portrait_mode) bottom_sprs[0].set_horizontal_flip(true);
    else top_sprs[0].set_vertical_flip(true);

    top_sprs[0].set_bg_priority(2);
    top_sprs[1].set_bg_priority(2);
    bottom_sprs[0].set_bg_priority(2);
    bottom_sprs[1].set_bg_priority(2);
}

bn::fixed PipePair::pos_moving_ax() const {
    if(portrait_mode) return top_sprs[0].y();
    return top_sprs[0].x();
}

void PipePair::set_pos_moving_ax(bn::fixed pos){
    if(portrait_mode){
        top_sprs[0].set_y(pos);
        top_sprs[1].set_y(pos);
        bottom_sprs[0].set_y(pos);
        bottom_sprs[1].set_y(pos);
        return;
    }
    top_sprs[0].set_x(pos);
    top_sprs[1].set_x(pos);
    bottom_sprs[0].set_x(pos);
    bottom_sprs[1].set_x(pos);
}

void PipePair::set_pos_static_ax(bn::fixed pos){
    if(portrait_mode){
        top_sprs[0].set_x(pos+half_gap+32);
        top_sprs[1].set_x(pos+half_gap+96);
        bottom_sprs[0].set_x(pos-half_gap-32);
        bottom_sprs[1].set_x(pos-half_gap-96);
        return;
    }
    top_sprs[0].set_y(pos-half_gap-32);
    top_sprs[1].set_y(pos-half_gap-96);
    bottom_sprs[0].set_y(pos+half_gap+32);
    bottom_sprs[1].set_y(pos+half_gap+96);
}

bool PipePair::check_collision(const bn::fixed_rect& bird_hitbox){
    bn::fixed_rect top_hitbox(top_sprs[0].x(), top_sprs[1].y()-32, 32, 256);
    bn::fixed_rect bottom_hitbox(bottom_sprs[0].x(), bottom_sprs[1].y()+32, 32, 256 );
    if(portrait_mode){
        top_hitbox = bn::fixed_rect(top_sprs[1].x()+32, top_sprs[0].y(), 256, 32);
        bottom_hitbox = bn::fixed_rect(bottom_sprs[1].x()-32, bottom_sprs[0].y(), 256, 32);
    }
    return top_hitbox.intersects(bird_hitbox) || bottom_hitbox.intersects(bird_hitbox);
}

bool PipePair::passed(const bn::fixed_rect& bird_hitbox){
    if(portrait_mode) return bird_hitbox.y() >= top_sprs[0].y();
    return bird_hitbox.x() >= top_sprs[0].x();
}


Pipes::Pipes(GlobalStuff& _global) : // -32 and 32 makes them close at them middle
    global(_global),
    portrait_mode(global.is_portrait_mode()),
    pipes{
        PipePair(PIPES_INITIAL_POS[portrait_mode][0], portrait_mode), 
        PipePair(PIPES_INITIAL_POS[portrait_mode][1], portrait_mode),
        PipePair(PIPES_INITIAL_POS[portrait_mode][2], portrait_mode), 
        PipePair(PIPES_INITIAL_POS[portrait_mode][3], portrait_mode)},
    current_pipe(0)
    {
    for(int i = 0; i < pipes.size(); i++) set_random_static_ax(i);
}

void Pipes::update(){
    for(int i = 0; i < pipes.size(); i++){
        pipes[i].set_pos_moving_ax(pipes[i].pos_moving_ax() - 1);
        if(pipes[i].pos_moving_ax() < MOVING_AX_LIMIT[portrait_mode]){
            pipes[i].set_pos_moving_ax(MOVING_AX_RESET[portrait_mode]);
            set_random_static_ax(i);
        }
    }
}

bool Pipes::check_collision(const bn::fixed_rect& bird_hitbox){
    for(PipePair& pipe : pipes){
        if(pipe.check_collision(bird_hitbox)) return true;
    }
    return false;
}

bool Pipes::check_passed(const bn::fixed_rect& bird_hitbox){
    if(pipes[current_pipe].passed(bird_hitbox)){
        current_pipe = (current_pipe + 1) % pipes.size();
        return true;
    }
    return false;
}

void Pipes::set_random_static_ax(int index){
    // Middle point is in the range [-48,32] for landscape
    // Middle point is in the range [-16,80] for portrait
    bn::fixed middle = (portrait_mode)? global.rng().get_unbiased_fixed(97)-16 : global.rng().get_unbiased_fixed(81)-48;
    pipes[index].set_pos_static_ax(middle);
}