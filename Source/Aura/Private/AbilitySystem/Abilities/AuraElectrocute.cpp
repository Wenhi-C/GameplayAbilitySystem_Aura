// Copyright Wenhi


#include "AbilitySystem/Abilities/AuraElectrocute.h"

FString UAuraElectrocute::GetDescription(int32 Level)
{
	const int32 ScaledDamage = Damage.GetValueAtLevel(Level);
	const float ManaCost = GetManaCost(Level);
	const float Cooldown = GetCooldown(Level);
	return FString::Printf(TEXT(
			"<Title>电击</>\n"
			
			"<Small>等级: </><Level>%d</>\n"
			"<Small>法力消耗: </><ManaCost>%.1f</>\n"
			"<Small>冷却时间: </><Cooldown>%.1f</>\n"
			
			"<Default>电击一个主目标，并且连锁电击%d个此目标，每秒造成</><Damage> %d </><Default>点雷电伤害，在结束电击时有概率使目标眩晕</>"
			),
			Level,
			ManaCost,
			Cooldown,
			FMath::Min(Level, MaxNumAttackTargets),
			ScaledDamage * 10);
}

FString UAuraElectrocute::GetNextLevelDescription(int32 Level)
{
	const int32 ScaledDamage = Damage.GetValueAtLevel(Level);
	const float ManaCost = GetManaCost(Level);
	const float Cooldown = GetCooldown(Level);
	return FString::Printf(TEXT(
			"<Title>下一级</>\n"
			
			"<Small>等级: </><Level>%d</>\n"
			"<Small>法力消耗: </><ManaCost>%.1f</>\n"
			"<Small>冷却时间: </><Cooldown>%.1f</>\n"
			
			"<Default>电击一个主目标，并且连锁电击%d个此目标，每秒造成</><Damage> %d </><Default>点雷电伤害，在结束电击时有概率使目标眩晕</>"
			),
			Level,
			ManaCost,
			Cooldown,
			FMath::Min(Level, MaxNumAttackTargets),
			ScaledDamage * 10);
}
