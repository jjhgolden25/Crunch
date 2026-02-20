// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "CAttributeSet.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
 	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
 	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
 	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
 	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)
/**
 * 
 */
UCLASS()
class UCAttributeSet : public UAttributeSet
{
	GENERATED_BODY()
	
public:
	ATTRIBUTE_ACCESSORS(UCAttributeSet, Health)
	ATTRIBUTE_ACCESSORS(UCAttributeSet, MaxHealth)
	ATTRIBUTE_ACCESSORS(UCAttributeSet, Mana)
	ATTRIBUTE_ACCESSORS(UCAttributeSet, MaxMana)
	virtual void GetLifetimeReplicatedProps(TArray< class FLifetimeProperty >& OutLifetimeProps) const override;

	/**
	 *	An "On Aggregator Change" type of event could go here, and that could be called when active gameplay effects are added or removed to an attribute aggregator.
	 *	It is difficult to give all the information in these cases though - aggregators can change for many reasons: being added, being removed, being modified, having a modifier change, immunity, stacking rules, etc.
	 */

	 /**
	  *	Called just before any modification happens to an attribute. This is lower level than PreAttributeModify/PostAttribute modify.
	  *	There is no additional context provided here since anything can trigger this. Executed effects, duration based effects, effects being removed, immunity being applied, stacking rules changing, etc.
	  *	This function is meant to enforce things like "Health = Clamp(Health, 0, MaxHealth)" and NOT things like "trigger this extra thing if damage is applied, etc".
	  *
	  *	NewValue is a mutable reference so you are able to clamp the newly applied value as well.
	  */
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;

	/**
	 *	Called just after a GameplayEffect is executed to modify the base value of an attribute. No more changes can be made.
	 *	Note this is only called during an 'execute'. E.g., a modification to the 'base value' of an attribute. It is not called during an application of a GameplayEffect, such as a 5 ssecond +10 movement speed buff.
	 */
	virtual void PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data) override;


private:
	UPROPERTY(ReplicatedUsing = OnRep_Health)
	FGameplayAttributeData Health;

	UPROPERTY(ReplicatedUsing = OnRep_MaxHealth)
	FGameplayAttributeData MaxHealth;

	UPROPERTY(ReplicatedUsing = OnRep_Mana)
	FGameplayAttributeData Mana;

	UPROPERTY(ReplicatedUsing = OnRep_MaxMana)
	FGameplayAttributeData MaxMana;
		
	// OnRep_Health와 같은 **OnRep 함수(RepNotify)**는 기본적으로 서버가 아닌 클라이언트에서 실행되는 것이 원칙입니다.

	// void OnRep_Health(const FGameplayAttributeData& OldValue); 에서 OldValue가 들어가는 이유
	// 클라이언트의 메모리에서 Health 변수는 서버로부터 받은 새로운 값으로 이미 덮어씌워진 상태에서 OnRep_Health 함수가 호출됩니다.
	// 단계 1: 서버가 "야, Health 이제 80이야"라고 패킷을 보냄.
    // 단계 2: 클라이언트 컴퓨터가 자기 메모리의 Health를 80으로 바꿈.
    // 단계 3 : 그 직후에 OnRep_Health(OldValue)를 실행함.이때 인자로 들어오는 OldValue는 바뀌기 전의 값(예 : 100)입니다.
	// 왜 OldValue(옛날 값)가 필요한가요 ?
	// 새로운 값은 이미 this->Health를 통해 언제든 접근할 수 있습니다.하지만 * *"얼마나 변했는가?" * *를 알기 위해서는 이전 값이 반드시 필요합니다.
	// (1) UI 연출 : 체력이 100에서 80으로 깎일 때, UI에서 빨간색 게이지가 스르륵 줄어드는 효과를 주려면 100이라는 시작점이 필요합니다.
	// (2) 로직 판단 : 만약 OldValue가 100이고 현재값이 0이라면 "아, 방금 막 죽었구나!"라는 판정을 내릴 수 있습니다.
	// (3) 델리게이트(Delegate) 호출 : 언리얼의 GAS 시스템은 내부적으로 "값이 바뀌었다"는 이벤트를 브로드캐스팅합니다.이때 "100에서 80으로 바뀌었다"는 정보를 통째로 넘겨주기 위해 OldValue를 사용하는 것입니다.
	UFUNCTION()
	void OnRep_Health(const FGameplayAttributeData& OldValue);
	UFUNCTION()
	void OnRep_MaxHealth(const FGameplayAttributeData& OldValue);
	UFUNCTION()
	void OnRep_Mana(const FGameplayAttributeData& OldValue);
	UFUNCTION()
	void OnRep_MaxMana(const FGameplayAttributeData& OldValue);
};
