// Copyright Wenhi


#include "AbilitySystem/Abilities/AuraFireBlast.h"

FString UAuraFireBlast::GetDescription(int32 Level)
{
	const int32 ScaledDamage = Damage.GetValueAtLevel(Level);
	const float ManaCost = GetManaCost(Level);
	const float Cooldown = GetCooldown(Level);

	return FString::Printf(TEXT(
		"<Title>火焰冲击</>\n"
		
		"<Small>等级: </><Level>%d</>\n"
		"<Small>法力消耗: </><ManaCost>%.1f</>\n"
		"<Small>冷却时间: </><Cooldown>%.1f</>\n"
		
		"<Default>向周围发射%d个火球，每个都会回到自身并且在回来的途中产生爆炸，造成</><Damage> %d </><Default>点火焰伤害，有概率使目标着火</>"
		),
		Level,
		ManaCost,
		Cooldown,
		NumFireBalls,
		ScaledDamage);
}

FString UAuraFireBlast::GetNextLevelDescription(int32 Level)
{
	const int32 ScaledDamage = Damage.GetValueAtLevel(Level);
	const float ManaCost = GetManaCost(Level);
	const float Cooldown = GetCooldown(Level);
	return FString::Printf(TEXT(
		"<Title>下一级</>\n"
		
		"<Small>等级: </><Level>%d</>\n"
		"<Small>法力消耗: </><ManaCost>%.1f</>\n"
		"<Small>冷却时间: </><Cooldown>%.1f</>\n"
		
		"<Default>向周围发射%d个火球，每个都会回到自身并且在回来的途中产生爆炸，造成</><Damage> %d </><Default>点火焰伤害，有概率使目标着火</>"
		),
		Level,
		ManaCost,
		Cooldown,
		NumFireBalls,
		ScaledDamage);
}
