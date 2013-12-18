#include "Army.h"


Army::Army():m_techLvl(0),m_numInf(3), m_numCav(2), m_numArt(1),m_morale(1.0)
{
	{	
		for(int i = 0; i < 7; i++)
		{
			if(i < m_numInf)
				s[i].setType(INFANTRY);
			else if(i-m_numInf < m_numCav)
				s[i].setType(CAVALIER);
			else if(i-(m_numInf+m_numCav) < m_numArt)
				s[i].setType(ARTILLERY);
			s[i].setCombatVals(s[i].m_type);
		}
	}
	ProvID = 0;
}

void Army::setNation(Nation* n)
{
	this->n = n;
}

void Army::moveTo(int prov)
{
	ProvID = prov;
}