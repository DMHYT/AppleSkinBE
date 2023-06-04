#include "../../../toolchain/ndk/arm/include/c++/4.9.x/cmath"

#include <hook.h>
#include <symbol.h>
#include <static_symbol.h>
#include <innercore/item_registry.h>
#include <innercore/global_context.h>
#include <innercore/vtable.h>

#include <HudRenderers.hpp>
#include <MinecraftUIRenderContext.hpp>
#include <Level.hpp>
#include <RectangleArea.hpp>

#include "main.hpp"
#include "hunger.hpp"
#include "heart.hpp"


std::vector<Vec2> AppleHeartModule::healthBarOffsets;


void AppleHeartModule::drawHealthOverlay(ScreenContext& ctx, int health, int modifiedHealth, float left, float top, float alpha) {
    if(modifiedHealth <= health) return;
    int startHealthBars = (int) fmaxf(0.0f, ceilf((float) health) / 2.0f);
    int endHealthBars = (int) fmaxf(0.0f, ceilf((float) modifiedHealth / 2.0f));
    mce::MaterialPtr material = mce::RenderMaterialGroup::common.getMaterial(HashedString("ui_textured_and_glcolor"));
    for(int i = startHealthBars; i < endHealthBars; ++i) {
        if(healthBarOffsets.size() <= i) continue;
        Vec2 offset = healthBarOffsets[i];
        float x = left + offset.x;
        float y = top + offset.y;
        std::string textureToUse("textures/ui/heart");
        if(i * 2 + 1 == modifiedHealth) textureToUse = "textures/ui/heart_half";
        AppleMainModule::blit(ctx, &material, x, y, 9.0f, 9.0f, textureToUse, 9.0f, 9.0f, alpha);
    }
}

int AppleHeartModule::getEstimatedHealthIncrement(Item* foodItem, LocalPlayer* player) {
    if(!player->isHurt()) return 0;
    auto foodValues = AppleHungerModule::HungerHelper::getFoodValues(foodItem);
    float playerFoodLevel = player->getHunger();
    int foodLevel = (int) fminf(playerFoodLevel + (float) foodValues->nutrition, 20.0f);
    int healthIncrement = 0;
    STATIC_SYMBOL(GameRules_getBool, "_ZNK9GameRules7getBoolE10GameRuleId", (GameRules*, GameRuleId*), bool)
    GameRuleId naturalRegenID(15);
    bool naturalRegenEnabled = GameRules_getBool(player->getLevel()->getGameRules(), &naturalRegenID);
    if(foodLevel >= 18 && naturalRegenEnabled) {
        float playerSaturationLevel = player->getSaturation();
        float saturationLevel = fminf(playerSaturationLevel + foodValues->getSaturationIncrement(), (float) foodLevel);
        float exhaustionLevel = player->getExhaustion();
        healthIncrement = getEstimatedHealthIncrement(foodLevel, saturationLevel, exhaustionLevel);
    }
    for(const FoodItemComponentLegacy::Effect& effect : foodValues->effects) {
        if(effect.id == 10) {
            healthIncrement += (int) floorf((float) effect.duration / fmaxf((float) (50 >> effect.amplifier), 1.0f));
            break;
        }
    }
    return healthIncrement;
}

int AppleHeartModule::getEstimatedHealthIncrement(int nutrition, float saturation, float exhaustion) {
    int health = 0;
    while(nutrition >= 18) {
        while(exhaustion > 4.0f) {
            exhaustion -= 4.0f;
            if(saturation > 0) saturation = fmaxf(saturation - 1.0f, 0.0f);
            else nutrition--;
        }
        if(nutrition >= 20 && saturation > 0.0f) {
            float limitedSaturationLevel = fminf(saturation, 6.0f);
            float exhaustionUntilAboveMax = nextafterf(4.0f, 5.0f) - exhaustion;
            int numIterationsUntilAboveMax = (int) fmaxf(1.0f, ceilf(exhaustionUntilAboveMax / limitedSaturationLevel));
            health += (int) ( limitedSaturationLevel / 6.0f * numIterationsUntilAboveMax );
            exhaustion += limitedSaturationLevel * numIterationsUntilAboveMax;
        } else if(nutrition >= 18) {
            ++health;
            exhaustion += 6.0f;
        }
    }
    return health;
}

bool AppleHeartModule::shouldShowEstimatedHealth(LocalPlayer* player) {
    if(!AppleMainModule::ModConfig::SHOW_FOOD_HEALTH_HUD_OVERLAY) return false;
    Level* level = player->getLevel();
    if(level == nullptr || level->getDifficulty() == 0) return false;
    if(player->getHunger() >= 18.0f) return false;
    if(player->hasEffect(*MobEffect::getById(19))) return false; // poison
    if(player->hasEffect(*MobEffect::getById(20))) return false; // wither
    if(player->hasEffect(*MobEffect::getById(10))) return false; // regeneration
    return true;
}

