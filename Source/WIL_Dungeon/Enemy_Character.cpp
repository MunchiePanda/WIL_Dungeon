// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy_Character.h"

// Sets default values
AEnemy_Character::AEnemy_Character()
{
 	PrimaryActorTick.bCanEverTick = true;
    MaxHealth = 100.0f;
    CurrentHealth = MaxHealth;

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
