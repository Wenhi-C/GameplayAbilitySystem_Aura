// Copyright Wenhi


#include "Game/LoadScreenSaveGame.h"

FSaveMap ULoadScreenSaveGame::GetSaveMapWithMapName(const FString& InMapName)
{
	for (const FSaveMap& Map : SaveMaps)
	{
		if (Map.MapAssetName == InMapName)
		{
			return Map;
		}
	}
	return FSaveMap();
}

bool ULoadScreenSaveGame::HasMap(const FString& InMapName)
{
	for (const FSaveMap& Map : SaveMaps)
	{
		if (Map.MapAssetName == InMapName)
		{
			return true;
		}
	}
	return false;
}
