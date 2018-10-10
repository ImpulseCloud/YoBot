#include "ScoutManager.h"
#include "YoAgent.h"
#include "Util.h"
#include "Micro.h"

ScoutManager::ScoutManager(YoAgent & bot)
	: m_bot(bot)
	, m_scoutUnit(nullptr)
	, m_numScouts(0)
	, m_scoutUnderAttack(false)
	, m_scoutStatus("None")
	, m_previousScoutHP(0.0f)
{
}

void ScoutManager::onStart()
{

}

void ScoutManager::onFrame()
{
	moveScouts();
	drawScoutInformation();
}

void ScoutManager::setWorkerScout(const sc2::Unit * tag)
{
	// if we have a previous worker scout, release it back to the worker manager
	if (m_scoutUnit) { m_bot.m_workerManager.finishedWithWorker(m_scoutUnit); }

	m_scoutUnit = tag;
	m_bot.m_workerManager.setScoutWorker(m_scoutUnit);
}

void ScoutManager::drawScoutInformation()
{
	//if (!m_bot.Config().DrawScoutInfo) return;
	return;

	std::stringstream ss;
	ss << "Scout Info: " << m_scoutStatus;

	m_bot.m_map.drawTextScreen(sc2::Point2D(0.1f, 0.6f), ss.str());
}

void ScoutManager::moveScouts()
{
	auto workerScout = m_scoutUnit;
	if (!workerScout || workerScout->health <= 0) { return; }

	float scoutHP = workerScout->health + workerScout->shield;

	const BaseLocation * enemyBaseLocation = m_bot.m_baseLocationManager.getPlayerStartingBaseLocation(Players::Enemy); // get the enemy base location, if we have one

	int scoutDistanceThreshold = 20;

	if (enemyBaseLocation) // if we know where the enemy region is and where our scout is
	{
		int scoutDistanceToEnemy = m_bot.m_map.getGroundDistance(workerScout->pos, enemyBaseLocation->getPosition());
		bool scoutInRangeOfenemy = enemyBaseLocation->containsPosition(workerScout->pos);

		// we only care if the scout is under attack within the enemy region
		// this ignores if their scout worker attacks it on the way to their base
		if (scoutHP < m_previousScoutHP) { m_scoutUnderAttack = true; }

		if (scoutHP == m_previousScoutHP && !enemyWorkerInRadiusOf(workerScout->pos))
		{
			m_scoutUnderAttack = false;
		}

		if (scoutInRangeOfenemy) // if the scout is in the enemy region
		{
			const sc2::Unit * closestEnemyWorkerUnit = closestEnemyWorkerTo(workerScout->pos); // get the closest enemy worker

			if (!m_scoutUnderAttack) // if the worker scout is not under attack
			{
				// if there is a worker nearby, harass it
				if ( true && closestEnemyWorkerUnit && (Util::Dist(workerScout->pos, closestEnemyWorkerUnit->pos) < 12)) //m_bot.Config().ScoutHarassEnemy
				{
					m_scoutStatus = "Harass enemy worker";
					Micro::SmartAttackUnit(m_scoutUnit, closestEnemyWorkerUnit, m_bot);
				}
				else // otherwise keep moving to the enemy base location
				{
					m_scoutStatus = "Moving to enemy base location";
					Micro::SmartMove(m_scoutUnit, enemyBaseLocation->getPosition(), m_bot);
				}
			}
			else // if the worker scout is under attack
			{
				m_scoutStatus = "Under attack inside, fleeing";
				Micro::SmartMove(m_scoutUnit, getFleePosition(), m_bot);
			}
		}
		else if (m_scoutUnderAttack) // if the scout is not in the enemy region
		{
			m_scoutStatus = "Under attack outside, fleeing";

			Micro::SmartMove(m_scoutUnit, getFleePosition(), m_bot);
		}
		else
		{
			m_scoutStatus = "Enemy region known, going there";

			Micro::SmartMove(m_scoutUnit, enemyBaseLocation->getPosition(), m_bot); // move to the enemy region
		}

	}

	// for each start location in the level
	if (!enemyBaseLocation)
	{
		m_scoutStatus = "Enemy base unknown, exploring";

		for (const BaseLocation * startLocation : m_bot.m_baseLocationManager.getStartingBaseLocations())
		{
			// if we haven't explored it yet then scout it out
			// TODO: this is where we could change the order of the base scouting, since right now it's iterator order
			if (!m_bot.m_map.isExplored(startLocation->getPosition()))
			{
				Micro::SmartMove(m_scoutUnit, startLocation->getPosition(), m_bot);
				return;
			}
		}
	}

	m_previousScoutHP = scoutHP;
}

const sc2::Unit * ScoutManager::closestEnemyWorkerTo(const sc2::Point2D & pos) const
{
	if (!m_scoutUnit) { return nullptr; }

	UnitTag enemyWorkerTag = 0;
	float minDist = std::numeric_limits<float>::max();

	// for each enemy worker
	for (auto & unit : m_bot.m_unitInfoManager.getUnits(Players::Enemy))
	{
		if (Util::IsWorker(unit))
		{
			float dist = Util::Dist(unit->pos, m_scoutUnit->pos);

			if (dist < minDist)
			{
				minDist = dist;
				enemyWorkerTag = unit->tag;
			}
		}
	}

	return m_bot.Observation()->GetUnit(enemyWorkerTag);
}
bool ScoutManager::enemyWorkerInRadiusOf(const sc2::Point2D & pos) const
{
	for (auto & unit : m_bot.m_unitInfoManager.getUnits(Players::Enemy))
	{
		if (Util::IsWorker(unit) && Util::Dist(unit->pos, pos) < 10)
		{
			return true;
		}
	}

	return false;
}

sc2::Point2D ScoutManager::getFleePosition() const
{
	// TODO: make this follow the perimeter of the enemy base again, but for now just use home base as flee direction
	return m_bot.Observation()->GetStartLocation();
}