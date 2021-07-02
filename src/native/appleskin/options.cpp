#include <nativejs.h>
#include "recovered.h"

JS_MODULE_VERSION(ASOptionsJSAdapter, 1);

JS_EXPORT(ASOptionsJSAdapter, getProfile, "I()", (JNIEnv* env){
    return GlobalContext::getMinecraftClient()->getOptions()->getUIProfile();
});