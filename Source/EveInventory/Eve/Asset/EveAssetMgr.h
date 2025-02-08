// Copyright Night Gamer, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/AssetManager.h"
#include "Engine/DataTable.h"
#include "EveAssetMgr.generated.h"

UCLASS(BlueprintType, Blueprintable)
class EVEINVENTORY_API UEveAssetMgr : public UAssetManager
{
	GENERATED_BODY()

public:
	static UEveAssetMgr& Get()
	{
		check(GEngine);

		if (UEveAssetMgr* Singleton = Cast<UEveAssetMgr>(GEngine->AssetManager))
		{
			return *Singleton;
		}

		UE_LOG(LogTemp, Fatal, TEXT("Invalid AssetManager!"));
		return *NewObject<UEveAssetMgr>();
	}

	template <typename AssetClass>
	AssetClass* GetAssetSync(const TSoftObjectPtr<AssetClass>& SoftPtr)
	{
		AssetClass* Asset = SoftPtr.LoadSynchronous();
		return Asset;
	}

	UPROPERTY(EditDefaultsOnly, Category = "DT")
	TSoftObjectPtr<UDataTable> DTItem;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> InventoryClass;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> ItemClass;
};