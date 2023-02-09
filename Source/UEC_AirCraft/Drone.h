// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Missle.h"
#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Drone.generated.h"

UCLASS()
class UEC_AIRCRAFT_API ADrone : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ADrone();

	UPROPERTY(VisibleAnywhere)
	class UStaticMeshComponent* Mesh;

	UPROPERTY(VisibleAnywhere)
	class UStaticMeshComponent* Paddle1;

	UPROPERTY(VisibleAnywhere)
	class UStaticMeshComponent* Paddle2;

	UPROPERTY(VisibleAnywhere)
	class UStaticMeshComponent* Paddle3;

	UPROPERTY(VisibleAnywhere)
	class UStaticMeshComponent* Paddle4;

	UPROPERTY(VisibleAnywhere)
	class UBoxComponent* OutCollision;

	// 物理推进器
	/*UPROPERTY(VisibleAnywhere)
	class UPhysicsThrusterComponent* Thurster;*/

	UPROPERTY(VisibleAnywhere)
	class UPhysicsThrusterComponent* UpThruster;

	UPROPERTY(VisibleAnywhere)
	class UPhysicsThrusterComponent* ForwardThruster;

	// 力的增速值
	// EditAnywhere 是对值来说可以修改，VisibleAnywhere 是对指针来说可以修改
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "Init")
	float LiftAcc = 300.f;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "Init")
	float LiftThrustMax = 3000.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Init")
	float ForwardAcc = 300.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Init")
	float ForwardThrustMax = 3000.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Init")
	float TurnStrength = 500000.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Init")
	float PaddleRotateSpeed = 500.0f;
	
	// 这是一个 类 蓝图类 不是一个具体的对象
	UPROPERTY(EditAnywhere)
	TSubclassOf<AMissle> Bullet;
	
protected:
	//ADrone();
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:

	UFUNCTION()
	void Lift(float value);
	
	UFUNCTION()
	void Forward(float value);

	UFUNCTION()
	void Turn(float value);

	TArray<UStaticMeshComponent*> Paddles;


	void RotatePaddle(float DeltaTime);

	UFUNCTION()
	void Fire();

};
