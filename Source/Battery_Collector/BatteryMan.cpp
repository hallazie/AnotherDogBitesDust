/*
 * @Author: Xiao Shanghua
 * @Date: 2020-10-11 19:19:02
 * @LastEditTime: 2020-10-25 16:54:52
 * @LastEditors: Xiao Shanghua
 * @Description: 
 * @FilePath: \Battery_Collector\Source\Battery_Collector\BatteryMan.cpp
 */
// Fill out your copyright notice in the Description page of Project Settings.


#include "BatteryMan.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/Actor.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ABatteryMan::ABatteryMan()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bDead = false;
	bAttacking = false;
	bDancing = false;
	Power = 100.0f;
	DefaultMaxWalkSpeed = 600.0f;
	Power_Threshold = 3.0f;
	SprintMultiplier = 2.0f;

	GetCapsuleComponent()->InitCapsuleSize(42.0f, 96.0f);

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f);
	GetCharacterMovement()->JumpZVelocity = 600.0f;
	GetCharacterMovement()->AirControl = 0.2f;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f;
	CameraBoom->bUsePawnControlRotation = true;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	//static ConstructorHelpers::FObjectFinder<UAnimSequence> MeleeFistAttackObject(TEXT("AnimSequence'/Game/Animation/SWAT/Hook_Punch.Hook_Punch'"));
	//if(MeleeFistAttackObject.Succeeded()){
	//	MeleeFistAttack = MeleeFistAttackObject.Object;
	//}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> MeleeFistAttackMontageObject(TEXT("AnimMontage'/Game/Animation/SWAT/BPM_MeleeFistAttack.BPM_MeleeFistAttack'"));
	if (MeleeFistAttackMontageObject.Succeeded()) {
		MeleeFistAttackMontage = MeleeFistAttackMontageObject.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> DanceMontageObject(TEXT("AnimMontage'/Game/Animation/SWAT/BPM_Dance.BPM_Dance'"));
	if (DanceMontageObject.Succeeded()) {
		DanceMontage = DanceMontageObject.Object;
	}
}

// Called when the game starts or when spawned
void ABatteryMan::BeginPlay()
{
	Super::BeginPlay();
	
	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &ABatteryMan::OnBeginOverlap);

	if(Player_Power_Widget_Class != nullptr){
		Player_Power_Widget = CreateWidget(GetWorld(), Player_Power_Widget_Class);
		Player_Power_Widget->AddToViewport();
	}
	
}

// Called every frame
void ABatteryMan::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	Power -= DeltaTime * Power_Threshold;

	if(Power <= 0 && !bDead){
		bDead = true;
		GetMesh()->SetSimulatePhysics(true);
		FTimerHandle UnusedHandle;
		GetWorldTimerManager().SetTimer(
			UnusedHandle, this, &ABatteryMan::RestartGame, 3.0f, false
		);
	}

}

// Called to bind functionality to input
void ABatteryMan::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);
	PlayerInputComponent->BindAxis("MoveForward", this, &ABatteryMan::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ABatteryMan::MoveRight);

	PlayerInputComponent->BindAction("SprintHold", IE_Pressed, this, &ABatteryMan::SprintStart);
	PlayerInputComponent->BindAction("SprintHold", IE_Released, this, &ABatteryMan::SprintStop);

	//PlayerInputComponent->BindAction("Dance", IE_Pressed, this, &ABatteryMan::DanceStart);
	//PlayerInputComponent->BindAction("Dance", IE_Released, this, &ABatteryMan::DanceStop);
	PlayerInputComponent->BindAction("Dance", IE_Pressed, this, &ABatteryMan::DanceLoop);
	PlayerInputComponent->BindAction("Attack", IE_Pressed, this, &ABatteryMan::AttackStart);
	PlayerInputComponent->BindAction("Attack", IE_Released, this, &ABatteryMan::AttackStop);

}

void ABatteryMan::ResumeMovingStatus() {
	//if (bAttacking || bDancing) {
	//	StopAnimMontage(MeleeFistAttackMontage);
	//	StopAnimMontage(DanceMontage);
	//}
}

void ABatteryMan::MoveForward(float Axis){
	if(!bDead){
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Axis);
	}
	ResumeMovingStatus();
}

void ABatteryMan::MoveRight(float Axis){
	if(!bDead){
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		AddMovementInput(Direction, Axis);
	}
	ResumeMovingStatus();
}

void ABatteryMan::OnBeginOverlap(
		class UPrimitiveComponent* HitComp, 
		class AActor* OtherActor,
		class UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult & SweepResult
){
	if(OtherActor->ActorHasTag("Recharge")){
		UE_LOG(LogTemp, Warning, TEXT("collided with"));
		Power += 10.0f;
		if(Power > 100.0f){
			Power = 100.0f;
		}
		OtherActor->Destroy();
	}
}

void ABatteryMan::RestartGame(){
	UGameplayStatics::OpenLevel(this, FName(*GetWorld()->GetName()), false);
}

void ABatteryMan::SprintStart(){
	GetCharacterMovement()->MaxWalkSpeed *= SprintMultiplier;
}

void ABatteryMan::SprintStop(){
	GetCharacterMovement()->MaxWalkSpeed /= SprintMultiplier;
}

void ABatteryMan::DanceStart(){
	bDancing = true;
	PlayAnimMontage(DanceMontage, 1.5f);
}

void ABatteryMan::DanceStop() {
	//bDancing = false;
}

void ABatteryMan::AttackStart(){
	bAttacking = true;
	GetCharacterMovement()->MaxWalkSpeed = 0.0f;
	PlayAnimMontage(MeleeFistAttackMontage, 2.0f);
}

void ABatteryMan::AttackStop() {
	//bAttacking = false;
	GetCharacterMovement()->MaxWalkSpeed = DefaultMaxWalkSpeed;
}

void ABatteryMan::DanceLoop() {
	if (bDancing) {
		bDancing = false;
		GetCharacterMovement()->MaxWalkSpeed = DefaultMaxWalkSpeed;
		StopAnimMontage(DanceMontage);
	}
	else {
		bDancing = true;
		GetCharacterMovement()->MaxWalkSpeed = 0.0f;
		PlayAnimMontage(DanceMontage, 1.5f);
	}
}