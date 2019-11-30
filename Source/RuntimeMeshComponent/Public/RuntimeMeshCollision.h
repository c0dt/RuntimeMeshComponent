// Copyright 2016-2019 Chris Conway (Koderz). All Rights Reserved.

#pragma once

#include "Engine/Engine.h"
#include "Components/MeshComponent.h"
#include "RuntimeMeshCore.h"
#include "RuntimeMeshCollision.generated.h"


USTRUCT(BlueprintType)
struct RUNTIMEMESHCOMPONENT_API FRuntimeMeshCollisionConvexMesh
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RuntimeMeshCollisionConvex)
	TArray<FVector> VertexBuffer;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RuntimeMeshCollisionConvex)
	FBox BoundingBox;

	friend FArchive& operator <<(FArchive& Ar, FRuntimeMeshCollisionConvexMesh& Section)
	{
		Ar << Section.VertexBuffer;
		Ar << Section.BoundingBox;
		return Ar;
	}
};


USTRUCT(BlueprintType)
struct RUNTIMEMESHCOMPONENT_API FRuntimeMeshCollisionSphere
{
	GENERATED_USTRUCT_BODY()

	/** Position of the sphere's origin */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RuntimeMeshCollisionSphere)
	FVector Center;

	/** Radius of the sphere */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RuntimeMeshCollisionSphere)
	float Radius;

	FRuntimeMeshCollisionSphere()
		: Center(FVector::ZeroVector)
		, Radius(1)
	{

	}

	FRuntimeMeshCollisionSphere(float r)
		: Center(FVector::ZeroVector)
		, Radius(r)
	{

	}

	friend FArchive& operator <<(FArchive& Ar, FRuntimeMeshCollisionSphere& Sphere)
	{
		Ar << Sphere.Center;
		Ar << Sphere.Radius;
		return Ar;
	}
};


USTRUCT(BlueprintType)
struct RUNTIMEMESHCOMPONENT_API FRuntimeMeshCollisionBox
{
	GENERATED_USTRUCT_BODY()

	/** Position of the box's origin */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RuntimeMeshCollisionBox)
	FVector Center;

	/** Rotation of the box */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RuntimeMeshCollisionBox)
	FRotator Rotation;

	/** Extents of the box */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RuntimeMeshCollisionBox)
	FVector Extents;


	FRuntimeMeshCollisionBox()
		: Center(FVector::ZeroVector)
		, Rotation(FRotator::ZeroRotator)
		, Extents(1, 1, 1)
	{

	}

	FRuntimeMeshCollisionBox(float s)
		: Center(FVector::ZeroVector)
		, Rotation(FRotator::ZeroRotator)
		, Extents(s, s, s)
	{

	}

	FRuntimeMeshCollisionBox(float InX, float InY, float InZ)
		: Center(FVector::ZeroVector)
		, Rotation(FRotator::ZeroRotator)
		, Extents(InX, InY, InZ)

	{

	}

	friend FArchive& operator <<(FArchive& Ar, FRuntimeMeshCollisionBox& Box)
	{
		Ar << Box.Center;
		Ar << Box.Rotation;
		Ar << Box.Extents;
		return Ar;
	}
};

USTRUCT(BlueprintType)
struct RUNTIMEMESHCOMPONENT_API FRuntimeMeshCollisionCapsule
{
	GENERATED_USTRUCT_BODY()

