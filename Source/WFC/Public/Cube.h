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
	Water,
};

UCLASS()
class WFC_API ACube : public AActor
{
	GENERATED_BODY()
	
public:	

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WFC")
	UStaticMeshComponent* CubeMeshComponent;
	
	ACube();

	UFUNCTION(BlueprintImplementableEvent)
	void Spawn(ECubeType CubeType);
};
