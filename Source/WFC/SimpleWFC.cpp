// Fill out your copyright notice in the Description page of Project Settings.


#include "SimpleWFC.h"
#include "Engine/Classes/Kismet/KismetMathLibrary.h"

#define en EProtoType::VE_None
#define e0 EProtoType::VE_0
#define e1 EProtoType::VE_1
#define e2 EProtoType::VE_2
#define e3 EProtoType::VE_3
#define e4 EProtoType::VE_4
#define e5 EProtoType::VE_5
#define e6 EProtoType::VE_6
#define e7 EProtoType::VE_7
#define e8 EProtoType::VE_8
#define e9 EProtoType::VE_9
#define e10 EProtoType::VE_10
#define e11 EProtoType::VE_11
#define e12 EProtoType::VE_12

// Sets default values
ASimpleWFC::ASimpleWFC(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	SimpleWFCCreateStream.Initialize(RandSeed);

	const float& HalfObjectSize=ObjectBoundBoxSize/2;
	const FVector2D& MinLoc=FVector2D(-HalfObjectSize*100);
	const FVector2D& MaxLoc=FVector2D(HalfObjectSize*100);
	const FBox2D& RootBox=FBox2D(MinLoc,MaxLoc);

	CellIndexTree=new TQuadTree<uint32>(RootBox);

	ObjectSize=ObjectBoundBoxSize/CellSize;
	uint32 CellNum=ObjectSize*ObjectSize;

	//Proto_None
	{
		FProto newProto;
		newProto.ProtoType=EProtoType::VE_None;
		newProto.NeighborTypeList_PX={en,e0,e3,e6};
		newProto.NeighborTypeList_NX={en,e2,e5,e8};
		newProto.NeighborTypeList_PY={en,e0,e1,e2};
		newProto.NeighborTypeList_NY={en,e6,e7,e8};
		ProtoDataList.Add(MoveTemp(newProto));
	}
	//Proto_0
	{
		FProto newProto;
		newProto.ProtoType = EProtoType::VE_0;
		newProto.NeighborTypeList_PX = { e1,e2,e9 };
		newProto.NeighborTypeList_NX = { e2,e5,e8 };
		newProto.NeighborTypeList_PY = { e3,e6,e9 };
		newProto.NeighborTypeList_NY = { e6,e7,e8 };
		ProtoDataList.Add(MoveTemp(newProto));
	}
	//Proto_1
	{
		FProto newProto;
		newProto.ProtoType = EProtoType::VE_1;
		newProto.NeighborTypeList_PX = { e1, e2, e9 };
		newProto.NeighborTypeList_NX = { e1, e0, e10 };
		newProto.NeighborTypeList_PY = { e4, e7, e11, e12 };
		newProto.NeighborTypeList_NY = { e6, e7, e8 };
		ProtoDataList.Add(MoveTemp(newProto));
	}
	//Proto_2
	{
		FProto newProto;
		newProto.ProtoType = EProtoType::VE_2;
		newProto.NeighborTypeList_PX = { e0, e3, e6 };
		newProto.NeighborTypeList_NX = { e1, e0, e10 };
		newProto.NeighborTypeList_PY = { e5, e8, e10 };
		newProto.NeighborTypeList_NY = { e6, e7, e8 };
		ProtoDataList.Add(MoveTemp(newProto));
	}
	//Proto_3
	{
		FProto newProto;
		newProto.ProtoType = EProtoType::VE_3;
		newProto.NeighborTypeList_PX = { e4, e5, e10, e12 };
		newProto.NeighborTypeList_NX = { e2, e5, e8 };
		newProto.NeighborTypeList_PY = { e3, e6, e9 };
		newProto.NeighborTypeList_NY = { e3, e0, e11 };
		ProtoDataList.Add(MoveTemp(newProto));
	}
	//Proto_4
	{
		FProto newProto;
		newProto.ProtoType = EProtoType::VE_4;
		newProto.NeighborTypeList_PX = { e4, e5, e10, e12 };
		newProto.NeighborTypeList_NX = { e4, e3, e9, e11 };
		newProto.NeighborTypeList_PY = { e4, e7, e11, e12 };
		newProto.NeighborTypeList_NY = { e4, e1, e9, e10 };
		ProtoDataList.Add(MoveTemp(newProto));
	}
	//Proto_5
	{
		FProto newProto;
		newProto.ProtoType = EProtoType::VE_5;
		newProto.NeighborTypeList_PX = { e0, e3, e6 };
		newProto.NeighborTypeList_NX = { e4, e3, e9, e11 };
		newProto.NeighborTypeList_PY = { e5, e8, e10 };
		newProto.NeighborTypeList_NY = { e2, e5, e12 };
		ProtoDataList.Add(MoveTemp(newProto));
	}
	//Proto_6
	{
		FProto newProto;
		newProto.ProtoType = EProtoType::VE_6;
		newProto.NeighborTypeList_PX = { e7, e8, e11 };
		newProto.NeighborTypeList_NX = { e2, e5, e8 };
		newProto.NeighborTypeList_PY = { e0, e1, e2 };
		newProto.NeighborTypeList_NY = { e0, e3, e11 };
		ProtoDataList.Add(MoveTemp(newProto));
	}
	//Proto_7
	{
		FProto newProto;
		newProto.ProtoType = EProtoType::VE_7;
		newProto.NeighborTypeList_PX = { e7, e8, e11 };
		newProto.NeighborTypeList_NX = { e7, e6, e12 };
		newProto.NeighborTypeList_PY = { e0, e1, e2 };
		newProto.NeighborTypeList_NY = { e1, e4, e9, e10 };
		ProtoDataList.Add(MoveTemp(newProto));
	}
	//Proto_8
	{
		FProto newProto;
		newProto.ProtoType = EProtoType::VE_8;
		newProto.NeighborTypeList_PX = { e0, e3, e6 };
		newProto.NeighborTypeList_NX = { e6, e7, e12 };
		newProto.NeighborTypeList_PY = { e0, e1, e2 };
		newProto.NeighborTypeList_NY = { e2, e5, e12 };
		ProtoDataList.Add(MoveTemp(newProto));
	}
	//Proto_9
	{
		FProto newProto;
		newProto.ProtoType = EProtoType::VE_9;
		newProto.NeighborTypeList_PX = { e4, e5, e10, e12 };
		newProto.NeighborTypeList_NX = { e0, e1, e10 };
		newProto.NeighborTypeList_PY = { e4, e7, e11, e12 };
		newProto.NeighborTypeList_NY = { e0, e3, e11 };
		ProtoDataList.Add(MoveTemp(newProto));
	}
	//Proto_10
	{
		FProto newProto;
		newProto.ProtoType = EProtoType::VE_10;
		newProto.NeighborTypeList_PX = { e1, e2, e9 };
		newProto.NeighborTypeList_NX = { e3, e4, e9, e11 };
		newProto.NeighborTypeList_PY = { e4, e7, e12, e11 };
		newProto.NeighborTypeList_NY = { e2, e5, e12 };
		ProtoDataList.Add(MoveTemp(newProto));
	}
	//Proto_11
	{
		FProto newProto;
		newProto.ProtoType = EProtoType::VE_11;
		newProto.NeighborTypeList_PX = { e4, e5, e10, e12 };
		newProto.NeighborTypeList_NX = { e6, e7, e10, e12 };
		newProto.NeighborTypeList_PY = { e3, e6, e9 };
		newProto.NeighborTypeList_NY = { e1, e4, e9, e10 };
		ProtoDataList.Add(MoveTemp(newProto));
	}
	//Proto_12
	{
		FProto newProto;
		newProto.ProtoType = EProtoType::VE_12;
		newProto.NeighborTypeList_PX = { e7, e8, e11 };
		newProto.NeighborTypeList_NX = { e3, e4, e9, e11 };
		newProto.NeighborTypeList_PY = { e5, e8, e10 };
		newProto.NeighborTypeList_NY = { e2, e4, e9, e10 };
		ProtoDataList.Add(MoveTemp(newProto));
	}

	CellList.Reset();
	CellIndexTree->Empty();
	for (int32 y = 0; y < ObjectSize; y++)
	{
		for (int32 x = 0; x < ObjectSize; x++)
		{
			FWFCCell newCell;
			const float HalfCellSize=CellSize/2*100;
			newCell.CellLocation=FVector(MinLoc.X,MinLoc.Y,0.f)+FVector(x*CellSize*100,y*CellSize*100,0.f)+FVector(HalfCellSize,HalfCellSize,0);
			newCell.CellListLocation=FIntVector(x,y,0);

			for (FProto& protoData : ProtoDataList)
			{
				newCell.PotentialProto.Add(&protoData);
			}
			CellList.Add(MoveTemp(newCell));

			const FVector2D& CellMinLoc=FVector2D(x*CellSize*100-HalfObjectSize,y*CellSize*100-HalfObjectSize);
			const FVector2D& CellMaxLoc = FVector2D(x * CellSize * 100 - HalfObjectSize + CellSize, y * CellSize * 100 - HalfObjectSize + CellSize);
			const FBox2D& CellBox = FBox2D(CellMinLoc, CellMaxLoc);
			CellIndexTree->Insert(x + y * ObjectSize, CellBox);
		}
	}
}

ASimpleWFC::~ASimpleWFC()
{
	if (CellIndexTree != nullptr)
	{
		CellIndexTree->Empty();
		delete(CellIndexTree);
		CellIndexTree = nullptr;
	}
}

bool ASimpleWFC::Observation(const FWFCCell& LastPropagationCell, FWFCCell& CurrPropagationCell, const int32& DirectionIndex)
{
	return false;
}

