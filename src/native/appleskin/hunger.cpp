#include "../../../toolchain/ndk/arm/include/c++/4.9.x/cmath"

#include <hook.h>
#include <symbol.h>
#include <innercore/item_registry.h>
#include <innercore/vtable.h>
#include <innercore/global_context.h>

#include <AttributeInstance.hpp>
#include <HudRenderers.hpp>
#include <MinecraftUIRenderContext.hpp>
#include <Level.hpp>
#include <MobEffect.hpp>
#include <RectangleArea.hpp>
#include <UIControl.hpp>

#include "main.hpp"
#include "hunger.hpp"


ItemStack* AppleHungerModule::HungerHelper::getCarriedItem(LocalPlayer* player) {
    VTABLE_FIND_OFFSET(Actor_getCarriedItem, _ZTV5Actor, _ZNK5Actor14getCarriedItemEv);
    return VTABLE_CALL<ItemStack*>(Actor_getCarriedItem, player);
}

bool AppleHungerModule::HungerHelper::isFood(Item* item) {
    if(item == nullptr) return false;
    VTABLE_FIND_OFFSET(Item_isFood, _ZTV4Item, _ZNK4Item6isFoodEv);
    return VTABLE_CALL<bool>(Item_isFood, item);
}

bool AppleHungerModule::HungerHelper::canConsume(Item* item, LocalPlayer* player) {
    if(!isFood(item)) return false;
    FoodItemComponentLegacy* food = getFoodValues(item);
    if(food == nullptr) return false;
    if(food->canAlwaysEat) return true;
    return player->getHunger() < 20.0f;
}

FoodItemComponentLegacy* AppleHungerModule::HungerHelper::getFoodValues(Item* item) {
    VTABLE_FIND_OFFSET(Item_getFood, _ZTV4Item, _ZNK4Item7getFoodEv);
    return VTABLE_CALL<FoodItemComponentLegacy*>(Item_getFood, item);
}

bool AppleHungerModule::HungerHelper::isRotten(Item* item) {
    FoodItemComponentLegacy* food = getFoodValues(item);
    if(food == nullptr) return false;
    if(food->effects.empty()) return false;
    for(FoodItemComponentLegacy::Effect& effect : food->effects) {
        MobEffect* mobEffect = MobEffect::getById(effect.id);
        if(mobEffect != nullptr && mobEffect->isHarmful()) return true;
    }
    return false;
}

bool AppleHungerModule::HungerHelper::isPlayerHoldingFood() {
    ItemStack* stack = getCarriedItem(GlobalContext::getLocalPlayer());
    if(stack == nullptr) return false;
    return isFood(ItemRegistry::getItemById(stack->getId()));
}


std::vector<Vec2> AppleHungerModule::foodBarOffsets;


void AppleHungerModule::drawVanillaHungerHud(ScreenContext& ctx, mce::MaterialPtr* material, float right, float top, LocalPlayer* player, float propagatedAlpha) {
    int foodLevel = (int) player->getHunger();
    bool hungerEffect = player->hasEffect(*MobEffect::getById(17));
    std::string texBack = hungerEffect ? "textures/ui/hunger_effect_background" : "textures/ui/hunger_background";
    for(int i = 0; i < 10; ++i) {
        Vec2& offset = foodBarOffsets.at(i);
        float x = right + offset.x;
        float y = top + offset.y;
        AppleMainModule::blit(ctx, material, x, y, 9.0f, 9.0f, texBack, 9.0f, 9.0f, propagatedAlpha);
        if(foodLevel > i * 2) {
            std::string texFront = hungerEffect ? "textures/ui/hunger_effect_full" : "textures/ui/hunger_full";
            if(i * 2 + 1 == foodLevel) texFront = hungerEffect ? "textures/ui/hunger_effect_half" : "textures/ui/hunger_half";
            AppleMainModule::blit(ctx, material, x, y, 9.0f, 9.0f, texFront, 9.0f, 9.0f, propagatedAlpha);
        }
    }
}

