#include "WeaponBase.h"

AWeaponBase::AWeaponBase()
{
    PrimaryActorTick.bCanEverTick = false;

    WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));
    RootComponent = WeaponMesh;

    Damage = 10.0f;
    WeaponType = EWeaponType::Sword;
    AttachSocketName = "WeaponSocket";
    AttackMontage = nullptr;
}