// Copyright Wenhi


#include "AbilitySystem/AuraAbilitySystemLibrary.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "AuraAbilityTypes.h"
#include "AuraGameplayTags.h"
#include "Engine/OverlapResult.h"
#include "Game/AuraGameModeBase.h"
#include "GeometryCollection/GeometryCollectionParticlesData.h"
#include "Interaction/CombatInterface.h"
#include "Kismet/GameplayStatics.h"
#include "Player/AuraPlayerState.h"
#include "UI/HUD/AuraHUD.h"
#include "UI/WidgetController/AuraWidgetController.h"

bool UAuraAbilitySystemLibrary::MakeWidgetControllerParams(const UObject* WorldContextObject,
	FWidgetControllerParams& OutWCParams, AAuraHUD*& OutAuraHUD)
{
	if (APlayerController* PC = UGameplayStatics::GetPlayerController(WorldContextObject, 0))
	{
		OutAuraHUD = Cast<AAuraHUD>(PC->GetHUD());
		if (OutAuraHUD)
		{
			AAuraPlayerState* PS = PC->GetPlayerState<AAuraPlayerState>();
			UAbilitySystemComponent* ASC = PS->GetAbilitySystemComponent();
			UAttributeSet* AS = PS->GetAttributeSet();
			OutWCParams.AttributeSet = AS;
			OutWCParams.PlayerController = PC;
			OutWCParams.AbilitySystemComponent = ASC;
			OutWCParams.PlayerState = PS;
			return true;
		}
	}
	return false;
}

UOverlayWidgetController* UAuraAbilitySystemLibrary::GetOverlayWidgetController(const UObject* WorldContextObject)
{
	FWidgetControllerParams WCParams;
	AAuraHUD* AuraHUD;
	if (MakeWidgetControllerParams(WorldContextObject, WCParams, AuraHUD))
	{
		return AuraHUD->GetOverlayWidgetController(WCParams);
	}
	return nullptr;
}

UAttributeMenuWidgetController* UAuraAbilitySystemLibrary::GetAttributeMenuWidgetController(
	const UObject* WorldContextObject)
{
	FWidgetControllerParams WCParams;
	AAuraHUD* AuraHUD;
	if (MakeWidgetControllerParams(WorldContextObject, WCParams, AuraHUD))
	{
		return AuraHUD->GetAttributeMenuWidgetController(WCParams);
	}
	return nullptr;
}

USpellMenuWidgetController* UAuraAbilitySystemLibrary::GetSpellMenuWidgetController(const UObject* WorldContextObject)
{
	FWidgetControllerParams WCParams;
	AAuraHUD* AuraHUD;
	if (MakeWidgetControllerParams(WorldContextObject, WCParams, AuraHUD))
	{
		return AuraHUD->GetSpellMenuWidgetController(WCParams);
	}
	return nullptr;
}

void UAuraAbilitySystemLibrary::InitializeDefaultAttributes(const UObject* WorldContextObject, ECharacterClass CharacterClass, float Level, UAbilitySystemComponent* ASC)
{
	UCharacterClassInfo* CharacterClassInfo = GetCharacterClassInfo(WorldContextObject);
	if (CharacterClassInfo == nullptr) return; 

	AActor* AvatarActor = ASC->GetAvatarActor();
	FGameplayEffectContextHandle EffectContextHandle = ASC->MakeEffectContext();
	EffectContextHandle.AddSourceObject(AvatarActor);
	
	FCharacterClassDefaultInfo ClassDefaultInfo = CharacterClassInfo->GetClassDefaultInfo(CharacterClass);
	
	const FGameplayEffectSpecHandle PrimaryAttributesSpecHandle = ASC->MakeOutgoingSpec(ClassDefaultInfo.PrimaryAttributes, Level, EffectContextHandle);
	ASC->ApplyGameplayEffectSpecToSelf(*PrimaryAttributesSpecHandle.Data.Get());

	const FGameplayEffectSpecHandle SecondaryAttributesSpecHandle = ASC->MakeOutgoingSpec(CharacterClassInfo->SecondaryAttributes, Level, EffectContextHandle);
	ASC->ApplyGameplayEffectSpecToSelf(*SecondaryAttributesSpecHandle.Data.Get());

	const FGameplayEffectSpecHandle VitalAttributesSpecHandle = ASC->MakeOutgoingSpec(CharacterClassInfo->VitalAttributes, Level, EffectContextHandle);
	ASC->ApplyGameplayEffectSpecToSelf(*VitalAttributesSpecHandle.Data.Get());
}

void UAuraAbilitySystemLibrary::GiveStartupAbilities(const UObject* WorldContextObject,
	UAbilitySystemComponent* ASC, ECharacterClass CharacterClass)
{
	UCharacterClassInfo* CharacterClassInfo = GetCharacterClassInfo(WorldContextObject);
	if (CharacterClassInfo == nullptr) return;

	for (const TSubclassOf<UGameplayAbility>& Ability : CharacterClassInfo->CommonAbilities)
	{
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(Ability, 1);
		ASC->GiveAbility(AbilitySpec);
	}

	for (const TSubclassOf<UGameplayAbility>& Ability : CharacterClassInfo->GetClassDefaultInfo(CharacterClass).StartupAbilities)
	{
		
		if (ASC->GetAvatarActor()->Implements<UCombatInterface>())
		{
			FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(Ability, ICombatInterface::Execute_GetPlayerLevel(ASC->GetAvatarActor()));
			ASC->GiveAbility(AbilitySpec);
		}
		
	}
}

