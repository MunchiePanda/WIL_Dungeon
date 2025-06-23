#include "CombatComponent.h"
#include "WeaponBase.h"
#include "CharacterStatsComp.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"

UCombatComponent::UCombatComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void UCombatComponent::BeginPlay()
{
    Super::BeginPlay();

    OwnerCharacter = Cast<ACharacter>(GetOwner());
    if (OwnerCharacter)
    {
        StatsComp = OwnerCharacter->FindComponentByClass<UCharacterStatsComp>();
        InitializeWeapons();
        SetupInputBindings();
    }
}

void UCombatComponent::InitializeWeapons()
{
    if (!OwnerCharacter) return;

    // Destroy any existing
    for (AWeaponBase* Weapon : WeaponActors)
    {
        if (Weapon) Weapon->Destroy();
    }
    WeaponActors.Empty();

    // Spawn and attach weapons
    for (int32 i = 0; i < WeaponClasses.Num(); ++i)
    {
        if (WeaponClasses[i])
        {
            FActorSpawnParameters SpawnParams;
            SpawnParams.Owner = OwnerCharacter;
            AWeaponBase* NewWeapon = GetWorld()->SpawnActor<AWeaponBase>(WeaponClasses[i], FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams);
            if (NewWeapon)
            {
                NewWeapon->AttachToComponent(OwnerCharacter->GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, NewWeapon->AttachSocketName);
                NewWeapon->SetActorHiddenInGame(true);
                WeaponActors.Add(NewWeapon);
            }
        }
    }
    EquippedWeapon = nullptr;
}

void UCombatComponent::EquipWeapon(int32 WeaponIndex)
{
    if (WeaponActors.IsValidIndex(WeaponIndex))
    {
        // Hide all weapons
        for (AWeaponBase* Weapon : WeaponActors)
        {
            if (Weapon) Weapon->SetActorHiddenInGame(true);
        }
        EquippedWeapon = WeaponActors[WeaponIndex];
        if (EquippedWeapon)
        {
            EquippedWeapon->SetActorHiddenInGame(false);
        }
    }
}

void UCombatComponent::Attack()
{
    if (!EquippedWeapon || !OwnerCharacter) return;

    PlayAttackMontage();
    PerformAttackTrace();
}

void UCombatComponent::PlayAttackMontage()
{
    if (EquippedWeapon && EquippedWeapon->AttackMontage && OwnerCharacter)
    {
        OwnerCharacter->PlayAnimMontage(EquippedWeapon->AttackMontage);
    }
}

void UCombatComponent::PerformAttackTrace()
{
    if (!OwnerCharacter || !EquippedWeapon) return;

    FVector Start = OwnerCharacter->GetActorLocation() + OwnerCharacter->GetActorForwardVector() * 100.0f;
    FVector End = Start + OwnerCharacter->GetActorForwardVector() * 150.0f;
    float Radius = 75.0f;

    TArray<FHitResult> HitResults;
    FCollisionShape Sphere = FCollisionShape::MakeSphere(Radius);

    // Debug: Draw the trace area
    DrawDebugLine(GetWorld(), OwnerCharacter->GetActorLocation(), End, FColor::Green, false, 2.0f, 0, 2.0f);
    DrawDebugSphere(GetWorld(), End, Radius, 16, FColor::Red, false, 2.0f);

    bool bHit = GetWorld()->SweepMultiByChannel(
        HitResults,
        Start,
        End,
        FQuat::Identity,
        ECC_Pawn,
        Sphere
    );

    if (bHit)
    {
        for (const FHitResult& Hit : HitResults)
        {
            AActor* HitActor = Hit.GetActor();
            if (HitActor && HitActor != OwnerCharacter)
            {
                float FinalDamage = EquippedWeapon->Damage;
                if (StatsComp)
                {
                    FinalDamage *= StatsComp->AttackModifier;
                }
                UGameplayStatics::ApplyDamage(HitActor, FinalDamage, OwnerCharacter->GetController(), OwnerCharacter, nullptr);

                // Debug: Print the name of the hit actor
                if (GEngine)
                {
                    GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, FString::Printf(TEXT("Hit Actor: %s"), *HitActor->GetName()));
                }
            }
        }
    }
}

void UCombatComponent::SetupInputBindings()
{
    if (APlayerController* PC = Cast<APlayerController>(OwnerCharacter->GetController()))
    {
        // Only works for possessed player, for AI or other input systems, bind elsewhere
        // You may want to bind these in your PlayerController or Character class instead
    }
}

void UCombatComponent::OnEquipWeapon1() { EquipWeapon(0); }
void UCombatComponent::OnEquipWeapon2() { EquipWeapon(1); }
void UCombatComponent::OnEquipWeapon3() { EquipWeapon(2); }
void UCombatComponent::OnAttack() { Attack(); }