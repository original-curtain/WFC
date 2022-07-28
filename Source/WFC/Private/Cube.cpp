// Fill out your copyright notice in the Description page of Project Settings.


#include "Cube.h"

ACube::ACube()
{
	PrimaryActorTick.bCanEverTick = false;

	AllPossiableCubeTypeArr.Add(ECubeType::Soil);
	AllPossiableCubeTypeArr.Add(ECubeType::Grass);
	AllPossiableCubeTypeArr.Add(ECubeType::Water);
}

ACube::ACube(const ACube& Cube)
{
	AllPossiableCubeTypeArr.Empty();
	for each (ECubeType CubeType in Cube.AllPossiableCubeTypeArr)
	{
		AllPossiableCubeTypeArr.Add(CubeType);
	}
	CubeType=Cube.CubeType;
	IsObserved =Cube.IsObserved;
}

