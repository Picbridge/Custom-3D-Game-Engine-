#include "pch.h"
#include "ParticleComponent.h"
#include "ParticleManager.h"

ParticleComponent::ParticleComponent()
{
	defineMember();
}

ParticleComponent::~ParticleComponent()
{
}

void ParticleComponent::Init()
{
}

void ParticleComponent::Update()
{
	if (toggle)
		SERVICE_LOCATOR.GetParticleManager()->Emit(this);
}

void ParticleComponent::Shutdown()
{
}
