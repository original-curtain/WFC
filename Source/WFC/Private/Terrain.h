// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Cube.h"

#include "Terrain.generated.h"



UCLASS()
class ATerrain : public AActor
{
	GENERATED_BODY()
	
public:
	int CubeNum=100;
	TArray<TArray<ACube>> TerrainMatrix;

	ATerrain();

	//获取当前单元格的熵值
	float GetEntropy(ACube Cube);
	//指定位置观测
	void Observe(int X, int Y);
	//传播
	void Propagate(int OriginalX, int OriginalY);
	//状态塌缩
	void Collapse(TArray<ECubeType> RuleAllowedCubeTypeArr, int CollapseX, int CollapseY);
	//生成地形方块
	virtual void SpawnCube(ECubeType CubeType){}
};
