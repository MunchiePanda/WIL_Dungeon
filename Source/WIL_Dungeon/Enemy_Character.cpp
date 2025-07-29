// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy_Character.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h"

// Sets default values
AEnemy_Character::AEnemy_Character()
{
 	PrimaryActorTick.bCanEverTick = true;
    MaxHealth = 100.0f;
    CurrentHealth = MaxHealth;

    // AI Perception setup
    PerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("PerceptionComponent"));
    SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SightConfig"));
    if (SightConfig)
    {
        SightConfig->SightRadius = 1500.0f;
        SightConfig->LoseSightRadius = 1800.0f;
        SightConfig->PeripheralVisionAngleDegrees = 90.0f;
        SightConfig->SetMaxAge(5.0f);
        SightConfig->DetectionByAffiliation.bDetectEnemies = true;
        SightConfig->DetectionByAffiliation.bDetectFriendlies = true;
        SightConfig->DetectionByAffiliation.bDetectNeutrals = true;
        PerceptionComponent->ConfigureSense(*SightConfig);
        PerceptionComponent->SetDominantSense(SightConfig->GetSenseImplementation());
    }
    if (PerceptionComponent)
    {
        PerceptionComponent->OnPerceptionUpdated.AddDynamic(this, &AEnemy_Character::OnPerceptionUpdated);
    }
}

float AEnemy_Character::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
    float DamageApplied = FMath::Min(CurrentHealth, DamageAmount);
    CurrentHealth -= DamageApplied;

    OnDamaged(DamageApplied, DamageCauser);

    if (CurrentHealth <= 0.0f)
    {
        OnDeath();
        Destroy();
    }

    return DamageApplied;
}

void AEnemy_Character::OnDamaged_Implementation(float DamageAmount, AActor* DamageCauser)
{
    // Default: do nothing. Child classes can override.
}

void AEnemy_Character::OnDeath_Implementation()
{
    // Default: do nothing. Child classes can override.
	if (DropClass)
    {
        FActorSpawnParameters SpawnParams;
        SpawnParams.Owner = this;
        GetWorld()->SpawnActor<AActor>(DropClass, GetActorLocation(), FRotator::ZeroRotator, SpawnParams);
    }
}

void AEnemy_Character::OnPerceptionUpdated(const TArray<AActor*>& UpdatedActors)
{
    for (AActor* Actor : UpdatedActors)
    {
        APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
        if (Actor == PlayerPawn)
        {
            FActorPerceptionBlueprintInfo Info;
            PerceptionComponent->GetActorsPerception(Actor, Info);
            for (const FAIStimulus& Stimulus : Info.LastSensedStimuli)
            {
                if (Stimulus.WasSuccessfullySensed())
                {
                    OnSeePawn(PlayerPawn);
                    return;
                }
            }
        }
    }
}

void AEnemy_Character::OnSeePawn(APawn* SeenPawn)
{
    // Default: do nothing. Child classes override for custom behavior.
}
