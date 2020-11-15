#include <unistd.h>
#include <src/And64InlineHook/And64InlineHook.hpp>
#include <src/KittyMemory/MemoryPatch.h>
#include "KillButtonManager.hpp"

// ---------- Variables ---------- //
static bool initialized = false;

// ---------- Patch Table ---------- //
static bool noKillCooldown = false;

// ---------- Hooking Table ---------- //
static void __fastcall(*Old_SetCoolDown)(__int64, int, char, int, int, int, __int64, char, float, float);
static void __fastcall New_SetCoolDown(__int64 a1, int a2, char a3, int a4, int a5, int a6, __int64 a7, char a8, float a9, float a10) {
    if(noKillCooldown)
        a9 = 0.f;
    Old_SetCoolDown(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10);
}

void KillButtonManager::EnableNoKillCooldown() {
    if(initialized)
        noKillCooldown = true;
}

void KillButtonManager::DisableNoKillCooldown() {
    noKillCooldown = false;
}

KillButtonManager::KillButtonManager(uintptr_t pointer) : BaseObject(pointer) {}
KillButtonManager::~KillButtonManager() = default;

bool KillButtonManager::isInitialized() {
    return initialized;
}

bool KillButtonManager::isUsable() {
    return initialized;
}

void KillButtonManager::init() {
    if(initialized)
        return;

    A64HookFunction((void *) getAbsoluteAddress(libName, MethodOffset_SetCoolDown),
                    (void *) New_SetCoolDown,
                    (void **) &Old_SetCoolDown);

    initialized = true;
}
