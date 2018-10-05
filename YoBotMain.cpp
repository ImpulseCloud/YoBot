#include <iostream>
#include "sc2api/sc2_api.h"
#include "sc2lib/sc2_lib.h"
#include "sc2utils/sc2_manage_process.h"
#include "sc2utils/sc2_arg_parser.h"

#include "YoBot.h"
#include "LadderInterface.h"

#ifdef DEBUG
int main(int argc, char* argv[])
{
	YoBot bot;
	YoBot bot2;
	sc2::Coordinator coordinator;
	if (!coordinator.LoadSettings(argc, argv))
	{
		std::cout << "Unable to find or parse settings." << std::endl;
		return 1;
	}
	coordinator.SetStepSize(1);
	coordinator.SetRealtime(false);
	coordinator.SetMultithreaded(true);
	coordinator.SetParticipants({
		CreateParticipant(sc2::Race::Protoss, &bot),//CreateParticipant(sc2::Race::Protoss, &bot2)
		//sc2::PlayerSetup(sc2::PlayerType::Observer,Util::GetRaceFromString(enemyRaceString)),
		CreateComputer(sc2::Race::Terran, sc2::Difficulty::VeryHard)
	});
	// Start the game.
	coordinator.LaunchStarcraft();
	//"AcidPlantLE.SC2Map"//"Interloper LE" "16-Bit LE"
	//Redshift.SC2Map"; //DarknessSanctuary.SC2Map"; //16BitLE.SC2Map";
	auto map = "F:\\StarCraft II\\Maps\\CeruleanFallLE.SC2Map"; //LostAndFoundLE.SC2Map"; //InterloperLE.SC2Map");

	//coordinator.StartGame();
	// coordinator.StartGame("16-Bit LE");
	if (coordinator.StartGame(map)) {
		// Step forward the game simulation.
		while (coordinator.Update())
		{
		}
	}
	else {
		std::cout << "There was a problem loading the map : " << map << std::endl;
	}
}

#else
//*************************************************************************************************
int main(int argc, char* argv[]) 
{

	
	RunBot(argc, argv, new YoBot(), sc2::Race::Protoss);

	return 0;
}
#endif