#include <stl/string>
#include <stl/vector>

#ifndef APPLESKIN_FOODITEMCOMPONENTLEGACY_HPP
#define APPLESKIN_FOODITEMCOMPONENTLEGACY_HPP


enum FoodUseAction {
    UNKNOWN = -1,
    CHORUS_TELEPORT,
    SUSPICIOUS_STEW_EFFECT
};
class Item {public:};
class ItemStackBase {
    public:
    int getId() const;
    Item* getItem() const;
};
class ItemStack : public ItemStackBase {public:};

class FoodItemComponentLegacy {
    public:
    struct Effect {
        char* descriptionId; // 4
        unsigned int id; // 8
        int duration; // 12
        int amplifier; // 16
        float chance; // 20
    };
    char vtable[4]; // 4
    Item* item; // 8
    int nutrition; // 12
    float saturationModifier; // 16
    std::__ndk1::string usingConvertsTo; // 28
    FoodUseAction onUseAction; // 32
    float onUseRangeX; // 36
    float onUseRangeY; // 40
    float onUseRangeZ; // 44
    std::__ndk1::string cooldownType; // 56
    int cooldownTime; // 60
    bool canAlwaysEat; // 61 + 3
    std::__ndk1::vector<Effect> effects; // 76
    std::__ndk1::vector<unsigned int> removeEffects; // 88
    float getSaturationIncrement() const {
        return this->nutrition * this->saturationModifier * 2.0f;
    }
};


#endif //APPLESKIN_FOODITEMCOMPONENTLEGACY_HPP