UCharacterClassInfo* UAuraAbilitySystemLibrary::GetCharacterClassInfo(const UObject* WorldContextObject)
{
	AAuraGameModeBase* AuraGameMode = Cast<AAuraGameModeBase>(UGameplayStatics::GetGameMode(WorldContextObject));
	if (AuraGameMode == nullptr) return nullptr;
	return AuraGameMode->CharacterClassInfo;
}

UAbilityInfo* UAuraAbilitySystemLibrary::GetAbilityInfo(const UObject* WorldContextObject)
{
	AAuraGameModeBase* AuraGameMode = Cast<AAuraGameModeBase>(UGameplayStatics::GetGameMode(WorldContextObject));
	if (AuraGameMode == nullptr) return nullptr;
	return AuraGameMode->AbilityInfo;
}


int32 UAuraAbilitySystemLibrary::GetXPRewardForClassAndLevel(const UObject* WorldContextObject,
                                                             ECharacterClass CharacterClass, int32 CharacterLevel)
{
	UCharacterClassInfo* CharacterClassInfo = GetCharacterClassInfo(WorldContextObject);
	if (CharacterClassInfo == nullptr) return 0;

	const FCharacterClassDefaultInfo& Info = CharacterClassInfo->GetClassDefaultInfo(CharacterClass);
	const float XPReward = Info.XPReward.GetValueAtLevel(CharacterLevel);

	return static_cast<int32>(XPReward);
}

FGameplayEffectContextHandle UAuraAbilitySystemLibrary::ApplyDamageEffect(const FDamageEffectParams& Params)
{
 	const FAuraGameplayTags& GameplayTags = FAuraGameplayTags::Get();
	const AActor* SourceAvatarActor = Params.SourceAbilitySystemComponent->GetAvatarActor();
	FGameplayEffectContextHandle ContextHandle = Params.SourceAbilitySystemComponent->MakeEffectContext();
	ContextHandle.AddSourceObject(SourceAvatarActor);
	FGameplayEffectSpecHandle DamageSpecHandle = Params.SourceAbilitySystemComponent->MakeOutgoingSpec(Params.DamageGameplayEffectClass, Params.AbilityLevel, ContextHandle);
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(DamageSpecHandle, Params.DamageType, Params.BaseDamage);
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(DamageSpecHandle, GameplayTags.Debuff_Chance, Params.DebuffChance);
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(DamageSpecHandle, GameplayTags.Debuff_Damage, Params.DebuffDamage);
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(DamageSpecHandle, GameplayTags.Debuff_Duration, Params.DebuffDuration);
	Params.TargetAbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*DamageSpecHandle.Data.Get());
	return ContextHandle;
}

bool UAuraAbilitySystemLibrary::IsBlockHit(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FAuraGameplayEffectContext* AuraContext = static_cast<const FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
		return AuraContext->IsBlockHit();
	return false;
}

bool UAuraAbilitySystemLibrary::IsCriticalHit(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FAuraGameplayEffectContext* AuraContext = static_cast<const FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
		return AuraContext->IsCriticalHit();
	return false;
}

void UAuraAbilitySystemLibrary::SetIsBlockHit(FGameplayEffectContextHandle& EffectContextHandle, bool IsBlockHit)
{
	if (FAuraGameplayEffectContext* AuraContext = static_cast<FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
		AuraContext->SetIsCriticalHit(IsBlockHit);
}

void UAuraAbilitySystemLibrary::SetIsCriticalHit(FGameplayEffectContextHandle& EffectContextHandle, bool IsCriticalHit)
{
	if (FAuraGameplayEffectContext* AuraContext = static_cast<FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
		AuraContext->SetIsCriticalHit(IsCriticalHit);
}

void UAuraAbilitySystemLibrary::GetLivePlayersWithinRadius(const UObject* WorldContextObject,
	TArray<AActor*>& OutOverlappingActors, const TArray<AActor*>& ActorsToIgnore, float Radius,
	const FVector& SphereOrigin)
{
	FCollisionQueryParams SphereParams;
	SphereParams.AddIgnoredActors(ActorsToIgnore);

	TArray<FOverlapResult> Overlaps;
	if (const UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull))
	{
		World->OverlapMultiByObjectType(Overlaps, SphereOrigin, FQuat::Identity, FCollisionObjectQueryParams(FCollisionObjectQueryParams::InitType::AllDynamicObjects), FCollisionShape::MakeSphere(Radius), SphereParams);
		for (FOverlapResult& Overlap : Overlaps)
		{
 			if (Overlap.GetActor()->Implements<UCombatInterface>() && !ICombatInterface::Execute_IsDead(Overlap.GetActor()))
			{
				OutOverlappingActors.AddUnique(ICombatInterface::Execute_GetAvatar(Overlap.GetActor()));
			}
		}
	}
}

bool UAuraAbilitySystemLibrary::IsNotFriend(AActor* FirstActor, AActor* SecondActor)  
{
	return !(
		(FirstActor->ActorHasTag(FName("Player")) && SecondActor->ActorHasTag(FName("Player")))
		|| (FirstActor->ActorHasTag(FName("Enemy")) && SecondActor->ActorHasTag(FName("Enemy")))
		);
}


