// Copyright Night Gamer, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "EveItemData.generated.h"

class UEveItemCfg;

/**
 * @brief 物品数据结构体，继承自 `FTableRowBase`
 * 
 * 该结构体用于存储物品的基础数据，并且可以作为 `UDataTable` 的一部分，用于游戏的物品表格管理。
 */
USTRUCT(Blueprintable, BlueprintType)
struct FEveItemData : public FTableRowBase
{
	GENERATED_BODY()

public:
	/** 物品唯一 ID */
	UPROPERTY(EditDefaultsOnly)
	int32 TID;

	/** 物品名称 */
	UPROPERTY(EditDefaultsOnly)
	FString Name;

	/** 物品图标 */
	UPROPERTY(EditDefaultsOnly)
	UTexture2D* Icon;
};

/**
 * @brief 背包中的物品实例，存储物品的位置信息
 * 
 * 该类用于管理玩家背包中的物品，包括物品 ID、数量和格子索引。
 */
UCLASS(BlueprintType)
class UEveInventoryItem : public UObject
{
	GENERATED_BODY()

public:
	/** 物品唯一 ID（与 `FEveItemData` 的 TID 对应） */
	UPROPERTY(BlueprintReadWrite, Category = "Inventory")
	int32 TID = -1;

	/** 物品数量 */
	UPROPERTY(BlueprintReadWrite, Category = "Inventory")
	int32 Amount = 0;

	/** 物品在背包中的格子索引 */
	UPROPERTY(BlueprintReadWrite, Category = "Inventory")
	int32 PosIdx = 0;

	/**
	 * @brief 初始化物品信息
	 * 
	 * @param InTID 物品 ID
	 * @param InAmount 物品数量
	 * @param InPosIdx 物品在背包中的格子索引
	 */
	void Init(int32 InTID, int32 InAmount, int32 InPosIdx)
	{
		TID = InTID;
		Amount = InAmount;
		PosIdx = InPosIdx;
	}
};

/**
 * @brief 游戏内的物品对象，包含物品 ID 和配置信息
 * 
 * 该类用于存储物品的运行时信息，例如 TID（唯一 ID）和 XID（扩展 ID）。
 */
UCLASS(BlueprintType)
class UEveItem : public UObject
{
	GENERATED_BODY()

public:
	/** 物品唯一 ID（用于识别物品） */
	UPROPERTY(BlueprintReadWrite, Category = "Inventory")
	int32 TID = -1;

	/** 物品扩展 ID（用于某些特殊逻辑，比如区分同类物品） */
	UPROPERTY(BlueprintReadWrite, Category = "Inventory")
	int32 XID = -1;

	/** 物品的配置数据（软引用，避免循环依赖） */
	UPROPERTY(BlueprintReadWrite, Category = "Inventory")
	TWeakObjectPtr<UEveItemCfg> ItemCfg = nullptr;
};

/**
 * @brief 物品的配置信息，包含 `FEveItemData`
 * 
 * 该类用于存储物品的静态配置信息，如名称、图标等。
 */
UCLASS(Blueprintable, BlueprintType)
class UEveItemCfg : public UObject
{
	GENERATED_BODY()

public:
	/** 物品的基础数据，来源于 `FEveItemData` */
	UPROPERTY(BlueprintReadOnly)
	FEveItemData ItemData;

	/**
	 * @brief 从 `FEveItemData` 结构体创建 `UEveItemCfg` 实例
	 * 
	 * @param Data 物品的基础数据
	 * @return 创建的 `UEveItemCfg` 实例
	 */
	UFUNCTION(BlueprintCallable, Category = "Item")
	static UEveItemCfg* CreateFromStruct(const FEveItemData& Data)
	{
		UEveItemCfg* Obj = NewObject<UEveItemCfg>();
		Obj->ItemData = Data;
		return Obj;
	}
};
