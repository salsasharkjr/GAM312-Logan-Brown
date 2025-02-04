// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Camera/CameraComponent.h"
#include "Resource_M.h"
#include "Kismet/GameplayStatics.h"
#include "PlayerCharacter.generated.h"

UCLASS()
class GAM312_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// STATING UFUNCTIONS TO BE CALLED
	UFUNCTION()
	void MoveForward(float axisValue);

	UFUNCTION()
	void MoveRight(float axisValue);

	UFUNCTION()
	void StartJump();

	UFUNCTION()
	void StopJump();

	UFUNCTION()
	void FindObject();

	UPROPERTY(VisibleAnywhere)
	UCameraComponent* PlayerCamComp;



	// SETS DEFAULT HEALTH AT 100
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Stats")
	float Health = 100.0f;

	// SET DEFAULT HUNGER AT 100
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Stats")
	float Hunger = 100.0f;

	// SET DEFAULT STAMINA AT 100
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Stats")
	float Stamina = 100.0f;


	// CREATES RESOURCE INTEGERS
	UPROPERTY(EditAnywhere, Category = "Resources")
	int Wood;

	UPROPERTY(EditAnywhere, Category = "Resources")
	int Stone;

	UPROPERTY(EditAnywhere, Category = "Resources")
	int Berry;

	// CREATES ARRAY
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resources")
	TArray<int> ResourcesArray;

	// ADDS NAMES TO ARRAY
	UPROPERTY(EditAnywhere, Category = "Resources")
	TArray<FString> ResourcesNameArray;

	UPROPERTY(EditAnywhere, Category = "HitMarker")
	UMaterialInterface* hitDecal;



	// DECREASES HEALTH WITH TIMER
	UFUNCTION(BlueprintCallable)
	void SetHealth(float amount);

	// DECREASES HUNGER WITH TIMER
	UFUNCTION(BlueprintCallable)
	void SetHunger(float amount);

	// DECREASES STAMINA WITH TIMER
	UFUNCTION(BlueprintCallable)
	void SetStamina(float amount);

	// TIMER TO DECREASE STAT AMOUNTS
	UFUNCTION()
	void DecreaseStats();

	UFUNCTION()
	void GiveResources(float amount, FString resourceType);


};
