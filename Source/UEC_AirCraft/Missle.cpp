// Fill out your copyright notice in the Description page of Project Settings.


#include "Missle.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Enemy.h"
#include "UEC_AirCraftGameModeBase.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AMissle::AMissle()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = Mesh;

	// 炮弹移动组件
	MovementComp = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("MoveComp"));
	MovementComp->InitialSpeed = 1000.0f;

	Mesh->SetNotifyRigidBodyCollision(true);
	Mesh->BodyInstance.SetUseCCD(true);
	// 激活穿插事件
	Mesh->SetGenerateOverlapEvents(true);

	Mesh->SetCollisionProfileName(TEXT("OverlapAll"));

	// 绑定穿插事件
	//Mesh->OnComponentBeginOverlap.AddDynamic(this,&AMissle::OnOverlapBegin);
	FScriptDelegate OverlapBeginDelegate;
	OverlapBeginDelegate.BindUFunction(this, TEXT("OnOverlapBegin"));
	Mesh->OnComponentBeginOverlap.Add(OverlapBeginDelegate);
	Mesh->OnComponentBeginOverlap.AddDynamic(this,&AMissle::OnOverlapBegin);

	Mesh->OnComponentEndOverlap.AddDynamic(this, &AMissle::OverlapEndHandler);
	//Mesh->OnComponentHit.AddDynamic(this, &AMissle::OnHit);

	//Mesh->OnComponentHit.AddDynamic(this,&AMissle::)

	UE_LOG(LogTemp, Warning, TEXT("11"));

	// 设置导弹的生命周期为 8 s
	//SetLifeSpan(8.0f);
}

void AMissle::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, 
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("Overlap Begin"));
}

void AMissle::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	UE_LOG(LogTemp, Warning, TEXT("Hit"));
}


void AMissle::OverlapHandler(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("Routine Overlap is Success"));

	OtherActor->Destroy();
	if (OtherActor->IsA<AEnemy>())
	{
		OtherActor->Destroy();

		AUEC_AirCraftGameModeBase* gm = Cast<AUEC_AirCraftGameModeBase>(GetWorld()->GetAuthGameMode());

		if (gm != nullptr)
		{
			gm->points++;
		}

		this->Destroy();

	}
}


void AMissle::OverlapEndHandler(UPrimitiveComponent* OverlappedComponent, AActor*
	OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	UE_LOG(LogTemp, Warning, TEXT("Routine OverlapEnd is Success"));



}

// Called when the game starts or when spawned
void AMissle::BeginPlay()
{
	Super::BeginPlay();
	UGameplayStatics::PlaySound2D(this, MissleSound, 0.5f);
}

// Called every frame
void AMissle::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//UE_LOG(LogTemp, Warning, TEXT("32"));
	

}

// 


