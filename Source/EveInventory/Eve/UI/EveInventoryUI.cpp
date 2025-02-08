// Copyright Night Gamer, Inc. All Rights Reserved.


#include "EveInventoryUI.h"

#include "EveInventoryWidget.h"
#include "EveItemWidget.h"
#include "Components/Image.h"
#include "Components/UniformGridPanel.h"
#include "EveInventory/Eve/Asset/EveAssetMgr.h"
#include "EveInventory/Eve/Manager/EveInventoryMgr.h"

void UEveInventoryUI::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	GetWorld()->GetTimerManager().SetTimerForNextTick([this]()
	{
		CreateUI();
		BindUIEvent();
	});
}

void UEveInventoryUI::CreateUI()
{
	InventoryUI = Cast<UEveInventoryWidget>(UUserWidget::CreateWidgetInstance(*GetGameInstance(), UEveAssetMgr::Get().InventoryClass, TEXT("Inventory")));
	if (!ensure(InventoryUI)) return;

	InventoryUI->AddToViewport();
}

void UEveInventoryUI::BindUIEvent()
{
	UEveInventoryMgr* InventorySubsystem = GetGameInstance()->GetSubsystem<UEveInventoryMgr>();
	InventorySubsystem->OnInventoryUpdated.AddDynamic(this, &ThisClass::UpdateInventory);
}

void UEveInventoryUI::UpdateInventory()
{
	if (!ensure(InventoryUI)) return;
	if (!ensure(InventoryUI->Grid)) return;

	// 清理背包UI网格的ItemWidget
	for (const auto& ItemWidget : ItemUIPool)
	{
		InventoryUI->Grid->RemoveChild(ItemWidget.Get());
	}

	// 清理ItemUIPool引用池
	ItemUIPool.Empty();

	// 重新生成背包内全部ItemWidget
	UEveInventoryMgr* InventorySubsystem = GetGameInstance()->GetSubsystem<UEveInventoryMgr>();
	for (const auto& [ItemTID, InventoryItem] : InventorySubsystem->InventoryItems)
	{
		if (!ensure(InventoryItem)) continue;
		if (!ensure(InventorySubsystem->AllItemsCfg.Contains(InventoryItem->TID))) continue;
		if (!ensure(InventorySubsystem->AllItemsCfg[InventoryItem->TID]->ItemData.Icon)) continue;

		// 生成ItemWidget
		UEveItemWidget* ItemWidget = Cast<UEveItemWidget>(UUserWidget::CreateWidgetInstance(*GetGameInstance(), UEveAssetMgr::Get().ItemClass, FName(*FString::Printf(TEXT("Item_%d"), ItemTID))));
		if (!ensure(ItemWidget)) continue;

		ItemWidget->OwnerWidget = InventoryUI;
		ItemWidget->OwnerGrid = InventoryUI->Grid;
		ItemWidget->PosIdx = InventoryItem->PosIdx;
		ItemWidget->ItemTID = InventoryItem->TID;
		ItemWidget->Img->SetBrushFromTexture(InventorySubsystem->AllItemsCfg[InventoryItem->TID]->ItemData.Icon);
		ItemUIPool.Add(ItemWidget);

		// ItemWidget添加到Grid
		const int32 Row = InventoryItem->PosIdx / 3;
		const int32 Col = InventoryItem->PosIdx % 3;
		InventoryUI->Grid->AddChildToUniformGrid(ItemWidget, Row, Col);
	}
}

void UEveInventoryUI::Deinitialize()
{
	Super::Deinitialize();

	UEveInventoryMgr* InventorySubsystem = GetGameInstance()->GetSubsystem<UEveInventoryMgr>();
	InventorySubsystem->OnInventoryUpdated.RemoveAll(this);
}