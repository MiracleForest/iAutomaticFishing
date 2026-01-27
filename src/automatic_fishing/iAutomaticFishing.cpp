#include "automatic_fishing/iAutomaticFishing.h"
#include <ll/api/event/EventBus.h>
#include <ll/api/event/client/ClientExitLevelEvent.h>
#include <ll/api/event/client/ClientJoinLevelEvent.h>
#include <ll/api/event/server/ServerStartedEvent.h>
#include <ll/api/event/server/ServerStoppingEvent.h>
#include <ll/api/memory/Hook.h>
#include <ll/api/mod/RegisterHelper.h>
#include <ll/api/service/Bedrock.h>
#include <ll/api/service/GamingStatus.h>
#include <mc/client/network/ClientNetworkHandler.h>
#include <mc/network/packet/ActorEventPacket.h>
#include <mc/world/actor/FishingHook.h>
#include <mc/world/actor/player/Player.h>
#include <mc/world/gamemode/GameMode.h>
#include <mc/world/item/Item.h>
#include <mc/world/level/Level.h>

namespace automatic_fishing {

using namespace ll::memory_literals;

LL_TYPE_INSTANCE_HOOK(
    iAutomaticFishing::FishingHookedHook,
    HookPriority::Normal,
    FishingHook,
    &FishingHook::_serverHooked,
    bool
) {
    auto result = origin();
    if (!result || mTimeUntilHooked) return result;
    auto* player = getPlayerOwner();
    if (!player) return result;
    auto& item   = const_cast<ItemStack&>(player->getSelectedItem());
    player->mGameMode->baseUseItem(item);
    if (!item.isNull()) player->mGameMode->baseUseItem(item);
    return result;
}

LL_TYPE_INSTANCE_HOOK(
    iAutomaticFishing::HandleActorEventPacketHook,
    HookPriority::Normal,
    ClientNetworkHandler,
    // &ClientNetworkHandler::$handle,
    "?handle@ClientNetworkHandler@@UEAAXAEBVNetworkIdentifier@@AEBVActorEventPacket@@@Z"_sym,
    void,
    NetworkIdentifier const& source,
    ActorEventPacket const&  packet
) {
    if (packet.mEventId != ActorEvent::FishhookHooktime) return origin(source, packet);
    auto* entity = mLevel->mPointer->getRuntimeEntity(packet.mRuntimeId, false);
    if (!entity) return origin(source, packet);
    auto* player = entity->getPlayerOwner();
    if (!player) return origin(source, packet);
    auto& item = const_cast<ItemStack&>(player->getSelectedItem());
    player->mGameMode->baseUseItem(item);
    if (!item.isNull()) player->mGameMode->baseUseItem(item);
    origin(source, packet);
}

iAutomaticFishing& iAutomaticFishing::getInstance() {
    static iAutomaticFishing instance;
    return instance;
}

bool iAutomaticFishing::load() {
    auto& eventBus = ll::event::EventBus::getInstance();
    mListeners.insert(eventBus.emplaceListener<ll::event::ServerStartedEvent>([](auto&&) {
        FishingHookedHook::hook();
    }));
    mListeners.insert(eventBus.emplaceListener<ll::event::ServerStoppingEvent>([](auto&&) {
        FishingHookedHook::unhook();
    }));
    mListeners.insert(eventBus.emplaceListener<ll::event::ClientJoinLevelEvent>([](auto&&) {
        HandleActorEventPacketHook::hook();
    }));
    mListeners.insert(eventBus.emplaceListener<ll::event::ClientExitLevelEvent>([](auto&&) {
        HandleActorEventPacketHook::unhook();
    }));
    if (ll::service::getMinecraft(false)) FishingHookedHook::hook();
    else if (ll::service::getMinecraft(true)) HandleActorEventPacketHook::hook();
    return true;
}

bool iAutomaticFishing::enable() { return true; }

bool iAutomaticFishing::disable() { return true; }

bool iAutomaticFishing::unload() {
    auto& eventBus = ll::event::EventBus::getInstance();
    for (auto& listener : mListeners) eventBus.removeListener(listener);
    mListeners.clear();
    if (ll::getGamingStatus() != ll::GamingStatus::Stopping) {
        FishingHookedHook::unhook();
        HandleActorEventPacketHook::unhook();
    }
    return true;
}

} // namespace automatic_fishing

LL_REGISTER_MOD(automatic_fishing::iAutomaticFishing, automatic_fishing::iAutomaticFishing::getInstance());