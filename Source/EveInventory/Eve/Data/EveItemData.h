// Copyright Night Gamer, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "EveItemData.generated.h"

class UEveItemCfg;

USTRUCT(Blueprintable, BlueprintType)
struct FEveItemData : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly)
	int32 TID;

	UPROPERTY(EditDefaultsOnly)
	FString Name;

	UPROPERTY(EditDefaultsOnly)
	UTexture2D* Icon;
};

UCLASS(BlueprintType)
class UEveInventoryItem : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, Category = "Inventory")
	int32 TID = -1;

	UPROPERTY(BlueprintReadWrite, Category = "Inventory")
	int32 Amount = 0;

	UPROPERTY(BlueprintReadWrite, Category = "Inventory")
	int32 PosIdx = 0;

	void Init(int32 InTID, int32 InAmount, int32 InPosIdx)
	{
		TID = InTID;
		Amount = InAmount;
		PosIdx = InPosIdx;
	}
};

UCLASS(BlueprintType)
class UEveItem : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, Category = "Inventory")
	int32 TID = -1;

	UPROPERTY(BlueprintReadWrite, Category = "Inventory")
	int32 XID = -1;

	UPROPERTY(BlueprintReadWrite, Category = "Inventory")
	TWeakObjectPtr<UEveItemCfg> ItemCfg = nullptr;
};

UCLASS(Blueprintable, BlueprintType)
class UEveItemCfg : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly)
	FEveItemData ItemData;

	UFUNCTION(BlueprintCallable, Category = "Item")
	static UEveItemCfg* CreateFromStruct(const FEveItemData& Data)
	{
		UEveItemCfg* Obj = NewObject<UEveItemCfg>();
		Obj->ItemData = Data;
		return Obj;
	}
};