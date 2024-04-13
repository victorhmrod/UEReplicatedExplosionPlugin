// Replicated Explosion Plugin - Unreal Engine Plugin - (c) Wise Labs 2020-2024

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Damage_Interface.generated.h"

UINTERFACE()
class UDamage_Interface : public UInterface
{
	GENERATED_BODY()
};

class REPLICATEDEXPLOSIONPLUGIN_API IDamage_Interface
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="Damage")
	void OnExplosionHit(float Damage, AActor* ExplosionSource);
};
