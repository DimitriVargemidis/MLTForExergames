#pragma once

class Abstr_UI_Hitbox;

namespace UI_Update_CallbackFunctions
{
	void standardUpdateFunction(Abstr_UI_Hitbox * hitbox);

	void stopRecord(Abstr_UI_Hitbox * hitbox);

	void RecordCounter(Abstr_UI_Hitbox * hitbox);

	void updateScrolbarGestures(Abstr_UI_Hitbox * hitbox);

	void updatePlayVisual(Abstr_UI_Hitbox * hitbox);


};