void AppleHungerModule::drawExhaustionOverlay(ScreenContext& ctx, mce::MaterialPtr* material, float exhaustion, float x, float y, float propagatedAlpha) {
    float ratio = fminf(1.0f, fmaxf(0.0f, exhaustion / 4.0f));
    float width = (float) ((int) (ratio * 81));
    if(width > 0) {
        AppleMainModule::blit(ctx, material, x - width, y, width, 9.0f, "textures/ui/appleskin_exhaustion", 81.0f, 9.0f, propagatedAlpha * 0.75f);
    }
}

void AppleHungerModule::drawSaturationOverlay(ScreenContext& ctx, mce::MaterialPtr* material, float saturation, float saturationGained, float right, float top, float alpha) {
    if(saturation + saturationGained < 0) return;
    float modifiedSaturation = fmaxf(0.0f, fminf(saturation + saturationGained, 20.0f));
    int startSaturationBar = 0;
    int endSaturationBar = (int) ceilf(modifiedSaturation / 2.0f);
    if(saturationGained != 0) startSaturationBar = (int) fmaxf(saturation / 2.0f, 0.0f);
    for(int i = startSaturationBar; i < endSaturationBar; ++i) {
        Vec2& offset = foodBarOffsets.at(i);
        float x = right + offset.x;
        float y = top + offset.y;
        float effectiveSaturationOfBar = (modifiedSaturation / 2.0f) - i;
        std::string textureToUse("textures/ui/appleskin_saturation_0");
        if(effectiveSaturationOfBar >= 1.0f) textureToUse = "textures/ui/appleskin_saturation_3";
        else if(effectiveSaturationOfBar > 0.5f) textureToUse = "textures/ui/appleskin_saturation_2";
        else if(effectiveSaturationOfBar > 0.25f) textureToUse = "textures/ui/appleskin_saturation_1";
        AppleMainModule::blit(ctx, material, x, y, 9.0f, 9.0f, textureToUse, 9.0f, 9.0f, alpha);
    }
}

void AppleHungerModule::drawHungerOverlay(ScreenContext& ctx, mce::MaterialPtr* material, float hungerRestored, float foodLevel, float right, float top, float alpha, bool useRottenTextures) {
    if(hungerRestored <= 0) return;
    float modifiedFood = fmaxf(0.0f, fminf(20.0f, foodLevel + hungerRestored));
    int startFoodBars = fmaxf(0.0f, (float) ((int) foodLevel / 2));
    int endFoodBars = (int) ceilf(modifiedFood / 2.0f);
    for(int i = startFoodBars; i < endFoodBars; ++i) {
        Vec2& offset = foodBarOffsets.at(i);
        float x = right + offset.x;
        float y = top + offset.y;
        bool half = i * 2 + 1 == modifiedFood;
        std::string texBack("textures/ui/hunger_background");
        std::string texFront("textures/ui/hunger_full");
        if(half) texFront = "textures/ui/hunger_half";
        if(useRottenTextures) {
            texBack = "textures/ui/hunger_effect_background";
            texFront = half ? "textures/ui/hunger_effect_half" : "textures/ui/hunger_effect_full";
        }
        AppleMainModule::blit(ctx, material, x, y, 9.0f, 9.0f, texBack, 9.0f, 9.0f, alpha * 0.25f);
        AppleMainModule::blit(ctx, material, x, y, 9.0f, 9.0f, texFront, 9.0f, 9.0f, alpha);
    }
}

void AppleHungerModule::generateHungerBarOffsets(float right, float top, int ticks, LocalPlayer* player) {
    bool shouldAnimateFood = false;
    if(AppleMainModule::ModConfig::SHOW_VANILLA_ANIMATION_OVERLAY) {
        float saturationLevel = player->getSaturation();
        int foodLevel = (int) player->getHunger();
        shouldAnimateFood = saturationLevel <= 0.0f && ticks % (foodLevel * 3 + 1) == 0;
    }
    if(foodBarOffsets.size() != 10) foodBarOffsets.resize(10, Vec2 { 0.0f, 0.0f });
    Random* rand = GlobalContext::getLevel()->getRandom();
    rand->_setSeed(312871 * ticks);
    for(int i = 0; i < 10; ++i) {
        float x = right - i * 8 - 9;
        int y = top;
        if(shouldAnimateFood) y += rand->nextInt(2) - 1;
        Vec2& point = foodBarOffsets.at(i);
        point.x = x - right;
        point.y = y - top;
    }
}

