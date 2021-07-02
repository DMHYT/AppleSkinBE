#ifndef APPLESKIN_RECOVERED_H
#define APPLESKIN_RECOVERED_H


enum UIProfile {
    pe, classic
};

class Options {
    public:
    UIProfile getUIProfile() const;
};

class ClientInstance {
    public:
    Options* getOptions() const;
};

class FoodItemComponent {
    public:
    class SaturationModifier;
    int getNutrition() const;
    SaturationModifier* getSaturationModifier() const;
};

class FoodConstants {
    public:
    static const FoodItemComponent::SaturationModifier* FOOD_SATURATION_POOR;
    static const FoodItemComponent::SaturationModifier* FOOD_SATURATION_LOW;
    static const FoodItemComponent::SaturationModifier* FOOD_SATURATION_NORMAL;
    static const FoodItemComponent::SaturationModifier* FOOD_SATURATION_GOOD;
    static const FoodItemComponent::SaturationModifier* FOOD_SATURATION_MAX;
    static const FoodItemComponent::SaturationModifier* FOOD_SATURATION_SUPERNATURAL;
    static const float MAX_SATURATION;
    static const float MAX_EXHAUSTION;
};

class Item {
    public:
    bool isFood() const;
    FoodItemComponent* getFood() const;
};

class ItemStackBase {
    public:
    Item* getItem() const;
};

class Attribute;
class AttributeInstance {
    public:
    float getCurrentValue() const;
};

class Actor {
    public:
    float getHealth() const;
    float getMaxHealth() const;
    ItemStackBase* getCarriedItem() const;
    AttributeInstance* getAttribute(Attribute const&) const;
};

class Mob : public Actor {};

class Player : public Mob {
    public:
    static const Attribute* HUNGER;
    static const Attribute* SATURATION;
    static const Attribute* EXHAUSTION;
};

class LocalPlayer : public Player {};

class GameRuleId {
    public:
};
class GameRules {
    public:
    bool getBool(GameRuleId) const;
    GameRuleId nameToGameRuleIndex(std::string const&) const;
};

class Level {
    public:
    GameRules* getGameRules() const;
};

namespace GlobalContext {
    Level* getLevel();
    ClientInstance* getMinecraftClient();
    LocalPlayer* getLocalPlayer();
};


#endif //APPLESKIN_RECOVERED_H