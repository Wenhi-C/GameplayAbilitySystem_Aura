// Copyright Wenhi

#pragma once

#include "CoreMinimal.h"
#include "AuraCharacterBase.h"
#include "Interaction/PlayerInterface.h"
#include "AuraCharacter.generated.h"
class AAuraPlayerState;

/**
 * 需要在蓝图中设置：
 *	Weapon的资产
 *	WeaponTipSocketName
 *	StartupAbilities
 */
UCLASS()
class AURA_API AAuraCharacter : public AAuraCharacterBase, public  IPlayerInterface
{
	GENERATED_BODY()
public:
	AAuraCharacter();
	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_PlayerState() override;

	// Combat Interface
	virtual int32 GetPlayerLevel_Implementation() override;

	// End Combat Interface

	// Player Interface
	virtual void AddToXP_Implementation(int32 InXP) override;
	virtual void LevelUp_Implementation() override;
	// End Player Interface

private:
	virtual  void InitAbilityActorInfo() override;
};
