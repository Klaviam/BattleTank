// Fill out your copyright notice in the Description page of Project Settings.

#include "TankAIController.h"
#include "TankAimingComponent.h"
#include "BattleTank.h"
#include "Tank.h" // Implements OnDeath()

void ATankAIController::BeginPlay()
{
	Super::BeginPlay();

	// On player tank's death
	if (!ensure(GetWorld()->GetFirstPlayerController()->GetPawn())) { return; }

	auto playerTank = Cast<ATank>(GetWorld()->GetFirstPlayerController()->GetPawn());

	if (!ensure(playerTank)) { return; }

	playerTank->OnTankDeath.AddUniqueDynamic(this, &ATankAIController::OnPlayerTankDeath);
}

// Called every frame
void ATankAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	auto playerTank = GetWorld()->GetFirstPlayerController()->GetPawn();
	auto controlledTank = GetPawn();

	if (!(playerTank && controlledTank)) { return; }

	// Move towards the player
	MoveToActor(playerTank, acceptanceRadius);
		
	// Aim towards the player
	auto aimingComponent = controlledTank->FindComponentByClass<UTankAimingComponent>();
	aimingComponent->AimAt(playerTank->GetActorLocation());

	if (aimingComponent->GerFiringState() == EFiringStatus::Locked)
	{
		aimingComponent->Fire();
	}
}


void ATankAIController::SetPawn(APawn* InPawn)
{
	Super::SetPawn(InPawn);

	// On own tank's death
	auto owningTank = Cast<ATank>(InPawn);
	if (!owningTank) { return; }
	owningTank->OnTankDeath.AddUniqueDynamic(this, &ATankAIController::OnTankDeath);
}


void ATankAIController::OnTankDeath()
{
	if (!GetPawn()) { return; }

	GetPawn()->DetachFromControllerPendingDestroy();
}

void ATankAIController::OnPlayerTankDeath()
{

}
