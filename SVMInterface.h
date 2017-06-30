#ifndef SVMINTERFACE_H
#define SVMINTERFACE_H

#include <vector>
#include <map>
#include "libsvm-3.21\svm.h"

#include "GestureClass.h"
#include "Frame.h"


namespace SVMInterface {

	//The number of postures the gesture is split in. Just changing this number is enough. Everything else changes dynamically.
	const int		NB_OF_LABEL_DIVISIONS{ 4 };
	
	//The inverse number of postures the gesture is split in. This is only used to boost efficiency so it's not necessary to
	//recalculate this number every time.
	const double	THRESHOLD_FRACTION{ 1.0 / NB_OF_LABEL_DIVISIONS };

	//Train the SVM model with a given set of gesture classes.
	//Input: all gesture classes.
	//Output: an SVM model that can be used to predict a posture
	svm_model * train(std::map<int, std::pair<std::shared_ptr<GestureClass>, std::vector<Action>>> & projectMap);
	
	//Predict which label belongs to the given frame.
	//Input: the SVM model that is computed with train(...) and the frame of which should be know what label fits the best to it.
	//Output: the label of a posture (more or less equal to a frame) that is the most similar to the given frame.
	int test(svm_model & model, Frame & frame);
	
	//This function is used within the train(...) function. No need to call this yourself. This function checks which labels
	//overlap for frames that are equal and than makes sure the labels are equal if the frame is equal. The SVM model is
	//created after this function is called, so the labels may not correspond to what you may expect depending on the
	//given projectMap argument of train(projectMap).
	//This function can be slightly optimized by eliminating unnecessary frame/label comparisons.
	void prepareLabels(std::map<int, std::pair<std::shared_ptr<GestureClass>, std::vector<Action>>> & projectMap);
};

#endif //SVMINTERFACE_H