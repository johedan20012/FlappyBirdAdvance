#ifndef SAVE_H
#define SAVE_H

#include "bn_sram.h"
#include "bn_array.h"
#include "bn_string.h"

struct SaveData{
    bn::array<char,8> label;
    int high_score;
};

class Save{
private:
    SaveData _data;

public:
    Save();
    ~Save() = default;

    void save_to_sram();
    void load_from_sram();

    bool is_high_score(int score) const;
    int high_score() const;
    void set_high_score(int score);
};

#endif