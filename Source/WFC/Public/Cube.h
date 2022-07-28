// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Cube.generated.h"

UENUM(BlueprintType)
enum class ECubeType :uint8
{
	None,
	Soil,
	Grass,
	Water
};

UENUM(BlueprintType)
enum class EDirection :uint8
{
	Back,
	Front,
	Left,
	Right
};

USTRUCT(BlueprintType)
struct FAllowedType
{
	GENERATED_BODY()
public:
	UPROPERTY()
	TArray<ECubeType> AllowedType;
};

UCLASS()
class ACube : public AActor
{
	GENERATED_BODY()

public:
	ECubeType CubeType = ECubeType::None;
	bool IsObserved = false;
	TArray<ECubeType> AllPossiableCubeTypeArr;

	UPROPERTY(EditAnywhere, Category = "WFC")
	//∆¥Ω”πÊ‘Ú
	TMap<EDirection,FAllowedType> LinkRule;

	ACube();
	//ACube(const ACube& Cube);
};
