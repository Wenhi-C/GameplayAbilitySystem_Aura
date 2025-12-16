// Copyright Wenhi

#pragma once

#include "CoreMinimal.h"
#include "MVVMViewModelBase.h"
#include "MVVM_LoadSlot.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSetWidgetSwitcherIndex, int32, WidgetSwitcherIndex);

/**
 * 
 */
UCLASS()
class AURA_API UMVVM_LoadSlot : public UMVVMViewModelBase
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintAssignable)
	FSetWidgetSwitcherIndex SetWidgetSwitcherIndex;

	void InitializeSlot();

	UPROPERTY()
	int32 SlotIndex;

	/* Field Notifies */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, FieldNotify, Setter, Getter)
	FString PlayerName;

	void SetPlayerName(const FString& InPlayerName);

	FString GetPlayerName() const { return  PlayerName; }

	void SetLoadSlotName(const FString& InLoadSlotName);

	FString GetLoadSlotName() const { return LoadSlotName; }
	
private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, FieldNotify, Setter, Getter, meta = (AllowPrivateAccess = "true"))
	FString LoadSlotName;
};
