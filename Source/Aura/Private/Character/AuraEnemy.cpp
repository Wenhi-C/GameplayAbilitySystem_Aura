// Copyright Wenhi


#include "Character/AuraEnemy.h"

void AAuraEnemy::HighlightActor()
{
	DrawDebugSphere(GetWorld(), GetActorLocation(), 25.f, 12, FColor::Red);
}

void AAuraEnemy::UnHighlightActor()
{
	DrawDebugSphere(GetWorld(), GetActorLocation(), 25.f, 12, FColor::Black, false, 2.f);
}
