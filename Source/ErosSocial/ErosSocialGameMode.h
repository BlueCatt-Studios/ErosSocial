// Copyright BlueCatt Studios - All Rights Reserved
// ErosSocialGameMode.h
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "ErosSocialGameMode.generated.h"

// Forward declarations
class AErosSocialCharacter;
class UErosSocialGameInstance;

UCLASS()
class EROSSOCIAL_API AErosSocialGameMode : public AGameModeBase
{
    GENERATED_BODY()

public:
    AErosSocialGameMode();

    // Character class to spawn
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character")
    TSubclassOf<AErosSocialCharacter> ErosSocialCharacterClass;

protected:
    // ✅ ASSINATURA CORRETA PARA UE 5.4
    virtual void PostLogin(APlayerController* NewPlayer) override;

    // ✅ REMOVIDO: GetDefaultPawnClassForController_Implementation
    // Vamos usar DefaultPawnClass diretamente

private:
    void InitializeCharacterWithSavedData(APlayerController* PlayerController);
    void SyncPlayerState(APlayerController* PlayerController);
};