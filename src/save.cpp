#include "save.h"

#include "bn_log.h"

Save::Save() : _data() {}

void Save::save_to_sram() {
    bn::sram::write(_data);
}

void Save::load_from_sram() {
    bn::sram::read(_data);

    bn::array<char, 8> expected_label;
    bn::istring_base expected_label_istring(expected_label._data);
    bn::ostringstream expected_label_stream(expected_label_istring);
    expected_label_stream.append("FLAPPY");

    if(_data.label != expected_label) {
        BN_LOG("Save data not found");

        _data.label = expected_label;

        _data.high_score = 10;

        save_to_sram();
    }
}

bool Save::is_high_score(int score) const {
    return score > _data.high_score;
}

int Save::high_score() const {
    return _data.high_score;
}

void Save::set_high_score(int score) {
    _data.high_score = score;
}
