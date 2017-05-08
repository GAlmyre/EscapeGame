// Fill out your copyright notice in the Description page of Project Settings.

#include "EscapeProject.h"
#include "OpenDoor.h"

#define OUT 

// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();
	Owner = GetOwner();

	if(!PressurePlate)
	{
		UE_LOG(LogTemp, Error, TEXT("No PressurePlate found for %s"), *(GetOwner()->GetName()));
	}
}


// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	/// if the actorThatOpens is in the volume
	if (GetTotalMassOfActorsOnPlate() > TriggerMass)
	{
		UE_LOG(LogTemp, Warning, TEXT("Door opening"));
		OnOpen.Broadcast();
	}	

	/// check if it's time to close the door
	else
	{
		OnClose.Broadcast();
	}
}

float UOpenDoor::GetTotalMassOfActorsOnPlate()
{
	float TotalMass = 0.f;

	/// Find all overlapping actors
	TArray<AActor*> OverlappingActors;
	if (PressurePlate == nullptr) 
	{
		return TotalMass; 
	}
	PressurePlate->GetOverlappingActors(OUT OverlappingActors);

	/// iterate trhough them adding their masses
	for (const auto* CurrentActor : OverlappingActors)
	{
		TotalMass += CurrentActor->FindComponentByClass<UPrimitiveComponent>()->GetMass();

		UE_LOG(LogTemp, Warning, TEXT("Overlapping : %s		TotalMass = %f"), *(CurrentActor->GetName()), TotalMass)
	}

	return TotalMass;
}