// Copyright Wenhi

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/AuraBeamSpell.h"
#include "AuraElectrocute.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UAuraElectrocute : public UAuraBeamSpell
{
	GENERATED_BODY()
	virtual FString GetDescription(int32 Level) override;
	virtual FString GetNextLevelDescription(int32 Level) override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Meta = (AllowPrivateAccess = "true"))
	int32 MaxNumAttackTargets = 5;
};
