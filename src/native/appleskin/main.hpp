#include <mod.h>

#include <mce.hpp>
#include <ScreenContext.hpp>

#ifndef APPLESKIN_MAINMODULE_HPP
#define APPLESKIN_MAINMODULE_HPP


class AppleMainModule : public Module {
    public:
    class ModConfig {
        public:
        static bool SHOW_FOOD_VALUES_IN_TOOLTIP;
        static bool ALWAYS_SHOW_FOOD_VALUES_TOOLTIP;
        static bool SHOW_SATURATION_OVERLAY;
        static bool SHOW_FOOD_VALUES_OVERLAY;
        static bool SHOW_FOOD_VALUES_OVERLAY_WHEN_OFFHAND;
        static bool SHOW_FOOD_EXHAUSTION_UNDERLAY;
        static bool SHOW_FOOD_DEBUG_INFO;
        static bool SHOW_FOOD_HEALTH_HUD_OVERLAY;
        static bool SHOW_VANILLA_ANIMATION_OVERLAY;
        static float MAX_HUD_OVERLAY_FLASH_ALPHA;
    };
    static float unclampedFlashAlpha;
    static float flashAlpha;
    static short alphaDir;
    static void resetFlash() {
        unclampedFlashAlpha = flashAlpha = 0.0f;
        alphaDir = 1;
    }
    static inline bool shouldRenderAnyOverlays() {
        return ModConfig::SHOW_FOOD_VALUES_OVERLAY || ModConfig::SHOW_SATURATION_OVERLAY || ModConfig::SHOW_FOOD_HEALTH_HUD_OVERLAY;
    }
    static void blit(ScreenContext& ctx, mce::MaterialPtr* material, float x, float y, float width, float height, std::string const& texture, float textureWidth, float textureHeight, float alpha);
    AppleMainModule(): Module("appleskin") {};
    virtual void initialize();
};


#endif //APPLESKIN_MAINMODULE_HPP