
#include <hook.h>
// #include "string.h"
#include <mod.h>
#include <logger.h>
#include <symbol.h>
#include <nativejs.h>

class MainModule : public Module {
	public:
	MainModule(const char* id): Module(id) {};
	virtual void initialize(){

	}
};

MAIN {
	Module* main_module = new MainModule("appleskin");
};