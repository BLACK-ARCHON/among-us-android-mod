#include <unistd.h>
#include <src/And64InlineHook/And64InlineHook.hpp>
#include "src/KittyMemory/MemoryPatch.h"
#include "../Includes/Utils.h"
#include "ChatController.hpp"

// ---------- Variables ---------- //
static bool initialized = false;

// ---------- Patch Table ---------- //
static struct PatchTable {
    MemoryPatch CanSeeGhostsChat_A, CanSeeGhostsChat_B, CrewmateCanSeeImpostor;
} patchTable;

void ChatController::EnableCanSeeGhostsChat() {
    if(initialized) {
        patchTable.CanSeeGhostsChat_A.Modify();
        patchTable.CanSeeGhostsChat_B.Modify();
    }
}

void ChatController::DisableCanSeeGhostsChat() {
    if(initialized) {
        patchTable.CanSeeGhostsChat_A.Restore();
        patchTable.CanSeeGhostsChat_B.Restore();
    }
}

void ChatController::EnableCrewmateCanSeeImpostor() {
    if(initialized)
        patchTable.CrewmateCanSeeImpostor.Modify();
}

void ChatController::DisableCrewmateCanSeeImpostor() {
    if(initialized)
        patchTable.CrewmateCanSeeImpostor.Restore();
}

ChatController::ChatController(uintptr_t pointer) : BaseObject(pointer) { }
ChatController::~ChatController()  = default;

bool ChatController::isInitialized() {
    return initialized;
}

bool ChatController::isUsable() {
    return initialized;
}

void ChatController::init() {
    if(initialized)
        return;

    patchTable.CanSeeGhostsChat_A = MemoryPatch::createWithHex(libName, MethodOffset_AddChat_IsDead_A, "1F2003D5");
    patchTable.CanSeeGhostsChat_B = MemoryPatch::createWithHex(libName, MethodOffset_AddChat_IsDead_B, "1F2003D5");
    patchTable.CrewmateCanSeeImpostor = MemoryPatch::createWithHex(libName, MethodOffset_AddChat_IsImpostor, "1F2003D5");
    initialized = true;
}