// Copyright Wenhi


#include "AbilitySystem/Data/LevelUpInfo.h"

int32 ULevelUpInfo::FindLevelForXP(const int32 XP) const
{
	int32 MaxLevel = LevelUpInformation.Num() - 1;
	int32 MinLevel = 0;
	while (MinLevel <= MaxLevel)
	{
		const int32 CurrentLevel =  MinLevel + (MaxLevel - MinLevel) / 2;
		if (LevelUpInformation[CurrentLevel].LevelUpRequirement <= XP)
		{
			MinLevel = CurrentLevel + 1;
		}
		else
		{
			MaxLevel = CurrentLevel - 1;
		}
	}
	return MinLevel;
}
