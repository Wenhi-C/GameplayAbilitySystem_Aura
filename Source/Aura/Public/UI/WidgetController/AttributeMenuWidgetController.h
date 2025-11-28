// Copyright Wenhi

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetController/AuraWidgetController.h"
#include "AttributeMenuWidgetController.generated.h"

struct FGameplayAttribute;
struct FGameplayAttributeData;
struct FGameplayTag;
class UAttributeInfo;


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAttributeInfoSignature, const FAuraAttributeInfo&, Info);
/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class AURA_API UAttributeMenuWidgetController : public UAuraWidgetController
{
	GENERATED_BODY()

public:
	virtual void BroadcastInitialValues() override;
	virtual void BindCallbacksToDependencies() override;

	UPROPERTY(BlueprintAssignable, Category = "GAS|Attributes")
	FAttributeInfoSignature AttributeInfoDelegate;

	UPROPERTY(BlueprintAssignable, Category="GAS|AttributePoints")
	FOnPlayerStatChangeSignature OnAttributePointsChangedDelegate;

	UPROPERTY(BlueprintAssignable, Category="GAS|SpellPoints")
	FOnPlayerStatChangeSignature OnSpellPointsChangedDelegate;

	UFUNCTION(BlueprintCallable, Category = "GAS|Attributes")
	void UpgradeAttribute(const FGameplayTag& AttributeTag);
protected:
	
	
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UAttributeInfo> AttributeInfo;

private:
	
	void BroadcastAttributeInfo(const FGameplayTag& AttributeTag, const FGameplayAttribute& Attribute) const;
};
