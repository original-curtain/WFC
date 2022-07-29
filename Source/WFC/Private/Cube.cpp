// Fill out your copyright notice in the Description page of Project Settings.


#include "Cube.h"

ACube::ACube()
{
	PrimaryActorTick.bCanEverTick = false;
	
	CubeMeshComponent=CreateDefaultSubobject<UStaticMeshComponent>(FName("Cube"));
	SetRootComponent(CubeMeshComponent);
}

