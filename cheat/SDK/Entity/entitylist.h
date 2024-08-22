#pragma once
#include "../SDK.hpp"
#include <mutex>
#include <thread>
#include <chrono>
#include <memory>
#include <iostream> // For debugging

namespace SDK
{
    class Entity
    {
    private:
        std::vector<EntityData> EntityList_X;
    

        void cache()
        {
            while (true)
            {
                EntityList_X.clear();
               // std::cout << "[DEBUG] Starting new cache cycle.\n";

                uintptr_t localplayer = Driver->Read<uintptr_t>(storage::clientdll + offsets::dwLocalPlayerPawn);
                if (!localplayer)
                {
                  //  std::cout << "[ERROR] Failed to read localplayer.\n";
                    continue;
                }
               // std::cout << "[DEBUG] LocalPlayer: " << std::hex << localplayer << std::dec << "\n";

                INT localteamid = Driver->Read<INT>(localplayer + offsets::client_dll::m_iTeamNum);
                //std::cout << "[DEBUG] LocalTeamID: " << localteamid << "\n";
                this->List.clear();
                for (int i = 0; i < 64; i++) {
                  //  std::cout << "[DEBUG] Iterating entity list: " << i << "\n";

                    uintptr_t entity = Driver->Read<uintptr_t>(storage::clientdll + offsets::dwEntityList);
                    if (!entity) {
                   //     std::cout << "[ERROR] Entity list pointer is null.\n";
                        continue;
                    }
                  //  std::cout << "[DEBUG] Entity: " << std::hex << entity << std::dec << "\n";

                    uintptr_t listEntity = Driver->Read<uintptr_t>(entity + ((8 * (i & 0x7FFF) >> 9) + 16));
                    if (!listEntity) {
                    //    std::cout << "[ERROR] List entity is null for index " << i << ".\n";
                        continue;
                    }
                  //  std::cout << "[DEBUG] ListEntity: " << std::hex << listEntity << std::dec << "\n";

                    uintptr_t entityController = Driver->Read<uintptr_t>(listEntity + (120) * (i & 0x1FF));
                    if (!entityController) {
                    //    std::cout << "[ERROR] Entity controller is null for index " << i << ".\n";
                        continue;
                    }
                   // std::cout << "[DEBUG] EntityController: " << std::hex << entityController << std::dec << "\n";

                    uintptr_t entityControllerPawn = Driver->Read<uintptr_t>(entityController + 0x7DC);
                    if (!entityControllerPawn) {
                    //    std::cout << "[ERROR] Entity controller pawn is null for index " << i << ".\n";
                        continue;
                    }
                   // std::cout << "[DEBUG] EntityControllerPawn: " << std::hex << entityControllerPawn << std::dec << "\n";

                    listEntity = Driver->Read<uintptr_t>(entity + (0x8 * ((entityControllerPawn & 0x7FFF) >> 9) + 16));
                    if (!listEntity) {
                    //    std::cout << "[ERROR] List entity (re-read) is null for index " << i << ".\n";
                        continue;
                    }
                  //  std::cout << "[DEBUG] ListEntity (re-read): " << std::hex << listEntity << std::dec << "\n";

                    uintptr_t entityPawn = Driver->Read<uintptr_t>(listEntity + (120) * (entityControllerPawn & 0x1FF));
                    if (!entityPawn) {
                    //    std::cout << "[ERROR] Entity pawn is null for index " << i << ".\n";
                        continue;
                    }
                   // std::cout << "[DEBUG] EntityPawn: " << std::hex << entityPawn << std::dec << "\n";

                    INT health = Driver->Read<INT>(entityPawn + offsets::client_dll::m_iHealth);
                    //std::cout << "[DEBUG] Health of entity " << i << ": " << health << "\n";
                    if(config::cheat::deadcheck){ if (health == 0) continue; }
                    

                    INT teamid = Driver->Read<INT>(entityPawn + offsets::client_dll::m_iTeamNum);
                   // std::cout << "[DEBUG] TeamID of entity " << i << ": " << teamid << "\n";
                    if(config::cheat::teamcheck) { if (teamid == localteamid) continue; }
                  

                    uintptr_t entityNameAddress = Driver->Read<uintptr_t>(entityController + offsets::client_dll::m_sSanitizedPlayerName);
                    INT64 steamid = Driver->Read<INT64>(entityController + offsets::client_dll::m_steamID);
                   // std::cout << "[DEBUG] SteamID of entity " << i << ": " << steamid << "\n";

                    char entityName[128] = { 0 };
                    Math::Vector3 POS = Driver->Read<Math::Vector3>(entityPawn + offsets::client_dll::m_vOldOrigin);
                    Driver->Read(entityNameAddress, entityName, sizeof(entityName));
                   // std::cout << "[DEBUG] Entity name: " << entityName << "\n";
                   // std::cout << "[DEBUG] Position: [" << POS.x << ", " << POS.y << ", " << POS.z << "]\n";

                    if (config::cheat::botcheck)
                    {
                        if (steamid == 0) {
                            std::string botName = "BOT ";
                            botName += entityName;
                            std::strncpy(entityName, botName.c_str(), sizeof(entityName));
                          //  std::cout << "[DEBUG] Bot name assigned: " << entityName << "\n";
                        }
                    }
  

                    EntityData entityData(entityController, entityPawn, std::string(entityName), POS, teamid, health, steamid);

                    {
                      
                        EntityList_X.push_back(entityData);
                        this->List = EntityList_X;
                      //  std::cout << "[DEBUG] EntityData pushed and List updated.\n";
                    }
                }

              //  std::this_thread::sleep_for(std::chrono::milliseconds(10)); // Small delay to reduce CPU usage
                //std::cout << "[DEBUG] Cache cycle completed.\n";
            }
        }

    public:
        std::vector<EntityData> List;

        void Run()
        {
            std::cout << "[DEBUG] Starting Entity cache thread.\n";
            std::thread([&]() { this->cache(); }).detach();
            std::cout << "[DEBUG] Entity cache thread started.\n";
        }
    };

    inline std::unique_ptr<SDK::Entity> entities = std::make_unique<SDK::Entity>();
}
