// Copyright BlueCatt Studios - All Rights Reserved
// CharacterSaveData.h
// Estrutura de dados para salvamento de personagem

#pragma once

#include "CoreMinimal.h"
#include "CharacterSaveData.generated.h"


/**
 * Preset de rosto (base para customização)
 */
USTRUCT(BlueprintType)
struct FFacePresetData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Preset")
	FString PresetID = TEXT("default");

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Preset")
	FString PresetName = TEXT("Default");

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Preset")
	FString Gender = TEXT("Male");

	// Futuro: morphs do preset (por enquanto só ID)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Preset")
	TMap<FName, float> MorphValues;
};



USTRUCT(BlueprintType)
struct FBodyCustomization
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Body")
	float BreastSize = 0.5f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Body")
	float ButtSize = 0.5f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Body")
	float Height = 0.5f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Body")
	float Weight = 0.5f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Body")
	float Muscle = 0.5f;
};

USTRUCT(BlueprintType)
struct FAppearanceCustomization
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Face")
	FString FacePresetID = TEXT("male_preset_01");

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Appearance")
	FString HairStyle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Appearance")
	FLinearColor HairColor = FLinearColor::Black;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Appearance")
	FLinearColor SkinColor = FLinearColor::White;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Appearance")
	bool bHasMakeup = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Appearance")
	FLinearColor MakeupColor = FLinearColor::Red;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Appearance")
	bool bHasBodyHair = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Appearance")
	float BodyHairDensity = 0.5f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Appearance")
	FLinearColor EyeColor = FLinearColor::Blue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Face Advanced")
	TMap<FName, float> FaceMorphOverrides;
};

USTRUCT(BlueprintType)
struct FClothingItemData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Clothing")
	FString ItemID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Clothing")
	FString SlotType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Clothing")
	FString MeshPath;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Clothing")
	FString MaterialPath;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Clothing")
	FLinearColor Color = FLinearColor::White;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Clothing")
	bool bEquipped = true;
};

USTRUCT(BlueprintType)
struct FOutfitData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Outfit")
	FString OutfitName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Outfit")
	TArray<FClothingItemData> ClothingItems;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Outfit")
	int32 CreatedTimestamp = 0;
};

USTRUCT(BlueprintType)
struct FCharacterSaveData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Basic")
	FString CharacterName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Basic")
	FString CharacterGender;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Basic")
	int32 CharacterSlot;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Basic")
	int32 CreatedTimestamp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Basic")
	int32 LastModifiedTimestamp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Customization")
	FBodyCustomization BodyCustomization;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Customization")
	FAppearanceCustomization AppearanceCustomization;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Clothing")
	TArray<FClothingItemData> CurrentOutfit;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Clothing")
	TArray<FOutfitData> SavedOutfits;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Clothing")
	FString CurrentOutfitName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Metadata")
	int32 PlayHours = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Metadata")
	FString LastLocationName;
};