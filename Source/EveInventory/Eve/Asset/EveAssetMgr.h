// Copyright Night Gamer, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/AssetManager.h"
#include "Engine/DataTable.h"
#include "EveAssetMgr.generated.h"

/**
 * @brief 资产管理器类，负责游戏中的资产加载与管理。
 * 继承自 UAssetManager，用于集中管理资源，例如数据表、UI 组件等。
 */
UCLASS(BlueprintType, Blueprintable)
class EVEINVENTORY_API UEveAssetMgr : public UAssetManager
{
	GENERATED_BODY()

public:
	/**
	 * @brief 获取全局的 Asset Manager 实例
	 * 
	 * 该方法用于获取游戏的全局资源管理器实例。如果 `GEngine->AssetManager`
	 * 不是 `UEveAssetMgr`，则会触发 Fatal 错误，保证游戏正确运行。
	 * 
	 * @return 返回 `UEveAssetMgr` 的单例引用
	 */
	static UEveAssetMgr& Get()
	{
		check(GEngine);

		// 尝试从 GEngine 的 AssetManager 获取实例
		if (UEveAssetMgr* Singleton = Cast<UEveAssetMgr>(GEngine->AssetManager))
		{
			return *Singleton;
		}

		// 如果 AssetManager 不是 UEveAssetMgr，则抛出 Fatal 错误
		UE_LOG(LogTemp, Fatal, TEXT("Invalid AssetManager!"));
		return *NewObject<UEveAssetMgr>(); // 此处不会被执行，只是为了让编译器满意
	}

	/**
	 * @brief 同步加载资源
	 * 
	 * 通过 `TSoftObjectPtr` 进行同步加载（阻塞加载），用于确保资源可用。
	 * 适用于小型资源或初始化阶段的关键资源加载，不推荐在游戏运行过程中频繁使用。
	 * 
	 * @tparam AssetClass 资源的类型（如 UTexture2D, UDataTable, UUserWidget）
	 * @param SoftPtr 资源的 `TSoftObjectPtr`（软引用，指向未加载的资源）
	 * @return 成功加载的资源指针，如果加载失败，则返回 nullptr
	 */
	template <typename AssetClass>
	AssetClass* GetAssetSync(const TSoftObjectPtr<AssetClass>& SoftPtr)
	{
		AssetClass* Asset = SoftPtr.LoadSynchronous();
		return Asset;
	}

	/**
	 * @brief 物品数据表资源
	 * 
	 * 该资源用于存储游戏中的物品信息，例如道具名称、描述、图标等数据。
	 * 采用 `TSoftObjectPtr<UDataTable>` 软引用，支持异步加载，提高性能。
	 */
	UPROPERTY(EditDefaultsOnly, Category = "DT")
	TSoftObjectPtr<UDataTable> DTItem;

	/**
	 * @brief 背包 UI 资源
	 * 
	 * 该变量存储 `Inventory` UI 的蓝图类，允许在游戏中动态创建背包界面。
	 */
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> InventoryClass;

	/**
	 * @brief 物品 UI 资源
	 * 
	 * 该变量存储 `Item` UI 的蓝图类，允许在游戏 UI 中动态创建物品控件。
	 */
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> ItemClass;
};
