// Fill out your copyright notice in the Description page of Project Settings.

#include "GoKart.h"
#include <Components/InputComponent.h>
#include <Engine/World.h>
#include <DrawDebugHelpers.h>

FString GetEnumText(ENetRole Role)
{
	switch (Role)
	{
	case ENetRole::ROLE_Authority:
		return "Authority";
	case ENetRole::ROLE_AutonomousProxy:
		return "AutonomousProxy";
	case ENetRole::ROLE_None:
		return "None";
	case ENetRole::ROLE_SimulatedProxy:
		return "SimualtedProxy";
	default:
		return "Error";
	}
}

AGoKart::AGoKart()
{
	PrimaryActorTick.bCanEverTick = true;

	SetReplicates(true);
	SetReplicateMovement(false);
	
	MovementComponent = CreateDefaultSubobject<UGoKartMovementComponent>(TEXT("MovementComponent2"));
	MovementReplicator = CreateDefaultSubobject<UGoKartMovementReplicator>(TEXT("MovementReplicator2"));
}

void AGoKart::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority())
	{
		NetUpdateFrequency = 1;
	}
}

void AGoKart::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	DrawDebugString(GetWorld(), FVector(0, 0, 100), GetEnumText(GetLocalRole()), this, FColor::White, DeltaTime);
}

void AGoKart::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &AGoKart::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AGoKart::MoveRight);
}

void AGoKart::MoveForward(float Value)
{
	if (MovementComponent == NULL) { return; }
	
	MovementComponent->SetThrottle(Value);
}

void AGoKart::MoveRight(float Value)
{
	if (MovementComponent == NULL) { return; }

	MovementComponent->SetSteeringThrow(Value);
}