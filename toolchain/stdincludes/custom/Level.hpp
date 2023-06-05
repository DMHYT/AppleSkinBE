#ifndef APPLESKIN_LEVEL_HPP
#define APPLESKIN_LEVEL_HPP


double getTimeS();

class Core {
    public:
    class Random {
        public:
        void _setSeed(unsigned int);
    };
};

class Random : public Core::Random {
    public:
    int nextInt(int);
};

class GameRuleId {
    public:
    char filler[4];
    GameRuleId(int);
};

class GameRules;

class Level {
    public:
    bool isClientSide() const;
    Random* getRandom() const;
    int getDifficulty() const;
    GameRules* getGameRules();
};

class ServerLevel : public Level { public: };


#endif //APPLESKIN_LEVEL_HPP