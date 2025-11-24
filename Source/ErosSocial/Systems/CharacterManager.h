// Copyright BlueCatt Studios - All Rights Reserved
// CharacterManager.h
// Gerencia criação, deleção e listagem de personagens

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "CharacterSaveData.h"
#include "SaveGameManager.h"
#include "CharacterManager.generated.h"

/**
 * Gerenciador centralizado de personagens
 * Responsável por CRUD (Create, Read, Update, Delete) de personagens
 */
UCLASS(Blueprintable, BlueprintType)
class EROSSOCIAL_API UCharacterManager : public UObject
{
	GENERATED_BODY()

public:
	UCharacterManager();

	// ========== INICIALIZAÇÃO ==========

	/**
	 * Inicializa o gerenciador com um UserID
	 */
	UFUNCTION(BlueprintCallable, Category = "Character")
	void Initialize(const FString& InUserID);

	// ========== CRIAR PERSONAGEM ==========

	/**
	 * Cria um novo personagem
	 * @param CharacterName - Nome do personagem
	 * @param CharacterGender - Gênero (Male/Female)
	 * @param OutSlotIndex - Índice do slot onde foi criado (0 ou 1)
	 * @return true se criou com sucesso
	 */
	UFUNCTION(BlueprintCallable, Category = "Character")
	bool CreateCharacter(const FString& CharacterName, const FString& CharacterGender, int32& OutSlotIndex);

	// ========== CARREGAR PERSONAGEM ==========

	/**
	 * Carrega dados de um personagem
	 * @param SlotIndex - Índice do slot (0 ou 1)
	 * @param OutCharacterData - Estrutura com os dados
	 * @return true se carregou com sucesso
	 */
	UFUNCTION(BlueprintCallable, Category = "Character")
	bool LoadCharacter(int32 SlotIndex, FCharacterSaveData& OutCharacterData);

	// ========== DELETAR PERSONAGEM ==========

	/**
	 * Deleta um personagem
	 */
	UFUNCTION(BlueprintCallable, Category = "Character")
	bool DeleteCharacter(int32 SlotIndex);

	// ========== LISTAR PERSONAGENS ==========

	/**
	 * Carrega todos os personagens do usuário
	 */
	UFUNCTION(BlueprintCallable, Category = "Character")
	bool LoadAllCharacters(TArray<FCharacterSaveData>& OutCharacters);

	/**
	 * Verifica quantos slots estão preenchidos
	 */
	UFUNCTION(BlueprintCallable, Category = "Character")
	int32 GetCreatedCharacterCount();

	/**
	 * Verifica se pode criar novo personagem (máximo 2)
	 */
	UFUNCTION(BlueprintCallable, Category = "Character")
	bool CanCreateNewCharacter();

	/**
	 * Obtém o primeiro slot vazio (-1 se nenhum disponível)
	 */
	UFUNCTION(BlueprintCallable, Category = "Character")
	int32 GetNextAvailableSlot();

	// ========== ATUALIZAR PERSONAGEM ==========

	/**
	 * Atualiza dados de um personagem existente
	 */
	UFUNCTION(BlueprintCallable, Category = "Character")
	bool UpdateCharacter(int32 SlotIndex, const FCharacterSaveData& CharacterData);

	// ========== VALIDAÇÕES ==========

	/**
	 * Valida um nome de personagem
	 */
	UFUNCTION(BlueprintCallable, Category = "Character")
	bool ValidateCharacterName(const FString& CharacterName);

	/**
	 * Verifica se um personagem existe
	 */
	UFUNCTION(BlueprintCallable, Category = "Character")
	bool CharacterExists(int32 SlotIndex);

	/**
	 * Obtém o UserID atual
	 */
	UFUNCTION(BlueprintPure, Category = "Character")
	FString GetCurrentUserID() const;

protected:
	// Referência ao SaveGameManager
	UPROPERTY()
	USaveGameManager* SaveGameManager;

	// UserID do usuário atual
	UPROPERTY(VisibleAnywhere, Category = "Character")
	FString CurrentUserID;

	// ========== CONSTANTES ==========
	
	UPROPERTY(EditDefaultsOnly, Category = "Character")
	int32 MaxCharactersPerAccount = 2;

	UPROPERTY(EditDefaultsOnly, Category = "Character")
	int32 MinCharacterNameLength = 3;

	UPROPERTY(EditDefaultsOnly, Category = "Character")
	int32 MaxCharacterNameLength = 32;

	// ========== FUNÇÕES PRIVADAS ==========

	/**
	 * Valida caracteres permitidos no nome
	 */
	bool IsValidCharacterNameCharacter(TCHAR Character);

	/**
	 * Obtém timestamp atual
	 */
	int32 GetCurrentTimestamp() const;
};