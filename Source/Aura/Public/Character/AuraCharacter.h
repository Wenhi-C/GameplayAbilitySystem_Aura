// Copyright Wenhi

#pragma once

#include "CoreMinimal.h"
#include "AuraCharacterBase.h"
#include "AuraCharacter.generated.h"
class AAuraPlayerState;

/**
 * 需要在蓝图中设置：
 *	Weapon的资产
 *	WeaponTipSocketName
 *	StartupAbilities
 */
UCLASS()
class AURA_API AAuraCharacter : public AAuraCharacterBase
{
	GENERATED_BODY()
public:
	AAuraCharacter();
	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_PlayerState() override;

	// Combat Interface
	virtual int32 GetPlayerLevel() override;
	// End Combat Interface

private:
	virtual  void InitAbilityActorInfo() override;
};
