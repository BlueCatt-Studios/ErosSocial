// Copyright BlueCatt Studios - All Rights Reserved
// ClothingSystem.h
// Sistema modular de vestuário

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "CharacterSaveData.h"
#include "ClothingSystem.generated.h"

/**
 * Enum para tipos de slots de roupa
 */
UENUM(BlueprintType)
enum class EClothingSlot : uint8
{
	Top			UMETA(DisplayName = "Top"),
	Bottom		UMETA(DisplayName = "Bottom"),
	Shoes		UMETA(DisplayName = "Shoes"),
	Hat			UMETA(DisplayName = "Hat"),
	Accessories	UMETA(DisplayName = "Accessories"),
	Underwear	UMETA(DisplayName = "Underwear"),
	Socks		UMETA(DisplayName = "Socks"),
	Gloves		UMETA(DisplayName = "Gloves"),
	Count		UMETA(Hidden)
};

/**
 * Estrutura para uma peça de roupa em uso
 */
USTRUCT(BlueprintType)
struct FEquippedClothingItem
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Clothing")
	EClothingSlot SlotType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Clothing")
	FString ItemID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Clothing")
	FString ItemName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Clothing")
	FString MeshPath;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Clothing")
	FString MaterialPath;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Clothing")
	FLinearColor Color = FLinearColor::White;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Clothing")
	FString SocketName; // Socket no character onde vai attach
};

/**
 * Gerenciador centralizado de vestuário
 * Responsável por equipar/remover roupas e aplicar materiais
 */
UCLASS(Blueprintable, BlueprintType)
class EROSSOCIAL_API UClothingSystem : public UObject
{
	GENERATED_BODY()

public:
	UClothingSystem();

	// ========== INICIALIZAÇÃO ==========

	/**
	 * Inicializa o sistema com um character
	 */
	UFUNCTION(BlueprintCallable, Category = "Clothing")
	void Initialize(class APawn* TargetCharacter);

	// ========== EQUIPAR ROUPA ==========

	/**
	 * Equipa uma peça de roupa
	 */
	UFUNCTION(BlueprintCallable, Category = "Clothing")
	bool EquipClothing(const FEquippedClothingItem& ClothingItem);

	/**
	 * Remove uma peça de roupa de um slot
	 */
	UFUNCTION(BlueprintCallable, Category = "Clothing")
	bool UnequipClothing(EClothingSlot SlotType);

	/**
	 * Remove todas as roupas
	 */
	UFUNCTION(BlueprintCallable, Category = "Clothing")
	void UnequipAll();

	// ========== CONSULTAR ESTADO ==========

	/**
	 * Obtém a roupa equipada em um slot
	 */
	UFUNCTION(BlueprintCallable, Category = "Clothing")
	bool GetEquippedItem(EClothingSlot SlotType, FEquippedClothingItem& OutItem) const;

	/**
	 * Verifica se um slot tem roupa equipada
	 */
	UFUNCTION(BlueprintCallable, Category = "Clothing")
	bool IsSlotEquipped(EClothingSlot SlotType) const;

	/**
	 * Obtém todas as roupas equipadas
	 */
	UFUNCTION(BlueprintCallable, Category = "Clothing")
	void GetAllEquippedItems(TArray<FEquippedClothingItem>& OutItems) const;

	// ========== SALVAR/CARREGAR OUTFITS ==========

	/**
	 * Salva o outfit atual como um arquivo .finesse
	 */
	UFUNCTION(BlueprintCallable, Category = "Clothing")
	bool SaveCurrentOutfit(const FString& OutfitName, const FString& UserID);

	/**
	 * Carrega um outfit de um arquivo .finesse
	 */
	UFUNCTION(BlueprintCallable, Category = "Clothing")
	bool LoadOutfit(const FString& OutfitName, const FString& UserID);

	/**
	 * Converte roupas equipadas para FOutfitData
	 */
	UFUNCTION(BlueprintCallable, Category = "Clothing")
	FOutfitData GetCurrentOutfitData() const;

	/**
	 * Carrega um outfit a partir de FOutfitData
	 */
	UFUNCTION(BlueprintCallable, Category = "Clothing")
	bool ApplyOutfitData(const FOutfitData& OutfitData);

	// ========== INFORMAÇÕES ==========

	/**
	 * Obtém o nome do socket para um tipo de slot
	 */
	UFUNCTION(BlueprintPure, Category = "Clothing")
	FString GetSocketNameForSlot(EClothingSlot SlotType) const;

	/**
	 * Obtém o número total de slots
	 */
	UFUNCTION(BlueprintPure, Category = "Clothing")
	int32 GetTotalSlots() const;

	/**
	 * Obtém quantas peças estão equipadas
	 */
	UFUNCTION(BlueprintPure, Category = "Clothing")
	int32 GetEquippedItemCount() const;

protected:
	// Referência ao character
	UPROPERTY()
	class APawn* TargetCharacter;

	// Mapa de roupas equipadas por slot
	UPROPERTY(VisibleAnywhere, Category = "Clothing")
	TMap<EClothingSlot, FEquippedClothingItem> EquippedItems;

	// Referência ao SaveGameManager
	UPROPERTY()
	class USaveGameManager* SaveGameManager;

	// ========== CONSTANTES ==========

	// Mapa de sockets padrão para cada slot
	UPROPERTY(EditDefaultsOnly, Category = "Clothing")
	TMap<EClothingSlot, FString> SlotSocketMap;

	// ========== FUNÇÕES PRIVADAS ==========

	/**
	 * Inicializa o mapa de sockets
	 */
	void InitializeSocketMap();

	/**
	 * Aplica material a uma peça de roupa
	 */
	bool ApplyMaterialToClothing(const FEquippedClothingItem& Item);

	/**
	 * Remove material de uma peça
	 */
	bool RemoveMaterialFromClothing(EClothingSlot SlotType);

	/**
	 * Converte enum para string
	 */
	FString SlotToString(EClothingSlot SlotType) const;

	/**
	 * Converte string para enum
	 */
	EClothingSlot StringToSlot(const FString& SlotString) const;
};