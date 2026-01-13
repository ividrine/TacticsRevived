#include "GameplayTags/DedicatedServersTags.h"

namespace DedicatedServersTags
{
	namespace TacticsWebAPI
	{
		UE_DEFINE_GAMEPLAY_TAG_COMMENT(Fleets, "DedicatedServersTags.TacticsWebAPI.Fleets", "List fleets resource on Tactics Web API");

		UE_DEFINE_GAMEPLAY_TAG_COMMENT(Login, "DedicatedServersTags.TacticsWebAPI.Login", "Retrieves Access Token, Id Token, and Refresh Token from API");
		UE_DEFINE_GAMEPLAY_TAG_COMMENT(Logout, "DedicatedServersTags.TacticsWebAPI.Logout", "Signs user out and invalidates tokens from API");
	}
}
