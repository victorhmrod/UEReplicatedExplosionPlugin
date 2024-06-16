// Replicated Explosion Plugin - Unreal Engine Plugin - (c) Wise Labs 2020-2024

#include "Core/ExplosionComponent.h"

#include "NiagaraFunctionLibrary.h"
#include "Data/ExplosionProfileData.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"
#include "Kismet/GameplayStatics.h"
#include "Utils/ExplosionInterface.h"

UExplosionComponent::UExplosionComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	
	SetIsReplicatedByDefault(true);
}

void UExplosionComponent::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
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

void UExplosionComponent::Explode(const bool ShouldAutoDestroy)
{
	if (!ComponentOwner) return;
	if (!ExplosionProfile) return;

	Server_Explode();
	Multicast_Explode();

	if (ShouldAutoDestroy)
	{
		ComponentOwner->Destroy(true);
	}
}
void UExplosionComponent::Multicast_Explode_Implementation()
{
	if (!ComponentOwner) return;
	if (!ExplosionProfile) return;

	if (ExplosionProfile->ShouldUseCascadedAndNiagaraParticle && ExplosionProfile->CascadedParticle && ExplosionProfile->NiagaraParticle)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), ExplosionProfile->NiagaraParticle, ComponentOwner->GetActorLocation(), FRotator::ZeroRotator, ExplosionProfile->EffectScale, true);

		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplosionProfile->CascadedParticle, ComponentOwner->GetActorLocation(), FRotator::ZeroRotator, ExplosionProfile->EffectScale, true);
	}
	else if (ExplosionProfile->CascadedParticle)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplosionProfile->CascadedParticle, ComponentOwner->GetActorLocation(), FRotator::ZeroRotator, ExplosionProfile->EffectScale, true);
	}
	else if (ExplosionProfile->NiagaraParticle)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), ExplosionProfile->NiagaraParticle, ComponentOwner->GetActorLocation(), FRotator::ZeroRotator, ExplosionProfile->EffectScale, true);
	}
	
	if (ExplosionProfile->Sound)
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), ExplosionProfile->Sound, ComponentOwner->GetActorLocation(), FRotator::ZeroRotator, 1.f, 1.f, 0.f, ExplosionProfile->Attenuation);
	}
	
	if (ExplosionProfile->CamShakeClass)
	{
		UGameplayStatics::PlayWorldCameraShake(GetWorld(), ExplosionProfile->CamShakeClass, ComponentOwner->GetActorLocation(), ExplosionProfile->CameraShakeInnerRadius,  ExplosionProfile->Radius, ExplosionProfile->CameraShakeFalloff, false);
	}
}

void UExplosionComponent::Server_Explode_Implementation()
{
	if (!ComponentOwner) return;
	if (!ExplosionProfile) return;

	if (TArray<FHitResult> HitResult; UKismetSystemLibrary::SphereTraceMultiForObjects(GetWorld(), ComponentOwner->GetActorLocation(), ComponentOwner->GetActorLocation(),
	                                                                            ExplosionProfile->Radius,
	                                                                            ExplosionProfile->ObjectTypes, false,
	                                                                            ExplosionProfile->IgnoreThisActorsInExplosion, EDrawDebugTrace::None, HitResult,
	                                                                            true))
	{
		TArray<AActor*> NearbyActors;

		if (HitResult.Num() > 0)
		{
			for (const auto& HitElement : HitResult)
			{
				NearbyActors.Add(HitElement.GetActor());
			}
		}

		if (NearbyActors.Num() > 0)
		{
			for (const auto& NearbyElements : NearbyActors)
			{
				if (NearbyElements->Implements<UExplosionInterface>())
				{
					IExplosionInterface::Execute_OnExplosionHit(NearbyElements, FMath::FRandRange(ExplosionProfile->DamageRange.X, ExplosionProfile->DamageRange.Y), ComponentOwner);
				}
			}
		}

		NearbyActors.Empty();
	}
}

