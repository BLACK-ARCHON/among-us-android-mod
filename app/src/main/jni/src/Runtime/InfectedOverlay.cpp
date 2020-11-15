#include <unistd.h>
#include <src/And64InlineHook/And64InlineHook.hpp>
#include "InfectedOverlay.hpp"

// ---------- Variables ---------- //
static bool initialized = false;

// ---------- Patch Table ---------- //
static bool noSabotageCooldown = false;

// ---------- Hooking Table ---------- //
static __int64 __fastcall(*Old_get_CanUseDoors)(__int64);
static __int64 __fastcall New_get_CanUseDoors(__int64 a1) {
    return noSabotageCooldown ? true : Old_get_CanUseDoors(a1);
}

static __int64 __fastcall(*Old_get_CanUseSpecial)(__int64);
static __int64 __fastcall New_get_CanUseSpecial(__int64 a1) {
    return noSabotageCooldown ? true : Old_get_CanUseSpecial(a1);
}

void InfectedOverlay::EnableNoSabotageCooldown() {
    noSabotageCooldown = true;
}

void InfectedOverlay::DisableNoSabotageCooldown() {
    noSabotageCooldown = false;
}

InfectedOverlay::InfectedOverlay(uintptr_t pointer) : BaseObject(pointer) {}
InfectedOverlay::~InfectedOverlay() = default;

bool InfectedOverlay::isInitialized() {
    return initialized;
}

bool InfectedOverlay::isUsable() {
    return initialized;
}

void InfectedOverlay::init() {
    if(initialized)
        return;

    A64HookFunction((void *) getAbsoluteAddress(libName, MethodOffset_get_CanUseDoors),
                    (void *) New_get_CanUseDoors,
                    (void **) &Old_get_CanUseDoors);

    A64HookFunction((void *) getAbsoluteAddress(libName, MethodOffset_get_CanUseSpecial),
                    (void *) New_get_CanUseSpecial,
                    (void **) &Old_get_CanUseSpecial);

    initialized = true;
}