// Copyright Night Gamer, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Templates/SharedPointer.h"
#include "EveInventory/Eve/Data/EveItemData.h"
#include "EveInventoryMgr.generated.h"

/**
 * 背包管理系统，继承自 UGameInstanceSubsystem，
 * 负责管理物品的添加、移除、交换等功能。
 */
UCLASS()
class UEveInventoryMgr : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	/**
	 * 初始化背包管理系统，加载物品配置信息。
	 */
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	/**
	 * 反初始化，清理数据。
	 */
	virtual void Deinitialize() override;

public:
	/**
	 * 背包更新事件，当物品发生变化时触发。
	 */
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FEveOnInventoryUpdated);

	/**
	 * Blueprint 绑定的委托，当物品数据发生更新时触发。
	 */
	UPROPERTY(BlueprintAssignable)
	FEveOnInventoryUpdated OnInventoryUpdated;

public:
	/**
	 * 测试用：随机添加一个物品到背包。
	 */
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void InventoryTestAddBtn();

	/**
	 * 测试用：移除最近添加的一个物品。
	 */
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void InventoryTestRemoveBtn();

public:
	/**
	 * 添加物品到背包。
	 * @param Item 要添加的物品对象。
	 * @param PosIdx 目标格子索引，默认为 -1，表示自动寻找空闲位置。
	 */
	void AddItem(TObjectPtr<UEveItem> Item, int32 PosIdx = -1);

	/**
	 * 移除背包中的物品。
	 * @param TID 物品的唯一 ID。
	 */
	void RemoveItem(int32 TID);

	/**
	 * 交换两个物品的位置。
	 * @param OldPosIdx 旧的位置索引。
	 * @param NewPosIdx 新的位置索引。
	 */
	void ExchangeItem(int32 OldPosIdx, int32 NewPosIdx);

public:
	/**
	 * 物品数据表，存储所有物品的配置信息。
	 */
	UPROPERTY()
	TMap<int32, TObjectPtr<UEveItemCfg>> AllItemsCfg;

	/**
	 * 当前背包中存储的物品，键为 TID，值为对应的物品对象。
	 */
	UPROPERTY()
	TMap<int32, TObjectPtr<UEveInventoryItem>> InventoryItems;

	/**
	 * 通过位置索引查询 TID，记录了物品的存放位置。
	 */
	TMap<int32, int32> PosToTIDMap;

	/**
	 * 当前已被占用的格子索引集合。
	 */
	TSet<int32> CurPosIdxes;

	/**
	 * 背包的最大格子数量。
	 */
	const int32 SlotNum = 6;

	/**
	 * 记录最近添加的物品 ID（用于支持撤销移除功能）。
	 */
	TArray<int32> AddedItemsStack;

	/**
	 * 记录已添加的物品 ID，避免重复添加。
	 */
	TSet<int32> AddedItemsSet;
};
