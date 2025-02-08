// Copyright Night Gamer, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Templates/SharedPointer.h"
#include "EveInventory/Eve/Data/EveItemData.h"
#include "EveInventoryMgr.generated.h"

UCLASS()
class UEveInventoryMgr : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	void Initialize(FSubsystemCollectionBase& Collection) override;

	void Deinitialize() override;

public:
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FEveOnInventoryUpdated);

	UPROPERTY(BlueprintAssignable)
	FEveOnInventoryUpdated OnInventoryUpdated;

public:
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void InventoryTestAddBtn();

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void InventoryTestRemoveBtn();

public:
	void AddItem(TObjectPtr<UEveItem> Item, int32 PosIdx = -1);

	void RemoveItem(int32 TID);

	void ExchangeItem(int32 OldPosIdx, int32 NewPosIdx);

public:
	// 读表ItemData信息，所有的装备信息表
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Inventory")
	TMap<int32, TObjectPtr<UEveItemCfg>> AllItemsCfg;

	// 背包InventoryItem格子数据
	UPROPERTY()
	TMap<int32, TObjectPtr<UEveInventoryItem>> InventoryItems;

	// 通过Pos查询TID
	TMap<int32, int32> PosToTIDMap;

	// 当前占格位
	TSet<int32> CurPosIdxes;

	// 格子数量
	const int32 SlotNum = 6;

	TArray<int32> AddedItemsStack;
	TSet<int32> AddedItemsSet;
};