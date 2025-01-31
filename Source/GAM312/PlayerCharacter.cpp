// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"

// Sets default values
APlayerCharacter::APlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	// INITIAL SETUP OF CAMERA COMPONENT
	PlayerCamComp = CreateDefaultSubobject<UCameraComponent>(TEXT("First Person Cam"));

	// ATTACHING CAMERA TO THE CHARACTER MESH AND HEAD BONE
	PlayerCamComp->SetupAttachment(GetMesh(), "head");

	// SHARE ROTATION WITH CONTROLLER
	PlayerCamComp->bUsePawnControlRotation = true;



}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &APlayerCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &APlayerCharacter::MoveRight);
	PlayerInputComponent->BindAxis("LookUp", this, &APlayerCharacter::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("Turn", this, &APlayerCharacter::AddControllerYawInput);
	PlayerInputComponent->BindAction("JumpEvent", IE_Pressed, this, &APlayerCharacter::StartJump);
	PlayerInputComponent->BindAction("JumpEvent", IE_Released, this, &APlayerCharacter::StopJump);
	PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &APlayerCharacter::FindObject);

}

void APlayerCharacter::MoveForward(float axisValue)
{
	// ALLOWS PLAYER CHARACTER TO MOVE FORWARD AND BACKWARD
	FVector Direction = FRotationMatrix(Controller->GetControlRotation()).GetScaledAxis(EAxis::X);
	AddMovementInput(Direction, axisValue);
}

void APlayerCharacter::MoveRight(float axisValue)
{
	// ALLOWS PLAYER CHARACCTER TO MOVE RIGHT AND LEFT
	FVector Direction = FRotationMatrix(Controller->GetControlRotation()).GetScaledAxis(EAxis::Y);
	AddMovementInput(Direction, axisValue);
}

void APlayerCharacter::StartJump()
{
	// DETECTS IF JUMP BUTTON IS BEING PRESSED AND STARTS JUMP
	bPressedJump = true;
}

void APlayerCharacter::StopJump()
{
	// DETECTS IF JUMP BUTTON IS BEING DEPRESSED, AND WILL RESET VARIABLE TO FALSE
	bPressedJump = false;
}

void APlayerCharacter::FindObject()
{
	// NOT YET IMPLEMENTED
}

