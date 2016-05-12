#include "PuzzleActivity.h"
#include "PuzzleDataMgr.h"



CPuzzleActivity::CPuzzleActivity()
{

}

CPuzzleActivity::~CPuzzleActivity()
{

}

void CPuzzleActivity::Init(time_t tBeginTime, time_t tEndTime)
{
	CTimeActivity::Init( tBeginTime, tEndTime );
}

void CPuzzleActivity::OnActivityBegin()
{
	CPuzzleDataMgr::Instance().SetIsOpening( true );
}

void CPuzzleActivity::OnActivityEnd()
{
	CPuzzleDataMgr::Instance().SetIsOpening( false );
}


