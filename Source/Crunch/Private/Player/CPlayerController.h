// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "CPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class ACPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	// only called on the server
	void OnPossess(APawn* NewPawn) override;

	// only called on the client, also on the linstening server.
	void AcknowledgePossession(APawn* NewPawn) override;

	// 리슨서버에서는 OnPossess 함수와 AcknowledgePossession 함수 둘 다 호출됩니다. 따라서 리슨 서버에서 두 함수가 모두 실행되기 때문에, 중복 실행을 조심해야 합니다.
	// ** OnPossess** 에서는 서버에서만 일어나야 하는 일(예: 능력치 부여, 서버 DB 연동)을 작성하세요.
	// ** AcknowledgePossession** 에서는 로컬 플레이어의 화면에 보여야 하는 일(예: UI 위젯 생성, 로컬 카메라 애니메이션)을 작성하세요.


private:
	void SpawnGameplayWidget();

	UPROPERTY()
	class ACPlayerCharacter* CPlayerCharacter;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<class UGameplayWidget> GameplayWidgetClass;

	UPROPERTY()
	class UGameplayWidget* GameplayWidget;
};
