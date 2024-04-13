// Replicated Explosion Plugin - Unreal Engine Plugin - (c) Wise Labs 2020-2024

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Data/ExplosionProfileData.h"
#include "ExplosionComponent.generated.h"

UCLASS(ClassGroup=(EC), meta=(BlueprintSpawnableComponent))
class REPLICATEDEXPLOSIONPLUGIN_API UExplosionComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UExplosionComponent();

protected:
	virtual void BeginPlay() override;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Data")
	TObjectPtr<UExplosionProfile> ExplosionProfile;
	
	UFUNCTION(BlueprintCallable, Category="Explosion")
	void Explode();

private:
	UFUNCTION(NetMulticast, Reliable, Category="Explosion")
	void Multicast_PropagateEffects(const FVector InOwnerLocation);

	UFUNCTION(Server, Reliable, Category="Explosion")
	void Server_PropagateDamage(const FVector InOwnerLocation);

protected:
	UPROPERTY(Category="Info", BlueprintReadOnly, Transient)
	TWeakObjectPtr<AActor> ComponentOwner;
};
