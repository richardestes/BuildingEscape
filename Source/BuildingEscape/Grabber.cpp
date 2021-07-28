// Copyright Richard Estes 2021

#include "Grabber.h"
#include "DrawDebugHelpers.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"

#define OUT

// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}

// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();
	
	FindPhysicsHandle();
	SetupInputComponent();
}

// Checking for PhysicsHandle Component
void UGrabber::FindPhysicsHandle()
{
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (PhysicsHandle)
	{
		// Physics is found
	}
	else
	{
		UE_LOG(LogTemp,Error,TEXT("%s has no PhysicsHandle Component."), *GetOwner()->GetName());
	}
}

void UGrabber::SetupInputComponent()
{
	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
	if (InputComponent)
	{
		InputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
		InputComponent->BindAction("Grab", IE_Released, this, &UGrabber::Release);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("No InputComponent found for: %s"), *GetOwner()->GetName());
	}
}

void UGrabber::Grab()
{
	UE_LOG(LogTemp, Warning, TEXT("Grabber Pressed"));
	
	// Get players viewpoint
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;
	
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint
	(
		OUT PlayerViewPointLocation, //OUT does nothing, just meant to mark out params
		OUT PlayerViewPointRotation
	);
	
	FVector LineTraceEnd = PlayerViewPointLocation + PlayerViewPointRotation.Vector() * Reach;
	
	FHitResult HitResult = GetFirstPhysicsBodyInReach();
	UPrimitiveComponent* ComponentToGrab = HitResult.GetComponent();
	// If we hit something then attach physics handle.
	if (HitResult.GetActor())
	{
		PhysicsHandle->GrabComponentAtLocation
		(
			ComponentToGrab,
			NAME_None,
			LineTraceEnd
		);
	}
}

void UGrabber::Release()
{
	UE_LOG(LogTemp, Warning, TEXT("Grabber Released"));
	PhysicsHandle->ReleaseComponent();
}

// Draw a line from player showing Reach
void UGrabber::DebugViewRaycast(FVector Location, FRotator Rotation, FVector LineTraceEnd)
{
	
	DrawDebugLine(
		GetWorld(),
		Location,
		LineTraceEnd,
		FColor(0,230,0),
		false,
		0.f,
		0,
		5.f	
	);
}


// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	// Get players viewpoint
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;
	
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerViewPointLocation, //OUT does nothing, just meant to mark out params
		OUT PlayerViewPointRotation
	);
	
	FVector LineTraceEnd = PlayerViewPointLocation + PlayerViewPointRotation.Vector() * Reach;
	
	// If the physics handle is attached
	if (PhysicsHandle->GrabbedComponent)
	{		
		// Move the object we are holding.
		PhysicsHandle->SetTargetLocation(LineTraceEnd);
	}
		
}

FHitResult UGrabber::GetFirstPhysicsBodyInReach() const
{
	// Get players viewpoint
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;
	
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerViewPointLocation, //OUT does nothing, just meant to mark out params
		OUT PlayerViewPointRotation
	);
	
	FVector LineTraceEnd = PlayerViewPointLocation + PlayerViewPointRotation.Vector() * Reach;
	
	// DebugViewRaycast(PlayerViewPointLocation, PlayerViewPointRotation, LineTraceEnd);
	
	// Ray-cast out to a certain distance (Reach)
	FHitResult Hit;
	FCollisionQueryParams TraceParams(FName(TEXT("")),false, GetOwner());
	
	GetWorld()->LineTraceSingleByObjectType(
		OUT Hit,
		PlayerViewPointLocation,
		LineTraceEnd,
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		TraceParams
	);
		
	// See what it hits 
	AActor* ActorHit = Hit.GetActor();
	if (ActorHit)
	{
		UE_LOG(LogTemp,Warning, TEXT("Line trace has hit: %s"), *(ActorHit)->GetName());
	}
	
	return Hit;
}