#include <mod.h>

#include <Player.hpp>
#include <ScreenContext.hpp>
#include <UIControl.hpp>

#ifndef APPLESKIN_HEARTMODULE_HPP
#define APPLESKIN_HEARTMODULE_HPP


class AppleHeartModule : public Module {
    public:
    static std::vector<Vec2> healthBarOffsets;
    static void drawHealthOverlay(ScreenContext& ctx, int health, int modifiedHealth, float x, float y, float alpha);
    static int getEstimatedHealthIncrement(Item* foodItem, LocalPlayer* player);
    static int getEstimatedHealthIncrement(int nutrition, float saturation, float exhaustion);
    static bool shouldShowEstimatedHealth(LocalPlayer* player);
    static void generateHealthBarOffsets(float x, float y, int ticks, LocalPlayer* player, int uiProfileMultiplier);
    static void onRender(ScreenContext* ctx, Vec2* position, int ticks, int uiProfileMultiplier, float propagatedAlpha);
    AppleHeartModule(Module* parent): Module(parent, "appleskin.heart") {};
    virtual void initialize();
};


#endif //APPLESKIN_HEARTMODULE_HPP