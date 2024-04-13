// Replicated Explosion Plugin - Unreal Engine Plugin - (c) Wise Labs 2020-2024


#include "Core/ExplosionComponent.h"

#include "NiagaraFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Actor.h"
#include "Engine/World.h"
#include "Utils/Damage_Interface.h"

UExplosionComponent::UExplosionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	
	SetIsReplicatedByDefault(true);
	
}

void UExplosionComponent::BeginPlay()
{
	verify(IsValid(ExplosionProfile));
	
	Super::BeginPlay();

	ComponentOwner = CastChecked<AActor>(GetOwner());

	if (!IsValid(ExplosionProfile))
		UKismetSystemLibrary::PrintString(GetWorld(), "The explosion profile is empty");
}

void UExplosionComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                           FActorComponentTickFunction* ThisTickFunction)
{
	if (!ComponentOwner.IsValid()) return;
	if (!IsValid(ExplosionProfile)) return;

	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UExplosionComponent::Explode()
{
	if (!ComponentOwner.IsValid()) return;
	if (!IsValid(ExplosionProfile)) return;
	
	// ReSharper disable once CppTooWideScope
	bool DummyUnlocked = true;
	
	if (DummyUnlocked)
	{
		// ReSharper disable once CppAssignedValueIsNeverUsed
		DummyUnlocked = false;

		if (ExplosionProfile->CamShakeClass)
			UGameplayStatics::PlayWorldCameraShake(GetWorld(), ExplosionProfile->CamShakeClass,
			                                       GetOwner()->GetActorLocation(), 50.f, 2000.f, 1.f, false);
		
		if (UKismetSystemLibrary::IsStandalone(this) && GetOwner())
		{
			GetOwner()->MakeNoise(1.f, nullptr, GetOwner()->GetActorLocation(), ExplosionProfile->Radius * 5.f,
			                      ExplosionProfile->NoiseTag);
		}

		Server_PropagateDamage(GetOwner()->GetActorLocation());
		
		Multicast_PropagateEffects(GetOwner()->GetActorLocation());
	}
}

void UExplosionComponent::Multicast_PropagateEffects_Implementation(const FVector InOwnerLocation)
{
	if (!ComponentOwner.IsValid()) return;
	if (!IsValid(ExplosionProfile)) return;
	
	if (ExplosionProfile->CascadedParticle)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplosionProfile->CascadedParticle, InOwnerLocation,
		                                         FRotator(0.f, 0.f, 0.f), ExplosionProfile->EffectScale, true);
	}
	else if (ExplosionProfile->NiagaraParticle)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), ExplosionProfile->NiagaraParticle, InOwnerLocation,
		                                               FRotator(0.f, 0.f, 0.f), ExplosionProfile->EffectScale, true);
	}
		
	if (ExplosionProfile->Sound)
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), ExplosionProfile->Sound, InOwnerLocation,
		                                      FRotator(0.f, 0.f, 0.f), 1.f, 1.f, 0.f,
		                                      ExplosionProfile->Attenuation);
}

void UExplosionComponent::Server_PropagateDamage_Implementation(const FVector InOwnerLocation)
{
	if (!ComponentOwner.IsValid()) return;
	if (!IsValid(ExplosionProfile)) return;

	TArray<AActor*> IgnoredActors;
	IgnoredActors.Add(GetOwner() ? GetOwner() : nullptr);
	
	TArray<FHitResult> HitResults;

	// ReSharper disable once CppTooWideScope
	const bool bHit = UKismetSystemLibrary::SphereTraceMultiForObjects(GetWorld(), InOwnerLocation, InOwnerLocation,
	                                                                   ExplosionProfile->Radius,
	                                                                   ExplosionProfile->ObjectTypes, false,
	                                                                   IgnoredActors, EDrawDebugTrace::None, HitResults,
	                                                                   true);
	if (bHit)
	{
		TArray<AActor*> NearbyActors;
		
		for (const auto& ArrayElement : HitResults)
		{
			NearbyActors.AddUnique(ArrayElement.GetActor());
		}
		
		if (NearbyActors.Num() > 0)
			for (const auto& ArrayElement : NearbyActors)
			{
				IDamage_Interface::Execute_OnExplosionHit(ArrayElement,
				                                          FMath::FRandRange(
					                                          ExplosionProfile->DamageRange.X,
					                                          ExplosionProfile->DamageRange.Y), GetOwner());
			}
	}
}

