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


	// SET ARRAY SIZE AND NAMES
	BuildingArray.SetNum(3);
	ResourcesArray.SetNum(3);
	ResourcesNameArray.Add(TEXT("Wood"));
	ResourcesNameArray.Add(TEXT("Stone"));
	ResourcesNameArray.Add(TEXT("Berry"));



}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	FTimerHandle StatsTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(StatsTimerHandle, this, &APlayerCharacter::DecreaseStats, 2.0f, true);
	
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (isBuilding)
	{
		if (spawnedPart)
		{
			FVector StartLocation = PlayerCamComp->GetComponentLocation();
			FVector Direction = PlayerCamComp->GetForwardVector() * 400.0f;
			FVector EndLocation = StartLocation + Direction;
			spawnedPart->SetActorLocation(EndLocation);
		}
	}

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
	PlayerInputComponent->BindAction("RotPart", IE_Pressed, this, &APlayerCharacter::RotateBuilding);

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
	// DETERMINES IF TARGETED OBJECT IS VIABLE, THEN CREATES TRACE LINE AND LEAVES DECAL WHERE HIT IS DETECTED
	FHitResult HitResult;
	FVector StartLocation = PlayerCamComp->GetComponentLocation();
	FVector Direction = PlayerCamComp->GetForwardVector() * 800.0f;
	FVector EndLocation = StartLocation + Direction;

	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);
	QueryParams.bTraceComplex = true;
	QueryParams.bReturnFaceIndex = true;

	if (!isBuilding)
	{
		if (GetWorld()->LineTraceSingleByChannel(HitResult, StartLocation, EndLocation, ECC_Visibility, QueryParams))
		{
			AResource_M* HitResource = Cast<AResource_M>(HitResult.GetActor());

			if (Stamina > 5.0f)
			{

				if (HitResource)
				{
					FString hitName = HitResource->resourceName;
					int resourceValue = HitResource->resourceAmount;

					HitResource->totalResource = HitResource->totalResource - resourceValue;

					if (HitResource->totalResource > resourceValue)
					{
						GiveResources(resourceValue, hitName);

						check(GEngine != nullptr);
						GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("Resource Collected"));

						UGameplayStatics::SpawnDecalAtLocation(GetWorld(), hitDecal, FVector(10.0f, 10.0f, 10.0f), HitResult.Location, FRotator(-90, 0, 0), 2.0f);

						SetStamina(-5.0f);
					}
					else
					{
						HitResource->Destroy();
						check(GEngine != nullptr);
						GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("Resource Depleted"));
					}
				}
			}
		}

	}
	else
	{
		isBuilding = false;
	}

}

void APlayerCharacter::SetHealth(float amount)
{
	// RAISES OR LOWERS HEALTH AS LONG AS THE NEW FLOAT IS LOWER THAN 100
	if (Health + amount < 100)
	{ 
		Health = Health + amount;
	}
}

void APlayerCharacter::SetHunger(float amount)
{
	// RAISES OR LOWERS HUNGER AS LONG AS THE NEW FLOAT IS LOWER THAN 100
	if (Hunger + amount < 100)
	{
		Hunger = Hunger + amount;
	}
}

void APlayerCharacter::SetStamina(float amount)
{
	// RAISES OR LOWERS STAMINA AS LONG AS THE NEW FLOAT IS LOWER THAN 100
	if (Stamina + amount < 100)
	{
		Stamina = Stamina + amount;
	}
}

// ALTERS STAMINA, HUNGER, AND HEALTH WITH EACH PERIOD OF TIME
void APlayerCharacter::DecreaseStats()
{
	// ADDS STAMINA WITH TIME
	SetStamina(10.0f);
	
	// SUBTRACTS HUNGER WITH TIME IF HUNGER IS NOT ALREADY ZERO
	if (Hunger > 0)
	{
		SetHunger(-1.0f);
	}

	// SUBTRACTS HEALTH WITH TIME IF HUNGER IS ZERO
	if (Hunger <= 0)
	{
		SetHealth(-3.0f);
	}
}

void APlayerCharacter::GiveResources(float amount, FString resourceType)
{
	// DETERMINES RESOURCE TYPE THEN ADDS THAT AMOUNT TO THE APPROPRIATE ARRAY SLOT
	if (resourceType == "Wood")
	{
		ResourcesArray[0] = ResourcesArray[0] + amount;
	}

	if (resourceType == "Stone")
	{
		ResourcesArray[1] = ResourcesArray[1] + amount;
	}

	if (resourceType == "Berry")
	{
		ResourcesArray[2] = ResourcesArray[2] + amount;
	}
}

void APlayerCharacter::UpdateResources(float woodAmount, float stoneAmount, FString buildingObject)
{
	if (woodAmount <= ResourcesArray[0])
	{
		if (stoneAmount <= ResourcesArray[1])
		{
			ResourcesArray[0] = ResourcesArray[0] - woodAmount;
			ResourcesArray[1] = ResourcesArray[1] - stoneAmount;

			if (buildingObject == "Wall")
			{
				BuildingArray[0] = BuildingArray[0] + 1;
			}
			if (buildingObject == "Floor")
			{
				BuildingArray[1] = BuildingArray[1] + 1;
			}
			if (buildingObject == "Ceiling")
			{
				BuildingArray[2] = BuildingArray[2] + 1;
			}
		}
	}
}

void APlayerCharacter::SpawnBuilding(int buildingID, bool& isSuccess)
{
	if (!isBuilding)
	{
		if (BuildingArray[buildingID] >= 1)
		{
			isBuilding = true;
			FActorSpawnParameters SpawnParams;
			FVector StartLocation = PlayerCamComp->GetComponentLocation();
			FVector Direction = PlayerCamComp->GetForwardVector() * 400.0f;
			FVector EndLocation = StartLocation + Direction;
			FRotator myRot(0, 0, 0);

			BuildingArray[buildingID] = BuildingArray[buildingID] - 1;

			spawnedPart = GetWorld()->SpawnActor<ABuildingPart>(BuildPartClass, EndLocation, myRot, SpawnParams);

			isSuccess = true;
		}

		isSuccess = false;
	}
}

void APlayerCharacter::RotateBuilding()
{
	if (isBuilding)
	{
		spawnedPart->AddActorWorldRotation(FRotator(0, 90, 0));
	}
}