void AppleHeartModule::generateHealthBarOffsets(float left, float top, int ticks, LocalPlayer* player, int uiProfileMultiplier) {
    int maxHealth = player->getMaxHealth();
    int healthBars = (int) ceilf((float) maxHealth / 2.0f);
    int healthRows = (int) ceilf((float) healthBars / 10.0f);
    int healthRowHeight = 10; // VANILLA
    bool shouldAnimateHealth = false;
    if(AppleMainModule::ModConfig::SHOW_VANILLA_ANIMATION_OVERLAY) {
        shouldAnimateHealth = player->getHealth() <= 4;
    }
    if(healthBarOffsets.size() != healthBars) healthBarOffsets.resize(healthBars, Vec2 { 0.0f, 0.0f });
    Random* rand = GlobalContext::getLevel()->getRandom();
    for(int i = healthBars - 1; i >= 0; --i) {
        int row = (int) ceilf((float) (i + 1) / 10.0f) - 1;
        float x = left + (float) (i % 10 * 8);
        int y = top + uiProfileMultiplier * row * healthRowHeight;
        if(shouldAnimateHealth) y += rand->nextInt(2);
        Vec2& point = healthBarOffsets.at(i);
        point.x = x - left;
        point.y = y - top;
    }
}

void AppleHeartModule::onRender(ScreenContext* ctx, Vec2* position, int ticks, int uiProfileMultiplier) {
    if(AppleMainModule::shouldRenderAnyOverlays()) {
        LocalPlayer* player = GlobalContext::getLocalPlayer();
        generateHealthBarOffsets(position->x, position->y, ticks, player, uiProfileMultiplier);
        ItemStack* heldItemStack = AppleHungerModule::HungerHelper::getCarriedItem(player);
        if(heldItemStack == nullptr) return;
        int heldItemID = heldItemStack->getId();
        Item* heldItem = ItemRegistry::getItemById(heldItemID);
        if(heldItem == nullptr) return;
        if(AppleMainModule::ModConfig::SHOW_FOOD_VALUES_OVERLAY_WHEN_OFFHAND && !AppleHungerModule::HungerHelper::canConsume(heldItem, player)) {
            heldItemStack = player->getOffhandSlot();
            if(heldItemStack == nullptr) return;
            heldItemID = heldItemStack->getId();
            heldItem = ItemRegistry::getItemById(heldItemID);
            if(heldItem == nullptr) return;
        }
        if(!AppleHungerModule::HungerHelper::canConsume(heldItem, player)) {
            AppleMainModule::resetFlash();
            return;
        }
        if(!shouldShowEstimatedHealth(player)) return;
        int foodHealthIncrement = getEstimatedHealthIncrement(heldItem, player);
        int currentHealth = player->getHealth();
        int modifiedHealth = (int) fminf((float) (currentHealth + foodHealthIncrement), (float) player->getMaxHealth());
        float flashAlpha = AppleMainModule::ModConfig::FLASH_ALPHA_INTERPOLATION ? AppleMainModule::lerpFlashAlpha() : AppleMainModule::flashAlpha;
        drawHealthOverlay(*ctx, currentHealth, modifiedHealth, position->x, position->y, flashAlpha);
    }
}

void AppleHeartModule::initialize() {
    DLHandleManager::initializeHandle("libminecraftpe.so", "mcpe");
    HookManager::addCallback(
        SYMBOL("mcpe", "_ZN16HudHeartRendererC2Ev"),
        LAMBDA((HudHeartRenderer* renderer), {
            renderer->ticks = 0;
        }, ), HookManager::RETURN | HookManager::LISTENER
    );
    HookManager::addCallback(
        SYMBOL("mcpe", "_ZN16HudHeartRenderer6updateER15IClientInstanceR9UIControlRK7UIScene"),
        LAMBDA((HudHeartRenderer* renderer, ClientInstance& clientInstance, UIControl& control, const UIScene& scene), {
            ++renderer->ticks;
        }, ), HookManager::CALL | HookManager::LISTENER
    );
    HookManager::addCallback(
        SYMBOL("mcpe", "_ZN16HudHeartRenderer6renderER24MinecraftUIRenderContextR15IClientInstanceR9UIControliR13RectangleArea"),
        LAMBDA((HudHeartRenderer* renderer, MinecraftUIRenderContext& renderContext, ClientInstance& clientInstance, UIControl& control, int someInt, RectangleArea& area), {
            VTABLE_FIND_OFFSET(ClientInstance_getOptions, _ZTV14ClientInstance, _ZNK14ClientInstance10getOptionsEv);
            Options* options = VTABLE_CALL<Options*>(ClientInstance_getOptions, &clientInstance);
            int uiProfileMultiplier = options->getUIProfile() == 0 ? -1 : 1;
            onRender(renderContext.getScreenContext(), control.getPosition(), renderer->ticks, uiProfileMultiplier);
        }, ), HookManager::RETURN | HookManager::LISTENER
    );
}