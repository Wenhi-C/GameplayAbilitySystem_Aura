// Copyright Wenhi

#pragma once

#include "CoreMinimal.h"
#include "AuraCharacterBase.h"
#include "Interaction/PlayerInterface.h"
#include "AuraCharacter.generated.h"
class AAuraPlayerState;
class UNiagaraComponent; 
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
	virtual int32 GetXP_Implementation() const override;
	virtual void AddToXP_Implementation(int32 InXP) override;
	virtual void LevelUp_Implementation() override;
	virtual int32 FindLevelForXP_Implementation(int32 InXP) const override;
	virtual int32 GetAttributePointsReward_Implementation(int32 Level) const override;
	virtual int32 GetSpellPointsReward_Implementation(int32 Level) const override;
	virtual void AddToPlayerLevel_Implementation(int32 InPlayerLevel) override;
	virtual int32 GetAttributePoints_Implementation() const override;
	virtual void AddToAttributePoints_Implementation(int32 InAttributePoints) override;
	virtual int32 GetSpellPoints_Implementation() const override;
	virtual void AddToSpellPoints_Implementation(int32 InSpellPoints) override;
	// End Player Interface

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UNiagaraComponent> LevelUpNiagaraComponent;

	virtual void OnRep_Stunned();
	virtual void OnRep_Burned();
private:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UCameraComponent> TopDownCameraComponent;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class USpringArmComponent> CameraBoom;
	
	virtual  void InitAbilityActorInfo() override;

	UFUNCTION(NetMulticast, Reliable)
	void MulticastLevelUpParticles() const;
};
