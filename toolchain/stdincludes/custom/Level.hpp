#ifndef APPLESKIN_LEVEL_HPP
#define APPLESKIN_LEVEL_HPP


class Random {
    public:
    int nextInt(int);
};

class Level {
    public:
    bool isClientSide() const;
    Random* getRandom() const;
};


#endif //APPLESKIN_LEVEL_HPP