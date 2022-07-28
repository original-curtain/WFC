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
	for each (ECubeType Type in Cube.AllPossiableCubeTypeArr)
	{
		float P=1.f/Cube.AllPossiableCubeTypeArr.Num();
		Entropy+=P*FMath::Log2(P);
	}
	return Entropy;
}

void ATerrain::Observe(int X,int Y)
{
	if (X >= 0 && X < CubeNum && Y >= 0 && Y < CubeNum)
	{
		ACube ObserveCube=TerrainMatrix[X][Y];
		if (!ObserveCube.IsObserved)
		{
			int Idx = FMath::RandRange(0, ObserveCube.AllPossiableCubeTypeArr.Num() - 1);
			ObserveCube.CubeType = ObserveCube.AllPossiableCubeTypeArr[Idx];
			ObserveCube.IsObserved = true;
			Propagate(X,Y);
		}
	}
}

void ATerrain::Propagate(int OriginalX, int OriginalY)
{
	if(!TerrainMatrix[OriginalX][OriginalY].IsObserved) return;

	ACube OriginalCube= TerrainMatrix[OriginalX][OriginalY];
	//front
	if (OriginalY - 1 >= 0 && !TerrainMatrix[OriginalX][OriginalY - 1].IsObserved)
	{
		Collapse(OriginalCube.LinkRule[EDirection::Front], OriginalX, OriginalY - 1);
	}
	//back
	if (OriginalY + 1 < CubeNum && !TerrainMatrix[OriginalX][OriginalY + 1].IsObserved)
	{
		Collapse(OriginalCube.LinkRule[EDirection::Back], OriginalX, OriginalY + 1);
	}

	//left
	if (OriginalX - 1 >= 0 && !TerrainMatrix[OriginalX-1][OriginalY].IsObserved)
	{
		Collapse(OriginalCube.LinkRule[EDirection::Left], OriginalX-1, OriginalY);
	}
	//right
	if (OriginalX + 1 < CubeNum && !TerrainMatrix[OriginalX+1][OriginalY].IsObserved)
	{
		Collapse(OriginalCube.LinkRule[EDirection::Right], OriginalX+1, OriginalY);
	}
}

void ATerrain::Collapse(TArray<ECubeType> RuleAllowedCubeTypeArr,int CollapseX,int CollapseY)
{
	TArray<ECubeType> FinalAllowedCubeTypeArr;
	for each (ECubeType CubeType in RuleAllowedCubeTypeArr)
	{
		if (TerrainMatrix[CollapseX][CollapseY].AllPossiableCubeTypeArr.Find(CubeType))
		{
			FinalAllowedCubeTypeArr.Add(CubeType);
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

