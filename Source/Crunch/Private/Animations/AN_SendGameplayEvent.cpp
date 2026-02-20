// Fill out your copyright notice in the Description page of Project Settings.


#include "Animations/AN_SendGameplayEvent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "GameplayTagsManager.h"

void UAN_SendGameplayEvent::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (!MeshComp->GetOwner())
		return;

	// SkeletalMesh를 소유한 Actor가 AbilitySystemComponent를 가지고 있는지 확인 후,
	UAbilitySystemComponent* OwnerASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(MeshComp->GetOwner());
	if (!OwnerASC)
		return;
	// AbilitySystemComponent를 가지고 있다면 해당 Actor에게 EventTag와 GameplayEvent를 보냄. EventTag에는 다음 콤보 공격을 위한 태그가 들어 있음. 
	// ex) 만약 콤보 공격 애니메이션이 Combo1  애니메이션 재생 중이라면 EventTag에는 Ability.Combo.Change.Combo2 Tag가 들어가 있음.
	// 즉. 콤보 1 애니메이션 도중에 Ability.Combo.Change.Combo2 Tag를 붙여서  콤보 2 공격이 가능하게 만들어줌. 
	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(MeshComp->GetOwner(), EventTag, FGameplayEventData());
}

FString UAN_SendGameplayEvent::GetNotifyName_Implementation() const
{
	if (EventTag.IsValid())
	{
		TArray<FName> TagNames;
		UGameplayTagsManager::Get().SplitGameplayTagFName(EventTag, TagNames);
		return TagNames.Last().ToString();
	}

	return "None";
}
