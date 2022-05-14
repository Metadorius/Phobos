#include "LaserTrailClass.h"

#include <LaserDrawClass.h>
#include <EBolt.h>
#include <RadBeam.h>

#include <Utilities/TemplateDef.h>

// Draws LaserTrail if the conditions are suitable.
// Returns true if drawn, false otherwise.
bool LaserTrailClass::Update(CoordStruct location)
{
	bool result = false;

	if (!this->LastLocation.isset())
	{
		// The trail was just inited
		this->LastLocation = location;
	}
	else if (location.DistanceFrom(this->LastLocation.Get()) > this->Type->SegmentLength) // TODO reimplement IgnoreVertical properly?
	{
		if (this->Visible && (this->Type->IgnoreVertical ? (abs(location.X - this->LastLocation.Get().X) > 16 || abs(location.Y - this->LastLocation.Get().Y) > 16) : true))
		{
			// We spawn a new segment if the distance is long enough, the game will do the rest - Kerbiter
			if (this->Type->IsLaser)
			{
				LaserDrawClass* pLaser = GameCreate<LaserDrawClass>(
					this->LastLocation.Get(), location,
					this->CurrentColor, ColorStruct { 0, 0, 0 }, ColorStruct { 0, 0, 0 },
					this->Type->FadeDuration.Get());

				pLaser->Thickness = this->Type->Thickness;
				pLaser->IsHouseColor = true;
				pLaser->IsSupported = this->Type->IsIntense;
			}

			if (this->Type->IsElectric)
			{
				EBolt* pBolt = GameCreate<EBolt>();
				pBolt->Fire(location, this->LastLocation.Get(), 0);
			}

			if (this->Type->IsBeam)
			{
				// Type doesn't matter since we override the color
				RadBeam* pBeam = RadBeam::Allocate(RadBeamType::Temporal);
				pBeam->SetColor(this->CurrentColor);
				pBeam->SetCoordsSource(location);
				pBeam->SetCoordsTarget(this->LastLocation.Get());
				pBeam->Amplitude 
			}

			result = true;
		}

		this->LastLocation = location;
	}

	return result;
}

#pragma region Save/Load

template <typename T>
bool LaserTrailClass::Serialize(T& stm)
{
	return stm
		.Process(this->Type)
		.Process(this->Visible)
		.Process(this->FLH)
		.Process(this->IsOnTurret)
		.Process(this->CurrentColor)
		.Process(this->LastLocation)
		.Success();
};

bool LaserTrailClass::Load(PhobosStreamReader& stm, bool RegisterForChange)
{
	return Serialize(stm);
}

bool LaserTrailClass::Save(PhobosStreamWriter& stm) const
{
	return const_cast<LaserTrailClass*>(this)->Serialize(stm);
}

#pragma endregion