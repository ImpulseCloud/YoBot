#pragma once


#include "sc2api/sc2_interfaces.h"
#include "sc2api/sc2_agent.h"
#include "sc2api/sc2_map_info.h"
#include "sc2lib/sc2_lib.h"

#include "MapTopology.h"
#include "MapTools.h"
#include "BaseLocationManager.h" //also includes BaseLocation.h
#include "UnitInfoManager.h"
#include "WorkerManager.h"

#define DllExport   __declspec( dllexport )  

using namespace sc2;

// A Yo Agent, is an agent that already has some nice stuff it knows about world and game state
// that is not generally available in API
class YoAgent : public Agent {
public : 

	sc2::Race               m_playerRace[2];

	MapTools			m_map; //would rather this be m_mapTools, but settling with shorter for now
	BaseLocationManager m_baseLocationManager; //full clear names
	UnitInfoManager     m_unitInfoManager;
	WorkerManager		m_workerManager;

	YoAgent()
		: m_map(*this)
		, m_baseLocationManager(*this)
		, m_unitInfoManager(*this)
		, m_workerManager(*this)
	{

	}

	//void initialize() { map.init(Observation(), Query(), Debug()); }

	void OnGameStart() {
		map.init(Observation(), Query(), Debug()); //old YoBot MapTopology class

		// get my race
		auto playerID = Observation()->GetPlayerID();
		for (auto & playerInfo : Observation()->GetGameInfo().player_info)
		{
			if (playerInfo.player_id == playerID)
			{
				m_playerRace[Players::Self] = playerInfo.race_actual;
			}
			else
			{
				m_playerRace[Players::Enemy] = playerInfo.race_requested;
			}
		}

		//m_techTree.onStart(); //in child class
		//m_strategy.onStart(); //not used yet
		//m_map.onStart();      //in child class
		//m_bases.onStart();    //in child class
		//m_unitInfo.onStart(); //in child class
		//m_workerManager.onStart(); //in child class
	}


protected :
	MapTopology map; //would rather this be mapTopo, at least
};

