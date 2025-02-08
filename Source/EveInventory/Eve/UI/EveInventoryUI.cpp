// Copyright Night Gamer, Inc. All Rights Reserved.

#include "EveInventoryUI.h"

#include "EveInventoryWidget.h"
#include "EveItemWidget.h"
#include "Components/Image.h"
#include "Components/UniformGridPanel.h"
#include "EveInventory/Eve/Asset/EveAssetMgr.h"
#include "EveInventory/Eve/Manager/EveInventoryMgr.h"

/**
 * @brief 初始化背包 UI 子系统
 * 
 * 该方法会在 `GameInstance` 初始化时被调用，负责创建 UI 并绑定事件。
 */
void UEveInventoryUI::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	// 在下一帧创建 UI，确保有效
	GetWorld()->GetTimerManager().SetTimerForNextTick([this]()
	{
		CreateUI();
		BindUIEvent();
	});
}

/**
 * @brief 创建背包 UI
 * 
 * 该方法会创建 `InventoryUI` 并将其添加到视口。
 */
void UEveInventoryUI::CreateUI()
{
	// 从资产管理器 `UEveAssetMgr` 中获取 `InventoryClass` 并创建 UI
	InventoryUI = Cast<UEveInventoryWidget>(
		UUserWidget::CreateWidgetInstance(*GetGameInstance(), UEveAssetMgr::Get().InventoryClass, TEXT("Inventory"))
	);
	
	// 确保 UI 创建成功
	if (!ensure(InventoryUI)) return;

	// 将 UI 添加到屏幕上
	InventoryUI->AddToViewport();
}

/**
 * @brief 绑定背包 UI 相关的事件
 * 
 * 该方法会监听 `UEveInventoryMgr` 的 `OnInventoryUpdated` 事件，以便在背包数据更新时刷新 UI。
 */
void UEveInventoryUI::BindUIEvent()
{
	// 获取背包管理子系统
	UEveInventoryMgr* InventorySubsystem = GetGameInstance()->GetSubsystem<UEveInventoryMgr>();

	// 绑定 `OnInventoryUpdated` 事件，使 `UpdateInventory` 在背包更新时被调用
	InventorySubsystem->OnInventoryUpdated.AddDynamic(this, &ThisClass::UpdateInventory);
}

/**
 * @brief 更新背包 UI
 * 
 * 该方法会清空 `UniformGridPanel`，并根据 `InventoryItems` 重新生成 `ItemWidget`。
 */
void UEveInventoryUI::UpdateInventory()
{
	// 确保 UI 和网格组件有效
	if (!ensure(InventoryUI)) return;
	if (!ensure(InventoryUI->Grid)) return;

	// 清理 UI 网格中的 `ItemWidget`
	for (const auto& ItemWidget : ItemUIPool)
	{
		InventoryUI->Grid->RemoveChild(ItemWidget.Get());
	}

	// 清空 `ItemUIPool`
	ItemUIPool.Empty();

	// 获取背包管理子系统
	UEveInventoryMgr* InventorySubsystem = GetGameInstance()->GetSubsystem<UEveInventoryMgr>();

	// 遍历 `InventoryItems`，创建对应的 UI 组件
	for (const auto& [ItemTID, InventoryItem] : InventorySubsystem->InventoryItems)
	{
		// 确保物品数据有效
		if (!ensure(InventoryItem)) continue;
		if (!ensure(InventorySubsystem->AllItemsCfg.Contains(InventoryItem->TID))) continue;
		if (!ensure(InventorySubsystem->AllItemsCfg[InventoryItem->TID]->ItemData.Icon)) continue;

		// 创建 `ItemWidget`（单个物品 UI）
		UEveItemWidget* ItemWidget = Cast<UEveItemWidget>(
			UUserWidget::CreateWidgetInstance(*GetGameInstance(), UEveAssetMgr::Get().ItemClass, 
			FName(*FString::Printf(TEXT("Item_%d"), ItemTID)))
		);

		// 确保 `ItemWidget` 创建成功
		if (!ensure(ItemWidget)) continue;

		// 赋值 `ItemWidget` 的 UI 组件数据
		ItemWidget->OwnerWidget = InventoryUI;
		ItemWidget->OwnerGrid = InventoryUI->Grid;
		ItemWidget->PosIdx = InventoryItem->PosIdx;
		ItemWidget->ItemTID = InventoryItem->TID;
		ItemWidget->Img->SetBrushFromTexture(InventorySubsystem->AllItemsCfg[InventoryItem->TID]->ItemData.Icon);

		// 存入 `ItemUIPool`
		ItemUIPool.Add(ItemWidget);

		// 计算物品在网格中的位置
		const int32 Row = InventoryItem->PosIdx / 3; // 3 列布局
		const int32 Col = InventoryItem->PosIdx % 3;

		// 将 `ItemWidget` 添加到 `UniformGrid`
		InventoryUI->Grid->AddChildToUniformGrid(ItemWidget, Row, Col);
	}
}

/**
 * @brief 释放背包 UI 资源
 * 
 * 该方法在 `GameInstance` 关闭时调用，取消 `OnInventoryUpdated` 事件绑定。
 */
void UEveInventoryUI::Deinitialize()
{
	Super::Deinitialize();

	// 获取背包管理子系统
	UEveInventoryMgr* InventorySubsystem = GetGameInstance()->GetSubsystem<UEveInventoryMgr>();

	// 取消 `OnInventoryUpdated` 事件绑定
	InventorySubsystem->OnInventoryUpdated.RemoveAll(this);
}
