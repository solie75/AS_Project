#include "CoreMinimal.h"

// Bool
TAutoConsoleVariable<bool> CVarOffsetRootBoneEnabled(
	TEXT("AS.OffsetRootBone.Enable"),
	false,
	TEXT("Enable offset root bone"),
	ECVF_Default
);

TAutoConsoleVariable<bool> CVarThreadSafeAnimUpdate(
	TEXT("AS.ThreadSafeAnimationUpdate.Enable"),
	false,
	TEXT("Enable thread safe animation update"),
	ECVF_Default
);

TAutoConsoleVariable<bool> CVarDrawDebugShapes(
	TEXT("AS.DrawCharacterDebugShapes"),
	false,
	TEXT("Draw character debug shapes"),
	ECVF_Default
);

/*TAutoConsoleVariable<bool> CVarExperimentalStateMachine(
	TEXT("AS.ExperimentalStateMachine.Enable"),
	false,
	TEXT("Enable experimental state machine"),
	ECVF_Default
);*/

// Int
TAutoConsoleVariable<int32> CVarMMDatabaseLOD(
	TEXT("AS.MMDatabaseLOD"),
	0,
	TEXT("Motion Matching database LOD level"),
	ECVF_Default
);

TAutoConsoleVariable<int32> CVarLocomotionSetup(
	TEXT("AS.LocomotionSetupCMC"),
	0,
	TEXT("Locomotion setup mode"),
	ECVF_Default
);

// Float
TAutoConsoleVariable<float> CVarOffsetRootTranslationRadius(
	TEXT("AS.OffsetRootBone.TranslationRadius"),
	0.f,
	TEXT("Offset root bone translation radius"),
	ECVF_Default
);