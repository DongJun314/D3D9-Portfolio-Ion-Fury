#include "pch.h"
#include "IdleMoveTask.h"

#include "TimerManager.h"
#include "BlackBoardComponent.h"
#include "BehaviorTreeComponent.h"
#include "Actor.h"
#include "Monster.h"
using namespace std;

void CIdleMoveTask::Initialize()
{
	CBTTaskNode::Initialize();
}

void CIdleMoveTask::Release()
{
	CBTTaskNode::Release();
}

EBTNodeResult CIdleMoveTask::UpdateTask()
{
	// 블랙보드 컴포넌트를 불러옵니다.
	shared_ptr<CBlackBoardComponent> spBlackBoardComponent = m_wpBehaviorTreeComponent.lock()->GetBlackBoardComponent();
	FVector3 m_vDst = spBlackBoardComponent->GetValueAsFVector3(m_wstrKey);
	m_vDst.y -= m_vDst.y;

	// 비헤이비어 트리를 소유한 주인 액터를 불러옵니다.
	shared_ptr<CActor> spActor = m_wpBehaviorTreeComponent.lock()->GetOwnerActor().lock();

	// 출발지와 도착지 사이의 거리와 방향을 구합니다.
	FVector3 vSrc = spActor->GetActorWorldPosition();
	vSrc.y -= vSrc.y;

	FVector3 vLength = m_vDst - vSrc;
	FVector3 vDir = m_vDst - vSrc;

	// 출발지와 도착지 사이의 거리가 반지름보다 작은 경우
	if (D3DXVec3Length(&vLength) <= m_fRadius)
	{
		spBlackBoardComponent->SetValueAsFVector3(L"Start", spActor->GetActorWorldPosition());

		// 몬스터의 이동이 끝나면 상태를 IDLE로 업데이트합니다.
		if (spActor->GetActorLayer().find(L"Monster") != spActor->GetActorLayer().end())
			dynamic_pointer_cast<CMonster>(spActor)->SetMonsterState((int32)EMonsterState::IDLE);

		return EBTNodeResult::SUCCEEDED;
	}

	// 액터를 이동합니다.
	float fDeltaSeconds = CTimerManager::GetInstance()->GetDeltaSeconds(L"TimerInner");
	vDir.y -= vDir.y;
	D3DXVec3Normalize(&vDir, &vDir);
	spBlackBoardComponent->SetValueAsFVector3(L"MonsterDirection", vDir); // 몬스터 방향을 얻기위해 추가 - 박동연 -

	if (dynamic_pointer_cast<CMonster>(spActor)->GetIsTimeSlipped())
	{
		spActor->SetActorPosition(spActor->GetActorPosition() + vDir * 10.0f * fDeltaSeconds / 10.0f);
	}
	else
	{
		spActor->SetActorPosition(spActor->GetActorPosition() + vDir * 10.0f * fDeltaSeconds);
	}

	return EBTNodeResult::IN_PROGRESS;
}
