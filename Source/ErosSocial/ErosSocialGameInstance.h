// Copyright BlueCatt Studios - All Rights Reserved
// ErosSocialGameInstance.h
// Gerencia dados persistentes do jogo (login, personagem selecionado, etc)

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "CharacterSaveData.h"
#include "ErosSocialGameInstance.generated.h"

class UCharacterManager;
class USaveGameManager;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCharacterSelected, FCharacterSaveData, CharacterData);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCharacterListUpdated);

UCLASS()
class EROSSOCIAL_API UErosSocialGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UErosSocialGameInstance();

	// ========== INICIALIZAÇÃO ==========

	virtual void Init() override;

	// ========== AUTENTICAÇÃO ==========

	/**
	 * Define usuário logado
	 */
	UFUNCTION(BlueprintCallable, Category = "Authentication")
	void SetUserLoggedIn(const FString& InUsername, const FString& InUserID);

	/**
	 * Faz logout
	 */
	UFUNCTION(BlueprintCallable, Category = "Authentication")
	void Logout();

	/**
	 * Verifica se usuário está logado
	 */
	UFUNCTION(BlueprintPure, Category = "Authentication")
	bool IsUserLoggedIn() const { return bIsLoggedIn; }

	/**
	 * Obtém o UserID atual
	 */
	UFUNCTION(BlueprintPure, Category = "Authentication")
	FString GetUserID() const { return UserID; }

	// ========== GERENCIAMENTO DE PERSONAGENS ==========

	/**
	 * Cria um novo personagem
	 */
	UFUNCTION(BlueprintCallable, Category = "Character")
	bool CreateCharacter(const FString& CharacterName, const FString& CharacterGender);

	/**
	 * Carrega todos os personagens do usuário
	 */
	UFUNCTION(BlueprintCallable, Category = "Character")
	bool LoadAllCharacters();

	/**
	 * Seleciona um personagem para jogar
	 */
	UFUNCTION(BlueprintCallable, Category = "Character")
	bool SelectCharacter(int32 SlotIndex);

	/**
	 * Deleta um personagem
	 */
	UFUNCTION(BlueprintCallable, Category = "Character")
	bool DeleteCharacter(int32 SlotIndex);

	/**
	 * Verifica se pode criar novo personagem
	 */
	UFUNCTION(BlueprintPure, Category = "Character")
	bool CanCreateNewCharacter() const;

	/**
	 * Obtém a lista de personagens carregados
	 */
	UFUNCTION(BlueprintPure, Category = "Character")
	const TArray<FCharacterSaveData>& GetLoadedCharacters() const { return LoadedCharacters; }

	/**
	 * Obtém o personagem selecionado
	 */
	UFUNCTION(BlueprintPure, Category = "Character")
	const FCharacterSaveData& GetSelectedCharacter() const { return SelectedCharacter; }

	/**
	 * Obtém o índice do personagem selecionado
	 */
	UFUNCTION(BlueprintPure, Category = "Character")
	int32 GetSelectedCharacterSlot() const { return SelectedCharacterSlot; }

	/**
	 * Get character data from a specific slot
	 * @param SlotIndex - The slot to retrieve (0 or 1)
	 * @param OutCharacterData - Output parameter with character data
	 * @return true if slot has a character, false if empty
	 */
	UFUNCTION(BlueprintCallable, Category = "Character")
	bool GetCharacterData(int32 SlotIndex, FCharacterSaveData& OutCharacterData);

	/**
	 * Update only the appearance/customization of an existing character
	 * Does NOT change name, gender, or slot
	 * @param SlotIndex - The slot to update (0 or 1)
	 * @param NewBodyCustomization - New body morphs
	 * @param NewAppearanceCustomization - New appearance settings
	 * @param NewCurrentOutfit - New outfit data (array of clothing items)
	 * @return true if updated successfully, false otherwise
	 */
	UFUNCTION(BlueprintCallable, Category = "Character")
	bool UpdateCharacterAppearance(
		int32 SlotIndex,
		const FBodyCustomization& NewBodyCustomization,
		const FAppearanceCustomization& NewAppearanceCustomization,
		const TArray<FClothingItemData>& NewCurrentOutfit
	);

	// ========== GERENCIADORES ==========

	/**
	 * Obtém o Character Manager
	 */
	UFUNCTION(BlueprintPure, Category = "Managers")
	UCharacterManager* GetCharacterManager() const { return CharacterManager; }

	/**
	 * Obtém o Save Game Manager
	 */
	UFUNCTION(BlueprintPure, Category = "Managers")
	USaveGameManager* GetSaveGameManager() const { return SaveGameManager; }

	// ========== EVENTOS ==========

	/** Disparado quando um personagem é selecionado */
	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnCharacterSelected OnCharacterSelected;

	/** Disparado quando a lista de personagens é atualizada */
	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnCharacterListUpdated OnCharacterListUpdated;

protected:

	// ========== DADOS DO USUÁRIO ==========

	UPROPERTY(VisibleAnywhere, Category = "Authentication")
	FString Username;

	UPROPERTY(VisibleAnywhere, Category = "Authentication")
	FString UserID;

	UPROPERTY(VisibleAnywhere, Category = "Authentication")
	bool bIsLoggedIn;

	// ========== DADOS DE PERSONAGEM ==========

	UPROPERTY(VisibleAnywhere, Category = "Character")
	TArray<FCharacterSaveData> LoadedCharacters;

	UPROPERTY(VisibleAnywhere, Category = "Character")
	FCharacterSaveData SelectedCharacter;

	UPROPERTY(VisibleAnywhere, Category = "Character")
	int32 SelectedCharacterSlot;

	// ========== GERENCIADORES ==========

	UPROPERTY()
	UCharacterManager* CharacterManager;

	UPROPERTY()
	USaveGameManager* SaveGameManager;

	// ========== FUNÇÕES PRIVADAS ==========

	void InitializeManagers();
};