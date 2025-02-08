// Copyright Night Gamer, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "EveInventoryWidget.h"
#include "EveItemWidget.generated.h"

/**
 * @brief 背包物品 UI 组件
 * 
 * `UEveItemWidget` 继承自 `UUserWidget`，用于显示和管理背包中的单个物品。
 * 主要功能：
 * - 监听物品拖拽事件
 * - 响应鼠标点击事件
 * - 处理物品在网格中的显示
 */
UCLASS()
class UEveItemWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	/** 
	 * @brief UI 初始化
	 * 
	 * 该方法在 `UserWidget` 被构造时调用，用于确保物品 UI 正确绑定和初始化。
	 */
	virtual void NativeConstruct() override;

public:
	/**
	 * @brief 拖拽开始时调用
	 * 
	 * - 启动拖拽操作。
	 * 
	 * @param InGeometry 当前控件的几何信息
	 * @param InMouseEvent 鼠标事件信息
	 * @param OutOperation 输出的拖拽操作
	 */
	virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation) override;

	/**
	 * @brief 拖拽取消时调用
	 * 
	 * - 处理拖拽取消事件，恢复物品状态。
	 * 
	 * @param InDragDropEvent 拖拽事件信息
	 * @param InOperation 拖拽操作实例
	 */
	virtual void NativeOnDragCancelled(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;

	/**
	 * @brief 鼠标点击时调用
	 * 
	 * - 处理鼠标按下事件，通常用于物品的选择或交互。
	 * 
	 * @param InGeometry 当前控件的几何信息
	 * @param InMouseEvent 鼠标事件信息
	 * @return 返回 FReply 以继续处理事件
	 */
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

public:
	/** 
	 * @brief 物品是否正在被拖拽
	 * 
	 * `bIsDragging` 用于表示物品是否被用户拖动。
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "ItemWidget")
	bool bIsDragging = false;

	/** 
	 * @brief 物品的网格位置索引
	 * 
	 * `PosIdx` 表示物品在网格中的位置，默认为 -1，表示物品未放置。
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "ItemWidget")
	int32 PosIdx = -1;

	/** 
	 * @brief 物品的唯一 ID
	 * 
	 * `ItemTID` 用于唯一标识物品。
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "ItemWidget")
	int32 ItemTID = -1;

	/** 
	 * @brief 物品所属的背包 UI 控件
	 * 
	 * `OwnerWidget` 用于指向背包的 UI 控件，避免循环引用。
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "ItemWidget")
	TWeakObjectPtr<class UEveInventoryWidget> OwnerWidget;

	/** 
	 * @brief 物品所在的网格控件
	 * 
	 * `OwnerGrid` 用于存储物品的网格控件。
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "ItemWidget")
	TWeakObjectPtr<class UUniformGridPanel> OwnerGrid;

private:
	/** 
	 * @brief 物品在拖拽时的缩放比例
	 * 
	 * `ScaleSize` 控制拖拽时物品的大小。
	 */
	const float ScaleSize = 2.0f;

	/** 
	 * @brief 网格的行数
	 * 
	 * `KNumRows` 用于定义网格的行数，决定物品槽的布局。
	 */
	const int32 KNumRows = 2;

	/** 
	 * @brief 网格的列数
	 * 
	 * `KNumColumns` 用于定义网格的列数，决定物品槽的布局。
	 */
	const int32 KNumColumns = 3;

	/** 
	 * @brief 所有有效的物品位置索引
	 * 
	 * `AllPosIdx` 定义了物品可能的有效位置，确保背包有足够的槽位。
	 */
	const TSet<int32> AllPosIdx{0, 1, 2, 3, 4, 5};

public:
	/** 
	 * @brief 物品显示图像控件
	 * 
	 * `Img` 控件用于显示物品的图像，在蓝图中绑定该控件来显示物品。
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(BindWidget))
	TObjectPtr<class UImage> Img;
};
