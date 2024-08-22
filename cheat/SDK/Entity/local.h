#pragma once
#include "../SDK.hpp"
#include <thread>
#include <chrono>
#include <memory>
#include <iostream>

namespace SDK
{
    class Local
    {
    private:
        std::vector<LocalEntityData> LocalData;

        void cache()
        {
            while (true)
            {
                {
                    LocalData.clear();
                }

                uintptr_t localplayer = Driver->Read<uintptr_t>(storage::clientdll + offsets::dwLocalPlayerPawn);
                if (!localplayer)
                {
                    continue;
                }

                INT localteamid = Driver->Read<INT>(localplayer + offsets::client_dll::m_iTeamNum);

                INT health = Driver->Read<INT>(localplayer + offsets::client_dll::m_iHealth);
                if (config::cheat::deadcheck && health == 0) continue;

                INT teamid = Driver->Read<INT>(localplayer + offsets::client_dll::m_iTeamNum);
                if (config::cheat::teamcheck && teamid == localteamid) continue;

                uintptr_t entityNameAddress = Driver->Read<uintptr_t>(localplayer + offsets::client_dll::m_sSanitizedPlayerName);
                INT64 steamid = Driver->Read<INT64>(localplayer + offsets::client_dll::m_steamID);

                char entityName[128] = { 0 };
                Math::Vector3 POS = Driver->Read<Math::Vector3>(localplayer + offsets::client_dll::m_vOldOrigin);
                Driver->Read(entityNameAddress, entityName, sizeof(entityName));

                LocalEntityData entityData(localplayer, std::string(entityName), POS, teamid, health, steamid);

                {
                    LocalData.push_back(entityData);
                    this->List = LocalData;
                }

            }
        }

    public:
        std::vector<LocalEntityData> List;

        void Run()
        {
            std::cout << "[DEBUG] Starting Local Entity cache thread.\n";
            std::thread([&]() { this->cache(); }).detach();
            std::cout << "[DEBUG] Local Entity cache thread started.\n";
        }
    };

    inline std::unique_ptr<SDK::Local> local = std::make_unique<SDK::Local>();

}
