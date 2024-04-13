// Replicated Explosion Plugin - Unreal Engine Plugin - (c) Wise Labs 2020-2024

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

class FReplicatedExplosionPluginModule : public IModuleInterface
{
public:

	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};
