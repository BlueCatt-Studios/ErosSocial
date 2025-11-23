// Copyright BlueCatt Studios - All rights Reserved
// ErosSocialGameInstance.h
// Gerencia dados persistentes do jogo (login, personagem selecionado, etc)

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "ErosSocialGameInstance.generated.h"

USTRUCT(BlueprintType)
struct FCharacterData
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite, Category = "Character")
    FString CharacterName;

    UPROPERTY(BlueprintReadWrite, Category = "Character")
    FString CharacterGender;

    UPROPERTY(BlueprintReadWrite, Category = "Character")
    int32 CharacterSlot;

    UPROPERTY(BlueprintReadWrite, Category = "Character")
    bool bIsCreated;

    FCharacterData()
        : CharacterName(TEXT(""))
        , CharacterGender(TEXT("Male"))
        , CharacterSlot(0)
        , bIsCreated(false)
    {
    }
};

UCLASS()
class EROSSOCIAL_API UErosSocialGameInstance : public UGameInstance
{
    GENERATED_BODY()

public:
    UErosSocialGameInstance();

    UPROPERTY(BlueprintReadWrite, Category = "User")
    FString Username;

    UPROPERTY(BlueprintReadWrite, Category = "User")
    FString UserID;

    UPROPERTY(BlueprintReadWrite, Category = "User")
    bool bIsLoggedIn;

    UPROPERTY(BlueprintReadWrite, Category = "Character")
    TArray<FCharacterData> PlayerCharacters;

    UPROPERTY(BlueprintReadWrite, Category = "Character")
    int32 SelectedCharacterSlot;

    UPROPERTY(BlueprintReadWrite, Category = "Character")
    FCharacterData CurrentCharacter;

    UFUNCTION(BlueprintCallable, Category = "Authentication")
    void SetUserLoggedIn(const FString& InUsername, const FString& InUserID);

    UFUNCTION(BlueprintCallable, Category = "Authentication")
    void Logout();

    UFUNCTION(BlueprintCallable, Category = "Character")
    void AddCharacter(const FCharacterData& NewCharacter);

    UFUNCTION(BlueprintCallable, Category = "Character")
    void SelectCharacter(int32 SlotIndex);

    UFUNCTION(BlueprintCallable, Category = "Character")
    bool CanCreateNewCharacter();

    UFUNCTION(BlueprintPure, Category = "Character")
    FCharacterData GetSelectedCharacter() const;

protected:
    virtual void Init() override;
};