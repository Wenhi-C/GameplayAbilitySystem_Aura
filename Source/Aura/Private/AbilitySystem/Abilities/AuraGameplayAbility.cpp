// Copyright Wenhi


#include "AbilitySystem/Abilities/AuraGameplayAbility.h"

#include "AuraGameplayTags.h"
#include "AbilitySystem/AuraAttributeSet.h"


FString UAuraGameplayAbility::GetDescription(int32 Level)
{
	return FString::Printf(TEXT("<Default>%s,</><Level>%d</>"), L"默认技能名字", Level);
}

FString UAuraGameplayAbility::GetNextLevelDescription(int32 Level)
{
	return FString::Printf(TEXT("<Default>下一级: </><Level>%d</>\n <Default> 造成更多伤害. </>"),  Level);
}

FString UAuraGameplayAbility::GetLockedDescription(int32 Level)
{
	return FString::Printf(TEXT("<Default>技能未解锁</>\n<Level>%d</><Default> 级可解锁该技能</>"), Level);
}

float UAuraGameplayAbility::GetManaCost(float InLevel) const
{
	float ManaCost = 0.f;
	if (const UGameplayEffect* CostEffect = GetCostGameplayEffect())
	{
		for (const FGameplayModifierInfo& Mod : CostEffect->Modifiers)
		{
			if (Mod.Attribute == UAuraAttributeSet::GetManaAttribute())
			{
				Mod.ModifierMagnitude.GetStaticMagnitudeIfPossible(InLevel, ManaCost);
				break;
			}
		}
	}
	return -ManaCost;
	
}

float UAuraGameplayAbility::GetCooldown(float InLevel) const
{
	float Cooldown = 0.f;
	if (const UGameplayEffect* CooldownEffect = GetCooldownGameplayEffect())
	{
		CooldownEffect->DurationMagnitude.GetStaticMagnitudeIfPossible(InLevel, Cooldown);
	}
	return Cooldown;
}
