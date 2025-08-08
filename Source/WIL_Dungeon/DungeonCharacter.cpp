// DungeonCharacter.cpp
#include "DungeonCharacter.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"

ADungeonCharacter::ADungeonCharacter()
{
    PrimaryActorTick.bCanEverTick = true;

    // Set up capsule size
    GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

    // Configure character movement
    GetCharacterMovement()->bOrientRotationToMovement = false;
    GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f);
    GetCharacterMovement()->JumpZVelocity = 600.f;
    GetCharacterMovement()->AirControl = 0.2f;

    // Create spring arm and camera
    SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
    SpringArm->SetupAttachment(RootComponent);
    SpringArm->TargetArmLength = 0.f;
    SpringArm->bUsePawnControlRotation = true;

    Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
    Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);
    Camera->bUsePawnControlRotation = false;

    // Disable default controller rotation (we'll handle it with input)
    bUseControllerRotationPitch = false;
    bUseControllerRotationYaw = false;
    bUseControllerRotationRoll = false;

    // Create Stats Component & set defaults
    Stats = CreateDefaultSubobject<UCharacterStatsComp>(TEXT("Stats"));
    Stats->MaxHealth = 100.0f;
    Stats->CurrentHealth = Stats->MaxHealth;
    Stats->MaxSystemExposure = 100;
    Stats->CurrentSystemExposure = Stats->MaxSystemExposure;
    Stats->DamageModifier = 1.0f;       //100%
    Stats->MaxStamina = 10.0f;
    Stats->CurrentStamina = Stats->MaxStamina;
    Stats->StaminaRecoveryRate = 0.5f;
    Stats->WalkSpeed = 400.0f;
    Stats->SprintSpeed = 800.0f;
    Stats->CarryCapacity = 16;  //4x4 grid
    Stats->BaseAttack = 10.0f;
    Stats->AttackModifier = 1.0f;   //100%

    // Create Inventory Component
    Inventory = CreateDefaultSubobject<UInventoryComponent>(TEXT("Inventory"));

    // Create Advanced Combat Component
    //AdvancedCombat = CreateDefaultSubobject<UAdvancedCombatComponent>(TEXT("AdvancedCombat"));
}

void ADungeonCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    // Bind axis inputs for movement and looking
    PlayerInputComponent->BindAxis("MoveForward", this, &ADungeonCharacter::MoveForward);
    PlayerInputComponent->BindAxis("MoveRight", this, &ADungeonCharacter::MoveRight);
    PlayerInputComponent->BindAxis("LookUp", this, &ADungeonCharacter::LookUp);
    PlayerInputComponent->BindAxis("LookRight", this, &ADungeonCharacter::LookRight);

    // Bind action input for jumping
    PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
    PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	// Bind action input for sprinting
    PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &ADungeonCharacter::StartSprint);
    PlayerInputComponent->BindAction("Sprint", IE_Released, this, &ADungeonCharacter::StopSprint);

    // Bind advanced combat inputs
    //PlayerInputComponent->BindAction("LightAttack", IE_Pressed, this, &ADungeonCharacter::OnLightAttack);
    //PlayerInputComponent->BindAction("HeavyAttack", IE_Pressed, this, &ADungeonCharacter::OnHeavyAttack);
    //PlayerInputComponent->BindAction("Block", IE_Pressed, this, &ADungeonCharacter::OnStartBlock);
    //PlayerInputComponent->BindAction("Block", IE_Released, this, &ADungeonCharacter::OnEndBlock);
    //PlayerInputComponent->BindAction("Dodge", IE_Pressed, this, &ADungeonCharacter::OnDodge);
    //PlayerInputComponent->BindAction("Aim", IE_Pressed, this, &ADungeonCharacter::OnStartAim);
    //PlayerInputComponent->BindAction("Aim", IE_Released, this, &ADungeonCharacter::OnEndAim);
    //PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &ADungeonCharacter::OnFire);
    //PlayerInputComponent->BindAction("Reload", IE_Pressed, this, &ADungeonCharacter::OnReload);
    //PlayerInputComponent->BindAction("SwitchFireMode", IE_Pressed, this, &ADungeonCharacter::OnSwitchFireMode);
}

void ADungeonCharacter::MoveForward(float Value)
{
    if (Controller != nullptr && Value != 0.0f)
    {
        const FRotator Rotation = Controller->GetControlRotation();
        const FRotator YawRotation(0, Rotation.Yaw, 0);
        const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
        AddMovementInput(ForwardDirection, Value);
    }
}

