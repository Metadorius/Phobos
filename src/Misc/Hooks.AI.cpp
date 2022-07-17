#include <New/Entity/AttachmentClass.h>

#include <Utilities/Macro.h>

DEFINE_HOOK(0x55B6B3, LogicClass_AI_After, 0x5)
{
	for (auto const& pAttachment : AttachmentClass::Array)
	{
		pAttachment->AI();
		pAttachment->SkipNextUpdate = false;
	}

	return 0x0;
}
