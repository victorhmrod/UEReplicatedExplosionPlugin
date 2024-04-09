// All rights reserved Wise Labs ®

#pragma once

#include "CoreMinimal.h"
#include "NiagaraSystem.h"
#include "Engine/DataAsset.h"
#include "ExplosionProfileData.generated.h"


UCLASS(Blueprintable, BlueprintType)
class UExplosionProfile : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VFX")
	UParticleSystem* CascadedParticle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VFX")
	UNiagaraSystem* NiagaraParticle;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VFX")
	FVector EffectScale = {1.f, 1.f, 1.f};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VFX")
	TSubclassOf<UCameraShakeBase> CamShakeClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SFX")
	USoundBase* Sound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Misc")
	FName NoiseTag = "1";
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SFX")
	USoundAttenuation* Attenuation;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
	float Radius = 1000.0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
	FVector2D DamageRange = {15, 30};
};

class REPLICATEDEXPLOSIONPLUGIN_API UExplosionProfileData
{
	
public:
	UExplosionProfileData();
	~UExplosionProfileData();
};
