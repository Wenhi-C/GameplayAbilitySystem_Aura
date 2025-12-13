 // Copyright Wenhi


#include "AbilitySystem/Debuff/DebuffNiagaraComponent.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "Interaction/CombatInterface.h"

UDebuffNiagaraComponent::UDebuffNiagaraComponent()
{
	bAutoActivate = false;

	
	
}

void UDebuffNiagaraComponent::BeginPlay()
{
	Super::BeginPlay();
	UAbilitySystemComponent* OwnerASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetOwner());
	ICombatInterface* CombatInterface = Cast<ICombatInterface>(GetOwner());
	if (OwnerASC)
	{
		OwnerASC->RegisterGameplayTagEvent(DebuffTag).AddLambda(
			[this](const FGameplayTag DebuffTag, int32 NewCount)
			{
				if (IsValid(GetOwner()) && GetOwner()->Implements<UCombatInterface>() && !ICombatInterface::Execute_IsDead(GetOwner()) && NewCount > 0)
				{
					Activate();
				}
				else
				{
					Deactivate();
				}
			});
	}
	else if (CombatInterface)
	{
		CombatInterface->GetOnASCRegisteredDelegate().AddWeakLambda(this,
			[this](UAbilitySystemComponent* InASC)
			{
				InASC->RegisterGameplayTagEvent(DebuffTag).AddLambda(
				[this](const FGameplayTag DebuffTag, int32 NewCount)
				{
					if (NewCount > 0)
					{
						Activate();
					}
					else
					{
						Deactivate();
					}
				});
			});
	}
	if (CombatInterface)
	{
		CombatInterface->GetOnDeathDelegate().AddDynamic(this, &UDebuffNiagaraComponent::OnOwnerDeath);
	}
	
	
}

void UDebuffNiagaraComponent::OnOwnerDeath(AActor* DeadActor)
{
	Deactivate();
}
