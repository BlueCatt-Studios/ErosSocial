// Copyright BlueCatt Studios - All Rights Reserved
// ClothingSystem.cpp

#include "ClothingSystem.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/Character.h"
#include "SaveSystem/SaveGameManager.h"

UClothingSystem::UClothingSystem()
	: TargetCharacter(nullptr)
	, SaveGameManager(nullptr)
{
}

void UClothingSystem::Initialize(APawn* InTargetCharacter)
{
	if (!InTargetCharacter)
	{
		UE_LOG(LogTemp, Error, TEXT("ClothingSystem::Initialize - TargetCharacter is null!"));
		return;
	}

	TargetCharacter = InTargetCharacter;

	// Criar SaveGameManager se não existir
	if (!SaveGameManager)
	{
		SaveGameManager = NewObject<USaveGameManager>();
	}

	InitializeSocketMap();

	UE_LOG(LogTemp, Warning, TEXT("ClothingSystem::Initialize - Initialized for character: %s"), *InTargetCharacter->GetName());
}

bool UClothingSystem::EquipClothing(const FEquippedClothingItem& ClothingItem)
{
	if (!TargetCharacter)
	{
		UE_LOG(LogTemp, Error, TEXT("ClothingSystem::EquipClothing - TargetCharacter is null!"));
		return false;
	}

	// Se já tem roupa nesse slot, remover antes
	if (EquippedItems.Contains(ClothingItem.SlotType))
	{
		UnequipClothing(ClothingItem.SlotType);
	}

	// Adicionar ao mapa
	EquippedItems.Add(ClothingItem.SlotType, ClothingItem);

	// Aplicar material (implementar depois com mesh)
	ApplyMaterialToClothing(ClothingItem);

	UE_LOG(LogTemp, Warning, TEXT("ClothingSystem::EquipClothing - Equipped %s in slot %d"), 
		   *ClothingItem.ItemName, (int32)ClothingItem.SlotType);

	return true;
}

bool UClothingSystem::UnequipClothing(EClothingSlot SlotType)
{
	if (!EquippedItems.Contains(SlotType))
	{
		UE_LOG(LogTemp, Warning, TEXT("ClothingSystem::UnequipClothing - No item equipped in slot %d"), (int32)SlotType);
		return false;
	}

	RemoveMaterialFromClothing(SlotType);
	EquippedItems.Remove(SlotType);

	UE_LOG(LogTemp, Warning, TEXT("ClothingSystem::UnequipClothing - Unequipped slot %d"), (int32)SlotType);

	return true;
}

void UClothingSystem::UnequipAll()
{
	TArray<EClothingSlot> SlotsToRemove;
	EquippedItems.GetKeys(SlotsToRemove);

	for (EClothingSlot Slot : SlotsToRemove)
	{
		UnequipClothing(Slot);
	}

	UE_LOG(LogTemp, Warning, TEXT("ClothingSystem::UnequipAll - Unequipped all items"));
}

bool UClothingSystem::GetEquippedItem(EClothingSlot SlotType, FEquippedClothingItem& OutItem) const
{
	if (EquippedItems.Contains(SlotType))
	{
		OutItem = EquippedItems[SlotType];
		return true;
	}

	return false;
}

bool UClothingSystem::IsSlotEquipped(EClothingSlot SlotType) const
{
	return EquippedItems.Contains(SlotType);
}

void UClothingSystem::GetAllEquippedItems(TArray<FEquippedClothingItem>& OutItems) const
{
	EquippedItems.GenerateValueArray(OutItems);
}

bool UClothingSystem::SaveCurrentOutfit(const FString& OutfitName, const FString& UserID)
{
	if (!SaveGameManager || OutfitName.IsEmpty() || UserID.IsEmpty())
	{
		return false;
	}

	FOutfitData OutfitData = GetCurrentOutfitData();
	OutfitData.OutfitName = OutfitName;
	OutfitData.CreatedTimestamp = static_cast<int32>(FDateTime::Now().ToUnixTimestamp());

	return SaveGameManager->SaveOutfit(OutfitData, UserID, OutfitName);
}

bool UClothingSystem::LoadOutfit(const FString& OutfitName, const FString& UserID)
{
	if (!SaveGameManager || OutfitName.IsEmpty() || UserID.IsEmpty())
	{
		return false;
	}

	FOutfitData OutfitData;
	if (!SaveGameManager->LoadOutfit(OutfitData, UserID, OutfitName))
	{
		return false;
	}

	return ApplyOutfitData(OutfitData);
}

