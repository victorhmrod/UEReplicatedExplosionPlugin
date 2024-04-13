// Replicated Explosion Plugin - Unreal Engine Plugin - (c) Wise Labs 2020-2024

#pragma once

#include "CoreMinimal.h"
#include "NiagaraSystem.h"
#include "Camera/CameraShakeBase.h"
#include "Sound/SoundBase.h"
#include "Engine/DataAsset.h"
#include "ExplosionProfileData.generated.h"


UCLASS(Blueprintable, BlueprintType)
class UExplosionProfile : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VFX")
	TObjectPtr<UParticleSystem> CascadedParticle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VFX")
	TObjectPtr<UNiagaraSystem> NiagaraParticle;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VFX")
	FVector EffectScale = {1.f, 1.f, 1.f};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VFX")
	TSubclassOf<UCameraShakeBase> CamShakeClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SFX")
	TObjectPtr<USoundBase> Sound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Misc")
	FName NoiseTag = "1";
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SFX")
	TObjectPtr<USoundAttenuation> Attenuation;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
	float Radius = 1000.0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
	FVector2D DamageRange = {15, 30};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
};

class REPLICATEDEXPLOSIONPLUGIN_API UExplosionProfileData
{
	
public:
	UExplosionProfileData();
	~UExplosionProfileData();
};
