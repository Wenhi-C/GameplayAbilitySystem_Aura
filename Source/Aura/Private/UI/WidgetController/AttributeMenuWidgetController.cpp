// Copyright Wenhi


#include "UI/WidgetController/AttributeMenuWidgetController.h"
#include "AbilitySystem/Data/AttributeInfo.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "AuraGameplayTags.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "Player/AuraPlayerState.h"

void UAttributeMenuWidgetController::BroadcastInitialValues()
{
	UAuraAttributeSet* AS = Cast<UAuraAttributeSet>(AttributeSet);
	
	check(AttributeInfo);

	for (auto& Pair :AS->TagsToAttributes)
	{
		BroadcastAttributeInfo(Pair.Key, Pair.Value());
	}


	if (AAuraPlayerState* AuraPlayerState = Cast<AAuraPlayerState>(PlayerState))
	{
		OnAttributePointsChangedDelegate.Broadcast(AuraPlayerState->GetAttributePoints());
		OnSpellPointsChangedDelegate.Broadcast(AuraPlayerState->GetSpellPoints());
	}
}

void UAttributeMenuWidgetController::BindCallbacksToDependencies()
{
	UAuraAttributeSet* AS = Cast<UAuraAttributeSet>(AttributeSet);
	for (auto Pair : AS->TagsToAttributes)
	{
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(Pair.Value()).AddLambda(
			[this, Pair](const FOnAttributeChangeData& Data)
			{
				BroadcastAttributeInfo(Pair.Key, Pair.Value());
			}
			);
	}
	if (AAuraPlayerState* AuraPlayerState = Cast<AAuraPlayerState>(PlayerState))
	{
		AuraPlayerState->OnAttributePointsChangedDelegate.AddLambda(
			[this](int32 NewAttributePoints)
			{
				OnAttributePointsChangedDelegate.Broadcast(NewAttributePoints);
			});

		AuraPlayerState->OnSpellPointsChangedDelegate.AddLambda(
			[this](int32 NewSpellPoints)
			{
				OnSpellPointsChangedDelegate.Broadcast(NewSpellPoints);
			});
	}
	
}

void UAttributeMenuWidgetController::UpgradeAttribute(const FGameplayTag& AttributeTag)

{
	if (UAuraAbilitySystemComponent* AuraASC = Cast<UAuraAbilitySystemComponent>(AbilitySystemComponent))
	{
		AuraASC->UpgradeAttribute(AttributeTag);
	}
}

void UAttributeMenuWidgetController::BroadcastAttributeInfo(const FGameplayTag& AttributeTag,
                                                            const FGameplayAttribute& Attribute) const
{
	FAuraAttributeInfo Info = AttributeInfo->FindAttributeInfoForTag(AttributeTag);
	Info.AttributeValue = Attribute.GetNumericValue(AttributeSet);
	AttributeInfoDelegate.Broadcast(Info);
}
