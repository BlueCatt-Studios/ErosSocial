// ErosSocialPlayerState.cpp

#include "ErosSocialPlayerState.h"
#include "Net/UnrealNetwork.h"
#include "TimerManager.h"

AErosSocialPlayerState::AErosSocialPlayerState()
{
    CharacterName = TEXT("");
    CharacterGender = TEXT("Male");
    UserID = TEXT("");
    PlayerStatus = EPlayerStatus::Online;
    bHasPartner = false;
    PartnerPlayerState = nullptr;
    PartnerName = TEXT("");
    LastActivityTime = 0.0f;
}

void AErosSocialPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    // Replicar todas as variáveis marcadas como Replicated
    DOREPLIFETIME(AErosSocialPlayerState, CharacterName);
    DOREPLIFETIME(AErosSocialPlayerState, CharacterGender);
    DOREPLIFETIME(AErosSocialPlayerState, UserID);
    DOREPLIFETIME(AErosSocialPlayerState, PlayerStatus);
    DOREPLIFETIME(AErosSocialPlayerState, LastActivityTime);
    DOREPLIFETIME(AErosSocialPlayerState, bHasPartner);
    DOREPLIFETIME(AErosSocialPlayerState, PartnerPlayerState);
    DOREPLIFETIME(AErosSocialPlayerState, PartnerName);
    DOREPLIFETIME(AErosSocialPlayerState, FriendsList);
}

void AErosSocialPlayerState::BeginPlay()
{
    Super::BeginPlay();

    // Inicia timer para verificar AFK a cada 30 segundos
    if (GetWorld())
    {
        GetWorld()->GetTimerManager().SetTimer(
            AFKCheckTimer,
            this,
            &AErosSocialPlayerState::CheckAFK,
            30.0f,
            true
        );
    }

    UpdateActivity();
}

void AErosSocialPlayerState::InitializeCharacter(const FString& InCharacterName, const FString& InGender, const FString& InUserID)
{
    CharacterName = InCharacterName;
    CharacterGender = InGender;
    UserID = InUserID;
    PlayerStatus = EPlayerStatus::Online;

    UE_LOG(LogTemp, Warning, TEXT("Character initialized: %s (%s)"), *CharacterName, *CharacterGender);
}

void AErosSocialPlayerState::SetPlayerStatus(EPlayerStatus NewStatus)
{
    PlayerStatus = NewStatus;

    UE_LOG(LogTemp, Log, TEXT("Player %s status changed to: %s"),
        *CharacterName, *GetStatusAsString());
}

FString AErosSocialPlayerState::GetStatusAsString() const
{
    switch (PlayerStatus)
    {
    case EPlayerStatus::Online:     return TEXT("Online");
    case EPlayerStatus::Busy:       return TEXT("Busy");
    case EPlayerStatus::InPartner:  return TEXT("In Partner");
    case EPlayerStatus::AFK:        return TEXT("AFK");
    default:                        return TEXT("Unknown");
    }
}

void AErosSocialPlayerState::UpdateActivity()
{
    LastActivityTime = GetWorld()->GetTimeSeconds();

    // Se estava AFK e voltou a atividade, muda status
    if (PlayerStatus == EPlayerStatus::AFK)
    {
        SetPlayerStatus(EPlayerStatus::Online);
    }
}

void AErosSocialPlayerState::CheckAFK()
{
    if (!GetWorld()) return;

    float CurrentTime = GetWorld()->GetTimeSeconds();
    float TimeSinceLastActivity = CurrentTime - LastActivityTime;

    // Se passou 5 minutos (300 segundos) sem atividade, marca como AFK
    if (TimeSinceLastActivity > 300.0f && PlayerStatus != EPlayerStatus::AFK)
    {
        SetPlayerStatus(EPlayerStatus::AFK);
        UE_LOG(LogTemp, Warning, TEXT("Player %s is now AFK"), *CharacterName);
    }
}

void AErosSocialPlayerState::SetPartner(AErosSocialPlayerState* NewPartner)
{
    if (NewPartner && NewPartner != this)
    {
        PartnerPlayerState = NewPartner;
        PartnerName = NewPartner->CharacterName;
        bHasPartner = true;
        SetPlayerStatus(EPlayerStatus::InPartner);

        UE_LOG(LogTemp, Warning, TEXT("%s is now partnered with %s"),
            *CharacterName, *PartnerName);
    }
}

void AErosSocialPlayerState::RemovePartner()
{
    if (bHasPartner)
    {
        UE_LOG(LogTemp, Warning, TEXT("%s partnership with %s ended"),
            *CharacterName, *PartnerName);

        PartnerPlayerState = nullptr;
        PartnerName = TEXT("");
        bHasPartner = false;
        SetPlayerStatus(EPlayerStatus::Online);
    }
}

void AErosSocialPlayerState::AddFriend(const FString& FriendUserID)
{
    if (!FriendUserID.IsEmpty() && !FriendsList.Contains(FriendUserID))
    {
        FriendsList.Add(FriendUserID);
        UE_LOG(LogTemp, Log, TEXT("%s added friend: %s"), *CharacterName, *FriendUserID);
    }
}

void AErosSocialPlayerState::RemoveFriend(const FString& FriendUserID)
{
    if (FriendsList.Contains(FriendUserID))
    {
        FriendsList.Remove(FriendUserID);
        UE_LOG(LogTemp, Log, TEXT("%s removed friend: %s"), *CharacterName, *FriendUserID);
    }
}

bool AErosSocialPlayerState::IsFriend(const FString& FriendUserID) const
{
    return FriendsList.Contains(FriendUserID);
}