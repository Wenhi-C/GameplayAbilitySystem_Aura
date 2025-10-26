// Copyright Wenhi

#pragma once

#include "CoreMinimal.h"
#include "AuraCharacterBase.h"
#include "Interaction/EnemyInterface.h"
#include "UI/WidgetController/OverlayWidgetController.h"
#include "AuraEnemy.generated.h"

class UWidgetComponent;
class UAuraUserWidget;

/**
 * 
 */
UCLASS()
class AURA_API AAuraEnemy : public AAuraCharacterBase, public  IEnemyInterface
{
	GENERATED_BODY()
public:
	AAuraEnemy();
	
	// Enemy Interface
	virtual void HighlightActor() override;
	virtual void UnHighlightActor() override;
	// End Enemy Interface

	// Combat Interface
	virtual int32 GetPlayerLevel() override;
	// End Combat Interface
	
protected:
	virtual void BeginPlay() override;
	
	UPROPERTY(BlueprintAssignable, Category="GAS|Attribute")
	FOnAttributeChangeSignature OnHealthChange;

	UPROPERTY(BlueprintAssignable, Category="GAS|Attribute")
	FOnAttributeChangeSignature OnMaxHealthChange;
	

	virtual void InitAbilityActorInfo() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character Class Defaults")
	int32 Level = 1;

	UPROPERTY(VisibleAnywhere, Category = "Widget", BlueprintType)
	TObjectPtr<UWidgetComponent> HealthBarWidget;
	
};
