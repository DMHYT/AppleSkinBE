#include <mce.hpp>

#ifndef APPLESKIN_HUDRENDERERS_HPP
#define APPLESKIN_HUDRENDERERS_HPP


class UICustomRenderer {
    public:
    void** vtable; // 4
    float propagatedAlpha; // 8
    void setPropagatedAlpha(float);
};

class MinecraftUICustomRenderer : public UICustomRenderer {public:};

class HudHungerRenderer : public MinecraftUICustomRenderer {
    public:
    int ticks; // 12
    bool someBool; // 13 + 3
    mce::TexturePtr textureHungerBackground; // 56
    mce::TexturePtr textureHungerBlink; // 96
    mce::TexturePtr textureHungerEffectBackground; // 136
    mce::TexturePtr textureHungerFull; // 176
    mce::TexturePtr textureHungerHalf; // 216
    mce::TexturePtr textureHungerFlashFull; // 256
    mce::TexturePtr textureHungerFlashHalf; // 296
    mce::TexturePtr textureHungerEffectFull; // 336
    mce::TexturePtr textureHungerEffectHalf; // 376
    mce::TexturePtr textureHungerEffectFlashFull; // 416
    mce::TexturePtr textureHungerEffectFlashHalf; // 456
};


#endif //APPLESKIN_HUDRENDERERS_HPP