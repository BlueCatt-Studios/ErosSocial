// Copyright BlueCatt Studios LTDA. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "CharacterSaveData.h"
#include "ErosSocialCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
class UClothingSystem;
class AErosSocialPlayerState;
struct FInputActionValue;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

UCLASS(config = Game)
class AErosSocialCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;

	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* LookAction;

public:
	AErosSocialCharacter();

	// ========== CUSTOMIZAÇÃO DO PERSONAGEM ==========

	/**
	 * Inicializa o personagem com dados salvos
	 */
	UFUNCTION(BlueprintCallable, Category = "Character|Customization")
	void InitializeCharacter(const FCharacterSaveData& CharacterData);

	/**
	 * Aplica customizações de corpo (morphs)
	 */
	UFUNCTION(BlueprintCallable, Category = "Character|Customization")
	void ApplyBodyCustomization(const FBodyCustomization& BodyCustomization);

	/**
	 * Aplica customizações de aparência (cabelo, pele, etc)
	 */
	UFUNCTION(BlueprintCallable, Category = "Character|Customization")
	void ApplyAppearanceCustomization(const FAppearanceCustomization& AppearanceCustomization);

	/**
	 * Obtém o sistema de roupas
	 */
	UFUNCTION(BlueprintCallable, Category = "Character|Clothing")
	class UClothingSystem* GetClothingSystem() const { return ClothingSystem; }

	/**
	 * Obtém os dados do personagem atual
	 */
	UFUNCTION(BlueprintCallable, Category = "Character|Data")
	FCharacterSaveData GetCharacterData() const { return CurrentCharacterData; }

	/**
	 * Sincroniza dados com PlayerState
	 */
	UFUNCTION(BlueprintCallable, Category = "Character|Sync")
	void SyncWithPlayerState();

protected:

	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);

	// ========== COMPONENTES ==========

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Character|Clothing")
	class UClothingSystem* ClothingSystem;

	// ========== DADOS DO PERSONAGEM ==========

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Character|Data")
	FCharacterSaveData CurrentCharacterData;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Character|Data")
	AErosSocialPlayerState* PlayerStateRef;

	// ========== MORPHS DO CORPO (Variáveis para controlar morphs) ==========

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Character|Morphs")
	float BreastSizeMorph = 0.5f;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Character|Morphs")
	float ButtSizeMorph = 0.5f;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Character|Morphs")
	float HeightMorph = 0.5f;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Character|Morphs")
	float WeightMorph = 0.5f;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Character|Morphs")
	float MuscleMorph = 0.5f;

	// ========== FUNÇÕES PRIVADAS ==========

	UFUNCTION()
	void OnCharacterDataReceived();

	void UpdateMorphTargets();

	void ApplySkinColor(const FLinearColor& SkinColor);

	void ApplyHairColor(const FLinearColor& HairColor);

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// To add mapping context
	virtual void BeginPlay() override;

	virtual void PossessedBy(AController* NewController) override;

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
};