#include "Hexes/TileData.h"
#include "Misc/Crc.h"

#if UE_BUILD_DEBUG
uint32 GetTypeHash(const FLandmarkKey& Thing)
{
	uint32 Hash = FCrc::MemCrc32(&Thing, sizeof(FLandmarkKey));
	return Hash;
}
#endif