// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/CAttributeSet.h"
#include "Net/UnrealNetwork.h"

// 클라이언트에서만 실행됨.
void UCAttributeSet::OnRep_Health(const FGameplayAttributeData& OldValue)
{
	//"UCAttributeSet의 Health 값이 OldValue에서 새로운 값으로 바뀌었으니, 관련된 UI나 로직들에게 이 사실을 전파하라"는 뜻입니다.
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCAttributeSet,Health, OldValue);
}

void UCAttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& OldValue)
{	
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCAttributeSet, MaxHealth, OldValue);
}

void UCAttributeSet::OnRep_Mana(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCAttributeSet, Mana, OldValue);
}

void UCAttributeSet::OnRep_MaxMana(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCAttributeSet, MaxMana, OldValue);
}



void UCAttributeSet::GetLifetimeReplicatedProps(TArray< class FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME_CONDITION_NOTIFY(UCAttributeSet, Health, COND_None, REPNOTIFY_Always);   //  REPNOTIFY_Always : 서버에서 값이 바뀌었을 때, 본인이 서버라 할지라도(리슨 서버일 경우) 이 함수를 강제로 실행하라는 설정. 따라서 리슨서버일 경우에도 실행됨.
	DOREPLIFETIME_CONDITION_NOTIFY(UCAttributeSet, MaxHealth, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UCAttributeSet, Mana, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UCAttributeSet, MaxMana, COND_None, REPNOTIFY_Always);
}