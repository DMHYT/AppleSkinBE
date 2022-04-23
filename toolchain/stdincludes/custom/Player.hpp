#include <MobEffect.hpp>

#ifndef APPLESKIN_PLAYER_HPP
#define APPLESKIN_PLAYER_HPP

#ifndef APPLESKIN_FOODITEMCOMPONENT_LEGACY_HPP
    class ItemStack;
#endif

class ActorUniqueID {
    public:
    long long id;
    ActorUniqueID(long long);
};

class Actor {
    public:
    static Actor* wrap(long long);
    ItemStack* getOffhandSlot() const;
    ActorUniqueID* getUniqueID() const;
    bool hasEffect(MobEffect const&) const;
};

class Mob : public Actor {public:};

class Attribute {public:};

class Player : public Mob {
    public:
    static Attribute HUNGER;
    float getExhaustion() const;
    float getHunger() const;
    float getSaturation() const;
};

class LocalPlayer : public Player {public:};
class ServerPlayer : public Player {public:};


#endif //APPLESKIN_PLAYER_HPP