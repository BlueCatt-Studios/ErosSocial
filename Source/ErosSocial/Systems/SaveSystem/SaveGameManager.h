// Copyright BlueCatt Studios - All Rights Reserved
// SaveGameManager.h

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Data/CharacterSaveData.h"
#include "SaveGameManager.generated.h"

UCLASS(Blueprintable, BlueprintType)
class EROSSOCIAL_API USaveGameManager : public UObject
{
	GENERATED_BODY()

public:
	USaveGameManager();

	UFUNCTION(BlueprintCallable, Category = "SaveGame")
	bool SaveCharacterData(const FCharacterSaveData& CharacterData, const FString& UserID, int32 SlotIndex);

	UFUNCTION(BlueprintCallable, Category = "SaveGame")
	bool LoadCharacterData(FCharacterSaveData& OutCharacterData, const FString& UserID, int32 SlotIndex);

	UFUNCTION(BlueprintCallable, Category = "SaveGame")
	bool CharacterExists(const FString& UserID, int32 SlotIndex);

	UFUNCTION(BlueprintCallable, Category = "SaveGame")
	bool DeleteCharacter(const FString& UserID, int32 SlotIndex);

	UFUNCTION(BlueprintCallable, Category = "SaveGame")
	bool SaveOutfit(const FOutfitData& OutfitData, const FString& UserID, const FString& OutfitName);

	UFUNCTION(BlueprintCallable, Category = "SaveGame")
	bool LoadOutfit(FOutfitData& OutOutfitData, const FString& UserID, const FString& OutfitName);

	UFUNCTION(BlueprintCallable, Category = "SaveGame")
	bool SaveWorldMap(const FString& MapData, const FString& UserID, const FString& MapName);

	UFUNCTION(BlueprintCallable, Category = "SaveGame")
	bool LoadWorldMap(FString& OutMapData, const FString& UserID, const FString& MapName);

	UFUNCTION(BlueprintPure, Category = "SaveGame")
	FString GetSaveGamePath(const FString& UserID) const;

	UFUNCTION(BlueprintCallable, Category = "SaveGame")
	void EnsureSaveDirectoriesExist(const FString& UserID);

	UFUNCTION(BlueprintPure, Category = "SaveGame")
	int32 GetCurrentTimestamp() const;

protected:
	UPROPERTY(EditDefaultsOnly, Category = "SaveGame")
	FString SaveGameDirectory = TEXT("Saved/ErosSocial/");

	UPROPERTY(EditDefaultsOnly, Category = "SaveGame")
	FString CharacterDataFileName = TEXT("character_slot_{0}.json");

	UPROPERTY(EditDefaultsOnly, Category = "SaveGame")
	FString OutfitFileName = TEXT("{0}.finesse");

	UPROPERTY(EditDefaultsOnly, Category = "SaveGame")
	FString WorldMapFileName = TEXT("{0}.athenas");

	FString SerializeCharacterData(const FCharacterSaveData& CharacterData) const;
	bool DeserializeCharacterData(const FString& JsonString, FCharacterSaveData& OutCharacterData) const;
	bool ValidateSaveFile(const FString& FilePath) const;
	void CreateBackup(const FString& FilePath);
};