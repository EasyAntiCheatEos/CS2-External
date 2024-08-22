#pragma once
#pragma warning(disable:4996)

inline namespace storage
{
	inline uintptr_t clientdll;
	
}
namespace config
{
	inline bool showwindow = true;
	inline INT currenttab;
	inline HWND HWND_X;
	
	namespace cheat
	{
		inline bool streamproof;
		inline bool teamcheck;
		inline bool deadcheck;
		inline bool botcheck;
	}
}

namespace offsets
{
	inline DWORD64 dwEntityList = 0x1964198;
	inline DWORD64 dwLocalPlayerPawn = 0x17CF698;
	inline DWORD64 m_pInGameMoneyServices = 0x6F0;
	namespace client_dll
	{
		// moneyservices
		inline DWORD64 m_iAccount = 0x40; // int32
		inline DWORD64 m_iStartAccount = 0x44; // int32
		inline DWORD64 m_iTotalCashSpent = 0x48; // int32
		inline DWORD64 m_iCashSpentThisRound = 0x4C; // int32

		inline DWORD64 m_iTeamNum = 0x3C3;
		inline DWORD64 m_steamID = 0x6B8;
		inline DWORD64 m_iHealth = 0x324;
		inline DWORD64 m_sSanitizedPlayerName = 0x740;
		inline DWORD64 m_vOldOrigin = 0x1274;
	}
}