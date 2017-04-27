#ifndef FILEWRITER_H
#define FILEWRITER_H

#include <string>
#include "Gesture.h"
#include "GestureClass.h"
#include "Project.h"

namespace Filewriter
{
	const std::string frameString{ "frame" };
	const std::string gestureString{ "gesture" };
	const std::string gestureClassString{ "gestclass" };
	const std::string projectGestureString{ "projgest" };
	const std::string actionString{ "action" };
	const std::string labelString{ "label" };

	const std::string gestureExtension{ ".gest" };
	const std::string gestureClassExtension{ ".gestclass" };
	const std::string projectExtension{ ".project" };

	void save(Gesture & gesture);
	void save(GestureClass & gestureClass);
	void save(Project & project);

	void write(std::string & fileName, std::string & data);
	void overwrite(std::string & fileName, std::string & data, bool endWithNewline = false);
}

#endif //FILEWRITER_H