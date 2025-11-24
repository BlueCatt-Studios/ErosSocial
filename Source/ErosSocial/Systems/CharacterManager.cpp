// Copyright BlueCatt Studios - All Rights Reserved
// CharacterManager.cpp

#include "CharacterManager.h"
#include "Misc/Paths.h"

UCharacterManager::UCharacterManager()
	: SaveGameManager(nullptr)
	, CurrentUserID(TEXT(""))
{
}

void UCharacterManager::Initialize(const FString& InUserID)
{
	if (InUserID.IsEmpty())
	{
		UE_LOG(LogTemp, Error, TEXT("CharacterManager::Initialize - UserID is empty!"));
		return;
	}

	CurrentUserID = InUserID;

	// Criar SaveGameManager se não existir
	if (!SaveGameManager)
	{
		SaveGameManager = NewObject<USaveGameManager>();
		if (!SaveGameManager)
		{
			UE_LOG(LogTemp, Error, TEXT("CharacterManager::Initialize - Failed to create SaveGameManager!"));
			return;
		}
	}

	UE_LOG(LogTemp, Warning, TEXT("CharacterManager::Initialize - Initialized for UserID: %s"), *CurrentUserID);
}

bool UCharacterManager::CreateCharacter(const FString& CharacterName, const FString& CharacterGender, int32& OutSlotIndex)
{
	// Validar inicialização
	if (CurrentUserID.IsEmpty())
	{
		UE_LOG(LogTemp, Error, TEXT("CharacterManager::CreateCharacter - Not initialized!"));
		return false;
	}

	if (!SaveGameManager)
	{
		UE_LOG(LogTemp, Error, TEXT("CharacterManager::CreateCharacter - SaveGameManager not available!"));
		return false;
	}

	// Validar parâmetros
	if (!ValidateCharacterName(CharacterName))
	{
		UE_LOG(LogTemp, Error, TEXT("CharacterManager::CreateCharacter - Invalid character name: %s"), *CharacterName);
		return false;
	}

	if (CharacterGender != TEXT("Male") && CharacterGender != TEXT("Female"))
	{
		UE_LOG(LogTemp, Error, TEXT("CharacterManager::CreateCharacter - Invalid gender: %s"), *CharacterGender);
		return false;
	}

	// Verificar se pode criar
	if (!CanCreateNewCharacter())
	{
		UE_LOG(LogTemp, Error, TEXT("CharacterManager::CreateCharacter - Maximum characters reached!"));
		return false;
	}

	// Encontrar slot disponível
	OutSlotIndex = GetNextAvailableSlot();
	if (OutSlotIndex < 0)
	{
		UE_LOG(LogTemp, Error, TEXT("CharacterManager::CreateCharacter - No available slots!"));
		return false;
	}

	// Criar dados do personagem
	FCharacterSaveData NewCharacterData;
	NewCharacterData.CharacterName = CharacterName;
	NewCharacterData.CharacterGender = CharacterGender;
	NewCharacterData.CharacterSlot = OutSlotIndex;
	NewCharacterData.CreatedTimestamp = GetCurrentTimestamp();
	NewCharacterData.LastModifiedTimestamp = GetCurrentTimestamp();

	// Inicializar customizações com valores padrão
	NewCharacterData.BodyCustomization.BreastSize = 0.5f;
	NewCharacterData.BodyCustomization.ButtSize = 0.5f;
	NewCharacterData.BodyCustomization.Height = 0.5f;
	NewCharacterData.BodyCustomization.Weight = 0.5f;
	NewCharacterData.BodyCustomization.Muscle = 0.5f;

	NewCharacterData.AppearanceCustomization.HairStyle = TEXT("Default");
	NewCharacterData.AppearanceCustomization.HairColor = FLinearColor::Black;
	NewCharacterData.AppearanceCustomization.SkinColor = FLinearColor::White;

	// Salvar personagem
	if (!SaveGameManager->SaveCharacterData(NewCharacterData, CurrentUserID, OutSlotIndex))
	{
		UE_LOG(LogTemp, Error, TEXT("CharacterManager::CreateCharacter - Failed to save character!"));
		return false;
	}

	UE_LOG(LogTemp, Warning, TEXT("CharacterManager::CreateCharacter - Successfully created '%s' in slot %d"), 
		   *CharacterName, OutSlotIndex);

	return true;
}

bool UCharacterManager::LoadCharacter(int32 SlotIndex, FCharacterSaveData& OutCharacterData)
{
	if (CurrentUserID.IsEmpty() || !SaveGameManager)
	{
		return false;
	}

	if (SlotIndex < 0 || SlotIndex > 1)
	{
		UE_LOG(LogTemp, Error, TEXT("CharacterManager::LoadCharacter - Invalid slot index: %d"), SlotIndex);
		return false;
	}

	if (!SaveGameManager->LoadCharacterData(OutCharacterData, CurrentUserID, SlotIndex))
	{
		UE_LOG(LogTemp, Warning, TEXT("CharacterManager::LoadCharacter - Character not found in slot %d"), SlotIndex);
		return false;
	}

	UE_LOG(LogTemp, Warning, TEXT("CharacterManager::LoadCharacter - Successfully loaded '%s' from slot %d"), 
		   *OutCharacterData.CharacterName, SlotIndex);

	return true;
}

