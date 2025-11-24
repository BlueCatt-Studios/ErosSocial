// Copyright BlueCatt Studios LTDA. All Rights Reserved.

#include "ErosSocialCharacter.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "Systems/ClothingSystem.h"
#include "ErosSocialPlayerState.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

//////////////////////////////////////////////////////////////////////////
// AErosSocialCharacter

AErosSocialCharacter::AErosSocialCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);

	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	// Create a camera boom
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f;
	CameraBoom->bUsePawnControlRotation = true;

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	// Criar ClothingSystem
	ClothingSystem = CreateDefaultSubobject<UClothingSystem>(TEXT("ClothingSystem"));
}

void AErosSocialCharacter::BeginPlay()
{
	Super::BeginPlay();

	// Inicializar ClothingSystem
	if (ClothingSystem)
	{
		ClothingSystem->Initialize(this);
	}

	// Obter PlayerState
	PlayerStateRef = Cast<AErosSocialPlayerState>(GetPlayerState());

	UE_LOG(LogTemplateCharacter, Warning, TEXT("AErosSocialCharacter::BeginPlay - Character initialized"));
}

void AErosSocialCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	// Sincronizar com PlayerState quando possed
	SyncWithPlayerState();
}

//////////////////////////////////////////////////////////////////////////
// Customization

void AErosSocialCharacter::InitializeCharacter(const FCharacterSaveData& CharacterData)
{
	CurrentCharacterData = CharacterData;

	// Aplicar customizações de corpo
	ApplyBodyCustomization(CharacterData.BodyCustomization);

	// Aplicar customizações de aparência
	ApplyAppearanceCustomization(CharacterData.AppearanceCustomization);

	// Aplicar outfit atual
	if (ClothingSystem && !CharacterData.CurrentOutfit.IsEmpty())
	{
		FOutfitData OutfitData;
		OutfitData.ClothingItems = CharacterData.CurrentOutfit;
		ClothingSystem->ApplyOutfitData(OutfitData);
	}

	UE_LOG(LogTemplateCharacter, Warning, TEXT("AErosSocialCharacter::InitializeCharacter - Initialized with '%s'"),
		*CharacterData.CharacterName);
}

void AErosSocialCharacter::ApplyBodyCustomization(const FBodyCustomization& BodyCustomization)
{
	// Armazenar os valores dos morphs
	BreastSizeMorph = BodyCustomization.BreastSize;
	ButtSizeMorph = BodyCustomization.ButtSize;
	HeightMorph = BodyCustomization.Height;
	WeightMorph = BodyCustomization.Weight;
	MuscleMorph = BodyCustomization.Muscle;

	// Atualizar morphs no skeletal mesh
	UpdateMorphTargets();

	UE_LOG(LogTemplateCharacter, Warning, TEXT("AErosSocialCharacter::ApplyBodyCustomization - Applied body morphs"));
}

void AErosSocialCharacter::ApplyAppearanceCustomization(const FAppearanceCustomization& AppearanceCustomization)
{
	// Aplicar cor de pele
	ApplySkinColor(AppearanceCustomization.SkinColor);

	// Aplicar cor de cabelo
	ApplyHairColor(AppearanceCustomization.HairColor);

	// Outros parâmetros de aparência podem ser adicionados aqui

	UE_LOG(LogTemplateCharacter, Warning, TEXT("AErosSocialCharacter::ApplyAppearanceCustomization - Applied appearance"));
}

void AErosSocialCharacter::SyncWithPlayerState()
{
	if (!PlayerStateRef)
	{
		PlayerStateRef = Cast<AErosSocialPlayerState>(GetPlayerState());
	}

	if (PlayerStateRef)
	{
		// Sincronizar nome do personagem
		PlayerStateRef->SetPlayerName(*CurrentCharacterData.CharacterName);

		// Sincronizar gênero
		PlayerStateRef->CharacterGender = CurrentCharacterData.CharacterGender;

		UE_LOG(LogTemplateCharacter, Warning, TEXT("AErosSocialCharacter::SyncWithPlayerState - Synced with PlayerState"));
	}
}

void AErosSocialCharacter::UpdateMorphTargets()
{
	// Obter o skeletal mesh
	USkeletalMeshComponent* MeshComponent = GetMesh();
	if (!MeshComponent)
	{
		return;
	}

	// Aplicar morphs (estes nomes precisam existir no skeleton)
	MeshComponent->SetMorphTarget(FName("BreastSize"), BreastSizeMorph);
	MeshComponent->SetMorphTarget(FName("ButtSize"), ButtSizeMorph);
	MeshComponent->SetMorphTarget(FName("Height"), HeightMorph);
	MeshComponent->SetMorphTarget(FName("Weight"), WeightMorph);
	MeshComponent->SetMorphTarget(FName("Muscle"), MuscleMorph);

	UE_LOG(LogTemplateCharacter, Log, TEXT("AErosSocialCharacter::UpdateMorphTargets - Updated morph targets"));
}

void AErosSocialCharacter::ApplySkinColor(const FLinearColor& SkinColor)
{
	USkeletalMeshComponent* MeshComponent = GetMesh();
	if (!MeshComponent)
	{
		return;
	}

	// Criar material dinâmico para pele
	// Isto será implementado quando você tiver o material de pele configurado

	UE_LOG(LogTemplateCharacter, Log, TEXT("AErosSocialCharacter::ApplySkinColor - Applied skin color"));
}

void AErosSocialCharacter::ApplyHairColor(const FLinearColor& HairColor)
{
	USkeletalMeshComponent* MeshComponent = GetMesh();
	if (!MeshComponent)
	{
		return;
	}

	// Criar material dinâmico para cabelo
	// Isto será implementado quando você tiver o material de cabelo configurado

	UE_LOG(LogTemplateCharacter, Log, TEXT("AErosSocialCharacter::ApplyHairColor - Applied hair color"));
}

void AErosSocialCharacter::OnCharacterDataReceived()
{
	// Placeholder para quando integrar com rede
	UE_LOG(LogTemplateCharacter, Log, TEXT("AErosSocialCharacter::OnCharacterDataReceived - Data received"));
}

//////////////////////////////////////////////////////////////////////////
// Input

void AErosSocialCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}

	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AErosSocialCharacter::Move);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AErosSocialCharacter::Look);
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input component!"), *GetNameSafe(this));
	}
}

void AErosSocialCharacter::Move(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void AErosSocialCharacter::Look(const FInputActionValue& Value)
{
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}