#include "../../../toolchain/ndk/arm/include/c++/4.9.x/cmath"

#include <hook.h>
#include <nativejs.h>
#include <symbol.h>

#include <Level.hpp>

#include "main.hpp"
#include "hunger.hpp"


bool AppleMainModule::ModConfig::SHOW_FOOD_VALUES_IN_TOOLTIP = true;
bool AppleMainModule::ModConfig::ALWAYS_SHOW_FOOD_VALUES_TOOLTIP = true;
bool AppleMainModule::ModConfig::SHOW_SATURATION_OVERLAY = true;
bool AppleMainModule::ModConfig::SHOW_FOOD_VALUES_OVERLAY = true;
bool AppleMainModule::ModConfig::SHOW_FOOD_VALUES_OVERLAY_WHEN_OFFHAND = true;
bool AppleMainModule::ModConfig::SHOW_FOOD_EXHAUSTION_UNDERLAY = true;
bool AppleMainModule::ModConfig::SHOW_FOOD_DEBUG_INFO = true;
bool AppleMainModule::ModConfig::SHOW_FOOD_HEALTH_HUD_OVERLAY = true;
bool AppleMainModule::ModConfig::SHOW_VANILLA_ANIMATION_OVERLAY = true;
float AppleMainModule::ModConfig::MAX_HUD_OVERLAY_FLASH_ALPHA = 0.65f;


float AppleMainModule::unclampedFlashAlpha = 0.0f;
float AppleMainModule::flashAlpha = 0.0f;
short AppleMainModule::alphaDir = 1;


void AppleMainModule::blit(ScreenContext& ctx, mce::MaterialPtr* material, float x, float y, float width, float height, std::string const& texture, float textureWidth, float textureHeight, float alpha) {
	float maxU = width / textureWidth;
	float maxV = height / textureHeight;
	ctx.tessellator->begin(4, false);
	ctx.tessellator->vertexUV(x, y + height, 0.0f, 0.0f, maxV);
	ctx.tessellator->vertexUV(x + width, y + height, 0.0f, maxU, maxV);
	ctx.tessellator->vertexUV(x + width, y, 0.0f, maxU, 0.0f);
	ctx.tessellator->vertexUV(x, y, 0.0f, 0.0f, 0.0f);
	ctx.shaderColor->setColor(mce::Color { 1.0f, 1.0f, 1.0f, alpha });
	RenderMesh::endTessellationAndRenderImmediately(ctx, *ctx.tessellator, material, texture);
	ctx.shaderColor->setColor(mce::Color { 1.0f, 1.0f, 1.0f, 1.0f });
}

void AppleMainModule::initialize() {
	DLHandleManager::initializeHandle("libminecraftpe.so", "mcpe");
	HookManager::addCallback(
		SYMBOL("mcpe", "_ZN5Level4tickEv"),
		LAMBDA((Level* level), {
			if(level->isClientSide()) {
				unclampedFlashAlpha += alphaDir * 0.125f;
				if(unclampedFlashAlpha >= 1.5f) alphaDir = -1;
				else if(unclampedFlashAlpha <= -0.5f) alphaDir = 1;
				flashAlpha = fmaxf(0.0f, fminf(1.0f, unclampedFlashAlpha)) * ModConfig::MAX_HUD_OVERLAY_FLASH_ALPHA;
			}
		}, ), HookManager::RETURN | HookManager::LISTENER
	);
}


MAIN {
	AppleMainModule* mainModule = new AppleMainModule();
	AppleHungerModule* hungerModule = new AppleHungerModule(mainModule);
}


JS_MODULE_VERSION(AppleSkinConfig, 1)
JS_EXPORT(
	AppleSkinConfig, initModConfig, "V(BBBBBBBBBF)",
	(JNIEnv* env, bool b1, bool b2, bool b3, bool b4, bool b5, bool b6, bool b7, bool b8, bool b9, float f1) {
		AppleMainModule::ModConfig::SHOW_FOOD_VALUES_IN_TOOLTIP = b1;
		AppleMainModule::ModConfig::ALWAYS_SHOW_FOOD_VALUES_TOOLTIP = b2;
		AppleMainModule::ModConfig::SHOW_SATURATION_OVERLAY = b3;
		AppleMainModule::ModConfig::SHOW_FOOD_VALUES_OVERLAY = b4;
		AppleMainModule::ModConfig::SHOW_FOOD_VALUES_OVERLAY_WHEN_OFFHAND = b5;
		AppleMainModule::ModConfig::SHOW_FOOD_EXHAUSTION_UNDERLAY = b6;
		AppleMainModule::ModConfig::SHOW_FOOD_DEBUG_INFO = b7;
		AppleMainModule::ModConfig::SHOW_FOOD_HEALTH_HUD_OVERLAY = b8;
		AppleMainModule::ModConfig::SHOW_VANILLA_ANIMATION_OVERLAY = b9;
		AppleMainModule::ModConfig::MAX_HUD_OVERLAY_FLASH_ALPHA = fmaxf(0.0f, fminf(1.0f, f1));
	}
)