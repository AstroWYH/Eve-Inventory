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

void UEveItemWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (!ensure(Img)) return;
}

void UEveItemWidget::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)
{
	Super::NativeOnDragDetected(InGeometry, InMouseEvent, OutOperation);

	UEveInventoryMgr* InventorySys = GetGameInstance()->GetSubsystem<UEveInventoryMgr>();
	if (!ensure(InventorySys->AllItemsCfg.Contains(ItemTID))) return;
	if (!ensure(InventorySys->AllItemsCfg[ItemTID])) return;

	UImage* DragImage = NewObject<UImage>();
	UTexture2D* DragTexture = InventorySys->AllItemsCfg[ItemTID]->ItemData.Icon;
	DragImage->SetBrushFromTexture(DragTexture);
	DragImage->SetRenderScale(FVector2D(ScaleSize));

	UDragDropOperation* DragDropOpr = NewObject<UDragDropOperation>(GetTransientPackage());
	DragDropOpr->DefaultDragVisual = DragImage;
	DragDropOpr->Payload = this;
	DragDropOpr->Pivot = EDragPivot::CenterCenter;
	OutOperation = DragDropOpr;

	SetVisibility(ESlateVisibility::Hidden);
}

void UEveItemWidget::NativeOnDragCancelled(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Super::NativeOnDragCancelled(InDragDropEvent, InOperation);
	if (!ensure(OwnerGrid.IsValid())) return;

	FVector2D MousePosition = InDragDropEvent.GetScreenSpacePosition();
	FVector2D GridMousePosition = OwnerGrid->GetCachedGeometry().AbsoluteToLocal(MousePosition);
	FVector2D GirdSize = OwnerGrid->GetCachedGeometry().GetLocalSize();

	float CellWidth = GirdSize.X / KNumColumns;
	float CellHeight = GirdSize.Y / KNumRows;

	int32 Row = FMath::FloorToInt(GridMousePosition.Y / CellHeight);
	int32 Col = FMath::FloorToInt(GridMousePosition.X / CellWidth);
	int32 MousePosIdx = Row * KNumColumns + Col;

	bool bCrossBorder = false;
	if (Row < 0 || Row >= KNumRows || Col < 0 || Col >= KNumColumns)
	{
		bCrossBorder = true;
	}

	UEveInventoryMgr* InventorySys = GetGameInstance()->GetSubsystem<UEveInventoryMgr>();
	if (!ensure(OwnerWidget.IsValid())) return;

	if (MousePosIdx == PosIdx || !AllPosIdx.Contains(MousePosIdx) || bCrossBorder)
	{
		SetVisibility(ESlateVisibility::Visible);
	}
	else if (!InventorySys->CurPosIdxes.Contains(MousePosIdx))
	{
		OwnerWidget->DragToOtherEmptySlot(ItemTID, PosIdx, MousePosIdx);
	}
	else if (InventorySys->CurPosIdxes.Contains(MousePosIdx))
	{
		OwnerWidget->DragToExchange(PosIdx, MousePosIdx);
	}
}

FReply UEveItemWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);

	return UWidgetBlueprintLibrary::DetectDragIfPressed(InMouseEvent, this, EKeys::LeftMouseButton).NativeReply;
}