void AppleHungerModule::onRender(ScreenContext* ctx, Vec2* position, int ticks, float propagatedAlpha) {
    mce::MaterialPtr material = mce::RenderMaterialGroup::common.getMaterial(HashedString("ui_textured_and_glcolor"));
    if(AppleMainModule::ModConfig::SHOW_FOOD_EXHAUSTION_UNDERLAY) {
        drawExhaustionOverlay(*ctx, &material, GlobalContext::getLocalPlayer()->getExhaustion(), position->x, position->y, propagatedAlpha);
    }
    LocalPlayer* player = GlobalContext::getLocalPlayer();
    generateHungerBarOffsets(position->x, position->y, ticks, player);
    drawVanillaHungerHud(*ctx, &material, position->x, position->y, player, propagatedAlpha);
    if(AppleMainModule::shouldRenderAnyOverlays()) {
        float playerSaturation = player->getSaturation();
        if(AppleMainModule::ModConfig::SHOW_SATURATION_OVERLAY) {
            drawSaturationOverlay(*ctx, &material, playerSaturation, 0.0f, position->x, position->y, propagatedAlpha);
        }
        ItemStack* heldItemStack = HungerHelper::getCarriedItem(player);
        if(heldItemStack == nullptr) return;
        int heldItemID = heldItemStack->getId();
        Item* heldItem = ItemRegistry::getItemById(heldItemID);
        if(heldItem == nullptr) return;
        if(AppleMainModule::ModConfig::SHOW_FOOD_VALUES_OVERLAY_WHEN_OFFHAND && !HungerHelper::canConsume(heldItem, player)) {
            heldItemStack = player->getOffhandSlot();
            if(heldItemStack == nullptr) return;
            heldItemID = heldItemStack->getId();
            heldItem = ItemRegistry::getItemById(heldItemID);
            if(heldItem == nullptr) return;
        }
        if(!HungerHelper::canConsume(heldItem, player)) {
            AppleMainModule::resetFlash();
            return;
        }
        FoodItemComponentLegacy* foodValues = HungerHelper::getFoodValues(heldItem);
        if(!AppleMainModule::ModConfig::SHOW_FOOD_VALUES_OVERLAY) return;
        float flashAlpha = AppleMainModule::ModConfig::FLASH_ALPHA_INTERPOLATION ? AppleMainModule::lerpFlashAlpha() : AppleMainModule::flashAlpha;
        flashAlpha *= propagatedAlpha;
        int foodHunger = foodValues->nutrition;
        float foodSaturationIncrement = foodValues->getSaturationIncrement();
        float playerHunger = player->getHunger();
        drawHungerOverlay(*ctx, &material, foodHunger, playerHunger, position->x, position->y, flashAlpha, HungerHelper::isRotten(heldItem) || player->hasEffect(*(MobEffect::getById(17))));
        float newFoodValue = playerHunger + foodHunger;
        float newSaturationValue = playerSaturation + foodSaturationIncrement;
        float saturationGained = newSaturationValue > newFoodValue ? newFoodValue - playerSaturation : foodSaturationIncrement;
        drawSaturationOverlay(*ctx, &material, playerSaturation, saturationGained, position->x, position->y, flashAlpha);
    }
}

void AppleHungerModule::initialize() {
    DLHandleManager::initializeHandle("libminecraftpe.so", "mcpe");
    HookManager::addCallback(
        SYMBOL("mcpe", "_ZN17HudHungerRenderer6renderER24MinecraftUIRenderContextR15IClientInstanceR9UIControliR13RectangleArea"),
        LAMBDA((HookManager::CallbackController* controller, HudHungerRenderer* renderer, MinecraftUIRenderContext& renderContext, ClientInstance& clientInstance, UIControl& control, int someInt, RectangleArea& area), {
            controller->prevent();
            onRender(renderContext.getScreenContext(), control.getPosition(), renderer->ticks, renderer->propagatedAlpha);
        }, ), HookManager::CALL | HookManager::LISTENER | HookManager::CONTROLLER
    );
}