// Copyright Wenhi


#include "Character/AuraCharacter.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "Animation/AnimNode_Inertialization.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Player/AuraPlayerController.h"
#include "Player/AuraPlayerState.h"
#include "UI/HUD/AuraHUD.h"

AAuraCharacter::AAuraCharacter()
{
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 400.f, 0.f);
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;
}

// Init Ability actor info for the Server
void AAuraCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	
	InitAbilityActorInfo();

	// 添加角色初始能力，只需要在服务器端执行
	AddCharacterAbilities();
}

// Init Ability actor info for the Client
void AAuraCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
	
	InitAbilityActorInfo();
}

int32 AAuraCharacter::GetPlayerLevel()
{
	const AAuraPlayerState* AuraPlayerState = Cast<AAuraPlayerState>(GetPlayerState());
	check(AuraPlayerState);
	return AuraPlayerState->GetPlayerLevel();
}



// 初始化能力角色信息
void AAuraCharacter::InitAbilityActorInfo()
{
	AAuraPlayerState* AuraPlayerState = Cast<AAuraPlayerState>(GetPlayerState());
	check(AuraPlayerState);
	
	AbilitySystemComponent = AuraPlayerState->GetAbilitySystemComponent();
	AttributeSet = AuraPlayerState->GetAttributeSet();
	
	AbilitySystemComponent->InitAbilityActorInfo(AuraPlayerState, this);
	// 调用蓝图函数
	Cast<UAuraAbilitySystemComponent>(AbilitySystemComponent)->AbilityActorInfoSet();
	
	if (AAuraPlayerController* AuraPlayerController = Cast<AAuraPlayerController>(GetController()))
	{
		if (AAuraHUD* AuraHUD = Cast<AAuraHUD>(AuraPlayerController->GetHUD()))
		{
			AuraHUD->InitOverlay(AuraPlayerController, AuraPlayerState, AbilitySystemComponent, AttributeSet);
		}
	}

	InitializeDefaultAttributes();
}
