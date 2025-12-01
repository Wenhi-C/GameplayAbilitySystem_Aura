// Copyright Wenhi


#include "UI/WidgetController/SpellMenuWidgetController.h"

#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/Data/AbilityInfo.h"
#include "Player/AuraPlayerState.h"

void USpellMenuWidgetController::BroadcastInitialValues()
{
	BroadcastAbilityInfo();
	if (GetAuraPlayerState())
	{
		OnSpellPointsChangedDelegate.Broadcast(GetAuraPlayerState()->GetSpellPoints());
	}
}

void USpellMenuWidgetController::BindCallbacksToDependencies()
{
	GetAuraAbilitySystemComponent()->AbilityStatusChangedDelegate.AddLambda(
		[this](const FGameplayTag& AbilityTag, const FGameplayTag& StatusTag)
	{
		if (AbilityInfo)
		{
			FAuraAbilityInfo Info = AbilityInfo->FindAbilityInfoForTag(AbilityTag);
			Info.StatusTag = StatusTag;
			AbilityInfoDelegate.Broadcast(Info);
		}
	});

	if (GetAuraPlayerState())
	{
		GetAuraPlayerState()->OnSpellPointsChangedDelegate.AddLambda(
			[this](int32 SpellPoints)
			{
				OnSpellPointsChangedDelegate.Broadcast(SpellPoints);
			});
	}
	
}
