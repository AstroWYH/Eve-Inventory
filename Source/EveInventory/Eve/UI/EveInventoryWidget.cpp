// Copyright Night Gamer, Inc. All Rights Reserved.

#include "EveInventoryWidget.h"

#include "EveInventory/Eve/Manager/EveInventoryMgr.h"

void UEveInventoryWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (!ensure(Grid)) return;
}

void UEveInventoryWidget::DragToOtherEmptySlot(int32 TID, int32 OldPosIdx, int32 NewPosIdx) const
{
	UEveInventoryMgr* InventorySys = GetGameInstance()->GetSubsystem<UEveInventoryMgr>();
	InventorySys->RemoveItem(TID);

	TObjectPtr<UEveItem> Item = NewObject<UEveItem>();
	Item->TID = TID;
	if (!ensure(InventorySys->AllItemsCfg.Contains(TID))) return;
	
	Item->ItemCfg = InventorySys->AllItemsCfg[TID];
	InventorySys->AddItem(Item, NewPosIdx);
}

void UEveInventoryWidget::DragToExchange(int32 OldPosIdx, int32 NewPosIdx) const
{
	UEveInventoryMgr* InventorySys = GetGameInstance()->GetSubsystem<UEveInventoryMgr>();
	InventorySys->ExchangeItem(OldPosIdx, NewPosIdx);
}