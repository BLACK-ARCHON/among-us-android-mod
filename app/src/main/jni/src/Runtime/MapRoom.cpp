#include <unistd.h>
#include <src/And64InlineHook/And64InlineHook.hpp>
#include <src/KittyMemory/MemoryPatch.h>
#include "MapRoom.hpp"

// ---------- Variables ---------- //
static bool initialized = false;

// ---------- Patch Table ---------- //
static bool noSabotageCooldown = false;

// ---------- Function Table ---------- //
struct PatchTable {
    MemoryPatch SabotageDoors_IsLessThanZero;
} patchTable;

__int64 __fastcall(*PF_get_Parent)(__int64);

// ---------- Hooking Table ---------- //
static void __fastcall(*Old_SetSpecialActive)(__int64, int, char, int, int, int, __int64, char, float);
static void __fastcall New_SetSpecialActive(__int64 a1, int a2, char a3, int a4, int a5, int a6, __int64 a7, char a8, float a9) {
    if(noSabotageCooldown)
        a9 = 0.f;
    Old_SetSpecialActive(a1, a2, a3, a4, a5, a6, a7, a8, a9);
}

void __fastcall (*Old_OOBUpdate)(__int64, int, char, int, int, int, __int64, char);
void __fastcall New_OOBUpdate(__int64 a1, int a2, char a3, int a4, int a5, int a6, __int64 a7, char a8) {
    if(!noSabotageCooldown)
        Old_OOBUpdate(a1, a2, a3, a4, a5, a6, a7, a8);
}

void MapRoom::EnableNoSabotageCooldown() {
    if(initialized) {
        patchTable.SabotageDoors_IsLessThanZero.Modify();
        noSabotageCooldown = true;
    }
}

void MapRoom::DisableNoSabotageCooldown() {
    if(initialized) {
        patchTable.SabotageDoors_IsLessThanZero.Restore();
        noSabotageCooldown = false;
    }
}

MapRoom::MapRoom(uintptr_t pointer) : BaseObject(pointer) {}
MapRoom::~MapRoom() = default;

bool MapRoom::isInitialized() {
    return initialized;
}

bool MapRoom::isUsable() {
    return initialized;
}

void MapRoom::init() {
    if(initialized)
        return;

    A64HookFunction((void *) getAbsoluteAddress(libName, MethodOffset_SetSpecialActive),
                    (void *) New_SetSpecialActive,
                    (void **) &Old_SetSpecialActive);

    A64HookFunction((void *) getAbsoluteAddress(libName, MethodOffset_OOBUpdate),
                    (void *) New_OOBUpdate,
                    (void **) &Old_OOBUpdate);

    patchTable.SabotageDoors_IsLessThanZero = MemoryPatch::createWithHex(libName, MethodOffset_SabotageDoors_IsLessThanZero, "06000094");

    initialized = true;
}