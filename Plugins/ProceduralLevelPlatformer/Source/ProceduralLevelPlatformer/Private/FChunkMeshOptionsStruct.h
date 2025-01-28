#pragma once

#include "FChunkMeshOptionsStruct.generated.h"

USTRUCT(BlueprintType)
struct FChunkMeshOptionsStruct
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "StructNodeChunkOptions")
	bool bUseOffsetOverrides = false;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "StructNodeChunkOptions")
	UStaticMesh* FloorStaticMesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "StructNodeChunkOptions")
	UMaterial* FloorMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "StructNodeChunkOptions")
	UStaticMesh* RightWallStaticMesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "StructNodeChunkOptions")
	FTransform RightWallTransformOffsetOverride;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "StructNodeChunkOptions")
	UMaterial* RightWallMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "StructNodeChunkOptions")
	UStaticMesh* LeftWallStaticMesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "StructNodeChunkOptions")
	FTransform LeftWallTransformOffsetOverride;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "StructNodeChunkOptions")
	UMaterial* LeftWallMaterial;	

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "StructNodeChunkOptions")
	UStaticMesh* CeilingStaticMesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "StructNodeChunkOptions")
	FTransform CeilingTransformOffsetOverride;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "StructNodeChunkOptions")
	UMaterial* CeilingMaterial;
};
