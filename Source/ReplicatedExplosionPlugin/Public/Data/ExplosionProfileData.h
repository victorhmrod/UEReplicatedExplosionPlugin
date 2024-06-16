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
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "VFX|Particles")
	TObjectPtr<UParticleSystem> CascadedParticle;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "VFX|Particles")
	TObjectPtr<UNiagaraSystem> NiagaraParticle;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "VFX|Particles")
	bool ShouldUseCascadedAndNiagaraParticle;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "VFX|Particles")
	FVector EffectScale = {1.f, 1.f, 1.f};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "VFX|CameraShake")
	TSubclassOf<UCameraShakeBase> CamShakeClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "VFX|CameraShake")
	float CameraShakeInnerRadius{50.f};
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "VFX|CameraShake")
	float CameraShakeFalloff{50.f};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SFX|Sound")
	TObjectPtr<USoundBase> Sound;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Damage")
	TArray<AActor*> IgnoreThisActorsInExplosion;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI")
	FName NoiseTag = "1";
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SFX|Sound")
	TObjectPtr<USoundAttenuation> Attenuation;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Damage")
	float Radius = 1000.0;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Damage")
	FVector2D DamageRange = {15, 30};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Damage")
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
};

class REPLICATEDEXPLOSIONPLUGIN_API UExplosionProfileData
{
	
public:
	UExplosionProfileData();
	~UExplosionProfileData();
};
