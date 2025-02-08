// Copyright Night Gamer, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "EveInventoryUI.generated.h"

UCLASS()
class UEveInventoryUI : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	virtual void Deinitialize() override;

public:
	virtual void CreateUI();

	virtual void BindUIEvent();

public:
	UFUNCTION()
	void UpdateInventory();

private:
	UPROPERTY()
	TObjectPtr<class UEveInventoryWidget> InventoryUI;

	UPROPERTY()
	TArray<TObjectPtr<class UEveItemWidget>> ItemUIPool;

};