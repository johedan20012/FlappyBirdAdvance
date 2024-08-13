#ifndef TEXT_NUMBER_GENERATOR_H
#define TEXT_NUMBER_GENERATOR_H

#include "bn_vector.h"
#include "bn_sprite_item.h"
#include "bn_sprite_ptr.h"

class TextNumberGenerator{
private:
    bn::sprite_item font;
    bn::fixed number_width;

public:
    TextNumberGenerator(bn::sprite_item _font);
    TextNumberGenerator(bn::sprite_item _font,int _number_width);
    ~TextNumberGenerator() = default;

    void generate(int number,bn::fixed_point pos, bn::ivector<bn::sprite_ptr>& number_sprites, bool portrait_mode = false);
};


#endif