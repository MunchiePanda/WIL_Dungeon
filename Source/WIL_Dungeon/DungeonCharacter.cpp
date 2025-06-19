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