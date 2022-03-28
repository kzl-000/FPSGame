#include "ShooterGame.h"
#include "Pickups/ShooterPickup_Speed.h"
#include "OnlineSubsystemUtils.h"
#include "Player/ShooterCharacter.h"
#include "Weapons/ShooterWeapon.h"

AShooterPickup_Speed::AShooterPickup_Speed(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	LastTime = 20;
}

bool AShooterPickup_Speed::CanBePickedUp(class AShooterCharacter* TestPawn) const
{
	return true;
}

void AShooterPickup_Speed::GivePickupTo(class AShooterCharacter* Pawn)
{
	if (Pawn)
	{
		Pawn->SetRunningSpeedModifier(20);
		AShooterPlayerController* PC = Cast<AShooterPlayerController>(Pawn->Controller);
		if (PC)
		{
			PC->SetGodMode(true);
		}
		//Pawn->SetActorHiddenInGame(true)
		//if (Pawn->GetWeapon())
		//{
		//	Pawn->GetWeapon()->SetActorHiddenInGame(true);
		//}
		// Fire event for collected health
		const UWorld* World = GetWorld();
		const IOnlineEventsPtr Events = Online::GetEventsInterface(World);
		const IOnlineIdentityPtr Identity = Online::GetIdentityInterface(World);

		if (Events.IsValid() && Identity.IsValid())
		{
			AShooterPlayerController* PC = Cast<AShooterPlayerController>(Pawn->Controller);
			if (PC)
			{
				ULocalPlayer* LocalPlayer = Cast<ULocalPlayer>(PC->Player);

				if (LocalPlayer)
				{
					const int32 UserIndex = LocalPlayer->GetControllerId();
					TSharedPtr<const FUniqueNetId> UniqueID = Identity->GetUniquePlayerId(UserIndex);
					if (UniqueID.IsValid())
					{
						FVector Location = Pawn->GetActorLocation();

						FOnlineEventParms Params;

						Params.Add(TEXT("SectionId"), FVariantData((int32)0)); // unused
						Params.Add(TEXT("GameplayModeId"), FVariantData((int32)1)); // @todo determine game mode (ffa v tdm)
						Params.Add(TEXT("DifficultyLevelId"), FVariantData((int32)0)); // unused

						Params.Add(TEXT("ItemId"), FVariantData((int32)0)); // @todo come up with a better way to determine item id, currently health is 0 and ammo counts from 1
						Params.Add(TEXT("AcquisitionMethodId"), FVariantData((int32)0)); // unused
						Params.Add(TEXT("LocationX"), FVariantData(Location.X));
						Params.Add(TEXT("LocationY"), FVariantData(Location.Y));
						Params.Add(TEXT("LocationZ"), FVariantData(Location.Z));

						Events->TriggerEvent(*UniqueID, TEXT("CollectPowerup"), Params);
					}
				}
			}
		}
	}
}
