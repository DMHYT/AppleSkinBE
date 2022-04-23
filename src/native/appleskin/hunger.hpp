#include <mod.h>

#include <FoodItemComponentLegacy.hpp>
#include <Player.hpp>
#include <ScreenContext.hpp>
#include <UIControl.hpp>

#ifndef APPLESKIN_HUNGERMODULE_HPP
#define APPLESKIN_HUNGERMODULE_HPP


class AppleHungerModule : public Module {
    public:
    class HungerHelper {
        public:
        static ItemStack* getCarriedItem(LocalPlayer*);
        static bool isFood(Item*);
        static bool canConsume(Item*, LocalPlayer*);
        static FoodItemComponentLegacy* getFoodValues(Item*);
        static bool isRotten(Item*);
    };
    static std::vector<Vec2> foodBarOffsets;
    static void drawExhaustionOverlay(ScreenContext& ctx, float exhaustion, float x, float y);
    static void drawSaturationOverlay(ScreenContext& ctx, float saturation, float saturationGained, float x, float y, float alpha);
    static void drawHungerOverlay(ScreenContext& ctx, float hungerRestored, float foodLevel, float x, float y, float alpha, bool useRottenTextures);
    static void generateHungerBarOffsets(float x, float y, int ticks, LocalPlayer* player);
    static void onPreRender(ScreenContext* ctx, Vec2* position);
    static void onRender(ScreenContext* ctx, Vec2* position, int ticks);
    AppleHungerModule(Module* parent): Module(parent, "appleskin.hunger") {};
    virtual void initialize();
};


#endif //APPLESKIN_HUNGERMODULE_HPP