// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask_FindPatrolPos.h"

#include "AIController.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"

EBTNodeResult::Type UBTTask_FindPatrolPos::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	// AI 컨트롤러를 상속받은 클래스의 인스턴스를 가져와 빙의를 하고 있는 Pawn이 있는지 유효성 확인
	APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (nullptr == ControllingPawn)
	{
		return EBTNodeResult::Failed;
	}

	// 네비게이션 시스템 얻기
	UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetNavigationSystem(ControllingPawn->GetWorld()); //현재 폰이 위치한 월드를 가져옴
	if (nullptr == NavSystem)
	{
		return EBTNodeResult::Failed;
	}

	// 블랙보드 가져오기
	FVector Origin = OwnerComp.GetBlackboardComponent()->GetValueAsVector(TEXT("HomePos"));
	//float PatrolRadius = AIPawn->GetAIPatrolRadius();
	FNavLocation NextPatrolPos;

	// 네비게이션 시스템에 블랙보드에 저장된 HomePos값, 정찰 반경, 찾은 값을 받아올 수 있도록 변수에 저장
	if (NavSystem->GetRandomPointInNavigableRadius(Origin, 500.0F, NextPatrolPos))
	{
		// 블랙 보드의 Value값 세팅 -> PatrolPos키에 찾은 값을 넣도록 세팅
		OwnerComp.GetBlackboardComponent()->SetValueAsVector(TEXT("PatrolPos"), NextPatrolPos.Location);	
		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;
}
