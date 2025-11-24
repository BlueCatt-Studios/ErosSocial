// Copyright BlueCatt Studios - All Rights Reserved
// SaveGameManager.cpp

#include "SaveGameManager.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"
#include "Json.h"
#include "JsonUtilities.h"

USaveGameManager::USaveGameManager()
{
}

bool USaveGameManager::SaveCharacterData(const FCharacterSaveData& CharacterData, const FString& UserID, int32 SlotIndex)
{
	if (UserID.IsEmpty() || SlotIndex < 0 || SlotIndex > 1)
	{
		UE_LOG(LogTemp, Error, TEXT("SaveCharacterData: Invalid parameters"));
		return false;
	}

	EnsureSaveDirectoriesExist(UserID);

	FString FileName = FString::Printf(*CharacterDataFileName, SlotIndex);
	FString FilePath = GetSaveGamePath(UserID) + FileName;

	FString JsonString = SerializeCharacterData(CharacterData);

	if (JsonString.IsEmpty())
	{
		UE_LOG(LogTemp, Error, TEXT("SaveCharacterData: Failed to serialize"));
		return false;
	}

	if (FPaths::FileExists(*FilePath))
	{
		CreateBackup(FilePath);
	}

	if (!FFileHelper::SaveStringToFile(JsonString, *FilePath))
	{
		UE_LOG(LogTemp, Error, TEXT("SaveCharacterData: Failed to save file"));
		return false;
	}

	UE_LOG(LogTemp, Warning, TEXT("SaveCharacterData: Successfully saved '%s'"), *CharacterData.CharacterName);
	return true;
}

bool USaveGameManager::LoadCharacterData(FCharacterSaveData& OutCharacterData, const FString& UserID, int32 SlotIndex)
{
	if (UserID.IsEmpty() || SlotIndex < 0 || SlotIndex > 1)
	{
		UE_LOG(LogTemp, Error, TEXT("LoadCharacterData: Invalid parameters"));
		return false;
	}

	FString FileName = FString::Printf(*CharacterDataFileName, SlotIndex);
	FString FilePath = GetSaveGamePath(UserID) + FileName;

	if (!FPaths::FileExists(*FilePath))
	{
		UE_LOG(LogTemp, Warning, TEXT("LoadCharacterData: File not found"));
		return false;
	}

	FString JsonString;
	if (!FFileHelper::LoadFileToString(JsonString, *FilePath))
	{
		UE_LOG(LogTemp, Error, TEXT("LoadCharacterData: Failed to read file"));
		return false;
	}

	if (!DeserializeCharacterData(JsonString, OutCharacterData))
	{
		UE_LOG(LogTemp, Error, TEXT("LoadCharacterData: Failed to deserialize"));
		return false;
	}

	UE_LOG(LogTemp, Warning, TEXT("LoadCharacterData: Successfully loaded '%s'"), *OutCharacterData.CharacterName);
	return true;
}

bool USaveGameManager::CharacterExists(const FString& UserID, int32 SlotIndex)
{
	if (UserID.IsEmpty() || SlotIndex < 0 || SlotIndex > 1)
	{
		return false;
	}

	FString FileName = FString::Printf(*CharacterDataFileName, SlotIndex);
	FString FilePath = GetSaveGamePath(UserID) + FileName;
	return FPaths::FileExists(*FilePath);
}

bool USaveGameManager::DeleteCharacter(const FString& UserID, int32 SlotIndex)
{
	if (UserID.IsEmpty() || SlotIndex < 0 || SlotIndex > 1)
	{
		return false;
	}

	FString FileName = FString::Printf(*CharacterDataFileName, SlotIndex);
	FString FilePath = GetSaveGamePath(UserID) + FileName;

	if (!FPaths::FileExists(*FilePath))
	{
		return false;
	}

	return FPlatformFileManager::Get().GetPlatformFile().DeleteFile(*FilePath);
}

bool USaveGameManager::SaveOutfit(const FOutfitData& OutfitData, const FString& UserID, const FString& OutfitName)
{
	if (UserID.IsEmpty() || OutfitName.IsEmpty())
	{
		return false;
	}

	EnsureSaveDirectoriesExist(UserID);
	FString FileName = FString::Printf(*OutfitFileName, *OutfitName);
	FString FilePath = GetSaveGamePath(UserID) + TEXT("Outfits/") + FileName;

	TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject());
	JsonObject->SetStringField(TEXT("OutfitName"), OutfitData.OutfitName);
	JsonObject->SetNumberField(TEXT("CreatedTimestamp"), OutfitData.CreatedTimestamp);

	TArray<TSharedPtr<FJsonValue>> ClothingArray;
	for (const FClothingItemData& Item : OutfitData.ClothingItems)
	{
		TSharedPtr<FJsonObject> ItemObject = MakeShareable(new FJsonObject());
		ItemObject->SetStringField(TEXT("ItemID"), Item.ItemID);
		ItemObject->SetStringField(TEXT("SlotType"), Item.SlotType);
		ItemObject->SetStringField(TEXT("Color"), Item.Color.ToString());
		ClothingArray.Add(MakeShareable(new FJsonValueObject(ItemObject)));
	}
	JsonObject->SetArrayField(TEXT("ClothingItems"), ClothingArray);

	FString OutputString;
	TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&OutputString);
	FJsonSerializer::Serialize(JsonObject.ToSharedRef(), Writer);

	return FFileHelper::SaveStringToFile(OutputString, *FilePath);
}

