// Fill out your copyright notice in the Description page of Project Settings.


#include "Terrain.h"

ATerrain::ATerrain()
{
	PrimaryActorTick.bCanEverTick = true;

	for (int i = 0; i < CubeNum; i++)
	{
		TerrainMatrix.Add(TArray<FCell>());
		for (int j = 0; j < CubeNum; j++)
		{
			FCell Cell;
			Cell.AllPossiableCubeTypeArr.Add(ECubeType::Soil);
			Cell.AllPossiableCubeTypeArr.Add(ECubeType::Grass);
			Cell.AllPossiableCubeTypeArr.Add(ECubeType::Water);
			TerrainMatrix[i].Add(Cell);
		}
	}
}

void ATerrain::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (!SpawnQueue.IsEmpty() && !InSpawn)
	{
		InSpawn=true;
		ACube* Cube;
		SpawnQueue.Dequeue(Cube);
		Cube->Spawn(Cube->CurType);
		GetWorldTimerManager().SetTimer(UnusedHandle,this,&ATerrain::EnableSpawn,0.4f,false);
	}
}

float ATerrain::GetEntropy(FCell Cell)
{
	float Entropy = 0.f;
	for (int i = 0; i < Cell.AllPossiableCubeTypeArr.Num(); i++)
	{
		float P = 1.f / Cell.AllPossiableCubeTypeArr.Num();
		Entropy += P * FMath::Log2(P);
	}
	return Entropy;
}

void ATerrain::WFC(int X,int Y)
{
	if(X<0||X>=CubeNum||Y<0||Y>=CubeNum) return;

	for (int i = X; i < CubeNum; i++)
	{
		for (int j = Y; j < CubeNum; j++)
		{
			if (!TerrainMatrix[i][j].IsObserved)
			{
				Observe(i,j);
			}
		}
		for (int j = Y; j >= 0; j--)
		{
			if (!TerrainMatrix[i][j].IsObserved)
			{
				Observe(i, j);
			}
		}
	}

	for (int i = X; i >= 0; i--)
	{
		for (int j = Y; j < CubeNum; j++)
		{
			if (!TerrainMatrix[i][j].IsObserved)
			{
				Observe(i, j);
			}
		}
		for (int j = Y; j >= 0; j--)
		{
			if (!TerrainMatrix[i][j].IsObserved)
			{
				Observe(i, j);
			}
		}
	}
}

bool ATerrain::Observe(int X, int Y)
{
	if (X >= 0 && X < CubeNum && Y >= 0 && Y < CubeNum)
	{
		if (!TerrainMatrix[X][Y].IsObserved)
		{
			int Idx = FMath::RandRange(0, TerrainMatrix[X][Y].AllPossiableCubeTypeArr.Num() - 1);
			TerrainMatrix[X][Y].CubeType = TerrainMatrix[X][Y].AllPossiableCubeTypeArr[Idx];
			TerrainMatrix[X][Y].IsObserved = true;
			SpawnCube(TerrainMatrix[X][Y].CubeType,X,Y);
			Propagate(X, Y);
			return true;
		}
	}
	return false;
}

void ATerrain::Propagate(int OriginalX, int OriginalY)
{
	if (!TerrainMatrix[OriginalX][OriginalY].IsObserved) return;

	//front
	if (OriginalY - 1 >= 0 && !TerrainMatrix[OriginalX][OriginalY - 1].IsObserved)
	{
		Collapse(LinkRule[TerrainMatrix[OriginalX][OriginalY].CubeType].Front, OriginalX, OriginalY - 1);
	}
	//back
	if (OriginalY + 1 < CubeNum && !TerrainMatrix[OriginalX][OriginalY + 1].IsObserved)
	{
		Collapse(LinkRule[TerrainMatrix[OriginalX][OriginalY].CubeType].Back, OriginalX, OriginalY + 1);
	}

	//left
	if (OriginalX - 1 >= 0 && !TerrainMatrix[OriginalX - 1][OriginalY].IsObserved)
	{
		Collapse(LinkRule[TerrainMatrix[OriginalX][OriginalY].CubeType].Left, OriginalX - 1, OriginalY);
	}
	//right
	if (OriginalX + 1 < CubeNum && !TerrainMatrix[OriginalX + 1][OriginalY].IsObserved)
	{
		Collapse(LinkRule[TerrainMatrix[OriginalX][OriginalY].CubeType].Right, OriginalX + 1, OriginalY);
	}
}

void ATerrain::Collapse(TArray<ECubeType> RuleAllowedCubeTypeArr, int CollapseX, int CollapseY)
{
	TArray<ECubeType> FinalAllowedCubeTypeArr;
	for (int i = 0; i < RuleAllowedCubeTypeArr.Num(); i++)
	{
		if (TerrainMatrix[CollapseX][CollapseY].AllPossiableCubeTypeArr.Find(RuleAllowedCubeTypeArr[i])!=INDEX_NONE)
		{
			FinalAllowedCubeTypeArr.Add(RuleAllowedCubeTypeArr[i]);
		}
	}

	TerrainMatrix[CollapseX][CollapseY].AllPossiableCubeTypeArr = FinalAllowedCubeTypeArr;
	if (FinalAllowedCubeTypeArr.Num() == 1)
	{
		Observe(CollapseX,CollapseY);
	}
}

void ATerrain::SpawnCube(ECubeType CubeType, int SpawnX, int SpawnY)
{
	FVector WorldLoc=LocalToWorld(FVector(SpawnX,SpawnY,0));
	FActorSpawnParameters SpawnInfo;
	ACube* Cube=GetWorld()->SpawnActor<ACube>(CubeClass, WorldLoc, FRotator(0.f, 0.f, 0.f), SpawnInfo);
	Cube->CurType=CubeType;
	SpawnQueue.Enqueue(Cube);
}

FVector ATerrain::LocalToWorld(FVector LocalLoc)
{
	FVector WorldLoc;
	WorldLoc.X=LocalLoc.Y*100+50;
	WorldLoc.Y=LocalLoc.X*100+50;
	WorldLoc.Z=70.f;
	return WorldLoc;
}

FVector ATerrain::WorldToLocal(FVector WorldLoc)
{
	FVector LocalLoc;
	LocalLoc.X=WorldLoc.Y/100;
	LocalLoc.Y=WorldLoc.X/100;
	return LocalLoc;
}

void ATerrain::EnableSpawn()
{
	InSpawn=false;
}
