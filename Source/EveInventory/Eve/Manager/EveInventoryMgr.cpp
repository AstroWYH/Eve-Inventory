// Copyright Night Gamer, Inc. All Rights Reserved.

#include "EveInventoryMgr.h"
#include "Engine/DataTable.h"
#include "EveInventory/Eve/Asset/EveAssetMgr.h"

/**
 * 初始化库存管理器，加载物品数据表。
 */
void UEveInventoryMgr::Initialize(FSubsystemCollectionBase& Collection)
{
    Super::Initialize(Collection);

    // 获取物品数据表
    const UDataTable* DataTable = UEveAssetMgr::Get().GetAssetSync(UEveAssetMgr::Get().DTItem);
    TArray<FEveItemData*> AllFItemsCfg;
    DataTable->GetAllRows<FEveItemData>(TEXT("ItemDataContext"), AllFItemsCfg);

    // 将数据表中的数据存入 AllItemsCfg
    for (const FEveItemData* FItemData : AllFItemsCfg)
    {
        if (!ensure(FItemData)) return;
        AllItemsCfg.Add(FItemData->TID, UEveItemCfg::CreateFromStruct(*FItemData));
    }
}

/**
 * 测试添加物品按钮功能，确保不会重复添加相同的物品。
 */
void UEveInventoryMgr::InventoryTestAddBtn()
{
    if (AddedItemsSet.Num() >= 6) return; // 背包最多存放 6 个物品

    // 计算当前可用的 TID（排除已添加的 TID）
    TArray<int32> AvailableTIDs;
    for (int32 TID = 100; TID <= 105; ++TID)
    {
        if (!AddedItemsSet.Contains(TID))
        {
            AvailableTIDs.Add(TID);
        }
    }

    if (AvailableTIDs.Num() == 0) return; // 没有可添加的物品

    // 随机选择一个可用的 TID
    int32 SelectedTID = AvailableTIDs[FMath::RandRange(0, AvailableTIDs.Num() - 1)];
    TObjectPtr<UEveItem> Item = NewObject<UEveItem>();
    Item->TID = SelectedTID;
    if (!ensure(AllItemsCfg.Contains(SelectedTID))) return;
    Item->ItemCfg = AllItemsCfg[SelectedTID];

    // 添加到集合
    AddedItemsSet.Add(SelectedTID);
    AddedItemsStack.Push(SelectedTID);

    AddItem(Item);
}

/**
 * 测试移除物品按钮功能。
 */
void UEveInventoryMgr::InventoryTestRemoveBtn()
{
    if (AddedItemsStack.Num() == 0) return; // 没有物品可以移除

    int32 LastAddedTID = AddedItemsStack.Pop(); // 获取最后添加的物品
    AddedItemsSet.Remove(LastAddedTID); // 从集合中移除

    RemoveItem(LastAddedTID);
}

/**
 * 添加物品到库存。
 */
void UEveInventoryMgr::AddItem(const TObjectPtr<UEveItem> Item, const int32 PosIdx)
{
    if (!ensure(Item)) return;
    if (!ensure(InventoryItems.Num() < SlotNum)) return; // 背包容量限制

    int32 TID = Item->TID;

    // 物品已存在，则增加数量
    if (InventoryItems.Contains(TID))
    {
        if (!ensure(InventoryItems[TID])) return;
        InventoryItems[TID]->Amount++;
    }
    else
    {
        TObjectPtr<UEveInventoryItem> NewInventoryItem = NewObject<UEveInventoryItem>();
        int32 SavePosIdx = 0;

        // 计算第一个空位存放物品
        if (PosIdx == -1)
        {
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
        else
        {
            SavePosIdx = PosIdx;
        }

        NewInventoryItem->Init(TID, 1, SavePosIdx);
        InventoryItems.Add(TID, NewInventoryItem);
        CurPosIdxes.Add(SavePosIdx);
        PosToTIDMap.Add(SavePosIdx, TID);
    }

    OnInventoryUpdated.Broadcast(); // 触发库存更新事件
}

/**
 * 从库存移除物品。
 */
void UEveInventoryMgr::RemoveItem(const int32 TID)
{
    if (!ensure(InventoryItems.Contains(TID))) return;
    if (!ensure(InventoryItems[TID])) return;

    CurPosIdxes.Remove(InventoryItems[TID]->PosIdx);
    PosToTIDMap.Remove(InventoryItems[TID]->PosIdx);
    InventoryItems.Remove(TID);

    OnInventoryUpdated.Broadcast(); // 触发库存更新事件
}

/**
 * 交换两个物品的位置。
 */
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

    OnInventoryUpdated.Broadcast(); // 触发库存更新事件
}

/**
 * 反初始化，清空所有库存数据。
 */
void UEveInventoryMgr::Deinitialize()
{
    Super::Deinitialize();

    InventoryItems.Empty();
    CurPosIdxes.Empty();
    PosToTIDMap.Empty();
    AddedItemsStack.Empty();
}
