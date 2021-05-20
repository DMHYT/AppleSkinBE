/// <reference path="core-engine.d.ts" />
declare interface NativeUI {
    getProfile(): number;
    getScale(): number;
}
declare interface NativeFood {
    getNutrition(id: number, data: number): number;
    getSaturation(id: number, data: number): number;
}
declare function WRAP_NATIVE(module: "NativeUI"): NativeUI;
declare function WRAP_NATIVE(module: "NativeFood"): NativeFood;