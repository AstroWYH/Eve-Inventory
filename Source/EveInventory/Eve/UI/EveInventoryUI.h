// Copyright Night Gamer, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "EveInventoryUI.generated.h"

/**
 * @brief 背包 UI 子系统（UEveInventoryUI）
 * 
 * 该类继承自 `UGameInstanceSubsystem`，负责管理背包 UI，包括：
 * - 创建 UI
 * - 绑定事件
 * - 更新 UI
 * - 释放资源
 * 
 * @note 该子系统在 `GameInstance` 生命周期内保持有效，无需手动创建。
 */
UCLASS()
class UEveInventoryUI : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	/**
	 * @brief 初始化背包 UI 子系统
	 * 
	 * 该方法在 `GameInstance` 初始化时调用：
	 * - 创建 UI
	 * - 绑定事件
	 * 
	 * @param Collection 系统集合（用于管理其他 `Subsystem`）
	 */
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	/**
	 * @brief 释放背包 UI 资源
	 * 
	 * 该方法在 `GameInstance` 关闭时调用：
	 * - 取消 `OnInventoryUpdated` 事件绑定
	 */
	virtual void Deinitialize() override;

public:
	/**
	 * @brief 创建背包 UI
	 * 
	 * - 通过 `UEveAssetMgr` 生成 `InventoryWidget`
	 * - 将 `InventoryWidget` 添加到视口
	 */
	virtual void CreateUI();

	/**
	 * @brief 绑定背包 UI 事件
	 * 
	 * - 监听 `UEveInventoryMgr::OnInventoryUpdated`
	 * - 在背包数据变更时，调用 `UpdateInventory`
	 */
	virtual void BindUIEvent();

public:
	/**
	 * @brief 更新背包 UI
	 * 
	 * - 清空 `UniformGridPanel`
	 * - 遍历 `InventoryItems`，创建 `ItemWidget`
	 * - 计算 `Row/Col` 位置，添加到 `Grid`
	 */
	UFUNCTION()
	void UpdateInventory();

private:
	/** 背包 UI 根组件 */
	UPROPERTY()
	TObjectPtr<class UEveInventoryWidget> InventoryUI;

	/** 物品 UI 组件池（用于管理 `ItemWidget`） */
	UPROPERTY()
	TArray<TObjectPtr<class UEveItemWidget>> ItemUIPool;
};
