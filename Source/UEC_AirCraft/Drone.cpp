// Fill out your copyright notice in the Description page of Project Settings.


#include "Drone.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "PhysicsEngine/PhysicsThrusterComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/InputComponent.h"

// Sets default values
ADrone::ADrone()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	OutCollision = CreateAbstractDefaultSubobject<UBoxComponent>(TEXT("OutCollision"));
	RootComponent = OutCollision;
	
	// ���� Box �ķ�Χ
	OutCollision->SetSimulatePhysics(true);
	OutCollision->BodyInstance.bLockXRotation = true;
	OutCollision->BodyInstance.bLockYRotation = true;
	OutCollision->SetBoxExtent(FVector(100.0f, 100.0f, 100.0f));

	// ���˻� X Y ����ת����
	OutCollision->BodyInstance.bLockRotation = true;
	OutCollision->BodyInstance.bLockRotation = true;

	//Mesh ���ò��
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(OutCollision);
	Paddle1 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Paddle1"));
	Paddle2 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Paddle2"));
	Paddle3 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Paddle3"));
	Paddle4 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Paddle4"));

	Paddle1->SetupAttachment(Mesh,TEXT("Paddle1"));
	Paddle2->SetupAttachment(Mesh,TEXT("Paddle2"));
	Paddle3->SetupAttachment(Mesh,TEXT("Paddle3"));
	Paddle4->SetupAttachment(Mesh,TEXT("Paddle4"));

	Paddles.Add(Paddle1);
	Paddles.Add(Paddle2);
	Paddles.Add(Paddle3);
	Paddles.Add(Paddle4);

	//�ƽ��� ���½����ƽ�
	UpThruster = CreateDefaultSubobject<UPhysicsThrusterComponent>(TEXT("UpThruster"));
	UpThruster->SetupAttachment(RootComponent);
	UpThruster->ThrustStrength = 980.f;
	UpThruster->SetAutoActivate(true);
	UpThruster->SetWorldRotation(UKismetMathLibrary::MakeRotFromX(-this->GetActorUpVector()));

	//�ƽ��� �������ƽ�
	ForwardThruster = CreateDefaultSubobject<UPhysicsThrusterComponent>(TEXT("ForwardThruster"));
	ForwardThruster->SetupAttachment(RootComponent);
	ForwardThruster->ThrustStrength = 0.0f;
	ForwardThruster->SetAutoActivate(true);
	ForwardThruster->SetWorldRotation(UKismetMathLibrary::MakeRotFromX(-this->GetActorForwardVector()));
	
}

// Called when the game starts or when spawned
void ADrone::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ADrone::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (GetInputAxisValue(TEXT("Lift")) == 0.0f)
	{
		UpThruster->ThrustStrength = 0.f;
	}

	// ǰ������
	if (GetInputAxisValue(TEXT("Forward")) == 0.0f)
	{
		ForwardThruster->ThrustStrength = 0.f;

		if (Mesh->GetRelativeRotation().Pitch != 0.0f) {

			float currentPitch = Mesh->GetRelativeRotation().Pitch;
			// �ָ���Ҷ����תֵ
			Mesh->AddRelativeRotation(FRotator(-currentPitch * DeltaTime, 0.0f, 0.0f));
			if (FMath::Abs(Mesh->GetRelativeRotation().Pitch) <= KINDA_SMALL_NUMBER)
			{
				Mesh->SetRelativeRotation(FRotator(.0f, .0f, .0f));
			}
		}
	}

	RotatePaddle(DeltaTime);
}

// Called to bind functionality to input
void ADrone::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(TEXT("Lift"), this, &ADrone::Lift);
	PlayerInputComponent->BindAxis(TEXT("Forward"), this, &ADrone::Forward);
	PlayerInputComponent->BindAxis(TEXT("Turn"), this, &ADrone::Turn);
	PlayerInputComponent->BindAction(TEXT("Fire"), IE_Pressed, this, &ADrone::Fire);
}

// �����½�
void ADrone::Lift(float value)
{
	UpThruster->ThrustStrength += value * LiftAcc * GetWorld()->DeltaTimeSeconds;
	UpThruster->ThrustStrength = FMath::Clamp(UpThruster->ThrustStrength,-LiftThrustMax, LiftThrustMax);
}

// ǰ������
void ADrone::Forward(float value)
{
	ForwardThruster->ThrustStrength += value * ForwardAcc * GetWorld()->DeltaTimeSeconds;
	ForwardThruster->ThrustStrength = FMath::Clamp(ForwardThruster->ThrustStrength, -ForwardThrustMax, ForwardThrustMax);
	float pitch = Mesh->GetRelativeRotation().Pitch;

	if (FMath::Abs(pitch) < 30.0f)
	{
		Mesh->AddRelativeRotation(FRotator(-100.0f * GetWorld()->DeltaTimeSeconds * value, 0.0f, 0.0f));
	}

}

void ADrone::Turn(float value)
{
	// ת�� 
	OutCollision->AddTorqueInDegrees(-this->GetActorUpVector() * value * TurnStrength);
}

// ��ת��Ҷ
void ADrone::RotatePaddle(float DeltaTime)
{
	for (auto paddle : Paddles)
	{
		// AddRelativeRotation ���ϸ���ǰ��ֵ ������ֵ�ĵ���
		paddle->AddRelativeRotation(FRotator(0.f, DeltaTime * PaddleRotateSpeed, .0f));
	}

}

// ����
void ADrone::Fire()
{
	// ��ȡ������ Socket 
	FTransform firingpoint =  Mesh->GetSocketTransform(TEXT("FireingSocket"));
	GetWorld()->SpawnActor<AMissle>(Bullet, firingpoint);


}

