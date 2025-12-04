// Copyright Wenhi


#include "AbilitySystem/Abilities/AuraFireBolt.h"

#include "AuraGameplayTags.h"

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