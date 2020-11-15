#include <unistd.h>
#include "PlayerInfo.hpp"

PlayerInfo::PlayerInfo(uintptr_t pointer) : BaseObject(pointer) {
    _init();
}

PlayerInfo::~PlayerInfo() = default;

bool PlayerInfo::isInitialized() {
    return initialized;
}

void PlayerInfo::_init() {
    init();
}

void PlayerInfo::init() {
    if(getPointer() != NULL)
        initialized = true;
}

bool PlayerInfo::isUsable() {
    return initialized && get<DetermineFieldType<FieldOffset_Disconnected>>(FieldOffset_Disconnected);
}

