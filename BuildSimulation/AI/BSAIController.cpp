// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BSAIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"

ABSAIController::ABSAIController()
{
	static ConstructorHelpers::FObjectFinder<UBlackboardData> BBAssetRef(TEXT("/Game/Blueprints/Core/AI/BB_BSUnit.BB_BSUnit"));
	if (nullptr != BBAssetRef.Object)
	{
		BBAsset = BBAssetRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UBehaviorTree> BTAssetRef(TEXT("/Script/AIModule.BehaviorTree'/Game/Blueprints/Core/AI/BT_BSUnit.BT_BSUnit'"));
	if (nullptr != BTAssetRef.Object)
	{
		BTAsset = BTAssetRef.Object;
	}
}

void ABSAIController::RunAI()
{
	// Blackboard 애셋에 대한 컴포넌트값 얻어오기
	UBlackboardComponent* BlackboardPtr = Blackboard.Get();
	if (UseBlackboard(BBAsset, BlackboardPtr))
	{
		// 블랙보드의 HomePos값을 시작할 때 설정하고 비헤이비어트리를 실행한다.
		Blackboard->SetValueAsVector(TEXT("HomePos"), GetPawn()->GetActorLocation());
		
		bool RunResult = RunBehaviorTree(BTAsset);
		ensure(RunResult);
	}	
}

void ABSAIController::StopAI()
{
	UBehaviorTreeComponent* BTComponent = Cast<UBehaviorTreeComponent>(BrainComponent);
	if (BTComponent)
	{
		BTComponent->StopTree();
	}
}

void ABSAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	RunAI();
}