bool USaveGameManager::LoadOutfit(FOutfitData& OutOutfitData, const FString& UserID, const FString& OutfitName)
{
	if (UserID.IsEmpty() || OutfitName.IsEmpty())
	{
		return false;
	}

	FString FileName = FString::Printf(*OutfitFileName, *OutfitName);
	FString FilePath = GetSaveGamePath(UserID) + TEXT("Outfits/") + FileName;

	if (!FPaths::FileExists(*FilePath))
	{
		return false;
	}

	FString JsonString;
	if (!FFileHelper::LoadFileToString(JsonString, *FilePath))
	{
		return false;
	}

	TSharedPtr<FJsonObject> JsonObject;
	TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(JsonString);

	if (!FJsonSerializer::Deserialize(Reader, JsonObject) || !JsonObject.IsValid())
	{
		return false;
	}

	OutOutfitData.OutfitName = JsonObject->GetStringField(TEXT("OutfitName"));
	OutOutfitData.CreatedTimestamp = JsonObject->GetIntegerField(TEXT("CreatedTimestamp"));

	return true;
}

bool USaveGameManager::SaveWorldMap(const FString& MapData, const FString& UserID, const FString& MapName)
{
	if (UserID.IsEmpty() || MapName.IsEmpty() || MapData.IsEmpty())
	{
		return false;
	}

	EnsureSaveDirectoriesExist(UserID);
	FString FileName = FString::Printf(*WorldMapFileName, *MapName);
	FString FilePath = GetSaveGamePath(UserID) + TEXT("WorldMaps/") + FileName;

	return FFileHelper::SaveStringToFile(MapData, *FilePath);
}

bool USaveGameManager::LoadWorldMap(FString& OutMapData, const FString& UserID, const FString& MapName)
{
	if (UserID.IsEmpty() || MapName.IsEmpty())
	{
		return false;
	}

	FString FileName = FString::Printf(*WorldMapFileName, *MapName);
	FString FilePath = GetSaveGamePath(UserID) + TEXT("WorldMaps/") + FileName;

	if (!FPaths::FileExists(*FilePath))
	{
		return false;
	}

	return FFileHelper::LoadFileToString(OutMapData, *FilePath);
}

FString USaveGameManager::GetSaveGamePath(const FString& UserID) const
{
	return SaveGameDirectory + UserID + TEXT("/");
}

void USaveGameManager::EnsureSaveDirectoriesExist(const FString& UserID)
{
	FString BasePath = GetSaveGamePath(UserID);
	FString OutfitsPath = BasePath + TEXT("Outfits/");
	FString WorldMapsPath = BasePath + TEXT("WorldMaps/");

	IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();

	if (!PlatformFile.DirectoryExists(*BasePath))
	{
		PlatformFile.CreateDirectory(*BasePath);
	}

	if (!PlatformFile.DirectoryExists(*OutfitsPath))
	{
		PlatformFile.CreateDirectory(*OutfitsPath);
	}

	if (!PlatformFile.DirectoryExists(*WorldMapsPath))
	{
		PlatformFile.CreateDirectory(*WorldMapsPath);
	}
}

int32 USaveGameManager::GetCurrentTimestamp() const
{
	return static_cast<int32>(FDateTime::Now().ToUnixTimestamp());
}

FString USaveGameManager::SerializeCharacterData(const FCharacterSaveData& CharacterData) const
{
	TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject());

	JsonObject->SetStringField(TEXT("CharacterName"), CharacterData.CharacterName);
	JsonObject->SetStringField(TEXT("CharacterGender"), CharacterData.CharacterGender);
	JsonObject->SetNumberField(TEXT("CharacterSlot"), CharacterData.CharacterSlot);

	FString OutputString;
	TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&OutputString);
	FJsonSerializer::Serialize(JsonObject.ToSharedRef(), Writer);

	return OutputString;
}

bool USaveGameManager::DeserializeCharacterData(const FString& JsonString, FCharacterSaveData& OutCharacterData) const
{
	TSharedPtr<FJsonObject> JsonObject;
	TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(JsonString);

	if (!FJsonSerializer::Deserialize(Reader, JsonObject) || !JsonObject.IsValid())
	{
		return false;
	}

	OutCharacterData.CharacterName = JsonObject->GetStringField(TEXT("CharacterName"));
	OutCharacterData.CharacterGender = JsonObject->GetStringField(TEXT("CharacterGender"));
	OutCharacterData.CharacterSlot = JsonObject->GetIntegerField(TEXT("CharacterSlot"));

	return true;
}

bool USaveGameManager::ValidateSaveFile(const FString& FilePath) const
{
	if (!FPaths::FileExists(*FilePath))
	{
		return false;
	}

	FString JsonString;
	if (!FFileHelper::LoadFileToString(JsonString, *FilePath))
	{
		return false;
	}

	TSharedPtr<FJsonObject> JsonObject;
	TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(JsonString);

	return FJsonSerializer::Deserialize(Reader, JsonObject) && JsonObject.IsValid();
}

void USaveGameManager::CreateBackup(const FString& FilePath)
{
	FString BackupPath = FilePath + TEXT(".backup");
	FPlatformFileManager::Get().GetPlatformFile().CopyFile(*BackupPath, *FilePath);
}