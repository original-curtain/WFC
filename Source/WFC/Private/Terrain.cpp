// Fill out your copyright notice in the Description page of Project Settings.


#include "Terrain.h"

ATerrain::ATerrain()
{
	PrimaryActorTick.bCanEverTick = false;

	for (int i = 0; i < CubeNum; i++)
	{
		TerrainMatrix.Add(TArray<ACube>());
		for (int j = 0; j < CubeNum; j++)
		{
			TerrainMatrix[i].Add(ACube());
		}
	}
}

float ATerrain::GetEntropy(ACube Cube)
{
	float Entropy=0.f;
	for (int i = 0; i < Cube.AllPossiableCubeTypeArr.Num(); i++)
	{
		float P = 1.f / Cube.AllPossiableCubeTypeArr.Num();
		Entropy += P * FMath::Log2(P);
	}
	return Entropy;
}

void ATerrain::Observe(int X,int Y)
{
	if (X >= 0 && X < CubeNum && Y >= 0 && Y < CubeNum)
	{
		if (!TerrainMatrix[X][Y].IsObserved)
		{
			int Idx = FMath::RandRange(0, TerrainMatrix[X][Y].AllPossiableCubeTypeArr.Num() - 1);
			TerrainMatrix[X][Y].CubeType = TerrainMatrix[X][Y].AllPossiableCubeTypeArr[Idx];
			TerrainMatrix[X][Y].IsObserved = true;
			Propagate(X,Y);
		}
	}
}

void ATerrain::Propagate(int OriginalX, int OriginalY)
{
	if(!TerrainMatrix[OriginalX][OriginalY].IsObserved) return;

	//front
	if (OriginalY - 1 >= 0 && !TerrainMatrix[OriginalX][OriginalY - 1].IsObserved)
	{
		Collapse(TerrainMatrix[OriginalX][OriginalY].LinkRule[EDirection::Front].AllowedType, OriginalX, OriginalY - 1);
	}
	//back
	if (OriginalY + 1 < CubeNum && !TerrainMatrix[OriginalX][OriginalY + 1].IsObserved)
	{
		Collapse(TerrainMatrix[OriginalX][OriginalY].LinkRule[EDirection::Back].AllowedType, OriginalX, OriginalY + 1);
	}

	//left
	if (OriginalX - 1 >= 0 && !TerrainMatrix[OriginalX-1][OriginalY].IsObserved)
	{
		Collapse(TerrainMatrix[OriginalX][OriginalY].LinkRule[EDirection::Left].AllowedType, OriginalX-1, OriginalY);
	}
	//right
	if (OriginalX + 1 < CubeNum && !TerrainMatrix[OriginalX+1][OriginalY].IsObserved)
	{
		Collapse(TerrainMatrix[OriginalX][OriginalY].LinkRule[EDirection::Right].AllowedType, OriginalX+1, OriginalY);
	}
}

void ATerrain::Collapse(TArray<ECubeType> RuleAllowedCubeTypeArr,int CollapseX,int CollapseY)
{
	TArray<ECubeType> FinalAllowedCubeTypeArr;
	for (int i = 0; i < RuleAllowedCubeTypeArr.Num(); i++)
	{
		if (TerrainMatrix[CollapseX][CollapseY].AllPossiableCubeTypeArr.Find(RuleAllowedCubeTypeArr[i]))
		{
			FinalAllowedCubeTypeArr.Add(RuleAllowedCubeTypeArr[i]);
		}
	}

	TerrainMatrix[CollapseX][CollapseY].AllPossiableCubeTypeArr = FinalAllowedCubeTypeArr;
	if (FinalAllowedCubeTypeArr.Num() == 1)
	{
		TerrainMatrix[CollapseX][CollapseY].CubeType = FinalAllowedCubeTypeArr[0];
		TerrainMatrix[CollapseX][CollapseY].IsObserved = true;
		Propagate(CollapseX, CollapseY);
	}
}

