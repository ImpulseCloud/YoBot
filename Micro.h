#pragma once

#include "Common.h"
#include "sc2api/sc2_api.h"

//class CCBot; //replace with YoBot or YoAgent?

namespace Micro
{
	void SmartStop(const sc2::Unit * attacker, sc2::Agent & bot);
	void SmartAttackUnit(const sc2::Unit * attacker, const sc2::Unit * target, sc2::Agent & bot);
	void SmartAttackMove(const sc2::Unit * attacker, const sc2::Point2D & targetPosition, sc2::Agent & bot);
	void SmartMove(const sc2::Unit * attacker, const sc2::Point2D & targetPosition, sc2::Agent & bot);
	void SmartRightClick(const sc2::Unit * unit, const sc2::Unit * target, sc2::Agent & bot);
	void SmartRepair(const sc2::Unit * unit, const sc2::Unit * target, sc2::Agent & bot);
	void SmartKiteTarget(const sc2::Unit * rangedUnit, const sc2::Unit * target, sc2::Agent & bot);
	void SmartBuild(const sc2::Unit * builder, const sc2::UnitTypeID & buildingType, sc2::Point2D pos, sc2::Agent & bot);
	void SmartBuildTarget(const sc2::Unit * builder, const sc2::UnitTypeID & buildingType, const sc2::Unit * target, sc2::Agent & bot);
	void SmartTrain(const sc2::Unit * builder, const sc2::UnitTypeID & buildingType, sc2::Agent & bot);
	void SmartAbility(const sc2::Unit * builder, const sc2::AbilityID & abilityID, sc2::Agent & bot);
};