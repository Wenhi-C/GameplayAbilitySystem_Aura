// Copyright Wenhi


#include "AbilitySystem/Abilities/AuraSummonAbility.h"

TArray<FVector> UAuraSummonAbility::GetSpawnLocation()
{
	const FVector Forward =  GetAvatarActorFromActorInfo()->GetActorForwardVector();
	const FVector Location = GetAvatarActorFromActorInfo()->GetActorLocation();
	const float DeltaSpread = SpawnSpread / NumMinions;
	
	const FVector LeftOfSpread = Forward.RotateAngleAxis(-SpawnSpread / 2.f, FVector::UpVector);
	// DrawDebugDirectionalArrow(GetWorld(), Location, Location + LeftOfSpread * MaxSpawnDistance, 4.f, FColor::Red, false, 3.f);

	const FVector RightOfSpread = Forward.RotateAngleAxis(SpawnSpread / 2.f, FVector::UpVector);
	// DrawDebugDirectionalArrow(GetWorld(), Location, Location + RightOfSpread * MaxSpawnDistance, 4.f, FColor::Green, false, 3.f);
	TArray<FVector> SpawnLocations;
	for (int32 i = 0; i < NumMinions; i++) 
	{
		const FVector Direction = RightOfSpread.RotateAngleAxis(-DeltaSpread * i, FVector::UpVector);
		FVector ChosenSpawnLocation = Location + Direction * FMath::FRandRange(MinSpawnDistance, MaxSpawnDistance);

		FHitResult Hit;
		GetWorld()->LineTraceSingleByChannel(Hit,
			ChosenSpawnLocation + FVector(0.f, 0.f, 400.f),
			ChosenSpawnLocation + FVector(0.f, 0.f, -400.f),
			ECC_Visibility);
		if (Hit.bBlockingHit)
		{
			ChosenSpawnLocation = Hit.ImpactPoint;
			SpawnLocations.Add(ChosenSpawnLocation);
		}
		
		// DrawDebugDirectionalArrow(GetWorld(), Location, Location + Direction * MaxSpawnDistance, 4.f, FColor::Yellow, false, 3.f);
		// DrawDebugSphere(GetWorld(), ChosenSpawnLocation, 30.f, 12, FColor::Yellow, false, 3.f);

	}
	return SpawnLocations;
}

TSubclassOf<APawn> UAuraSummonAbility::GetRandomMinionClass()
{
	const int32 Selection = FMath::RandRange(0, MinionClass.Num() - 1);
	return MinionClass[Selection];
}
