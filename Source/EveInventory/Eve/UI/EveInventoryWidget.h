// Copyright Night Gamer, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "EveInventoryWidget.generated.h"

/**
 * @brief 背包 UI 组件
 * 
 * `UEveInventoryWidget` 继承自 `UUserWidget`，用于管理和显示玩家的背包 UI。
 * 主要功能：
 * - 监听 UI 构造事件
 * - 处理物品拖拽：拖拽物品到空格、交换物品位置
 */
UCLASS()
class UEveInventoryWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	/** 
	 * @brief UI 初始化
	 * 
	 * 该方法在 `UserWidget` 被构造时调用，用于确保 `Grid` 组件正确绑定。
	 */
	virtual void NativeConstruct() override;
	
public:
	/**
	 * @brief 拖拽物品到一个空格（旧位置清除）
	 * 
	 * - 先移除旧物品
	 * - 在新位置创建新的物品对象
	 * 
	 * @param TID 物品唯一 ID
	 * @param OldPosIdx 旧位置索引
	 * @param NewPosIdx 新位置索引
	 */
	void DragToOtherEmptySlot(int32 TID, int32 OldPosIdx, int32 NewPosIdx) const;

	/**
	 * @brief 交换两个物品的位置
	 * 
	 * - 交换 `OldPosIdx` 和 `NewPosIdx` 位置上的物品
	 * 
	 * @param OldPosIdx 旧位置索引
	 * @param NewPosIdx 新位置索引
	 */
	void DragToExchange(int32 OldPosIdx, int32 NewPosIdx) const;

public:
	/** 
	 * @brief 背包网格组件
	 * 
	 * `UniformGridPanel` 组件用于存放 `EveItemWidget`（单个物品 UI）。
	 * 在 `UMG` 蓝图中需要绑定该控件，以支持 UI 逻辑。
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(BindWidget))
	TObjectPtr<class UUniformGridPanel> Grid;
};
