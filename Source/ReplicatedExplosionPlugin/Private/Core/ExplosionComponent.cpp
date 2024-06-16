// Replicated Explosion Plugin - Unreal Engine Plugin - (c) Wise Labs 2020-2024


#include "Core/ExplosionComponent.h"

#include "NiagaraFunctionLibrary.h"
#include "Data/ExplosionProfileData.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Actor.h"
#include "Engine/World.h"
#include "Utils/ExplosionInterface.h"

UExplosionComponent::UExplosionComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	
	SetIsReplicatedByDefault(true);
}

void UExplosionComponent::BeginPlay()
{
	verify(ExplosionProfile);
	
	Super::BeginPlay();

	ComponentOwner = CastChecked<AActor>(GetOwner());

	if (!ExplosionProfile)
	{
		UKismetSystemLibrary::PrintString(GetWorld(), "The explosion profile is empty/not set");
	}
}

void UExplosionComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                           FActorComponentTickFunction* ThisTickFunction)
{
	if (!ComponentOwner) return;
	if (!ExplosionProfile) return;

	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UExplosionComponent::Explode(const bool& ShouldAutoDestroy)
{
	if (!ComponentOwner) return;
	if (!ExplosionProfile) return;

	Server_Explode(ComponentOwner->GetActorLocation());
	Multicast_Explode(ComponentOwner->GetActorLocation());

	if (ShouldAutoDestroy)
	{
		ComponentOwner->Destroy(true);
	}
}

void UExplosionComponent::Multicast_Explode_Implementation(const FVector InOwnerLocation)
{
	if (!ComponentOwner) return;
	if (!IsValid(ExplosionProfile)) return;

	if (ExplosionProfile->ShouldUseCascadedAndNiagaraParticle && ExplosionProfile->CascadedParticle && ExplosionProfile->NiagaraParticle)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), ExplosionProfile->NiagaraParticle, InOwnerLocation, FRotator::ZeroRotator, ExplosionProfile->EffectScale, true);

		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplosionProfile->CascadedParticle, InOwnerLocation, FRotator::ZeroRotator, ExplosionProfile->EffectScale, true);
	}
	else if (ExplosionProfile->CascadedParticle)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplosionProfile->CascadedParticle, InOwnerLocation, FRotator::ZeroRotator, ExplosionProfile->EffectScale, true);
	}
	else if (ExplosionProfile->NiagaraParticle)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), ExplosionProfile->NiagaraParticle, InOwnerLocation, FRotator::ZeroRotator, ExplosionProfile->EffectScale, true);
	}
	
	if (ExplosionProfile->Sound)
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), ExplosionProfile->Sound, InOwnerLocation, FRotator::ZeroRotator, 1.f, 1.f, 0.f, ExplosionProfile->Attenuation);
	}
	
	if (ExplosionProfile->CamShakeClass)
	{
		UGameplayStatics::PlayWorldCameraShake(GetWorld(), ExplosionProfile->CamShakeClass, ComponentOwner->GetActorLocation(), ExplosionProfile->CameraShakeInnerRadius,  ExplosionProfile->Radius, ExplosionProfile->CameraShakeFalloff, false);
	}
}

void UExplosionComponent::Server_Explode_Implementation(const FVector InOwnerLocation)
{
	if (!ComponentOwner) return;
	if (!ExplosionProfile) return;

	if (FHitResult HitResult; UKismetSystemLibrary::SphereTraceSingleForObjects(GetWorld(), InOwnerLocation, InOwnerLocation,
	                                                                            ExplosionProfile->Radius,
	                                                                            ExplosionProfile->ObjectTypes, false,
	                                                                            IgnoreThisActorsInExplosion, EDrawDebugTrace::None, HitResult,
	                                                                            true))
	{
		TArray<AActor*> NearbyActors;
		
		NearbyActors.Add(HitResult.GetActor());
		
		if (NearbyActors.Num() > 0)
		{
			for (const auto& ArrayElement : NearbyActors)
			{
				if (ArrayElement->Implements<UExplosionInterface>())
				{
					IExplosionInterface::Execute_OnExplosionHit(ArrayElement, FMath::FRandRange(ExplosionProfile->DamageRange.X, ExplosionProfile->DamageRange.Y), ComponentOwner);
				}
			}
		}
	}
}

