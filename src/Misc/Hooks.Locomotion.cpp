#include <Phobos.h>

#include <Helpers/Macro.h>
#include <LocomotionClass.h>
#include <HouseClass.h>

void HandleLocomotorManipulationEnd(FootClass* pOwner)
{
    if (pOwner)
    {
        if (pOwner->IsAttackedByLocomotor || pOwner->IsLetGoByLocomotor)
        {
            pOwner->IsAttackedByLocomotor = false;
            pOwner->IsLetGoByLocomotor = false;
			pOwner->IsBeingManipulated = false;
			pOwner->IsCrashing = false;

            if (!pOwner->Owner->CurrentPlayer && pOwner->GetTechnoType()->ResourceGatherer)
                pOwner->QueueMission(Mission::Harvest, false);
        }

        pOwner->BeingManipulatedBy = nullptr;
        pOwner->ChronoWarpedByHouse = nullptr;
    }
}

DEFINE_HOOK(0x719EFB, TeleportLocomotionClass_EndPiggyback, 0x0)
{
	GET(FootClass*, pOwner, ECX);
	HandleLocomotorManipulationEnd(pOwner);
	return 0x719F0E;
}

// DEFINE_HOOK(0x70FF13, TechnoClass_StopInflictingLocomotor_HeightCheckSkip, 0x0)
// {
// 	R->EAX<int>(1);
// 	return 0x70FF1B;
// }