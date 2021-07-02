#include "recovered.h"

#ifndef APPLESKIN_FOOD_H
#define APPLESKIN_FOOD_H


class AppleSkinHelper {
    public:
    class FoodValues {
        public:
        FoodValues(int h, float s);
        const int hunger;
        const float saturationModifier;
        float getSaturationIncrement() const;
    };
    static int getPlayerHunger();
    static float getPlayerSaturation();
    static float getPlayerExhaustion();
    static float getMaxExhaustion();
    static float getSaturationValueFromModifier(FoodItemComponent::SaturationModifier* modifier);
    static bool isNaturalRegenerationEnabled();
    static bool shouldPlayerHeal(Player* player);
    static bool isFood(ItemStackBase* itemstack);
    static FoodValues* getFoodValues(ItemStackBase* itemstack);
    static bool isRotten(ItemStackBase* itemstack);
    static float getEstimatedHealthIncrement(ItemStackBase* itemstack, FoodValues* values, Player* player);
    static float getEstimatedHealthIncrementForValues(int foodLevel, float saturationLevel, float exhaustionLevel);
};


#endif //APPLESKIN_FOOD_H