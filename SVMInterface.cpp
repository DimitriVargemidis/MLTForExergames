#include "GestureClass.h"
#include "Gesture.h"
#include "Frame.h"
#include "Action.h"
#include "SVMInterface.h"

//Train the SVM model with a given set of gesture classes.
//Input: all gesture classes.
//Output: an SVM model that can be used to predict a posture
svm_model * SVMInterface::train(std::map<int, std::pair<std::shared_ptr<GestureClass>, std::vector<Action>>> & projectMap) {
	//Set parameters
	svm_parameter param;
	param.svm_type = C_SVC;
	param.kernel_type = RBF;
	param.degree = 3;
	param.gamma = 0.1;
	param.coef0 = 0;
	param.nu = 0.5;
	param.cache_size = 100;
	param.C = 10;
	param.eps = 1e-3;
	param.p = 0.1;
	param.shrinking = 1;
	param.probability = 0;
	param.nr_weight = 0;
	param.weight_label = NULL;
	param.weight = NULL;

	//Set problem
	svm_problem prob;

	//Get and set the problem size (= number of frames that were input)
	int problemSize{0};
	for (const auto & keyValue : projectMap)
	{
		for(const std::shared_ptr<Gesture> gesture : keyValue.second.first->getGestures())
		{
			problemSize = problemSize + gesture->getFrames().size();
		}
	}

	prob.l = problemSize;

	//Set labels and nodes
	double * labels = new double[problemSize];
	svm_node ** x = new svm_node*[problemSize];
	int rowCount{0};

	prepareLabels(projectMap);
	for (const auto & keyValue : projectMap)
	{
		for (const std::shared_ptr<Gesture> g : keyValue.second.first->getGestures())
		{
			for (const Frame & f : g->getFrames())
			{
				labels[rowCount] = f.getLabel();
				x[rowCount] = f.toArray();
				rowCount++;
			}
		}
	}

	prob.y = labels;
	prob.x = x;

	//Train model
	svm_model * model = svm_train(& prob, & param);
	svm_destroy_param(& param);
	
	return model;
}

//Predict which label belongs to the given frame.
//Input: the SVM model that is computed with train(...) and the frame of which should be know what label fits the best to it.
//Output: the label of a posture (more or less equal to a frame) that is the most similar to the given frame.
int SVMInterface::test(svm_model & model, Frame & frame)
{
	svm_node * testnode = frame.toArray();

	int resultLabel = svm_predict(& model, testnode);
	
	delete[] testnode;

	return resultLabel;
}

//This function is used within the train(...) function. No need to call this yourself. This function checks which labels
//overlap for frames that are equal and than makes sure the labels are equal if the frame is equal. The SVM model is
//created after this function is called, so the labels may not correspond to what you may expect depending on the
//given projectMap argument of train(projectMap).
//This function can be slightly optimized by eliminating unnecessary frame/label comparisons.
void SVMInterface::prepareLabels(std::map<int, std::pair<std::shared_ptr<GestureClass>, std::vector<Action>>> & projectMap)
{
	//Assign a label to each frame
	for (const auto & keyValue : projectMap)
	{
		for (std::shared_ptr<Gesture> g : keyValue.second.first->getGestures())
		{
			double fraction = 0;
			int counter = 0;
			int labelOffset = 0;
			for (Frame & f : g->getFrames())
			{
				if (!g->isPosture())
				{
					fraction = counter / ((double)g->getFrames().size());
					labelOffset = (int)(fraction / THRESHOLD_FRACTION);
					counter++;
				}
				f.setLabel(keyValue.first + labelOffset);
			}
		}
	}

	//Replace labels for frames that are equal so no two different labels refer to frames that are equal
	for (const auto & keyValue : projectMap)
	{
		std::vector<std::shared_ptr<Gesture>> gestures = keyValue.second.first->getGestures();
		int gesturesVectorSize = gestures.size();
		for (int x = 0; x < gesturesVectorSize; x++)
		{
			int gestureSize = gestures.at(x)->getFrames().size();
			int incrementVectorPosition = gestureSize / SVMInterface::NB_OF_LABEL_DIVISIONS;
			int offsetVectorPosition = incrementVectorPosition / 2;
			for (int i = 0; i < SVMInterface::NB_OF_LABEL_DIVISIONS; i++)
			{
				for (const auto & keyValue : projectMap)
				{
					std::vector<std::shared_ptr<Gesture>> gesturesOther = keyValue.second.first->getGestures();
					int gesturesVectorSizeOther = gesturesOther.size();
					for (int y = x; y < gesturesVectorSizeOther; y++)
					{
						int gestureSizeOther = gesturesOther.at(y)->getFrames().size();
						int incrementVectorPositionOther = gestureSizeOther / SVMInterface::NB_OF_LABEL_DIVISIONS;
						int offsetVectorPositionOther = incrementVectorPositionOther / 2;
						for (int j = 0; j < SVMInterface::NB_OF_LABEL_DIVISIONS; j++)
						{
							if (
								gestures.at(x)->getFrames().at(i*incrementVectorPosition + offsetVectorPosition).equals(
									gesturesOther.at(y)->getFrames().at(j*incrementVectorPositionOther + offsetVectorPositionOther)))
							{
								int labelToReplace = gesturesOther.at(y)->getFrames().at(j*incrementVectorPositionOther + offsetVectorPositionOther).getLabel();
								int labelToReplaceWith = gestures.at(x)->getFrames().at(i*incrementVectorPosition + offsetVectorPosition).getLabel();
								if (labelToReplace == labelToReplaceWith)
								{
									continue;
								}
								else
								{
									for (Frame & frame : gesturesOther.at(y)->getFrames())
									{
										if (frame.getLabel() == labelToReplace)
										{
											frame.setLabel(labelToReplaceWith);
										}
									}
								}
							}
						}
					}
				}
			}
		}
	}

	//Add all labels in the correct order to the gesture's ordered labels vector.
	for (const auto & keyValue : projectMap)
	{
		for (std::shared_ptr<Gesture> g : keyValue.second.first->getGestures())
		{
			int gestureSize = g->getFrames().size();
			int incrementVectorPosition = gestureSize / SVMInterface::NB_OF_LABEL_DIVISIONS;
			int offsetVectorPosition = incrementVectorPosition / 2;
			for (int i = 0; i < SVMInterface::NB_OF_LABEL_DIVISIONS; i++)
			{
				g->addLabel(g->getFrames().at(i*incrementVectorPosition + offsetVectorPosition).getLabel());
			}
		}
	}
}
