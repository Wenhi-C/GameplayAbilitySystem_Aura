// Copyright Wenhi

#pragma once

#include "CoreMinimal.h"
#include "AuraGameplayTags.h"
#include "GameplayTagContainer.h"
#include "UI/WidgetController/AuraWidgetController.h"
#include "SpellMenuWidgetController.generated.h"

struct FGameplayTag;
/**
 * 
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FSpellGlobeSelectedSignature, bool, bSpendPointButtonEnabled, bool, bEquipButtonEnabled, FString, DescriptionString, FString, NextLevelDescription);

UCLASS(BlueprintType, Blueprintable)
class AURA_API USpellMenuWidgetController : public UAuraWidgetController
{
	GENERATED_BODY()

	UPROPERTY(BlueprintAssignable, Category="GAS|Spell Points")
	FOnPlayerStatChangeSignature OnSpellPointsChangedDelegate;
public:
	virtual void BroadcastInitialValues() override;
	virtual void BindCallbacksToDependencies() override;

	UFUNCTION(BlueprintCallable)
	void SpendPointButtonPressed();

	UPROPERTY(BlueprintAssignable, Category="GAS|Spell Points")
	FSpellGlobeSelectedSignature SpellGlobeSelectedDelegate;
	

protected:
	UFUNCTION(BlueprintCallable, Category="GAS")
	void SpellGlobeSelected();

	void ShouldEnableButtons();
	
	bool bEnableSpendPoints = false;
	
	bool bEnableEquip = false;

	UPROPERTY(BlueprintReadWrite, Category="GAS")
	FGameplayTag SelectedAbilityTag = FAuraGameplayTags::Get().Abilities_None;

	UPROPERTY(BlueprintReadOnly, Category="GAS")
	FGameplayTag SelectedAbilityStatusTag = FAuraGameplayTags::Get().Abilities_Status_Locked;
	
};
