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
	//��ȡ��ǰ��Ԫ�����ֵ
	float GetEntropy(FCell Cell);
	UFUNCTION(BlueprintCallable)
	//ָ��λ�ù۲�,�����Ƿ�����
	bool Observe(int X, int Y);
	//����
	void Propagate(int OriginalX, int OriginalY);
	//״̬����
	void Collapse(TArray<ECubeType> RuleAllowedCubeTypeArr, int CollapseX, int CollapseY);

	UFUNCTION(BlueprintCallable)
	//���ɵ��η���
	void SpawnCube(ECubeType CubeType,int SpawnX,int SpawnY);
	UFUNCTION(BlueprintCallable)
	//��������ת������������
	FVector LocalToWorld(FVector LocalLoc);
	UFUNCTION(BlueprintCallable)
	//��������ת���ɱ�������
	FVector WorldToLocal(FVector WorldLoc);
};

