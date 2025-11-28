// Copyright BlueCatt Studios - All Rights Reserved
// ErosSocialGameMode.cpp

#include "ErosSocialGameMode.h"
#include "ErosSocialCharacter.h"
#include "ErosSocialPlayerState.h"
#include "ErosSocialGameInstance.h"
#include "GameFramework/PlayerController.h"

AErosSocialGameMode::AErosSocialGameMode()
{
    // Configurar classe de personagem padrão
    static ConstructorHelpers::FClassFinder<AErosSocialCharacter> CharacterClassFinder(
        TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter")
    );

    if (CharacterClassFinder.Succeeded())
    {
        DefaultPawnClass = CharacterClassFinder.Class;
        ErosSocialCharacterClass = CharacterClassFinder.Class;
    }
    else
    {
        // Fallback para a classe C++
        DefaultPawnClass = AErosSocialCharacter::StaticClass();
        ErosSocialCharacterClass = AErosSocialCharacter::StaticClass();
    }

    // Configurar PlayerState
    PlayerStateClass = AErosSocialPlayerState::StaticClass();

    UE_LOG(LogTemp, Warning, TEXT("ErosSocialGameMode initialized"));
}

void AErosSocialGameMode::PostLogin(APlayerController* NewPlayer)
{
    Super::PostLogin(NewPlayer);

    if (!NewPlayer)
    {
        UE_LOG(LogTemp, Error, TEXT("PostLogin: NewPlayer is null!"));
        return;
    }

    UE_LOG(LogTemp, Warning, TEXT("PostLogin called for player"));

    // Inicializar character com dados salvos
    InitializeCharacterWithSavedData(NewPlayer);

    // Sincronizar PlayerState
    SyncPlayerState(NewPlayer);
}

void AErosSocialGameMode::InitializeCharacterWithSavedData(APlayerController* PlayerController)
{
    if (!PlayerController)
    {
        UE_LOG(LogTemp, Error, TEXT("InitializeCharacterWithSavedData: PlayerController is null"));
        return;
    }

    // Obter GameInstance
    UErosSocialGameInstance* GameInstance = Cast<UErosSocialGameInstance>(GetGameInstance());
    if (!GameInstance)
    {
        UE_LOG(LogTemp, Error, TEXT("InitializeCharacterWithSavedData: GameInstance is null"));
        return;
    }

    // Verificar se está logado
    if (!GameInstance->IsUserLoggedIn())
    {
        UE_LOG(LogTemp, Warning, TEXT("InitializeCharacterWithSavedData: User not logged in"));
        return;
    }

    // Obter personagem selecionado
    FCharacterSaveData SelectedCharacter = GameInstance->GetSelectedCharacter();

    if (SelectedCharacter.CharacterName.IsEmpty())
    {
        UE_LOG(LogTemp, Warning, TEXT("InitializeCharacterWithSavedData: No character selected"));
        return;
    }

    // Obter o character spawned
    AErosSocialCharacter* Character = Cast<AErosSocialCharacter>(PlayerController->GetPawn());
    if (!Character)
    {
        UE_LOG(LogTemp, Error, TEXT("InitializeCharacterWithSavedData: Character is null"));
        return;
    }

    // Inicializar character com dados salvos
    Character->InitializeCharacter(SelectedCharacter);

    UE_LOG(LogTemp, Warning, TEXT("Character initialized: %s"), *SelectedCharacter.CharacterName);
}

void AErosSocialGameMode::SyncPlayerState(APlayerController* PlayerController)
{
    if (!PlayerController)
    {
        UE_LOG(LogTemp, Error, TEXT("SyncPlayerState: PlayerController is null"));
        return;
    }

    AErosSocialPlayerState* PlayerState = Cast<AErosSocialPlayerState>(PlayerController->PlayerState);
    if (!PlayerState)
    {
        UE_LOG(LogTemp, Error, TEXT("SyncPlayerState: PlayerState is null"));
        return;
    }

    // Obter GameInstance
    UErosSocialGameInstance* GameInstance = Cast<UErosSocialGameInstance>(GetGameInstance());
    if (!GameInstance || !GameInstance->IsUserLoggedIn())
    {
        return;
    }

    // Sincronizar dados do personagem
    FCharacterSaveData SelectedCharacter = GameInstance->GetSelectedCharacter();

    PlayerState->CharacterName = SelectedCharacter.CharacterName;
    PlayerState->CharacterGender = SelectedCharacter.CharacterGender;
    PlayerState->UserID = GameInstance->GetUserID();  // ✅ CORRIGIDO
    PlayerState->CharacterSlot = GameInstance->GetSelectedCharacterSlot();  // ✅ CORRIGIDO

    UE_LOG(LogTemp, Warning, TEXT("PlayerState synchronized for: %s"), *SelectedCharacter.CharacterName);
}