bool UCharacterManager::DeleteCharacter(int32 SlotIndex)
{
	if (CurrentUserID.IsEmpty() || !SaveGameManager)
	{
		return false;
	}

	if (SlotIndex < 0 || SlotIndex > 1)
	{
		return false;
	}

	// Verificar se personagem existe antes de deletar
	if (!SaveGameManager->CharacterExists(CurrentUserID, SlotIndex))
	{
		UE_LOG(LogTemp, Warning, TEXT("CharacterManager::DeleteCharacter - Character not found in slot %d"), SlotIndex);
		return false;
	}

	if (!SaveGameManager->DeleteCharacter(CurrentUserID, SlotIndex))
	{
		UE_LOG(LogTemp, Error, TEXT("CharacterManager::DeleteCharacter - Failed to delete character!"));
		return false;
	}

	UE_LOG(LogTemp, Warning, TEXT("CharacterManager::DeleteCharacter - Successfully deleted character from slot %d"), SlotIndex);

	return true;
}

bool UCharacterManager::LoadAllCharacters(TArray<FCharacterSaveData>& OutCharacters)
{
	if (CurrentUserID.IsEmpty() || !SaveGameManager)
	{
		return false;
	}

	OutCharacters.Empty();

	// Tentar carregar ambos os slots
	for (int32 SlotIndex = 0; SlotIndex < MaxCharactersPerAccount; ++SlotIndex)
	{
		if (SaveGameManager->CharacterExists(CurrentUserID, SlotIndex))
		{
			FCharacterSaveData CharacterData;
			if (SaveGameManager->LoadCharacterData(CharacterData, CurrentUserID, SlotIndex))
			{
				OutCharacters.Add(CharacterData);
			}
		}
	}

	UE_LOG(LogTemp, Warning, TEXT("CharacterManager::LoadAllCharacters - Loaded %d characters"), OutCharacters.Num());

	return true;
}

int32 UCharacterManager::GetCreatedCharacterCount()
{
	if (CurrentUserID.IsEmpty() || !SaveGameManager)
	{
		return 0;
	}

	int32 Count = 0;
	for (int32 SlotIndex = 0; SlotIndex < MaxCharactersPerAccount; ++SlotIndex)
	{
		if (SaveGameManager->CharacterExists(CurrentUserID, SlotIndex))
		{
			Count++;
		}
	}

	return Count;
}

bool UCharacterManager::CanCreateNewCharacter()
{
	return GetCreatedCharacterCount() < MaxCharactersPerAccount;
}

int32 UCharacterManager::GetNextAvailableSlot()
{
	if (CurrentUserID.IsEmpty() || !SaveGameManager)
	{
		return -1;
	}

	for (int32 SlotIndex = 0; SlotIndex < MaxCharactersPerAccount; ++SlotIndex)
	{
		if (!SaveGameManager->CharacterExists(CurrentUserID, SlotIndex))
		{
			return SlotIndex;
		}
	}

	return -1;
}

bool UCharacterManager::UpdateCharacter(int32 SlotIndex, const FCharacterSaveData& CharacterData)
{
	if (CurrentUserID.IsEmpty() || !SaveGameManager)
	{
		return false;
	}

	if (SlotIndex < 0 || SlotIndex > 1)
	{
		return false;
	}

	if (!SaveGameManager->CharacterExists(CurrentUserID, SlotIndex))
	{
		UE_LOG(LogTemp, Error, TEXT("CharacterManager::UpdateCharacter - Character not found in slot %d"), SlotIndex);
		return false;
	}

	FCharacterSaveData UpdatedData = CharacterData;
	UpdatedData.LastModifiedTimestamp = GetCurrentTimestamp();

	if (!SaveGameManager->SaveCharacterData(UpdatedData, CurrentUserID, SlotIndex))
	{
		UE_LOG(LogTemp, Error, TEXT("CharacterManager::UpdateCharacter - Failed to update character!"));
		return false;
	}

	UE_LOG(LogTemp, Warning, TEXT("CharacterManager::UpdateCharacter - Successfully updated '%s' in slot %d"), 
		   *CharacterData.CharacterName, SlotIndex);

	return true;
}

bool UCharacterManager::ValidateCharacterName(const FString& CharacterName)
{
	// Verificar comprimento
	if (CharacterName.Len() < MinCharacterNameLength || CharacterName.Len() > MaxCharacterNameLength)
	{
		UE_LOG(LogTemp, Warning, TEXT("CharacterManager::ValidateCharacterName - Name length invalid: %d (min: %d, max: %d)"),
			   CharacterName.Len(), MinCharacterNameLength, MaxCharacterNameLength);
		return false;
	}

	// Verificar caracteres válidos
	for (TCHAR Char : CharacterName)
	{
		if (!IsValidCharacterNameCharacter(Char))
		{
			UE_LOG(LogTemp, Warning, TEXT("CharacterManager::ValidateCharacterName - Invalid character: %c"), Char);
			return false;
		}
	}

	return true;
}

bool UCharacterManager::CharacterExists(int32 SlotIndex)
{
	if (CurrentUserID.IsEmpty() || !SaveGameManager)
	{
		return false;
	}

	return SaveGameManager->CharacterExists(CurrentUserID, SlotIndex);
}

FString UCharacterManager::GetCurrentUserID() const
{
	return CurrentUserID;
}

bool UCharacterManager::IsValidCharacterNameCharacter(TCHAR Character)
{
	// Permitir letras, números, espaço, hífen e underscore
	return FChar::IsAlnum(Character) || Character == TEXT(' ') || Character == TEXT('-') || Character == TEXT('_');
}

int32 UCharacterManager::GetCurrentTimestamp() const
{
	return static_cast<int32>(FDateTime::Now().ToUnixTimestamp());
}