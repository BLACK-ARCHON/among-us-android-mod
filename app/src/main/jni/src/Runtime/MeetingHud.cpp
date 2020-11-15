#include <unistd.h>
#include <src/And64InlineHook/And64InlineHook.hpp>
#include "src/KittyMemory/MemoryPatch.h"
#include "PlayerControl.hpp"
#include "MeetingHud.hpp"

// ---------- Variables ---------- //
static bool initialized = false;

// ---------- Patch Table ---------- //
static struct PatchTable {
    MemoryPatch CrewmateCanSeeImpostor;
} patchTable;

static bool oneShotOneExile = false;

// ---------- Hooking Table ---------- //
static void __fastcall (*Old_CmdCastVote)(__int64 a1, unsigned int a2, unsigned int a3);
static void __fastcall New_CmdCastVote(__int64 a1, unsigned int a2, unsigned int a3) {
    if(!oneShotOneExile) {
        Old_CmdCastVote(a1, a2, a3);
        return;
    }

    for(auto instance: PlayerControl::getInstances())
        Old_CmdCastVote(a1, instance.get<PlayerControl::DetermineFieldType<PlayerControl::FieldOffset_PlayerId>>(PlayerControl::FieldOffset_PlayerId), a3);
}

void MeetingHud::EnableCrewmateCanSeeImpostor() {
    if(initialized)
        patchTable.CrewmateCanSeeImpostor.Modify();
}

void MeetingHud::DisableCremwateCanSeeImpostor() {
    if(initialized)
        patchTable.CrewmateCanSeeImpostor.Restore();
}

void MeetingHud::EnableOneShotOneExile() {
    if(initialized)
        oneShotOneExile = true;
}

void MeetingHud::DisableOneShotOneExile() {
    if(initialized)
        oneShotOneExile = false;
}

MeetingHud::MeetingHud(uintptr_t pointer) : BaseObject(pointer) { }
MeetingHud::~MeetingHud() = default;

bool MeetingHud::isInitialized() {
    return initialized;
}

bool MeetingHud::isUsable() {
    return initialized && getPointer() != NULL;
}

void MeetingHud::init() {
    if(initialized)
        return;

    A64HookFunction((void *) getAbsoluteAddress(libName, MethodOffset_CmdCastVote),
                    (void *) New_CmdCastVote,
                    (void **) &Old_CmdCastVote);

    patchTable.CrewmateCanSeeImpostor = MemoryPatch::createWithHex(libName, MethodOffset_CreateButton_IsImpostor, "1F2003D5");
    initialized = true;
}
