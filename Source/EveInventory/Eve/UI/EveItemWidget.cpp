// Copyright Night Gamer, Inc. All Rights Reserved.

#include "EveItemWidget.h"

#include "Blueprint/DragDropOperation.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Components/UniformGridPanel.h"
#include "Blueprint/WidgetTree.h"
#include "Components/Image.h"
#include "Components/SizeBox.h"
#include "EveInventory/Eve/Manager/EveInventoryMgr.h"
#include "Framework/Application/SlateApplication.h"

/**
 * @brief 物品 UI 组件初始化
 * 
 * `NativeConstruct()` 在 `UUserWidget` 构造时调用，确保 `Img` 组件正确绑定。
 */
void UEveItemWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// 确保物品图片 `Img` 组件已绑定
	if (!ensure(Img)) return;
}

/**
 * @brief 处理物品拖拽开始事件
 * 
 * `NativeOnDragDetected()` 触发时：
 * - 创建一个 `UImage` 作为拖拽时的视觉对象
 * - 生成 `UDragDropOperation`，用于拖拽操作
 * - 将当前物品的 `TID` 传入 `Payload`
 * - 隐藏当前 `ItemWidget`
 * 
 * @param InGeometry 物品 UI 的几何信息
 * @param InMouseEvent 鼠标事件信息
 * @param OutOperation 拖拽操作（输出参数）
 */
void UEveItemWidget::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)
{
	Super::NativeOnDragDetected(InGeometry, InMouseEvent, OutOperation);

	// 获取库存管理系统
	UEveInventoryMgr* InventorySys = GetGameInstance()->GetSubsystem<UEveInventoryMgr>();
	if (!ensure(InventorySys->AllItemsCfg.Contains(ItemTID))) return;
	if (!ensure(InventorySys->AllItemsCfg[ItemTID])) return;

	// 创建拖拽时的图片
	UImage* DragImage = NewObject<UImage>();
	UTexture2D* DragTexture = InventorySys->AllItemsCfg[ItemTID]->ItemData.Icon;
	DragImage->SetBrushFromTexture(DragTexture);
	DragImage->SetRenderScale(FVector2D(ScaleSize));

	// 创建拖拽操作
	UDragDropOperation* DragDropOpr = NewObject<UDragDropOperation>(GetTransientPackage());
	DragDropOpr->DefaultDragVisual = DragImage;  // 设置拖拽显示的 UI
	DragDropOpr->Payload = this;  // 传递当前 `ItemWidget`
	DragDropOpr->Pivot = EDragPivot::CenterCenter;
	OutOperation = DragDropOpr;

	// 隐藏原来的 `ItemWidget`
	SetVisibility(ESlateVisibility::Hidden);
}

/**
 * @brief 处理拖拽取消事件（物品未成功放置）
 * 
 * `NativeOnDragCancelled()` 触发时：
 * - 计算鼠标是否在 `UniformGridPanel` 背包网格范围内
 * - 判断是否拖拽到了空格子或已有物品的格子
 * - 根据情况：
 *   - 还原 `ItemWidget` 可见性（如果无效拖拽）
 *   - 调用 `DragToOtherEmptySlot()` 或 `DragToExchange()`
 * 
 * @param InDragDropEvent 拖拽事件
 * @param InOperation 拖拽操作
 */
void UEveItemWidget::NativeOnDragCancelled(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Super::NativeOnDragCancelled(InDragDropEvent, InOperation);
	if (!ensure(OwnerGrid.IsValid())) return;

	// 获取鼠标位置
	FVector2D MousePosition = InDragDropEvent.GetScreenSpacePosition();
	FVector2D GridMousePosition = OwnerGrid->GetCachedGeometry().AbsoluteToLocal(MousePosition);
	FVector2D GridSize = OwnerGrid->GetCachedGeometry().GetLocalSize();

	// 计算网格单元格大小
	float CellWidth = GridSize.X / KNumColumns;
	float CellHeight = GridSize.Y / KNumRows;

	// 计算鼠标所处的行列索引
	int32 Row = FMath::FloorToInt(GridMousePosition.Y / CellHeight);
	int32 Col = FMath::FloorToInt(GridMousePosition.X / CellWidth);
	int32 MousePosIdx = Row * KNumColumns + Col;

	// 判断鼠标是否超出背包网格范围
	bool bCrossBorder = false;
	if (Row < 0 || Row >= KNumRows || Col < 0 || Col >= KNumColumns)
	{
		bCrossBorder = true;
	}

	// 获取库存管理系统
	UEveInventoryMgr* InventorySys = GetGameInstance()->GetSubsystem<UEveInventoryMgr>();
	if (!ensure(OwnerWidget.IsValid())) return;

	// 处理拖拽取消逻辑：
	if (MousePosIdx == PosIdx || !AllPosIdx.Contains(MousePosIdx) || bCrossBorder)
	{
		// 1. 还原 `ItemWidget` 可见性（无效拖拽）
		SetVisibility(ESlateVisibility::Visible);
	}
	else if (!InventorySys->CurPosIdxes.Contains(MousePosIdx))
	{
		// 2. 拖拽到空格子
		OwnerWidget->DragToOtherEmptySlot(ItemTID, PosIdx, MousePosIdx);
	}
	else if (InventorySys->CurPosIdxes.Contains(MousePosIdx))
	{
		// 3. 拖拽到另一个物品的位置（交换）
		OwnerWidget->DragToExchange(PosIdx, MousePosIdx);
	}
}

/**
 * @brief 处理鼠标按下事件（用于开始拖拽）
 * 
 * `NativeOnMouseButtonDown()` 触发时：
 * - 检测鼠标左键按下
 * - 调用 `DetectDragIfPressed()` 触发拖拽事件
 * 
 * @param InGeometry 物品 UI 的几何信息
 * @param InMouseEvent 鼠标事件信息
 * @return `FReply` 处理结果
 */
FReply UEveItemWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);

	// 监听鼠标左键点击，开始拖拽
	return UWidgetBlueprintLibrary::DetectDragIfPressed(InMouseEvent, this, EKeys::LeftMouseButton).NativeReply;
}
