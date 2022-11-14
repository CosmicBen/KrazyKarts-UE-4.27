// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include <Components/ActorComponent.h>
#include "GoKartMovementComponent.h"
#include "GoKartMovementReplicator.generated.h"

USTRUCT()
struct FGoKartState
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY()
	FGoKartMove LastMove;

	UPROPERTY()
	FTransform Transform;

	UPROPERTY()
	FVector Velocity;
};

struct FHermiteCubicSpline
{
	FVector StartLocation;
	FVector StartDerivative;
	FVector TargetLocation;
	FVector TargetDerivative;

	FVector InterpolateLocation(float LerpRatio) const
	{
		return FMath::CubicInterp(StartLocation, StartDerivative, TargetLocation, TargetDerivative, LerpRatio);
	}

	FVector InterpolateDerivative(float LerpRatio) const
	{
		return FMath::CubicInterpDerivative(StartLocation, StartDerivative, TargetLocation, TargetDerivative, LerpRatio);
	}
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class KRAZYKARTS_API UGoKartMovementReplicator : public UActorComponent
{
	GENERATED_BODY()

public:	
	UGoKartMovementReplicator();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY()
	UGoKartMovementComponent* MovementComponent;

	UPROPERTY()
	USceneComponent* MeshOffsetRoot;

	UPROPERTY(ReplicatedUsing = OnRep_ServerState)
	FGoKartState ServerState;

	FTransform ClientStartTransform;
	FVector ClientStartVelocity;
	float ClientTimeSinceUpdate;
	float ClientTimeBetweenLastUpdates;
	
	float ClientSimulatedTime;
	
	TArray<FGoKartMove> UnacknowledgeMoves;


	void ClearAcknowledgeMoves(FGoKartMove LastMove);
	
	void ClientTick(float DeltaTime);
	FHermiteCubicSpline CreateSpline();
	void InterpolateLocation(const FHermiteCubicSpline& Spline, float LerpRatio);
	void InterpolateRotation(float LerpRatio);
	void InterpolateVelocity(const FHermiteCubicSpline& Spline, float LerpRatio);
	float VelocityToDerivative();

	UFUNCTION()
	void OnRep_ServerState();
	void AutonomousProxy_OnRep_ServerState();
	void SimulatedProxy_OnRep_ServerState();

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_SendMove(FGoKartMove Move);

	UFUNCTION(BlueprintCallable)
	void SetMeshOffsetRoot(USceneComponent* Root) { MeshOffsetRoot = Root; }

	void UpdateServerState(const FGoKartMove& Move);
};