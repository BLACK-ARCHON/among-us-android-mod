#include <unistd.h>
#include <src/And64InlineHook/And64InlineHook.hpp>
#include "SabotageSystemType.hpp"

// ---------- Variables ---------- //
static bool initialized = false;

// ---------- Patch Table ---------- //
static bool noSabotageCooldown = false;

// ---------- Hooking Table ---------- //
__int64 __fastcall (*Old_get_AnyActive)(__int64, int, char, int, int, int, __int64, char);
__int64 __fastcall New_get_AnyActive(__int64 a1, int a2, char a3, int a4, int a5, int a6, __int64 a7, char a8) {
    if(!noSabotageCooldown)
        return Old_get_AnyActive(a1, a2, a3, a4, a5, a6, a7, a8);
    return 0;
}

float __fastcall (*Old_get_PercentCool)(__int64);
float __fastcall New_get_PercentCool(__int64 a1) {
    if(!noSabotageCooldown)
        return Old_get_PercentCool(a1);
    return 0.f;
}

float __fastcall (*Old_get_Timer)(__int64);
float __fastcall New_get_Timer(__int64 a1) {
    if(!noSabotageCooldown)
        return Old_get_Timer(a1);
    return 0.f;
}

void SabotageSystemType::EnableNoSabotageCooldown() {
    noSabotageCooldown = true;
}

void SabotageSystemType::DisableNoSabotageCooldown() {
    noSabotageCooldown = false;
}

SabotageSystemType::SabotageSystemType(uintptr_t pointer) : BaseObject(pointer) {}
SabotageSystemType::~SabotageSystemType() = default;

bool SabotageSystemType::isInitialized() {
    return initialized;
}

bool SabotageSystemType::isUsable() {
    return initialized;
}

void SabotageSystemType::init() {
    if(initialized)
        return;

    A64HookFunction((void *) getAbsoluteAddress(libName, MethodOffset_get_AnyActive),
                    (void *) New_get_AnyActive,
                    (void **) &Old_get_AnyActive);

    A64HookFunction((void *) getAbsoluteAddress(libName, MethodOffset_get_PercentCool),
                    (void *) New_get_PercentCool,
                    (void **) &Old_get_PercentCool);

    A64HookFunction((void *) getAbsoluteAddress(libName, MethodOffset_get_Timer),
                    (void *) New_get_Timer,
                    (void **) &Old_get_Timer);


    initialized = true;
}