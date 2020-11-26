// Copyright Epic Games, Inc. All Rights Reserved.

#include "BatteryMan.h"


// Sets default values
ABatteryMan::ABatteryMan()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bDead = false;
	bAttacking = false;
	bDancing = false;
	bInAir = false;
	bActivateSprint = false;

	Power = 100.0f;
	Health = 100.0f;
	Energy = 100.0f;
	DefaultMaxWalkSpeed = 600.0f;
	Power_Threshold = 1.0f;
	SprintMultiplier = 2.0f;
	ComboLoop = 1;
	DanceType = 0;
	LeftFistDamage = 0.0f;
	RightFistDamage = 0.0f;

	//HitActorArrayCache.Empty();

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

	LeftFistCollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("LeftFistCollesionBox"));
	LeftFistCollisionBox->SetupAttachment(RootComponent);
	RightFistCollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("RightFistCollesionBox"));
	RightFistCollisionBox->SetupAttachment(RootComponent);

	//static ConstructorHelpers::FObjectFinder<UAnimSequence> MeleeFistAttackObject(TEXT("AnimSequence'/Game/Character/SWAT/Hook_Punch.Hook_Punch'"));
	//if(MeleeFistAttackObject.Succeeded()){
	//	MeleeFistAttack = MeleeFistAttackObject.Object;
	//}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> MeleeFistAttackMtgObj(TEXT("AnimMontage'/Game/Characters/SWAT/BPM_MeleeFistAttack.BPM_MeleeFistAttack'"));
	if (MeleeFistAttackMtgObj.Succeeded()) {
		MeleeFistAttackMontage = MeleeFistAttackMtgObj.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> DanceRumbaMtgObj(TEXT("AnimMontage'/Game/Characters/SWAT/BPM_RumbaDance.BPM_RumbaDance'"));
	if (DanceRumbaMtgObj.Succeeded()) {
		DanceRumbaMontage = DanceRumbaMtgObj.Object;
		DanceMontage = DanceRumbaMontage;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> DanceSillyMtgObj(TEXT("AnimMontage'/Game/Characters/SWAT/BPM_SillyDance.BPM_SillyDance'"));
	if (DanceSillyMtgObj.Succeeded()) {
		DanceSillyMontage = DanceSillyMtgObj.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> CombatAttackMtgObj(TEXT("AnimMontage'/Game/Characters/SWAT/BPM_ComboFistAttack.BPM_ComboFistAttack'"));
	if (CombatAttackMtgObj.Succeeded()) {
		CombatAttackMontage = CombatAttackMtgObj.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> JumpMtgObj(TEXT("AnimMontage'/Game/Characters/SWAT/BPM_Jump.BPM_Jump'"));
	if (JumpMtgObj.Succeeded()) {
		JumpMontage = JumpMtgObj.Object;
	}

	static ConstructorHelpers::FObjectFinder<USoundWave> FootStepDirtSWObj(TEXT("SoundWave'/Game/Audios/step-dirt.step-dirt'"));
	if (FootStepDirtSWObj.Succeeded()) {
		FootStepDirtSoundWave = FootStepDirtSWObj.Object;
	}

	static ConstructorHelpers::FObjectFinder<USoundWave> FootStepGrassSWObj(TEXT("SoundWave'/Game/Audios/step-grass.step-grass'"));
	if (FootStepGrassSWObj.Succeeded()) {
		FootStepGrassSoundWave = FootStepGrassSWObj.Object;
	}

	static ConstructorHelpers::FObjectFinder<USoundWave> FootStepConcretSWObj(TEXT("SoundWave'/Game/Audios/step-concret.step-concret'"));
	if (FootStepConcretSWObj.Succeeded()) {
		FootStepConcretSoundWave = FootStepConcretSWObj.Object;
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

	const FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::KeepWorld, false);
	LeftFistCollisionBox->AttachToComponent(GetMesh(), AttachmentRules, "LeftFistCollision");
	LeftFistCollisionBox->SetHiddenInGame(false);
	LeftFistCollisionBox->SetCollisionProfileName("NoCollision");
	RightFistCollisionBox->AttachToComponent(GetMesh(), AttachmentRules, "RightFistCollision");
	RightFistCollisionBox->SetHiddenInGame(false);
	RightFistCollisionBox->SetCollisionProfileName("NoCollision");


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
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ABatteryMan::JumpInput);
	//PlayerInputComponent->BindAction("Jump", IE_Released, this, &ABatteryMan::JumpEnd);
	PlayerInputComponent->BindAxis("MoveForward", this, &ABatteryMan::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ABatteryMan::MoveRight);

	PlayerInputComponent->BindAction("SprintHold", IE_Pressed, this, &ABatteryMan::SprintStart);
	PlayerInputComponent->BindAction("SprintHold", IE_Released, this, &ABatteryMan::SprintStop);

	//PlayerInputComponent->BindAction("Dance", IE_Pressed, this, &ABatteryMan::DanceStart);
	//PlayerInputComponent->BindAction("Dance", IE_Released, this, &ABatteryMan::DanceStop);
	PlayerInputComponent->BindAction("Dance", IE_Pressed, this, &ABatteryMan::DanceLoop);
	PlayerInputComponent->BindAction("Attack", IE_Pressed, this, &ABatteryMan::AttackInput);
	//PlayerInputComponent->BindAction("Attack", IE_Released, this, &ABatteryMan::AttackStop);

}

void ABatteryMan::ResumeMovingStatus(bool stopAttack) {
	if (bDancing) {
		UE_LOG(LogTemp, Warning, TEXT("stop dancing in MoveForward with maxWalkSpeed=%f"), GetCharacterMovement()->MaxWalkSpeed);
		StopAnimMontage(DanceMontage);
		bDancing = false;
	}
	if (stopAttack && bAttacking) {
		bAttacking = false;
		StopAnimMontage(CombatAttackMontage);
		AttackStop();
	}
}

void ABatteryMan::MoveForward(float Axis){
	if(!bDead){
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Axis);
	} 
	if (GetCurrentSpeed() > 20.0f) {
		ResumeMovingStatus(false);
		if (bActivateSprint) {
			SprintStart();
		}
	}
}

void ABatteryMan::MoveRight(float Axis){
	if(!bDead){
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		AddMovementInput(Direction, Axis);
	}
	if (GetCurrentSpeed() > 20.0f) {
		ResumeMovingStatus(false);
		if (bActivateSprint) {
			SprintStart();
		}
	}
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
		UE_LOG(LogTemp, Warning, TEXT("collided with charger"));
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
	bActivateSprint = true;
	ResumeMovingStatus(true);
	GetCharacterMovement()->MaxWalkSpeed = DefaultMaxWalkSpeed * SprintMultiplier;
	UE_LOG(LogTemp, Warning, TEXT("sprint start speed: %f"), GetCharacterMovement()->MaxWalkSpeed);
}

void ABatteryMan::SprintStop(){
	GetCharacterMovement()->MaxWalkSpeed = DefaultMaxWalkSpeed;
	UE_LOG(LogTemp, Warning, TEXT("sprint stop speed: %f"), GetCharacterMovement()->MaxWalkSpeed);
	bActivateSprint = false;
}

void ABatteryMan::DanceStart(){
	bDancing = true;
	PlayAnimMontage(DanceMontage, 1.5f);
}

void ABatteryMan::DanceStop() {
	//bDancing = false;
}

void ABatteryMan::AttackInput() {
	if (!bAttacking && !bInAir) {
		bAttacking = true;
		GetCharacterMovement()->MaxWalkSpeed = 0.0f;
		FString MontageSection = "start_" + FString::FromInt(ComboLoop);
		switch (ComboLoop) {
		case 1:
			LeftFistCollisionBox->SetCollisionProfileName("Weapon");
			LeftFistDamage = 20.0f;
			RightFistCollisionBox->SetCollisionProfileName("NoCollision");
			RightFistDamage = 0.0f;
			break;
		case 2:
			LeftFistCollisionBox->SetCollisionProfileName("NoCollision");
			LeftFistDamage = 0.0f;
			RightFistCollisionBox->SetCollisionProfileName("Weapon");
			RightFistDamage = 20.0f;
			break;
		case 3:
			LeftFistCollisionBox->SetCollisionProfileName("Weapon");
			LeftFistDamage = 20.0f;
			RightFistCollisionBox->SetCollisionProfileName("NoCollision");
			RightFistDamage = 0.0f;
			break;
		default:
			LeftFistCollisionBox->SetCollisionProfileName("NoCollision");
			LeftFistDamage = 0.0f;
			RightFistCollisionBox->SetCollisionProfileName("NoCollision");
			RightFistDamage = 0.0f;
		}
		UE_LOG(LogTemp, Warning, TEXT("playing montage section: start_%d"), ComboLoop);
		PlayAnimMontage(CombatAttackMontage, 2.0f, FName(MontageSection));
		ComboLoop += 1;
		if (ComboLoop > 3) {
			ComboLoop = 1;
		}
	}
}

void ABatteryMan::AttackStart(){
	//HitActorArrayCache.Empty();
	FVector LeftFistLocation = GetMesh()->GetSocketLocation(TEXT("LeftFistCollision"));
	FVector RightFistLocation = GetMesh()->GetSocketLocation(TEXT("RightFistCollision"));
	//UAISense_Damage::ReportDamageEvent(LeftFistCollisionBox, nullptr, this, LeftFistDamage, GetActorLocation(), LeftFistLocation);
}

void ABatteryMan::AttackStop() {
	bAttacking = false;
	GetCharacterMovement()->MaxWalkSpeed = DefaultMaxWalkSpeed;
	//HitActorArrayCache.Empty();
}

void ABatteryMan::DanceLoop() {
	if (bDancing) {
		bDancing = false;
		GetCharacterMovement()->MaxWalkSpeed = DefaultMaxWalkSpeed;
		UE_LOG(LogTemp, Warning, TEXT("new max speed: %f"), GetCharacterMovement()->MaxWalkSpeed);
		StopAnimMontage(DanceMontage);
	}
	else {
		bDancing = true;
		DanceType++;
		if (DanceType > 1){
			DanceType = 0;
		}
		switch (DanceType) {
		case 0:
			DanceMontage = DanceRumbaMontage;
			break;
		case 1:
			DanceMontage = DanceSillyMontage;
			break;
		default:
			DanceMontage = DanceRumbaMontage;
		}
		UE_LOG(LogTemp, Warning, TEXT("raw max speed: %f"), GetCharacterMovement()->MaxWalkSpeed);
		GetCharacterMovement()->MaxWalkSpeed = 0.0f;
		PlayAnimMontage(DanceMontage, 1.5f);
	}
}

void ABatteryMan::JumpInput() {
	if (!bInAir) {
		//PlayAnimMontage(JumpMontage, 2.0f, FName("jump_start"));
		//PlayAnimMontage(JumpMontage, 2.0f, FName("jump_air"));
		//PlayAnimMontage(JumpMontage, 2.0f, FName("jump_end"));
		PlayAnimMontage(JumpMontage, 1.2f, FName("jump_air"));

	}
}

void ABatteryMan::JumpStart()
{
	bInAir = true;
	Super::Jump();
}

void ABatteryMan::JumpEnd() {
	bInAir = false;
}

/*
UTILS FUNCTIONS
*/

float ABatteryMan::GetCurrentSpeed() {
	float currentSpeed = FVector::DotProduct(GetVelocity(), GetActorRotation().Vector());
	return currentSpeed;
}

void ABatteryMan::TriggerFootStep() {
	//GEngine->AddOnScreenDebugMessage(-1, 4.5f, FColor::Orange, "playing footstep sound");
	UGameplayStatics::PlaySound2D(this, FootStepDirtSoundWave, 0.2f);
	//UAISense_Hearing::ReportNoiseEvent(GetMesh(), GetActorLocation(), 1.0f, this, 0.0f, TEXT("BatteryManFootStepNoise"));

}