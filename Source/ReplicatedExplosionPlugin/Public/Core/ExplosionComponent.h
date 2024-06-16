// Replicated Explosion Plugin - Unreal Engine Plugin - (c) Wise Labs 2020-2024

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ExplosionComponent.generated.h"

UCLASS(ClassGroup=(EC), meta=(BlueprintSpawnableComponent))
class REPLICATEDEXPLOSIONPLUGIN_API UExplosionComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UExplosionComponent();

	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

protected:
	virtual void BeginPlay() override;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	

	// Explode the owner of this component!
	UFUNCTION(BlueprintCallable, Category="Explosion")
	void Explode(const bool ShouldAutoDestroy);

private:	
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_Explode();

	UFUNCTION(Server, Reliable)
	void Server_Explode();

	UPROPERTY()
	AActor* ComponentOwner;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Data")
	class UExplosionProfile* ExplosionProfile;
};
