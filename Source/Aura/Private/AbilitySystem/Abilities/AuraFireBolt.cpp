// Copyright Wenhi


#include "AbilitySystem/Abilities/AuraFireBolt.h"

#include "AuraGameplayTags.h"
#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "Actor/AuraProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Interaction/CombatInterface.h"
#include "Kismet/KismetSystemLibrary.h"

FString UAuraFireBolt::GetDescription(int32 Level)
{
	const int32 ScaledDamage = Damage.GetValueAtLevel(Level);
	const float ManaCost = GetManaCost(Level);
	const float Cooldown = GetCooldown(Level);
	if (Level == 1)
	{
		return FString::Printf(TEXT(
			"<Title>火焰箭</>\n"
			
			"<Small>等级: </><Level>%d</>\n"
			"<Small>法力消耗: </><ManaCost>%.1f</>\n"
			"<Small>冷却时间: </><Cooldown>%.1f</>\n"
			
			"<Default>发射1个火焰弹，造成</><Damage> %d </><Default>点火焰伤害，有概率使目标着火</>"
			),
			Level,
			ManaCost,
			Cooldown,
			ScaledDamage);
	}
	else
	{
		return FString::Printf(TEXT(
			"<Title>火焰箭</>\n"
			
			"<Small>等级: </><Level>%d</>\n"
			"<Small>法力消耗: </><ManaCost>%.1f</>\n"
			"<Small>冷却时间: </><Cooldown>%.1f</>\n"
			
			"<Default>发射%d个火焰弹，造成</><Damage> %d </><Default>点火焰伤害，有概率使目标着火</>"
			),
			Level,
			ManaCost,
			Cooldown,
			FMath::Min(Level, NumProjectiles),
			ScaledDamage);
		
	}
}

FString UAuraFireBolt::GetNextLevelDescription(int32 Level)
{
	const int32 ScaledDamage = Damage.GetValueAtLevel(Level);
	const float ManaCost = GetManaCost(Level);
	const float Cooldown = GetCooldown(Level);
	return FString::Printf(TEXT(
			"<Title>下一级</>\n"
			
			"<Small>等级: </><Level>%d</>\n"
			"<Small>法力消耗: </><ManaCost>%.1f</>\n"
			"<Small>冷却时间: </><Cooldown>%.1f</>\n"
			
			"<Default>发射%d个火焰弹，造成</><Damage> %d </><Default>点火焰伤害，有概率使目标着火</>"
			),
			Level,
			ManaCost,
			Cooldown,
			FMath::Min(Level, NumProjectiles),
			ScaledDamage);
}

void UAuraFireBolt::SpawnProjectiles(const FVector& ProjectileTargetLocation, const FGameplayTag& SocketTag, bool bOverridePitch, float PitchOverride,
	AActor* HomingTarget)
{
	const bool bIsServer = GetAvatarActorFromActorInfo()->HasAuthority();
	if (!bIsServer) return;

	const int32 CurrentNumProjectiles = FMath::Min(NumProjectiles, GetAbilityLevel());
	const FVector SocketLocation = ICombatInterface::Execute_GetCombatSocketLocation(GetAvatarActorFromActorInfo(), SocketTag);
	FRotator Rotation = (ProjectileTargetLocation - SocketLocation).Rotation();
	if (bOverridePitch)
	{
		Rotation.Pitch = PitchOverride;
	}
	
	const FVector Forward = Rotation.Vector();

	TArray<FRotator> Rotators = UAuraAbilitySystemLibrary::EvenlySpacedRotators(Forward, FVector::UpVector, ProjectileSpread, CurrentNumProjectiles);

	for (const FRotator& Rot : Rotators)
	{
		FTransform SpawnTransform;
		SpawnTransform.SetLocation(SocketLocation);
		SpawnTransform.SetRotation(Rot.Quaternion());
		
		AAuraProjectile* Projectile = GetWorld()->SpawnActorDeferred<AAuraProjectile>(
			ProjectileClass,
			SpawnTransform,
			GetOwningActorFromActorInfo(),
			Cast<APawn>(GetOwningActorFromActorInfo()),
			ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
		
		Projectile->DamageEffectParams = MakeDamageEffectParamsFromClassDefaults();
		
		
		if (HomingTarget && HomingTarget->Implements<UCombatInterface>())
		{
			
			Projectile->ProjectileMovement->HomingTargetComponent = HomingTarget->GetRootComponent();
		}
		else
		{
			Projectile->HomingTargetSceneComponent = NewObject<USceneComponent>(USceneComponent::StaticClass());
			Projectile->HomingTargetSceneComponent->SetWorldLocation(ProjectileTargetLocation);
			Projectile->ProjectileMovement->HomingTargetComponent = Projectile->HomingTargetSceneComponent;
		}
		Projectile->ProjectileMovement->HomingAccelerationMagnitude = HomingAcceleration;
		Projectile->ProjectileMovement->bIsHomingProjectile = bLaunchHomingProjectile;

		Projectile->FinishSpawning(SpawnTransform);

	}
	
}
