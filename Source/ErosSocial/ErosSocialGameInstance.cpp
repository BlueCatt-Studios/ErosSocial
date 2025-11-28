// Copyright BlueCatt Studios - All Rights Reserved
// ErosSocialGameInstance.cpp

#include "ErosSocialGameInstance.h"
#include "Systems/CharacterManager.h"
#include "Systems/SaveSystem/SaveGameManager.h"

UErosSocialGameInstance::UErosSocialGameInstance()
	: Username(TEXT(""))
	, UserID(TEXT(""))
	, bIsLoggedIn(false)
	, SelectedCharacterSlot(-1)
	, CharacterManager(nullptr)
	, SaveGameManager(nullptr)
{
}

void UErosSocialGameInstance::Init()
{
	Super::Init();

	InitializeManagers();

	UE_LOG(LogTemp, Warning, TEXT("ErosSocialGameInstance::Init - Game Instance initialized"));
}

void UErosSocialGameInstance::InitializeManagers()
{
	// Criar SaveGameManager
	if (!SaveGameManager)
	{
		SaveGameManager = NewObject<USaveGameManager>();
	}

	// Criar CharacterManager
	if (!CharacterManager)
	{
		CharacterManager = NewObject<UCharacterManager>();
	}

	UE_LOG(LogTemp, Warning, TEXT("ErosSocialGameInstance::InitializeManagers - Managers initialized"));
}

void UErosSocialGameInstance::SetUserLoggedIn(const FString& InUsername, const FString& InUserID)
{
	Username = InUsername;
	UserID = InUserID;
	bIsLoggedIn = true;

	// Inicializar CharacterManager com o UserID
	if (CharacterManager)
	{
		CharacterManager->Initialize(InUserID);
	}

	UE_LOG(LogTemp, Warning, TEXT("ErosSocialGameInstance::SetUserLoggedIn - User logged in: %s (ID: %s)"),
		*InUsername, *InUserID);
}

void UErosSocialGameInstance::Logout()
{
	Username = TEXT("");
	UserID = TEXT("");
	bIsLoggedIn = false;
	SelectedCharacterSlot = -1;
	SelectedCharacter = FCharacterSaveData();
	LoadedCharacters.Empty();

	UE_LOG(LogTemp, Warning, TEXT("ErosSocialGameInstance::Logout - User logged out"));
}

bool UErosSocialGameInstance::CreateCharacter(const FString& CharacterName, const FString& CharacterGender)
{
	if (!bIsLoggedIn || !CharacterManager)
	{
		UE_LOG(LogTemp, Error, TEXT("ErosSocialGameInstance::CreateCharacter - Not logged in or CharacterManager not available"));
		return false;
	}

	int32 SlotIndex = -1;
	if (!CharacterManager->CreateCharacter(CharacterName, CharacterGender, SlotIndex))
	{
		UE_LOG(LogTemp, Error, TEXT("ErosSocialGameInstance::CreateCharacter - Failed to create character"));
		return false;
	}

	// Recarregar lista de personagens
	LoadAllCharacters();

	UE_LOG(LogTemp, Warning, TEXT("ErosSocialGameInstance::CreateCharacter - Created '%s' in slot %d"),
		*CharacterName, SlotIndex);

	return true;
}

bool UErosSocialGameInstance::GetCharacterData(int32 SlotIndex, FCharacterSaveData& OutCharacterData)
{
	// Validate slot index
	if (SlotIndex < 0 || SlotIndex >= LoadedCharacters.Num())
	{
		UE_LOG(LogTemp, Error, TEXT("GetCharacterData: Invalid slot index %d"), SlotIndex);
		return false;
	}

	// Get character data
	FCharacterSaveData& CharData = LoadedCharacters[SlotIndex];

	// Check if slot is empty (character name is empty)
	if (CharData.CharacterName.IsEmpty())
	{
		UE_LOG(LogTemp, Warning, TEXT("GetCharacterData: Slot %d is empty"), SlotIndex);
		return false;
	}

	// Return data
	OutCharacterData = CharData;

	UE_LOG(LogTemp, Log, TEXT("GetCharacterData: Retrieved character '%s' from slot %d"),
		*CharData.CharacterName, SlotIndex);

	return true;
}

