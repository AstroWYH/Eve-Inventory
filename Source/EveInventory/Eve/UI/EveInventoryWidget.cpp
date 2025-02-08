// Copyright Night Gamer, Inc. All Rights Reserved.

#include "EveInventoryWidget.h"
#include "EveInventory/Eve/Manager/EveInventoryMgr.h"

/**
 * @brief UI 初始化
 * 
 * 该方法在 `UserWidget` 被创建时调用，确保 `Grid` 组件有效。
 */
void UEveInventoryWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// 确保 `Grid` 组件存在，否则 UI 无法正常显示
	if (!ensure(Grid)) return;
}

/**
 * @brief 拖拽物品到一个空格（旧位置清除）
 * 
 * - 先移除旧物品
 * - 创建新物品并放置到新的位置
 * 
 * @param TID 物品的唯一 ID
 * @param OldPosIdx 旧位置索引
 * @param NewPosIdx 新位置索引
 */
void UEveInventoryWidget::DragToOtherEmptySlot(int32 TID, int32 OldPosIdx, int32 NewPosIdx) const
{
	// 获取背包管理系统
	UEveInventoryMgr* InventorySys = GetGameInstance()->GetSubsystem<UEveInventoryMgr>();
    
	// 先移除旧位置的物品
	InventorySys->RemoveItem(TID);

	// 创建新物品实例
	TObjectPtr<UEveItem> Item = NewObject<UEveItem>();
	Item->TID = TID;

	// 确保 `AllItemsCfg` 中存在该 `TID` 对应的配置
	if (!ensure(InventorySys->AllItemsCfg.Contains(TID))) return;
    
	// 关联物品配置
	Item->ItemCfg = InventorySys->AllItemsCfg[TID];

	// 将物品添加到新位置
	InventorySys->AddItem(Item, NewPosIdx);
}

/**
 * @brief 交换两个物品的位置
 * 
 * - 交换 `OldPosIdx` 和 `NewPosIdx` 位置上的物品
 * 
 * @param OldPosIdx 旧位置索引
 * @param NewPosIdx 新位置索引
 */
void UEveInventoryWidget::DragToExchange(int32 OldPosIdx, int32 NewPosIdx) const
{
	// 获取背包管理系统
	UEveInventoryMgr* InventorySys = GetGameInstance()->GetSubsystem<UEveInventoryMgr>();

	// 调用 `ExchangeItem` 方法，交换两个位置的物品
	InventorySys->ExchangeItem(OldPosIdx, NewPosIdx);
}
