#include "Arena.h"

//��������ʱʱ�䴥������;
void Arena::Tick()
{

}

bool Arena::UserIntoTheArena()
{
	return true;
}

bool Arena::UserLeaveTheArena()
{
	return true;
}

void Arena::OneDeskEnd()
{
	if (++m_nEndDeskNum == m_nDeskSum)
	{
		ArenaEnd();
	}
}

void Arena::ArenaStart()
{
}

void Arena::ArenaEnd()
{
}
