// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Cube.h"

#include "Terrain.generated.h"

USTRUCT(BlueprintType)
struct FCell
{
	GENERATED_BODY()
public:
	ECubeType CubeType = ECubeType::None;
	bool IsObserved = false;
	TArray<ECubeType> AllPossiableCubeTypeArr;
};

USTRUCT(BlueprintType)
struct FLinkRule
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WFC")
	TArray<ECubeType> Front;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WFC")
	TArray<ECubeType> Back;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WFC")
	TArray<ECubeType> Left;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WFC")
	TArray<ECubeType> Right;
};

UCLASS()
class ATerrain : public AActor
{
	GENERATED_BODY()

public:
	int CubeNum = 10;
	TArray<TArray<FCell>> TerrainMatrix;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="WFC")
	TSubclassOf<ACube> CubeClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WFC")
	TMap<ECubeType, FLinkRule> LinkRule;

	ATerrain();
	//获取当前单元格的熵值
	float GetEntropy(FCell Cell);
	UFUNCTION(BlueprintCallable)
	//指定位置观测,返回是否塌缩
	bool Observe(int X, int Y);
	//传播
	void Propagate(int OriginalX, int OriginalY);
	//状态塌缩
	void Collapse(TArray<ECubeType> RuleAllowedCubeTypeArr, int CollapseX, int CollapseY);

	UFUNCTION(BlueprintCallable)
	//生成地形方块
	void SpawnCube(ECubeType CubeType,int SpawnX,int SpawnY);
	UFUNCTION(BlueprintCallable)
	//本地坐标转化成世界坐标
	FVector LocalToWorld(FVector LocalLoc);
	UFUNCTION(BlueprintCallable)
	//世界坐标转化成本地坐标
	FVector WorldToLocal(FVector WorldLoc);
};

