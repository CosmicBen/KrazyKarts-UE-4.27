// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GoKartMovementComponent.generated.h"

USTRUCT()
struct FGoKartMove
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY()
	float Throttle;

	UPROPERTY()
	float SteeringThrow;

	UPROPERTY()
	float DeltaTime;

	UPROPERTY()
	float Time;

	bool IsValid() const
	{
		return FMath::Abs(Throttle) <= 1.0f && FMath::Abs(SteeringThrow) <= 1.0f && DeltaTime >= 0.0f;
	}
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class KRAZYKARTS_API UGoKartMovementComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UGoKartMovementComponent();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void SimulateMove(const FGoKartMove& Move);

	FGoKartMove GetLastMove() { return LastMove; }
	void SetSteeringThrow(float Value) { SteeringThrow = Value; }
	void SetThrottle(float Value) { Throttle = Value; }
	FVector GetVelocity() { return Velocity; }
	void SetVelocity(FVector Value) { Velocity = Value; }

protected:
	virtual void BeginPlay() override;
		
private:
	// The mass of the car (kg).
	UPROPERTY(EditAnywhere)
	float Mass = 1000;

	// The force applied to the car when the throttle is fully down (N).
	UPROPERTY(EditAnywhere)
	float MaxDrivingForce = 10000;

	// The minimum radius of the car turning circle at full lock (m)
	UPROPERTY(EditAnywhere)
	float MinTurningRadius = 10;

	// Higher means more drag (kg/m) (DragCoefficieent = MaxDrivingForce / (Top speed m/s)^2)
	UPROPERTY(EditAnywhere)
	float DragCoefficient = 16;

	// Higher means more rolling resistance (kg/m) (Default value from wikipedia)
	UPROPERTY(EditAnywhere)
	float RollingResistanceCoefficent = 0.015;

	FGoKartMove LastMove;
	FVector Velocity;
	float Throttle;
	float SteeringThrow;

	void ApplyRotation(float DeltaTime, float SteeringThrow);
	FGoKartMove CreateMove(float DeltaTime);
	FVector GetAirResistance();
	FVector GetRollingResistance();
	void UpdateLocationFromVelocity(float DeltaTime);
};