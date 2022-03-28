#pragma once

#include "ShooterPickup.h"
#include "ShooterPickup_Speed.generated.h"

class AShooterCharacter;

// A pickup object that replenishes character health
UCLASS(Abstract, Blueprintable)
class AShooterPickup_Speed : public AShooterPickup
{
	GENERATED_UCLASS_BODY()

		/** check if pawn can use this pickup */
		virtual bool CanBePickedUp(AShooterCharacter* TestPawn) const override;

protected:

	/** how much health does it give? */
	UPROPERTY(EditDefaultsOnly, Category = Pickup)
		float LastTime;

	/** give pickup */
	virtual void GivePickupTo(AShooterCharacter* Pawn) override;
};
