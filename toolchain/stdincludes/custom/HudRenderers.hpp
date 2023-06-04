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
    bool texturesWereLoaded; // 13 + 3
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

class HudHeartRenderer : public MinecraftUICustomRenderer {
    public:
    bool texturesWereLoaded; // 9 + 3
    int ticks; // 16
    mce::TexturePtr textureHeartBackground; // 56
    mce::TexturePtr textureHeartBlink; // 96
    mce::TexturePtr textureHeart; // 136
    mce::TexturePtr textureHeartHalf; // 176
    mce::TexturePtr texturePoisonHeart; // 216
    mce::TexturePtr texturePoisonHeartHalf; // 256
    mce::TexturePtr textureWitherHeart; // 296
    mce::TexturePtr textureWitherHeartHalf; // 336
    mce::TexturePtr textureAbsorptionHeart; // 376
    mce::TexturePtr textureAbsorptionHeartHalf; // 416
    mce::TexturePtr textureHeartFlash; // 456
    mce::TexturePtr textureHeartFlashHalf; // 496
    mce::TexturePtr texturePoisonHeartFlash; // 536
    mce::TexturePtr texturePoisonHeartFlashHalf; // 576
    mce::TexturePtr textureWitherHeartFlash; // 616
    mce::TexturePtr textureWitherHeartFlashHalf; // 656
};


#endif //APPLESKIN_HUDRENDERERS_HPP