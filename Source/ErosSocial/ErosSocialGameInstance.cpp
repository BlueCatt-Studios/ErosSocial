// Copyrigh BlueCatt Studios - All Rights Reserverd
// ErosSocialGameInstance.cpp

#include "ErosSocialGameInstance.h"

UErosSocialGameInstance::UErosSocialGameInstance()
    : Username(TEXT(""))
    , UserID(TEXT(""))
    , bIsLoggedIn(false)
    , SelectedCharacterSlot(-1)
{
    PlayerCharacters.SetNum(2);
}

void UErosSocialGameInstance::Init()
{
    Super::Init();
    UE_LOG(LogTemp, Warning, TEXT("ErosSocial Game Instance Initialized"));
}

void UErosSocialGameInstance::SetUserLoggedIn(const FString& InUsername, const FString& InUserID)
{
    Username = InUsername;
    UserID = InUserID;
    bIsLoggedIn = true;
    UE_LOG(LogTemp, Warning, TEXT("User logged in: %s"), *InUsername);
}

void UErosSocialGameInstance::Logout()
{
    Username = TEXT("");
    UserID = TEXT("");
    bIsLoggedIn = false;
    SelectedCharacterSlot = -1;
    CurrentCharacter = FCharacterData();
    UE_LOG(LogTemp, Warning, TEXT("User logged out"));
}

void UErosSocialGameInstance::AddCharacter(const FCharacterData& NewCharacter)
{
    if (NewCharacter.CharacterSlot >= 0 && NewCharacter.CharacterSlot < 2)
    {
        PlayerCharacters[NewCharacter.CharacterSlot] = NewCharacter;
        PlayerCharacters[NewCharacter.CharacterSlot].bIsCreated = true;
        UE_LOG(LogTemp, Warning, TEXT("Character created: %s in slot %d"),
            *NewCharacter.CharacterName, NewCharacter.CharacterSlot);
    }
}

void UErosSocialGameInstance::SelectCharacter(int32 SlotIndex)
{
    if (SlotIndex >= 0 && SlotIndex < 2 && PlayerCharacters[SlotIndex].bIsCreated)
    {
        SelectedCharacterSlot = SlotIndex;
        CurrentCharacter = PlayerCharacters[SlotIndex];
        UE_LOG(LogTemp, Warning, TEXT("Character selected: %s"), *CurrentCharacter.CharacterName);
    }
}

bool UErosSocialGameInstance::CanCreateNewCharacter()
{
    int32 CreatedCount = 0;
    for (const FCharacterData& CharData : PlayerCharacters)
    {
        if (CharData.bIsCreated)
        {
            CreatedCount++;
        }
    }
    return CreatedCount < 2;
}

FCharacterData UErosSocialGameInstance::GetSelectedCharacter() const
{
    return CurrentCharacter;
}