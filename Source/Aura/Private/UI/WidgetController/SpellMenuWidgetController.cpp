// Copyright Wenhi


#include "UI/WidgetController/SpellMenuWidgetController.h"

#include "AuraGameplayTags.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAbilitySystemLibrary.h"
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
		[this](const FGameplayTag& AbilityTag, const FGameplayTag& StatusTag, int32 NewLevel)
		{
			if (AbilityTag.MatchesTagExact(SelectedAbilityTag))
			{
				SelectedAbilityStatusTag = StatusTag;
				ShouldEnableButtons();
				FString Description;
				FString NextLevelDescription;
				GetAuraAbilitySystemComponent()->GetDescriptionByAbilityTag(SelectedAbilityTag, Description, NextLevelDescription);
				SpellGlobeSelectedDelegate.Broadcast(bEnableSpendPoints, bEnableEquip, Description, NextLevelDescription);
			}
			if (AbilityInfo)
			{
				FAuraAbilityInfo Info = AbilityInfo->FindAbilityInfoForTag(AbilityTag);
				Info.StatusTag = StatusTag;
				AbilityInfoDelegate.Broadcast(Info);
			}
		});

	GetAuraAbilitySystemComponent()->AbilityEquippedDelegate.AddUObject(this, &USpellMenuWidgetController::OnAbilityEquipped);
	
	if (GetAuraPlayerState())
	{
		GetAuraPlayerState()->OnSpellPointsChangedDelegate.AddLambda(
			[this](int32 SpellPoints)
			{
				OnSpellPointsChangedDelegate.Broadcast(SpellPoints);
				ShouldEnableButtons();

				FString Description;
				FString NextLevelDescription;
				GetAuraAbilitySystemComponent()->GetDescriptionByAbilityTag(SelectedAbilityTag, Description, NextLevelDescription);
				SpellGlobeSelectedDelegate.Broadcast(bEnableSpendPoints, bEnableEquip, Description, NextLevelDescription);
			});
	}
	
}

void USpellMenuWidgetController::SpendPointButtonPressed()
{
	if (GetAuraAbilitySystemComponent())
		GetAuraAbilitySystemComponent()->ServerSpendSpellPoints(SelectedAbilityTag);
}


void USpellMenuWidgetController::GlobeDeselect()
{
	if (bWaitingForEquipSelection)
	{
		FGameplayTag SelectedAbilityType = AbilityInfo->FindAbilityInfoForTag(SelectedAbilityTag).AbilityType;
		StopWaitingForEquipSelectionDelegate.Broadcast(SelectedAbilityType);
		bWaitingForEquipSelection = false;
	}
	SelectedAbilityTag = FAuraGameplayTags::Get().Abilities_None;
	SelectedAbilityStatusTag = FAuraGameplayTags::Get().Abilities_Status_Locked;
	bEnableSpendPoints = false;
	bEnableEquip = false;
	SpellGlobeSelectedDelegate.Broadcast(bEnableSpendPoints, bEnableEquip, FString(), FString());
}

void USpellMenuWidgetController::EquipButtonPressed()
{
	const FAuraAbilityInfo& Info = AbilityInfo->FindAbilityInfoForTag(SelectedAbilityTag);
	WaitForEquipSelectionDelegate.Broadcast(Info.AbilityType);
	bWaitingForEquipSelection = true;

	const FGameplayTag SelectedStatus = GetAuraAbilitySystemComponent()->GetStatusFromAbilityTag(SelectedAbilityTag);
	if (SelectedStatus.MatchesTagExact(FAuraGameplayTags::Get().Abilities_Status_Equipped))
	{
		SelectedSlot = GetAuraAbilitySystemComponent()->GetSlotFromAbilityTag(SelectedAbilityTag);
	}
}

void USpellMenuWidgetController::SpellRowGlobePressed(const FGameplayTag& SlotTag, const FGameplayTag& AbilityType)
{
	if (!bWaitingForEquipSelection) return;
	// Check Selected Ability against the slot's ability type
	// don't equip on offensive spell in a passive slot and vice versa
	const FGameplayTag& SelectedAbilityType = AbilityInfo->FindAbilityInfoForTag(SelectedAbilityTag).AbilityType;
	if (!SelectedAbilityType.MatchesTagExact(AbilityType)) return;

	GetAuraAbilitySystemComponent()->ServerEquipAbility(SelectedAbilityTag, SlotTag);
	
}

