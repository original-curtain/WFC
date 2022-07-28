// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Runtime/Engine/Classes/Components/ActorComponent.h"
#include "Runtime/Engine/Classes/Components/InstancedStaticMeshComponent.h"
#include "Runtime/Engine/Public/GenericQuadTree.h"
#include "Math/RandomStream.h"

#include "SimpleWFC.generated.h"

UENUM(BlueprintType)
enum class EProtoType :uint8
{
	VE_None,
	VE_0,
	VE_1,
	VE_2,
	VE_3,
	VE_4,
	VE_5,
	VE_6,
	VE_7,
	VE_8,
	VE_9,
	VE_10,
	VE_11,
	VE_12
};

USTRUCT(BlueprintType)
struct FProto
{

	GENERATED_BODY()
public:
	UPROPERTY(VisibleDefaultsOnly,BlueprintReadOnly,Category="Proto")
	TArray<EProtoType> NeighborTypeList_PX;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Proto")
	TArray<EProtoType> NeighborTypeList_NX;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Proto")
	TArray<EProtoType> NeighborTypeList_PY;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Proto")
	TArray<EProtoType> NeighborTypeList_NY;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Proto")
	EProtoType ProtoType=EProtoType::VE_None;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Proto")
	UStaticMesh* ProtoAsset=nullptr;
};

USTRUCT(BlueprintType)
struct FWFCCell
{
	GENERATED_BODY()
public:
	FWFCCell(){}

	bool bCollapse=false;
	bool bDirty=false;
	TArray<FProto*> PotentialProto;
	FIntVector CellListLocation=FIntVector(-1,-1,-1);

	UPROPERTY(VisibleDefaultsOnly,BlueprintReadOnly,Category="SimpleWFC")
	FVector CellLocation=FVector(0,0,0);
};


UCLASS()
class WFC_API ASimpleWFC : public AActor
{
	GENERATED_BODY()
	
public:	
	ASimpleWFC(const FObjectInitializer& ObjectInitializer);
	~ASimpleWFC();

	TQuadTree<uint32>* CellLIndexTree;

	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="SimpleWFC")
	int32 RandSeed=0;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SimpleWFC")
	TArray<FWFCCell> CellList;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SimpleWFC")
	TArray<FProto> ProtoDataList;

	UFUNCTION(BlueprintCallable,Category="SimpleWFC")
	void CollapseCellByIndex(int32 cellIndex);
	UFUNCTION(BlueprintCallable, Category = "SimpleWFC")
	bool Observation(const FWFCCell& LastPropagationCell,FWFCCell& CurrPropagationCell,const int32& DirectionIndex);

	UFUNCTION(BlueprintCallable, Category = "SimpleWFC")
	FProto GetCollapseProtoCopy(int32 Index)
	{
		if (CellList[Index].bCollapse) return *CellList[Index].PotentialProto[0];
		else return FProto();
	}

	void Propagation();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SimpleWFC")
	int32 CellSize=10;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SimpleWFC")
	int32 ObjectBoundBoxSize=210;
	int32 ObjectSize;

public:	
	TArray<FWFCCell*> LastPropagationCellsArray;
	TArray<FWFCCell*> CurrPropagationCellsArray;

	FRandomStream SimpleWFCCreateStream;
};
