#ifndef APPLESKIN_TIMER_HPP
#define APPLESKIN_TIMER_HPP


class Timer {
    public:
    float getAlpha() const;
};

class Minecraft {
    public:
    Timer* getTimer();
};

namespace GlobalContext {
    Minecraft* getMinecraft();
}


#endif //APPLESKIN_TIMER_HPP