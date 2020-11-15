#pragma once
#include "BaseObject.hpp"
#include <type_traits>

class PlayerInfo : public BaseObject {
private:
    bool initialized = false;

    void _init();

public:
    enum FieldOffset : uintptr_t {
        FieldOffset_ColorId = 0x20,
        FieldOffset_Disconnected = 0x30,
        FieldOffset_HatId = 0x24,
        FieldOffset_IsDead = 0x41,
        FieldOffset_IsImpostor = 0x40,
        FieldOffset_PetId = 0x28,
        FieldOffset_PlayerId = 0x10,
        FieldOffset_PlayerName = 0x18,
        FieldOffset_SkinId = 0x2C,
        FieldOffset_Tasks = 0x38,
        FieldOffset__object = 0x48
    };

    template<FieldOffset Offset>
    using DetermineFieldType =
    typename std::conditional<Offset == FieldOffset_ColorId || Offset == FieldOffset_HatId || Offset == FieldOffset_PlayerId, unsigned char,
    typename std::conditional<Offset == FieldOffset_Disconnected || Offset == FieldOffset_IsDead || Offset == FieldOffset_IsImpostor, bool,
    typename std::conditional<Offset == FieldOffset_PetId || Offset == FieldOffset_SkinId, unsigned int, uintptr_t>::type>::type>::type;

    PlayerInfo(uintptr_t pointer);
    ~PlayerInfo();

    virtual bool isInitialized() override;
    virtual bool isUsable() override;
    virtual void init() override;
};