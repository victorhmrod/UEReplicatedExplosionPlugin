// Replicated Explosion Plugin - Unreal Engine Plugin - (c) Wise Labs 2020-2024

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ExplosionInterface.generated.h"

UINTERFACE()
class UExplosionInterface : public UInterface
{
	GENERATED_BODY()
};

class REPLICATEDEXPLOSIONPLUGIN_API IExplosionInterface
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="Damage")
	void OnExplosionHit(float Damage, AActor* ExplosionSource);
};
