#include "text_number_generator.h"

TextNumberGenerator::TextNumberGenerator(bn::sprite_item _font) : 
    font(_font),
    number_width(_font.shape_size().width())
    {

    if(font.tiles_item().graphics_count() < 10) BN_ERROR("Font must have at least 10 tiles");
}

TextNumberGenerator::TextNumberGenerator(bn::sprite_item _font, int _number_width) : 
    font(_font),
    number_width(_number_width)
    {

    if(font.tiles_item().graphics_count() < 10) BN_ERROR("Font must have at least 10 tiles");
}

void TextNumberGenerator::generate(int number,bn::fixed_point pos, bn::ivector<bn::sprite_ptr> &number_sprites, bool portrait_mode){
    int i = 0;
    do{
        int digit = number % 10;
        if(portrait_mode){
            number_sprites.push_back(font.create_sprite(pos.x(), pos.y() - number_width * i, digit));
            number_sprites.back().set_rotation_angle(270);
        }else{
            number_sprites.push_back(font.create_sprite(pos.x() - number_width * i, pos.y(), digit));
        }
        number_sprites.back().set_bg_priority(0);
        number /= 10;
        i++;
    }while(number > 0);
}