	/** Position of the capsule's origin */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RuntimeMeshCollisionCapsule)
	FVector Center;

	/** Rotation of the capsule */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RuntimeMeshCollisionCapsule)
	FRotator Rotation;

	/** Radius of the capsule */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RuntimeMeshCollisionCapsule)
	float Radius;

	/** This is of line-segment ie. add Radius to both ends to find total length. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RuntimeMeshCollisionCapsule)
	float Length;

	FRuntimeMeshCollisionCapsule()
		: Center(FVector::ZeroVector)
		, Rotation(FRotator::ZeroRotator)
		, Radius(1), Length(1)

	{

	}

	FRuntimeMeshCollisionCapsule(float InRadius, float InLength)
		: Center(FVector::ZeroVector)
		, Rotation(FRotator::ZeroRotator)
		, Radius(InRadius), Length(InLength)
	{

	}

	friend FArchive& operator <<(FArchive& Ar, FRuntimeMeshCollisionCapsule& Capsule)
	{
		Ar << Capsule.Center;
		Ar << Capsule.Rotation;
		Ar << Capsule.Radius;
		Ar << Capsule.Length;
		return Ar;
	}
};

struct FRuntimeMeshCollisionVertexStream
{
private:
	TArray<FVector> Data;

public:
	FRuntimeMeshCollisionVertexStream() { }

	void SetNum(int32 NewNum, bool bAllowShrinking)
	{
		Data.SetNum(NewNum, bAllowShrinking);
	}

	int32 Num() const
	{
		return Data.Num();
	}

	void Empty(int32 Slack = 0)
	{
		Data.Empty(Slack);
	}

	FORCEINLINE int32 Add(const FVector& InPosition)
	{
		return Data.Add(InPosition);
	}

	FORCEINLINE const FVector& GetPosition(int32 Index) const
	{
		return Data[Index];
	}

	FORCEINLINE void SetPosition(int32 Index, const FVector& NewPosition)
	{
		Data[Index] = NewPosition;
	}

private:
	TArray<FVector>&& TakeContents()
	{
		return MoveTemp(Data);
	}

	friend class URuntimeMesh;
};

struct FRuntimeMeshCollisionTriangleStream
{
private:
	TArray<FTriIndices> Data;

public:
	FRuntimeMeshCollisionTriangleStream()
	{

	}

	void SetNum(int32 NewNum, bool bAllowShrinking)
	{
		Data.SetNum(NewNum, bAllowShrinking);
	}

	int32 Num() const
	{
		return Data.Num();
	}

	void Empty(int32 Slack = 0)
	{
		Data.Empty(Slack);
	}

	FORCEINLINE int32 Add(int32 IndexA, int32 IndexB, int32 IndexC)
	{
		FTriIndices NewTri;
		NewTri.v0 = IndexA;
		NewTri.v1 = IndexB;
		NewTri.v2 = IndexC;

		return Data.Add(NewTri);
	}

	FORCEINLINE void GetTriangleIndices(int32 TriangleIndex, int32& OutIndexA, int32& OutIndexB, int32& OutIndexC)
	{
		FTriIndices& Tri = Data[TriangleIndex];
		OutIndexA = Tri.v0;
		OutIndexB = Tri.v1;
		OutIndexC = Tri.v2;
	}

	FORCEINLINE void SetTriangleIndices(int32 TriangleIndex, int32 IndexA, int32 IndexB, int32 IndexC)
	{
		FTriIndices& Tri = Data[TriangleIndex];
		Tri.v0 = IndexA;
		Tri.v1 = IndexB;
		Tri.v2 = IndexC;
	}

private:
	TArray<FTriIndices>&& TakeContents()
	{
		return MoveTemp(Data);
	}

	friend class URuntimeMesh;
};

struct FRuntimeMeshCollisionTexCoordStream
{
private:
	TArray<TArray<FVector2D>> Data;

public:
	FRuntimeMeshCollisionTexCoordStream()
	{

	}

	void SetNumChannels(int32 NewNumChannels, bool bAllowShrinking)
	{
		Data.SetNum(NewNumChannels, bAllowShrinking);
	}

	void SetNumCoords(int32 ChannelId, int32 NewNumCoords, bool bAllowShrinking)
	{
		Data[ChannelId].SetNum(NewNumCoords, bAllowShrinking);
	}

	void SetNum(int32 NewNumChannels, int32 NewNumCoords, bool bAllowShrinking)
	{
		Data.SetNum(NewNumChannels, bAllowShrinking);
		for (int32 Index = 0; Index < Data.Num(); Index++)
		{
			Data[Index].SetNum(NewNumCoords, bAllowShrinking);
		}
	}

	int32 NumChannels() const
	{
		return Data.Num();
	}

	int32 NumTexCoords(int32 ChannelId)
	{
		return Data[ChannelId].Num();
	}

	void EmptyChannel(int32 ChannelId, int32 Slack = 0)
	{
		Data[ChannelId].Empty(Slack);
	}

	FORCEINLINE int32 Add(int32 ChannelId, const FVector2D& NewTexCoord)
	{
		return Data[ChannelId].Add(NewTexCoord);
	}

	FORCEINLINE FVector2D GetTexCoord(int32 ChannelId, int32 TexCoordIndex)
	{
		return Data[ChannelId][TexCoordIndex];
	}

	FORCEINLINE void SetTexCoord(int32 ChannelId, int32 TexCoordIndex, const FVector2D& NewTexCoord)
	{
		Data[ChannelId][TexCoordIndex] = NewTexCoord;
	}
private:
	TArray<TArray<FVector2D>>&& TakeContents()
	{
		return MoveTemp(Data);
	}

	friend class URuntimeMesh;
};

struct FRuntimeMeshCollisionMaterialIndexStream
{
private:
	TArray<uint16> Data;

public:
	FRuntimeMeshCollisionMaterialIndexStream() { }

	void SetNum(int32 NewNum, bool bAllowShrinking)
	{
		Data.SetNum(NewNum, bAllowShrinking);
	}

	int32 Num() const
	{
		return Data.Num();
	}

	void Empty(int32 Slack = 0)
	{
		Data.Empty(Slack);
	}

	FORCEINLINE int32 Add(uint16 NewMaterialIndex)
	{
		return Data.Add(NewMaterialIndex);
	}

	FORCEINLINE uint16 GetMaterialIndex(int32 Index) const
	{
		return Data[Index];
	}

	FORCEINLINE void SetMaterialIndex(int32 Index, uint16 NewMaterialIndex)
	{
		Data[Index] = NewMaterialIndex;
	}

private:
	TArray<uint16>&& TakeContents()
	{
		return MoveTemp(Data);
	}

	friend class URuntimeMesh;
};




USTRUCT(BlueprintType)
struct FRuntimeMeshCollisionSettings
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RuntimeMeshCollision)
	bool bUseComplexAsSimple;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RuntimeMeshCollision)
	bool bUseAsyncCooking;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RuntimeMeshCollision)
	ERuntimeMeshCollisionCookingMode CookingMode;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RuntimeMeshCollisionShapes)
	TArray<FRuntimeMeshCollisionConvexMesh> ConvexElements;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RuntimeMeshCollisionShapes)
	TArray<FRuntimeMeshCollisionSphere> Spheres;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RuntimeMeshCollisionShapes)
	TArray<FRuntimeMeshCollisionBox> Boxes; 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RuntimeMeshCollisionShapes)
	TArray<FRuntimeMeshCollisionCapsule> Capsules;

	FRuntimeMeshCollisionSettings()
		: bUseComplexAsSimple(true)
		, bUseAsyncCooking(false)
		, CookingMode(ERuntimeMeshCollisionCookingMode::CollisionPerformance)
	{

	}
};

struct FRuntimeMeshCollisionData
{
	FRuntimeMeshCollisionVertexStream Vertices;
	FRuntimeMeshCollisionTriangleStream Triangles;
	FRuntimeMeshCollisionTexCoordStream TexCoords;
	FRuntimeMeshCollisionMaterialIndexStream MaterialIndices;

	bool bFlipNormals;
	bool bDeformableMesh;
	bool bFastCook;
	bool bDisableActiveEdgePrecompute;

	FRuntimeMeshCollisionData()
		: bFlipNormals(false)
		, bDeformableMesh(false)
		, bFastCook(false)
		, bDisableActiveEdgePrecompute(false)
	{

	}
};