void USpellMenuWidgetController::SpellGlobeSelected(const FGameplayTag& InSelectedAbilityTag)
{
	if (bWaitingForEquipSelection)
	{
		if (SelectedAbilityTag.IsValid())
		{
			FGameplayTag SelectedAbilityType = AbilityInfo->FindAbilityInfoForTag(SelectedAbilityTag).AbilityType;
			StopWaitingForEquipSelectionDelegate.Broadcast(SelectedAbilityType);
		}
	}
	bWaitingForEquipSelection = false;
	
	SelectedAbilityTag = InSelectedAbilityTag;
	const FAuraGameplayTags& GameplayTags = FAuraGameplayTags::Get();
	const bool bTagValid = SelectedAbilityTag.IsValid();
	const bool bTagNone = SelectedAbilityTag.MatchesTagExact(GameplayTags.Abilities_None);
	FGameplayAbilitySpec* AbilitySpec = GetAuraAbilitySystemComponent()->GetSpecFromAbilityTag(SelectedAbilityTag);
	const bool bSpecValid = AbilitySpec  != nullptr;
	
	if (!bTagValid || bTagNone || !bSpecValid)
	{
		SelectedAbilityStatusTag = GameplayTags.Abilities_Status_Locked;
	}
	else
	{
		SelectedAbilityStatusTag = GetAuraAbilitySystemComponent()->GetStatusFromSpec(*AbilitySpec);
	}

	ShouldEnableButtons();
	FString Description;
	FString NextLevelDescription;
	GetAuraAbilitySystemComponent()->GetDescriptionByAbilityTag(SelectedAbilityTag, Description, NextLevelDescription);
	SpellGlobeSelectedDelegate.Broadcast(bEnableSpendPoints, bEnableEquip, Description, NextLevelDescription);
}

void USpellMenuWidgetController::ShouldEnableButtons()
{
	bEnableEquip = false;
	bEnableSpendPoints = false;

	const int32 CurrentSpellPoints = GetAuraPlayerState()->GetSpellPoints();
	// Check Enable Equip
	if (SelectedAbilityStatusTag.MatchesTagExact(FAuraGameplayTags::Get().Abilities_Status_Locked))
	{
		bEnableEquip = false;
		bEnableSpendPoints = false;
	}
	else if (SelectedAbilityStatusTag.MatchesTagExact(FAuraGameplayTags::Get().Abilities_Status_Eligible))
	{
		bEnableEquip = false;
		bEnableSpendPoints = 1 <= CurrentSpellPoints;
	}
	else if (SelectedAbilityStatusTag.MatchesTagExact(FAuraGameplayTags::Get().Abilities_Status_Unlocked))
	{
		bEnableEquip = true;
		bEnableSpendPoints = 1 <= CurrentSpellPoints;
	}
	else if (SelectedAbilityStatusTag.MatchesTagExact(FAuraGameplayTags::Get().Abilities_Status_Equipped))
	{
		bEnableEquip = true;
		bEnableSpendPoints = 1 <= CurrentSpellPoints;
	}
}

void USpellMenuWidgetController::OnAbilityEquipped(const FGameplayTag& AbilityTag, const FGameplayTag& Status,
	const FGameplayTag& Slot, const FGameplayTag& PreviousSlot)
{
 	bWaitingForEquipSelection = false;

	const FAuraGameplayTags& GameplayTags = FAuraGameplayTags::Get();
	FAuraAbilityInfo LastSlotInfo;
	LastSlotInfo.StatusTag = GameplayTags.Abilities_Status_Unlocked;
	LastSlotInfo.InputTag = PreviousSlot;
	LastSlotInfo.AbilityTag = GameplayTags.Abilities_None;
	// Broadcast empty info if PreviousSlot is a valid slot. Only if equipping an already-equipped spell
	AbilityInfoDelegate.Broadcast(LastSlotInfo);

	FAuraAbilityInfo Info = AbilityInfo->FindAbilityInfoForTag(AbilityTag);
	Info.StatusTag = Status;
	Info.InputTag = Slot;
	AbilityInfoDelegate.Broadcast(Info);
	StopWaitingForEquipSelectionDelegate.Broadcast(AbilityInfo->FindAbilityInfoForTag(AbilityTag).AbilityType);
	SpellGlobeReassignedDelegate.Broadcast(AbilityTag);
	GlobeDeselect();
}
