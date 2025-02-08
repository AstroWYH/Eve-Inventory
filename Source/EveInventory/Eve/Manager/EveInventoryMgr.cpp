// Copyright Night Gamer, Inc. All Rights Reserved.

#include "EveInventoryMgr.h"
#include "Engine/DataTable.h"
#include "EveInventory/Eve/Asset/EveAssetMgr.h"

void UEveInventoryMgr::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	const UDataTable* DataTable = UEveAssetMgr::Get().GetAssetSync(UEveAssetMgr::Get().DTItem);
	TArray<FEveItemData*> AllFItemsCfg;
	DataTable->GetAllRows<FEveItemData>(TEXT("ItemDataContext"), AllFItemsCfg);

	for (const FEveItemData* FItemData : AllFItemsCfg)
	{
		if (!ensure(FItemData)) return;
		AllItemsCfg.Add(FItemData->TID, UEveItemCfg::CreateFromStruct(*FItemData));
	}
}

void UEveInventoryMgr::InventoryTestAddBtn()
{
	if (AddedItemsSet.Num() >= 6) return;

	TArray<int32> AvailableTIDs;
	for (int32 TID = 100; TID <= 105; ++TID)
	{
		if (!AddedItemsSet.Contains(TID))
		{
			AvailableTIDs.Add(TID);
		}
	}

	if (AvailableTIDs.Num() == 0) return;

	int32 SelectedTID = AvailableTIDs[FMath::RandRange(0, AvailableTIDs.Num() - 1)];
	TObjectPtr<UEveItem> Item = NewObject<UEveItem>();
	Item->TID = SelectedTID;
	if (!ensure(AllItemsCfg.Contains(SelectedTID))) return;
	Item->ItemCfg = AllItemsCfg[SelectedTID];

	AddedItemsSet.Add(SelectedTID);
	AddedItemsStack.Push(SelectedTID);

	AddItem(Item);
}

void UEveInventoryMgr::InventoryTestRemoveBtn()
{
	if (AddedItemsStack.Num() == 0) return;

	int32 LastAddedTID = AddedItemsStack.Pop();
	AddedItemsSet.Remove(LastAddedTID);

	RemoveItem(LastAddedTID);
}

void UEveInventoryMgr::AddItem(const TObjectPtr<UEveItem> Item, const int32 PosIdx)
{
	// 不能超过格子总数
	if (!ensure(InventoryItems.Num() < SlotNum)) return;

	if (!ensure(Item)) return;
	int32 TID = Item->TID;

	if (InventoryItems.Contains(TID))
	{
		if (!ensure(InventoryItems[TID])) return;
		InventoryItems[TID]->Amount++;
	}
	else
	{
		TObjectPtr<UEveInventoryItem> NewInventoryItem = NewObject<UEveInventoryItem>();
		int32 SavePosIdx = 0;

		// 自动安排升序第1个空位存放
		if (PosIdx == -1)
		{
			// 因为Remove位置的不确定性，每次重新计算SavePosIdx的位置
			for (int32 Idx = 0; Idx < SlotNum; Idx++)
			{
				if (CurPosIdxes.Contains(Idx))
				{
					SavePosIdx++;
				}
				else
				{
					break;
				}
			}
		}
		// 指定位置存放
		else
		{
			SavePosIdx = PosIdx;
		}

		NewInventoryItem->Init(TID, 1, SavePosIdx);
		InventoryItems.Add(TID, NewInventoryItem);
		CurPosIdxes.Add(SavePosIdx);
		PosToTIDMap.Add(SavePosIdx, TID);
	}

	OnInventoryUpdated.Broadcast();
}

void UEveInventoryMgr::RemoveItem(const int32 TID)
{
	if (!ensure(InventoryItems.Contains(TID))) return;
	if (!ensure(InventoryItems[TID])) return;

	CurPosIdxes.Remove(InventoryItems[TID]->PosIdx);
	PosToTIDMap.Remove(InventoryItems[TID]->PosIdx);
	InventoryItems.Remove(TID);
	OnInventoryUpdated.Broadcast();
}

void UEveInventoryMgr::ExchangeItem(const int32 OldPosIdx, const int32 NewPosIdx)
{
	if (!ensure(CurPosIdxes.Contains(OldPosIdx))) return;
	if (!ensure(CurPosIdxes.Contains(NewPosIdx))) return;
	if (!ensure(PosToTIDMap.Contains(OldPosIdx))) return;
	if (!ensure(PosToTIDMap.Contains(NewPosIdx))) return;

	int32 OldTID = PosToTIDMap[OldPosIdx];
	int32 NewTID = PosToTIDMap[NewPosIdx];

	TObjectPtr<UEveInventoryItem>& OldInventoryItem = InventoryItems.FindOrAdd(OldTID);
	if (!ensure(OldInventoryItem)) return;
	OldInventoryItem->PosIdx = NewPosIdx;

	TObjectPtr<UEveInventoryItem>& NewInventoryItem = InventoryItems.FindOrAdd(NewTID);
	if (!ensure(NewInventoryItem)) return;
	NewInventoryItem->PosIdx = OldPosIdx;

	PosToTIDMap[OldPosIdx] = NewTID;
	PosToTIDMap[NewPosIdx] = OldTID;

	OnInventoryUpdated.Broadcast();
}

void UEveInventoryMgr::Deinitialize()
{
	Super::Deinitialize();

	InventoryItems.Empty();
	CurPosIdxes.Empty();
	PosToTIDMap.Empty();
	AddedItemsStack.Empty();
}