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

	//��ȡ��ǰ��Ԫ�����ֵ
	float GetEntropy(ACube Cube);
	//ָ��λ�ù۲�
	void Observe(int X, int Y);
	//����
	void Propagate(int OriginalX, int OriginalY);
	//״̬����
	void Collapse(TArray<ECubeType> RuleAllowedCubeTypeArr, int CollapseX, int CollapseY);
	//���ɵ��η���
	virtual void SpawnCube(ECubeType CubeType){}
};
