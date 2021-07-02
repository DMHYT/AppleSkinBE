#include <stl/algorithm>
#include "food.h"

AppleSkinHelper::FoodValues::FoodValues(int h, float s)
: hunger(h), saturationModifier(s){};
float AppleSkinHelper::FoodValues::getSaturationIncrement() const {
    return hunger * saturationModifier * 2.0;
}

int AppleSkinHelper::getPlayerHunger(){
    return (int) GlobalContext::getLocalPlayer()->getAttribute(*Player::HUNGER)->getCurrentValue();
};
float AppleSkinHelper::getPlayerSaturation(){
    return GlobalContext::getLocalPlayer()->getAttribute(*Player::SATURATION)->getCurrentValue();
};
float AppleSkinHelper::getPlayerExhaustion(){
    return GlobalContext::getLocalPlayer()->getAttribute(*Player::EXHAUSTION)->getCurrentValue();
};
float AppleSkinHelper::getMaxExhaustion(){
    return FoodConstants::MAX_EXHAUSTION;
};
float AppleSkinHelper::getSaturationValueFromModifier(FoodItemComponent::SaturationModifier* mod){
    if(mod == FoodConstants::FOOD_SATURATION_POOR) return 0.2;
    if(mod == FoodConstants::FOOD_SATURATION_LOW) return 0.6;
    if(mod == FoodConstants::FOOD_SATURATION_NORMAL) return 1.2;
    if(mod == FoodConstants::FOOD_SATURATION_GOOD) return 1.6;
    if(mod == FoodConstants::FOOD_SATURATION_MAX) return 1.6307692307692;
    if(mod == FoodConstants::FOOD_SATURATION_SUPERNATURAL) return 2.4;
};
bool AppleSkinHelper::isNaturalRegenerationEnabled(){
    GameRules* rules_obj = GlobalContext::getLevel()->getGameRules();
    const std::string nr = "naturalRegeneration";
    return rules_obj->getBool(rules_obj->nameToGameRuleIndex(nr));
};
bool AppleSkinHelper::shouldPlayerHeal(Player* player){
    return player->getHealth() > 0.0 && player->getHealth() < player->getMaxHealth();
};
bool AppleSkinHelper::isFood(ItemStackBase* itemstack){
    return itemstack->getItem()->isFood();
};
AppleSkinHelper::FoodValues* AppleSkinHelper::getFoodValues(ItemStackBase* itemstack){
    bool f = AppleSkinHelper::isFood(itemstack);
    int hunger = f ? itemstack->getItem()->getFood()->getNutrition() : 0;
    float saturationModifier = f ? AppleSkinHelper::getSaturationValueFromModifier(itemstack->getItem()->getFood()->getSaturationModifier()) : 0.0;
    FoodValues values(hunger, saturationModifier);
    FoodValues* values_ptr = &values;
    return values_ptr;
};
bool AppleSkinHelper::isRotten(ItemStackBase* itemstack){
    if(!AppleSkinHelper::isFood(itemstack)) return false;

};
float AppleSkinHelper::getEstimatedHealthIncrement(ItemStackBase* itemstack, FoodValues* values, Player* player){
    if(!AppleSkinHelper::isFood(itemstack)) return 0.0;
    if(!AppleSkinHelper::shouldPlayerHeal(player)) return 0.0;
    int foodLevel = std::min(AppleSkinHelper::getPlayerHunger() + values->hunger, 20);
    float healthIncrement = 0;
    if(foodLevel >= 18.0){
        return healthIncrement; //TODO
    }
};