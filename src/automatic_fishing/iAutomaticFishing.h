#pragma once
#include <ll/api/event/ListenerBase.h>
#include <ll/api/mod/NativeMod.h>

namespace automatic_fishing {

class iAutomaticFishing {
public:
    struct FishingHookedHook;
    struct HandleActorEventPacketHook;

public:
    static iAutomaticFishing& getInstance();

    iAutomaticFishing() : mSelf(*ll::mod::NativeMod::current()), mListeners() {}

    [[nodiscard]] ll::mod::NativeMod& getSelf() const { return mSelf; }

    bool load();
    bool enable();
    bool disable();
    bool unload();

private:
    ll::mod::NativeMod&                  mSelf;
    ll::DenseSet<ll::event::ListenerPtr> mListeners;
};

} // namespace automatic_fishing