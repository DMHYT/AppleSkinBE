
#include <hook.h>
#include <mod.h>
#include <logger.h>
#include <symbol.h>

#include <nativejs.h>


class MainModule : public Module {
public:
	MainModule(const char* id): Module(id) {};

	virtual void initialize() {	

    }
};

// class OtherModule : public Module {
// public:
// 	OtherModule(Module* parent, const char* id) : Module(parent, id) {};
// };

MAIN {
	Module* main_module = new MainModule("appleskin");
	// new OtherModule(main_module, "sample_library.other_module");
}



enum UIProfile {
	pe, classic
};

class Options {
	public:
	UIProfile getUIProfile() const;
};

class ClientInstance {
	public:
	Options* getOptions() const;
	int getGuiScale() const;
};

namespace GlobalContext {
	ClientInstance* getMinecraftClient();
};

JS_MODULE_VERSION(NativeUI, 1);

JS_EXPORT(NativeUI, getProfile, "I()", (JNIEnv* env){
	return GlobalContext::getMinecraftClient()->getOptions()->getUIProfile();
});
JS_EXPORT(NativeUI, getScale, "I()", (JNIEnv* env){
	return GlobalContext::getMinecraftClient()->getGuiScale();
});

// class ComponentItem:
// 	public:


// class ItemComponent:
// 	public:
	

// class FoodItemComponent:
// 	public:
// 	FoodItemComponent(ItemComponent*);
// 	int getNutrition() const;
// 	int getSaturationModifier() const;

// JS_MODULE_VERSION(NativeFood, 1);

// JS_EXPORT(NativeFood, getNutrition, ...);
// JS_EXPORT(NativeFood, getSaturation, ...);