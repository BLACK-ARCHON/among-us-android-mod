#include <unistd.h>
#include <src/And64InlineHook/And64InlineHook.hpp>
#include "InnerNetClient.hpp"

// ---------- Variables ---------- //
static bool initialized = false;

// ---------- Patch Table ---------- //
static bool oneShotOneBan = false;
static bool oneShotOneKick = false;

// ---------- Hooking Table ---------- //
static bool __fastcall(*Old_CanBan)(unsigned int*);
static bool __fastcall New_CanBan(unsigned int *a1) {
    return oneShotOneBan ? true : Old_CanBan(a1);
}

static bool __fastcall(*Old_CanKick)(unsigned int*);
static bool __fastcall New_CanKick(unsigned int *a1) {
    return oneShotOneKick ? true : Old_CanKick(a1);
}

//ALPHA
static void __fastcall(*Old_CmdAddVote)(__int64, unsigned int);
static void __fastcall New_CmdAddVote(__int64 a1, unsigned int a2) {
    if(!oneShotOneKick) {
        Old_CmdAddVote(a1, a2);
        return;
    }

    Old_CmdAddVote(a1, a2);
    Old_CmdAddVote(a1, a2);
    Old_CmdAddVote(a1, a2);
}


void InnerNetClient::EnableOneShotOneBan() {
    oneShotOneBan = true;
}

void InnerNetClient::DisableOneShotOneBan() {
    oneShotOneBan = false;
}

void InnerNetClient::EnableOneShotOneKick() {
    oneShotOneKick = true;
}

void InnerNetClient::DisableOneShotOneKick() {
    oneShotOneKick = false;
}

InnerNetClient::InnerNetClient(uintptr_t pointer) : BaseObject(pointer) {}
InnerNetClient::~InnerNetClient() = default;

bool InnerNetClient::isInitialized() {
    return initialized;
}

bool InnerNetClient::isUsable() {
    return initialized;
}

void InnerNetClient::init() {
    if(initialized)
        return;
/*
    A64HookFunction((void *) getAbsoluteAddress(libName, MethodOffset_CanBan),
                    (void *) New_CanBan,
                    (void **) &Old_CanBan);

    A64HookFunction((void *) getAbsoluteAddress(libName, MethodOffset_CanKick),
                    (void *) New_CanKick,
                    (void **) &Old_CanKick);

*/

    A64HookFunction((void *) getAbsoluteAddress(libName, MethodOffset_CmdAddVote),
                    (void *) New_CmdAddVote,
                    (void **) &Old_CmdAddVote);

    initialized = true;
}