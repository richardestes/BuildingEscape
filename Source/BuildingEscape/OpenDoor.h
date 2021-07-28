// Copyright Richard Estes 2021

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/TriggerVolume.h"
#include "OpenDoor.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BUILDINGESCAPE_API UOpenDoor : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UOpenDoor();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	void OpenDoor(float DeltaTime);
	void CloseDoor(float DeltaTime);
	
private:
	
	float InitialYaw;
	float CurrentYaw;
	
	
	/*Angle for Fully Open*/
	UPROPERTY(EditAnywhere)
	float OpenAngle = 90.f;
	
	float DoorLastOpened = 0.f;
	
	/*Door Close Delay (sec)*/
	UPROPERTY(EditAnywhere)
	float DoorCloseDelay = 2.f;
	
	/*Door Open Velocity*/
	UPROPERTY(EditAnywhere)
	float DoorOpenSpeed = 3.f;
	
	UPROPERTY(EditAnywhere)
	float DoorShutSpeed = 2.f;
	
	UPROPERTY(EditAnywhere)
	ATriggerVolume* PressurePlate;
	
	UPROPERTY(EditAnywhere)
	AActor* ActorThatOpens;
	
};