declare function WRAP_NATIVE(module: "AppleSkinConfig"): {
    initModConfig: (b1: boolean, b2: boolean, b3: boolean, b4: boolean, b5: boolean, b6: boolean, b7: boolean, b8: boolean, b9: boolean, f1: number) => void;
}

WRAP_NATIVE("AppleSkinConfig").initModConfig(
    __config__.getBool("showFoodValuesInTooltip"),
    __config__.getBool("showFoodValuesInTooltip"),
    __config__.getBool("showSaturationHudOverlay"),
    __config__.getBool("showFoodValuesHudOverlay"),
    __config__.getBool("showFoodValuesHudOverlayWhenOffhand"),
    __config__.getBool("showFoodExhaustionHudUnderlay"),
    __config__.getBool("showFoodStatsInDebugOverlay"),
    __config__.getBool("showFoodHealthHudOverlay"),
    __config__.getBool("showVanillaAnimationsOverlay"),
    __config__.getFloat("maxHudOverlayFlashAlpha")
);