bool UErosSocialGameInstance::UpdateCharacterAppearance(
	int32 SlotIndex,
	const FBodyCustomization& NewBodyCustomization,
	const FAppearanceCustomization& NewAppearanceCustomization,
	const TArray<FClothingItemData>& NewCurrentOutfit)
{
	// Validar se está logado e CharacterManager existe
	if (!bIsLoggedIn || !CharacterManager)
	{
		UE_LOG(LogTemp, Error, TEXT("UpdateCharacterAppearance: Not logged in or CharacterManager not available"));
		return false;
	}

	// Validar índice do slot
	if (SlotIndex < 0 || SlotIndex >= LoadedCharacters.Num())
	{
		UE_LOG(LogTemp, Error, TEXT("UpdateCharacterAppearance: Invalid slot index %d"), SlotIndex);
		return false;
	}

	// Obter referência ao personagem
	FCharacterSaveData& CharData = LoadedCharacters[SlotIndex];

	// Verificar se slot tem personagem
	if (CharData.CharacterName.IsEmpty())
	{
		UE_LOG(LogTemp, Error, TEXT("UpdateCharacterAppearance: Slot %d is empty, cannot update"), SlotIndex);
		return false;
	}

	// Atualizar apenas dados de aparência (NÃO altera nome, gênero, slot, etc.)
	CharData.BodyCustomization = NewBodyCustomization;
	CharData.AppearanceCustomization = NewAppearanceCustomization;
	CharData.CurrentOutfit = NewCurrentOutfit;

	// Atualizar timestamp (converter FDateTime para Unix timestamp int32)
	CharData.LastModifiedTimestamp = static_cast<int32>(FDateTime::Now().ToUnixTimestamp());

	// Salvar usando o CharacterManager (função UpdateCharacter)
	if (!CharacterManager->UpdateCharacter(SlotIndex, CharData))
	{
		UE_LOG(LogTemp, Error, TEXT("UpdateCharacterAppearance: Failed to save character to disk"));
		return false;
	}

	// Se o personagem atualizado for o selecionado, atualizar também SelectedCharacter
	if (SelectedCharacterSlot == SlotIndex)
	{
		SelectedCharacter = CharData;
		UE_LOG(LogTemp, Log, TEXT("UpdateCharacterAppearance: Updated selected character data"));
	}

	UE_LOG(LogTemp, Log, TEXT("UpdateCharacterAppearance: Successfully updated appearance for character '%s' in slot %d"),
		*CharData.CharacterName, SlotIndex);

	return true;
}

bool UErosSocialGameInstance::LoadAllCharacters()
{
	if (!bIsLoggedIn || !CharacterManager)
	{
		UE_LOG(LogTemp, Error, TEXT("ErosSocialGameInstance::LoadAllCharacters - Not logged in or CharacterManager not available"));
		return false;
	}

	LoadedCharacters.Empty();

	if (!CharacterManager->LoadAllCharacters(LoadedCharacters))
	{
		UE_LOG(LogTemp, Warning, TEXT("ErosSocialGameInstance::LoadAllCharacters - No characters found"));
		return false;
	}

	// Disparar evento
	OnCharacterListUpdated.Broadcast();

	UE_LOG(LogTemp, Warning, TEXT("ErosSocialGameInstance::LoadAllCharacters - Loaded %d characters"), LoadedCharacters.Num());

	return true;
}

bool UErosSocialGameInstance::SelectCharacter(int32 SlotIndex)
{
	if (!bIsLoggedIn || !CharacterManager)
	{
		UE_LOG(LogTemp, Error, TEXT("ErosSocialGameInstance::SelectCharacter - Not logged in or CharacterManager not available"));
		return false;
	}

	if (SlotIndex < 0 || SlotIndex > 1)
	{
		UE_LOG(LogTemp, Error, TEXT("ErosSocialGameInstance::SelectCharacter - Invalid slot index: %d"), SlotIndex);
		return false;
	}

	// Carregar personagem
	FCharacterSaveData CharacterData;
	if (!CharacterManager->LoadCharacter(SlotIndex, CharacterData))
	{
		UE_LOG(LogTemp, Error, TEXT("ErosSocialGameInstance::SelectCharacter - Failed to load character from slot %d"), SlotIndex);
		return false;
	}

	SelectedCharacterSlot = SlotIndex;
	SelectedCharacter = CharacterData;

	// Disparar evento
	OnCharacterSelected.Broadcast(SelectedCharacter);

	UE_LOG(LogTemp, Warning, TEXT("ErosSocialGameInstance::SelectCharacter - Selected '%s' from slot %d"),
		*CharacterData.CharacterName, SlotIndex);

	return true;
}

bool UErosSocialGameInstance::DeleteCharacter(int32 SlotIndex)
{
	if (!bIsLoggedIn || !CharacterManager)
	{
		UE_LOG(LogTemp, Error, TEXT("ErosSocialGameInstance::DeleteCharacter - Not logged in or CharacterManager not available"));
		return false;
	}

	if (!CharacterManager->DeleteCharacter(SlotIndex))
	{
		UE_LOG(LogTemp, Error, TEXT("ErosSocialGameInstance::DeleteCharacter - Failed to delete character"));
		return false;
	}

	// Se o personagem deletado era o selecionado, desselecionar
	if (SelectedCharacterSlot == SlotIndex)
	{
		SelectedCharacterSlot = -1;
		SelectedCharacter = FCharacterSaveData();
	}

	// Recarregar lista
	LoadAllCharacters();

	UE_LOG(LogTemp, Warning, TEXT("ErosSocialGameInstance::DeleteCharacter - Deleted character from slot %d"), SlotIndex);

	return true;
}

bool UErosSocialGameInstance::CanCreateNewCharacter() const
{
	if (!CharacterManager)
	{
		return false;
	}

	return CharacterManager->CanCreateNewCharacter();
}