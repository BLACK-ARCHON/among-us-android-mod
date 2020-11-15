#include <unistd.h>
#include <src/And64InlineHook/And64InlineHook.hpp>
#include "StatsManager.hpp"

// ---------- Variables ---------- //
static bool initialized = false;

// ---------- Patch Table ---------- //
static bool noLeavePenalty = false;

// ---------- Hooking Table ---------- //
bool __fastcall(*Old_get_AmBanned)(unsigned __int64*, int, char, int, int, int, __int64, char);
bool New_get_AmBanned(unsigned __int64 *a1, int a2, char a3, int a4, int a5, int a6, __int64 a7, char a8) {
    if(noLeavePenalty)
        return false;
    return Old_get_AmBanned(a1, a2, a3, a4, a5, a6, a7, a8);
}

void StatsManager::EnableNoLeavePenalty() {
    if(initialized)
        noLeavePenalty = true;
}

void StatsManager::DisableNoLeavePenalty() {
    noLeavePenalty = false;
}

StatsManager::StatsManager(uintptr_t pointer) : BaseObject(pointer) {}
StatsManager::~StatsManager() = default;

bool StatsManager::isInitialized() {
    return initialized;
}

bool StatsManager::isUsable() {
    return initialized;
}

void StatsManager::init() {
    if(initialized)
        return;

    A64HookFunction((void *) getAbsoluteAddress(libName, MethodOffset_get_Am_Banned),
                    (void *) New_get_AmBanned,
                    (void **) &Old_get_AmBanned);

    initialized = true;
}
