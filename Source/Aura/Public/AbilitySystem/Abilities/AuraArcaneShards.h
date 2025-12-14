// Copyright Wenhi

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/AuraDamageGameplayAbility.h"
#include "AuraArcaneShards.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UAuraArcaneShards : public UAuraDamageGameplayAbility
{
	GENERATED_BODY()
	virtual FString GetDescription(int32 Level) override;
	virtual FString GetNextLevelDescription(int32 Level) override;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Meta = (AllowPrivateAccess = "true"))
	int32 MaxNumShards = 11;
};
