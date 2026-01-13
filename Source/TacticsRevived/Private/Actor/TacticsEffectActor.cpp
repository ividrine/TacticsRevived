// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/TacticsEffectActor.h"
#include "AbilitySystemInterface.h"
#include "Core/Unit/UnitAttributeSet.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"


// Sets default values
ATacticsEffectActor::ATacticsEffectActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	SetRootComponent(StaticMesh);

	Sphere = CreateDefaultSubobject<USphereComponent>("Sphere");
	Sphere->SetupAttachment(GetRootComponent());
	
}

void ATacticsEffectActor::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (IAbilitySystemInterface* ASCInterface = Cast<IAbilitySystemInterface>(OtherActor))
	{
		Cast<UUnitAttributeSet>(ASCInterface->GetAbilitySystemComponent()->GetAttributeSet(UUnitAttributeSet::StaticClass()));
	}
}

void ATacticsEffectActor::EndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
}

// Called when the game starts or when spawned
void ATacticsEffectActor::BeginPlay()
{
	Super::BeginPlay();

	Sphere->OnComponentBeginOverlap.AddDynamic(this, &ATacticsEffectActor::OnOverlap);
	Sphere->OnComponentEndOverlap.AddDynamic(this, &ATacticsEffectActor::EndOverlap);
	
}

