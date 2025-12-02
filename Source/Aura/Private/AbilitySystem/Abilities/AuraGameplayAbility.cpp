// Copyright Wenhi


#include "AbilitySystem/Abilities/AuraGameplayAbility.h"


FString UAuraGameplayAbility::GetDescription(int32 Level)
{
	return FString::Printf(TEXT("<Default>%s,</><Level>%d</>"), L"默认技能名字 假文", Level);
}

FString UAuraGameplayAbility::GetNextLevelDescription(int32 Level)
{
	return FString::Printf(TEXT("<Default>下一级: </><Level>%d</>\n <Default> 造成更多伤害. </>"),  Level);
}

FString UAuraGameplayAbility::GetLockedDescription(int32 Level)
{
	return FString::Printf(TEXT("<Default>技能未解锁</>\n<Level>%d</><Default> 级可解锁该技能</>"), Level);
}
