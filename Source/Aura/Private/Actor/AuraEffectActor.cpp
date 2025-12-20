// Copyright Wenhi


#include "Actor/AuraEffectActor.h"
#include "GameplayEffect.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Interaction/CombatInterface.h"
#include "Kismet/KismetMathLibrary.h"


AAuraEffectActor::AAuraEffectActor()
{
	PrimaryActorTick.bCanEverTick = false;
	SetRootComponent(CreateDefaultSubobject<USceneComponent>("SceneRoot")); 
}

void AAuraEffectActor::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	RunningTime += DeltaSeconds;
	const float SinePeriod = 2 * PI / SinePeriodConstant;
	if (RunningTime > SinePeriod) RunningTime = 0.f;
	ItemMovement(DeltaSeconds);
}

void AAuraEffectActor::ItemMovement(float DeltaSeconds)
{
	if (bRotate)
	{
		const FRotator DeltaRotation(0.f, DeltaSeconds * RotationRate, 0.f);
		CalculatedRotation = UKismetMathLibrary::ComposeRotators(CalculatedRotation, DeltaRotation);
	}
	if (bSinusoidalMovement)
	{
		const float Sine = FMath::Sin(RunningTime * SinePeriodConstant);
		CalculatedLocation = InitialLocation + FVector(0.f, 0.f, Sine);
		
	}
}



void AAuraEffectActor::BeginPlay()
{
	Super::BeginPlay();
	InitialLocation = GetActorLocation();
	CalculatedLocation = InitialLocation;
	CalculatedRotation = GetActorRotation();
}

void AAuraEffectActor::StartSinusoidalMovement()
{
	bSinusoidalMovement = true;
	InitialLocation = GetActorLocation();
	CalculatedLocation = InitialLocation;

	
}

void AAuraEffectActor::StartRotation()
{
	bSinusoidalMovement = true;

	CalculatedRotation = GetActorRotation();
}

void AAuraEffectActor::ApplyEffectToTarget(AActor* TargetActor, TSubclassOf<UGameplayEffect> GameplayEffectClass)
{
	if (!TargetActor->Implements<UCombatInterface>()) return;
	if (TargetActor->ActorHasTag(FName("Enemy")) && !bApplyEffectsToEnemy) return;
	UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
	 if (TargetASC == nullptr) return;

	check(GameplayEffectClass);
	FGameplayEffectContextHandle EffectContextHandle = TargetASC->MakeEffectContext();
	EffectContextHandle.AddSourceObject(this);
	const FGameplayEffectSpecHandle EffectSpecHandle = TargetASC->MakeOutgoingSpec(GameplayEffectClass, ActorLevel, EffectContextHandle);
	const FActiveGameplayEffectHandle ActiveEffectHandle = TargetASC->ApplyGameplayEffectSpecToSelf(*EffectSpecHandle.Data.Get());

	if (const bool bIsInfinite = EffectSpecHandle.Data.Get()->Def.Get()->DurationPolicy == EGameplayEffectDurationType::Infinite)
	{
		ActiveEffectHandles.Add(ActiveEffectHandle, TargetASC);
	}
	if (bDestroyOnEffectApplication && EffectSpecHandle.Data.Get()->Def.Get()->DurationPolicy != EGameplayEffectDurationType::Infinite)
	{
		Destroy();
	}
		
}

void AAuraEffectActor::OnOverlap(AActor* TargetActor)
{
	if (!TargetActor->Implements<UCombatInterface>()) return;
	if (TargetActor->ActorHasTag(FName("Enemy")) && !bApplyEffectsToEnemy) return;
	if (InstantEffectApplicationPolicy ==  EEffectApplicationPolicy::ApplyOnOverlap)
	{
		ApplyEffectToTarget(TargetActor, InstantGameplayEffectClass);
	}
	if (DurationEffectApplicationPolicy ==  EEffectApplicationPolicy::ApplyOnOverlap)
	{
		ApplyEffectToTarget(TargetActor, DurationGameplayEffectClass);
	}
	if (InfiniteEffectApplicationPolicy ==  EEffectApplicationPolicy::ApplyOnOverlap)
	{
		ApplyEffectToTarget(TargetActor, InfiniteGameplayEffectClass);
	}
	if (bDestroyOnEffectApplication)
	{
		Destroy();
	}
}

void AAuraEffectActor::OnEndOverlap(AActor* TargetActor)
{
	if (!TargetActor->Implements<UCombatInterface>()) return;
	if (TargetActor->ActorHasTag(FName("Enemy")) && !bApplyEffectsToEnemy) return;
	
	if (InstantEffectApplicationPolicy ==  EEffectApplicationPolicy::ApplyOnOverlap)
	{
		ApplyEffectToTarget(TargetActor, InstantGameplayEffectClass);
	}
	if (DurationEffectApplicationPolicy ==  EEffectApplicationPolicy::ApplyOnEndOverlap)
	{
		ApplyEffectToTarget(TargetActor, DurationGameplayEffectClass);
	}
	if (InfiniteEffectApplicationPolicy ==  EEffectApplicationPolicy::ApplyOnEndOverlap)
	{
		ApplyEffectToTarget(TargetActor, InfiniteGameplayEffectClass);
	}
	if (InfiniteEffectRemovalPolicy ==  EEffectRemovalPolicy::RemoveOnEndOverlap)
	{
		UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
		if (!IsValid(TargetASC)) return;
		TArray<FActiveGameplayEffectHandle> HandlesToRemove;
		for (TPair<FActiveGameplayEffectHandle, UAbilitySystemComponent*> HandlePair : ActiveEffectHandles)
		{
			if (TargetASC == HandlePair.Value)
			{
				TargetASC->RemoveActiveGameplayEffect(HandlePair.Key, 1);
				HandlesToRemove.Add(HandlePair.Key);
			}
		}
		for (FActiveGameplayEffectHandle& Handle : HandlesToRemove)
		{
			ActiveEffectHandles.FindAndRemoveChecked(Handle);
		}
	}
}


