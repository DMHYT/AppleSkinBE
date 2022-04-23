#ifndef APPLESKIN_MOBEFFECT_HPP
#define APPLESKIN_MOBEFFECT_HPP


class MobEffect {
    public:
    static MobEffect* getById(int);
    bool isHarmful() const;
};


#endif //APPLESKIN_MOBEFFECT_HPP