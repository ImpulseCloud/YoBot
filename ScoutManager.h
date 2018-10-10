#pragma once

#include "Common.h"

class YoAgent;

class ScoutManager
{
	YoAgent &   m_bot;

	const sc2::Unit * m_scoutUnit;
	std::string     m_scoutStatus;
	int             m_numScouts;
	bool            m_scoutUnderAttack;
	float           m_previousScoutHP;

	bool            enemyWorkerInRadiusOf(const sc2::Point2D & pos) const;
	sc2::Point2D    getFleePosition() const;
	const sc2::Unit * closestEnemyWorkerTo(const sc2::Point2D & pos) const;
	void            moveScouts();
	void            drawScoutInformation();


public:

	ScoutManager(YoAgent & bot);

	void onStart();
	void onFrame();
	void setWorkerScout(const sc2::Unit * unit);
};