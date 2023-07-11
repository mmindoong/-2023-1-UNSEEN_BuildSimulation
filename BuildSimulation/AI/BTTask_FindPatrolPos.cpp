// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask_FindPatrolPos.h"

#include "AIController.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"

EBTNodeResult::Type UBTTask_FindPatrolPos::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	// AI ��Ʈ�ѷ��� ��ӹ��� Ŭ������ �ν��Ͻ��� ������ ���Ǹ� �ϰ� �ִ� Pawn�� �ִ��� ��ȿ�� Ȯ��
	APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (nullptr == ControllingPawn)
	{
		return EBTNodeResult::Failed;
	}

	// �׺���̼� �ý��� ���
	UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetNavigationSystem(ControllingPawn->GetWorld()); //���� ���� ��ġ�� ���带 ������
	if (nullptr == NavSystem)
	{
		return EBTNodeResult::Failed;
	}

	// ������ ��������
	FVector Origin = OwnerComp.GetBlackboardComponent()->GetValueAsVector(TEXT("HomePos"));
	//float PatrolRadius = AIPawn->GetAIPatrolRadius();
	FNavLocation NextPatrolPos;

	// �׺���̼� �ý��ۿ� �����忡 ����� HomePos��, ���� �ݰ�, ã�� ���� �޾ƿ� �� �ֵ��� ������ ����
	if (NavSystem->GetRandomPointInNavigableRadius(Origin, 500.0F, NextPatrolPos))
	{
		// �� ������ Value�� ���� -> PatrolPosŰ�� ã�� ���� �ֵ��� ����
		OwnerComp.GetBlackboardComponent()->SetValueAsVector(TEXT("PatrolPos"), NextPatrolPos.Location);	
		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;
}
