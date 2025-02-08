// Copyright Night Gamer, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "EveInventoryWidget.generated.h"

UCLASS()
class UEveInventoryWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	
public:
	void DragToOtherEmptySlot(int32 TID, int32 OldPosIdx, int32 NewPosIdx) const;

	void DragToExchange(int32 OldPosIdx, int32 NewPosIdx) const;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(BindWidget))
	TObjectPtr<class UUniformGridPanel> Grid;
};
