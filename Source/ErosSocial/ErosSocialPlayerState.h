// Copyright BlueCatt Studios - All Rights Reserved
// ErosSocialPlayerState.h
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "ErosSocialPlayerState.generated.h"

UENUM(BlueprintType)
enum class EPlayerStatus : uint8
{
    Online      UMETA(DisplayName = "Online"),
    Busy        UMETA(DisplayName = "Busy"),
    InPartner   UMETA(DisplayName = "In Partner"),
    AFK         UMETA(DisplayName = "AFK")
};

UCLASS()
class EROSSOCIAL_API AErosSocialPlayerState : public APlayerState
{
    GENERATED_BODY()

public:
    AErosSocialPlayerState();

    // ========== DADOS DO PERSONAGEM ==========

    UPROPERTY(Replicated, BlueprintReadWrite, Category = "Character")
    FString CharacterName;

    UPROPERTY(Replicated, BlueprintReadWrite, Category = "Character")
    FString CharacterGender;

    UPROPERTY(Replicated, BlueprintReadWrite, Category = "Character")
    FString UserID;

    // ✅ ADICIONADO: CharacterSlot
    UPROPERTY(Replicated, BlueprintReadWrite, Category = "Character")
    int32 CharacterSlot;

    // ========== STATUS DO JOGADOR ==========

    UPROPERTY(Replicated, BlueprintReadWrite, Category = "Status")
    EPlayerStatus PlayerStatus;

    UPROPERTY(Replicated, BlueprintReadWrite, Category = "Status")
    float LastActivityTime;

    // ========== SISTEMA DE PARTNER ==========

    UPROPERTY(Replicated, BlueprintReadWrite, Category = "Partner")
    bool bHasPartner;

    UPROPERTY(Replicated, BlueprintReadWrite, Category = "Partner")
    AErosSocialPlayerState* PartnerPlayerState;

    UPROPERTY(Replicated, BlueprintReadWrite, Category = "Partner")
    FString PartnerName;

    // ========== SISTEMA SOCIAL ==========

    UPROPERTY(Replicated, BlueprintReadWrite, Category = "Social")
    TArray<FString> FriendsList;

    // ========== FUNÇÕES PÚBLICAS ==========

    UFUNCTION(BlueprintCallable, Category = "Character")
    void InitializeCharacter(const FString& InCharacterName, const FString& InGender, const FString& InUserID);

    UFUNCTION(BlueprintCallable, Category = "Status")
    void SetPlayerStatus(EPlayerStatus NewStatus);

    UFUNCTION(BlueprintPure, Category = "Status")
    EPlayerStatus GetPlayerStatus() const { return PlayerStatus; }

    UFUNCTION(BlueprintPure, Category = "Status")
    FString GetStatusAsString() const;

    UFUNCTION(BlueprintCallable, Category = "Status")
    void UpdateActivity();

    UFUNCTION(BlueprintCallable, Category = "Status")
    void CheckAFK();

    UFUNCTION(BlueprintCallable, Category = "Partner")
    void SetPartner(AErosSocialPlayerState* NewPartner);

    UFUNCTION(BlueprintCallable, Category = "Partner")
    void RemovePartner();

    UFUNCTION(BlueprintPure, Category = "Partner")
    bool HasPartner() const { return bHasPartner; }

    UFUNCTION(BlueprintPure, Category = "Partner")
    AErosSocialPlayerState* GetPartner() const { return PartnerPlayerState; }

    UFUNCTION(BlueprintCallable, Category = "Social")
    void AddFriend(const FString& FriendUserID);

    UFUNCTION(BlueprintCallable, Category = "Social")
    void RemoveFriend(const FString& FriendUserID);

    UFUNCTION(BlueprintPure, Category = "Social")
    bool IsFriend(const FString& FriendUserID) const;

protected:
    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
    virtual void BeginPlay() override;

private:
    FTimerHandle AFKCheckTimer;
};