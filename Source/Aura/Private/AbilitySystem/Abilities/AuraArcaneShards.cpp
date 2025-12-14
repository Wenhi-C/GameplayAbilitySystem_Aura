// Copyright Wenhi


#include "AbilitySystem/Abilities/AuraArcaneShards.h"

FString UAuraArcaneShards::GetDescription(int32 Level)
{
	const int32 ScaledDamage = Damage.GetValueAtLevel(Level);
	const float ManaCost = GetManaCost(Level);
	const float Cooldown = GetCooldown(Level);
	
	return FString::Printf(TEXT(
		"<Title>奥术水晶</>\n"
		
		"<Small>等级: </><Level>%d</>\n"
		"<Small>法力消耗: </><ManaCost>%.1f</>\n"
		"<Small>冷却时间: </><Cooldown>%.1f</>\n"
		
		"<Default>生成%d个水晶，击退敌人并造成至多</><Damage> %d </><Default>点奥术伤害（依据距离降低）</>"
		),
		Level,
		ManaCost,
		Cooldown,
		FMath::Min(Level, MaxNumShards),
		ScaledDamage);
		
	
}

FString UAuraArcaneShards::GetNextLevelDescription(int32 Level)
{
	const int32 ScaledDamage = Damage.GetValueAtLevel(Level);
	const float ManaCost = GetManaCost(Level);
	const float Cooldown = GetCooldown(Level);
	return FString::Printf(TEXT(
		"<Title>奥术水晶</>\n"
		
		"<Small>等级: </><Level>%d</>\n"
		"<Small>法力消耗: </><ManaCost>%.1f</>\n"
		"<Small>冷却时间: </><Cooldown>%.1f</>\n"
		
		"<Default>生成%d个水晶，击退敌人并造成至多</><Damage> %d </><Default>点奥术伤害（依据距离降低）</>"
		),
		Level,
		ManaCost,
		Cooldown,
		FMath::Min(Level, MaxNumShards),
		ScaledDamage);
}