void ADungeonCharacter::MoveRight(float Value)
{
    if (Controller != nullptr && Value != 0.0f)
    {
        const FRotator Rotation = Controller->GetControlRotation();
        const FRotator YawRotation(0, Rotation.Yaw, 0);
        const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
        AddMovementInput(RightDirection, Value);
    }
}

void ADungeonCharacter::LookUp(float Value)
{
    if (Controller != nullptr && Value != 0.0f)
    {
        AddControllerPitchInput(Value);
    }
}

void ADungeonCharacter::LookRight(float Value)
{
    if (Controller != nullptr && Value != 0.0f)
    {
        AddControllerYawInput(Value);
    }
}

void ADungeonCharacter::StartSprint()
{
    GetWorld()->GetTimerManager().ClearTimer(StaminaTimer); //stop previous Stamina timer
    //If character has stamina left, begin sprinting
    if (Stats->CurrentStamina > 0) 
    {
        UE_LOG(LogTemp, Log, TEXT("ADungeonCharacter StartSprint(): Starting Sprint"));
        GetCharacterMovement()->MaxWalkSpeed = Stats->SprintSpeed;

        // Start stamina drain every 1 second, if drained, stop sprinting
        GetWorld()->GetTimerManager().SetTimer(StaminaTimer, [this]()
        {
            bool bDrained = Stats->DrainStamina(0.1f);     //How much stamina to drain
            if (bDrained)
            {
                GetWorld()->GetTimerManager().ClearTimer(StaminaTimer); //stop previous Stamina timer
                StopSprint();
            }
        },
        0.1f, true);
    } 
}

void ADungeonCharacter::StopSprint()
{
    UE_LOG(LogTemp, Log, TEXT("ADungeonCharacter StopSprint(): StoppingSprint"));
    
    GetCharacterMovement()->MaxWalkSpeed = Stats->WalkSpeed;
    // Start stamina drain every 1 second, if drained, stop sprinting
    GetWorld()->GetTimerManager().SetTimer(StaminaTimer, [this]()
        {
            bool bRecovered = Stats->RecoverStamina();     //How much stamina to drain
            if (bRecovered)
            {
                GetWorld()->GetTimerManager().ClearTimer(StaminaTimer); //stop previous Stamina timer
            }
        },
        1.0f, true);
}

// Advanced Combat Input Functions
/*
void ADungeonCharacter::OnLightAttack()
{
    if (AdvancedCombat)
    {
        AdvancedCombat->PerformAttack(EAttackType::Light);
    }
}

void ADungeonCharacter::OnHeavyAttack()
{
    if (AdvancedCombat)
    {
        AdvancedCombat->PerformAttack(EAttackType::Heavy);
    }
}

void ADungeonCharacter::OnStartBlock()
{
    if (AdvancedCombat)
    {
        AdvancedCombat->StartBlock();
    }
}

void ADungeonCharacter::OnEndBlock()
{
    if (AdvancedCombat)
    {
        AdvancedCombat->EndBlock();
    }
}

void ADungeonCharacter::OnDodge()
{
    if (AdvancedCombat)
    {
        // Get dodge direction based on movement input
        FVector DodgeDirection = GetVelocity().GetSafeNormal();
        if (DodgeDirection.IsNearlyZero())
        {
            DodgeDirection = GetActorForwardVector();
        }
        AdvancedCombat->PerformDodge(DodgeDirection);
    }
}

void ADungeonCharacter::OnStartAim()
{
    if (AdvancedCombat)
    {
        AdvancedCombat->SetCombatCamera(true);
    }
}

void ADungeonCharacter::OnEndAim()
{
    if (AdvancedCombat)
    {
        AdvancedCombat->SetCombatCamera(false);
    }
}

void ADungeonCharacter::OnFire()
{
    if (AdvancedCombat)
    {
        // This would typically be handled by the weapon system
        // For now, we'll use it as a special attack
        AdvancedCombat->PerformAttack(EAttackType::Special);
    }
}

void ADungeonCharacter::OnReload()
{
    if (AdvancedCombat)
    {
        // This would typically be handled by the weapon system
        // For now, we'll use it to reset combos
        AdvancedCombat->ResetCombo();
    }
}

void ADungeonCharacter::OnSwitchFireMode()
{
    if (AdvancedCombat)
    {
        // This would typically be handled by the weapon system
        // For now, we'll use it to start combos
        AdvancedCombat->StartCombo();
    }
}
*/


