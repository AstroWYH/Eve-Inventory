// Copyright Night Gamer, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "EveInventoryWidget.h"
#include "EveItemWidget.generated.h"

UCLASS()
class UEveItemWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

public:
	virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation) override;

	virtual void NativeOnDragCancelled(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;

	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "ItemWidget")
	bool bIsDragging = false;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "ItemWidget")
	int32 PosIdx = -1;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "ItemWidget")
	int32 ItemTID = -1;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "ItemWidget")
	TWeakObjectPtr<class UEveInventoryWidget> OwnerWidget;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "ItemWidget")
	TWeakObjectPtr<class UUniformGridPanel> OwnerGrid;

private:
	const float ScaleSize = 2.0f;
	const int32 KNumRows = 2;
	const int32 KNumColumns = 3;
	const TSet<int32> AllPosIdx{0, 1, 2, 3, 4, 5};

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(BindWidget))
	TObjectPtr<class UImage> Img;
};