FOutfitData UClothingSystem::GetCurrentOutfitData() const
{
	FOutfitData OutfitData;

	for (const auto& Pair : EquippedItems)
	{
		const FEquippedClothingItem& Item = Pair.Value;
		
		FClothingItemData ItemData;
		ItemData.ItemID = Item.ItemID;
		ItemData.SlotType = SlotToString(Item.SlotType);
		ItemData.MeshPath = Item.MeshPath;
		ItemData.MaterialPath = Item.MaterialPath;
		ItemData.Color = Item.Color;
		ItemData.bEquipped = true;

		OutfitData.ClothingItems.Add(ItemData);
	}

	return OutfitData;
}

bool UClothingSystem::ApplyOutfitData(const FOutfitData& OutfitData)
{
	UnequipAll();

	for (const FClothingItemData& ItemData : OutfitData.ClothingItems)
	{
		FEquippedClothingItem EquippedItem;
		EquippedItem.SlotType = StringToSlot(ItemData.SlotType);
		EquippedItem.ItemID = ItemData.ItemID;
		EquippedItem.MeshPath = ItemData.MeshPath;
		EquippedItem.MaterialPath = ItemData.MaterialPath;
		EquippedItem.Color = ItemData.Color;
		EquippedItem.ItemName = ItemData.ItemID;

		if (!EquipClothing(EquippedItem))
		{
			UE_LOG(LogTemp, Warning, TEXT("ClothingSystem::ApplyOutfitData - Failed to equip item: %s"), *ItemData.ItemID);
		}
	}

	return true;
}

FString UClothingSystem::GetSocketNameForSlot(EClothingSlot SlotType) const
{
	if (SlotSocketMap.Contains(SlotType))
	{
		return SlotSocketMap[SlotType];
	}

	return TEXT("");
}

int32 UClothingSystem::GetTotalSlots() const
{
	return (int32)EClothingSlot::Count;
}

int32 UClothingSystem::GetEquippedItemCount() const
{
	return EquippedItems.Num();
}

void UClothingSystem::InitializeSocketMap()
{
	SlotSocketMap.Empty();

	SlotSocketMap.Add(EClothingSlot::Top, TEXT("Chest"));
	SlotSocketMap.Add(EClothingSlot::Bottom, TEXT("Pelvis"));
	SlotSocketMap.Add(EClothingSlot::Shoes, TEXT("Feet"));
	SlotSocketMap.Add(EClothingSlot::Hat, TEXT("Head"));
	SlotSocketMap.Add(EClothingSlot::Accessories, TEXT("Spine01"));
	SlotSocketMap.Add(EClothingSlot::Underwear, TEXT("Pelvis"));
	SlotSocketMap.Add(EClothingSlot::Socks, TEXT("Feet"));
	SlotSocketMap.Add(EClothingSlot::Gloves, TEXT("Hand_R"));

	UE_LOG(LogTemp, Log, TEXT("ClothingSystem::InitializeSocketMap - Socket map initialized"));
}

bool UClothingSystem::ApplyMaterialToClothing(const FEquippedClothingItem& Item)
{
	// Implementação será feita quando integrar com Character Pawn
	// Por enquanto, apenas log
	UE_LOG(LogTemp, Log, TEXT("ClothingSystem::ApplyMaterialToClothing - Applied material to: %s"), *Item.ItemName);
	return true;
}

bool UClothingSystem::RemoveMaterialFromClothing(EClothingSlot SlotType)
{
	// Implementação será feita quando integrar com Character Pawn
	UE_LOG(LogTemp, Log, TEXT("ClothingSystem::RemoveMaterialFromClothing - Removed material from slot: %d"), (int32)SlotType);
	return true;
}

FString UClothingSystem::SlotToString(EClothingSlot SlotType) const
{
	switch (SlotType)
	{
		case EClothingSlot::Top: return TEXT("Top");
		case EClothingSlot::Bottom: return TEXT("Bottom");
		case EClothingSlot::Shoes: return TEXT("Shoes");
		case EClothingSlot::Hat: return TEXT("Hat");
		case EClothingSlot::Accessories: return TEXT("Accessories");
		case EClothingSlot::Underwear: return TEXT("Underwear");
		case EClothingSlot::Socks: return TEXT("Socks");
		case EClothingSlot::Gloves: return TEXT("Gloves");
		default: return TEXT("Unknown");
	}
}

EClothingSlot UClothingSystem::StringToSlot(const FString& SlotString) const
{
	if (SlotString == TEXT("Top")) return EClothingSlot::Top;
	if (SlotString == TEXT("Bottom")) return EClothingSlot::Bottom;
	if (SlotString == TEXT("Shoes")) return EClothingSlot::Shoes;
	if (SlotString == TEXT("Hat")) return EClothingSlot::Hat;
	if (SlotString == TEXT("Accessories")) return EClothingSlot::Accessories;
	if (SlotString == TEXT("Underwear")) return EClothingSlot::Underwear;
	if (SlotString == TEXT("Socks")) return EClothingSlot::Socks;
	if (SlotString == TEXT("Gloves")) return EClothingSlot::Gloves;

	return EClothingSlot::Top;
}