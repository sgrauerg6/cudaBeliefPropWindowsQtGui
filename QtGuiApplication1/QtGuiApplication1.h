#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_QtGuiApplication1.h"
#include <QPixmap>
#include <omp.h>

//needed for the current BP parameters for the costs and also the CUDA parameters such as thread block size
#include "bpStereoParameters.h"
#include "SingleThreadCPU/stereo.h"

//needed for functions to load input images/store resulting disp/movement image
#include "imageHelpers.h"

//needed to evaluate the disparity/Stereo found
#include "stereoResultsEval.h"

//needed to run the optimized implementation a stereo set using CPU
#include "OptimizeCPU/RunBpStereoOptimizedCPU.h"
#include "OptimizeCUDA/RunBpStereoSetOnGPUWithCUDA.h"

class QtGuiApplication1 : public QMainWindow
{
	Q_OBJECT

public:
	QtGuiApplication1(QWidget *parent = Q_NULLPTR);

	BPsettings initializeAndReturnBPSettings()
	{
		BPsettings startBPSettings;

		startBPSettings.smoothingSigma = SIGMA_BP;
		startBPSettings.numLevels = LEVELS_BP;
		startBPSettings.numIterations = ITER_BP;
		startBPSettings.lambda_bp = LAMBDA_BP;
		startBPSettings.data_k_bp = DATA_K_BP;
		startBPSettings.disc_k_bp = DISC_K_BP;

		//height/width determined when image read from file
		startBPSettings.widthImages = 0;
		startBPSettings.heightImages = 0;

		return startBPSettings;
	}

	void processButtonPress()
	{
		QObject* button = QObject::sender();
		if (button == ui.pushButton_2)
		{
			if (!stereo_processing_run_)
			{
				stereo_processing_run_ = true;
				float runTime = 0.0f;
				FILE* resultsFile = fopen("output.txt", "w");
				this->setCursor(Qt::WaitCursor);
				RunBpStereoSet<float>* runBp;
				if (ui.radioButton->isChecked())
				{
					runBp = new RunBpStereoCPUSingleThread<float>();
				}
				else if (ui.radioButton_2->isChecked())
				{
					runBp = new RunBpStereoOptimizedCPU<float>();
					omp_set_dynamic(1);
					omp_set_num_threads(4);
				}
				else
				{
					runBp = new RunBpStereoSetOnGPUWithCUDA<float>();
				}

				runTime = (*runBp)(DEFAULT_REF_IMAGE_PATH, DEFAULT_TEST_IMAGE_PATH, initializeAndReturnBPSettings(), SAVE_DISPARITY_IMAGE_PATH_1, resultsFile);
				delete runBp;
				this->setCursor(Qt::ArrowCursor);

				QPixmap newImage(SAVE_DISPARITY_IMAGE_PATH_1);
				ui.targetImage->setPixmap(newImage);
				ui.pushButton_2->setText("Reset");
				std::string labelString = "Computed Disparity Map (Runtime: " + std::to_string(runTime) + " )";
				ui.label_2->setText(labelString.c_str());
				repaint();
			}
			else
			{
				stereo_processing_run_ = false;
				QPixmap newImage(DEFAULT_REF_IMAGE_PATH);
				ui.targetImage->setPixmap(newImage);
				ui.pushButton_2->setText("Run Stereo Processing");
				std::string labelString = "Reference Image of Stereo Set";
				ui.label_2->setText(labelString.c_str());
				repaint();
			}
		}
	}

private:
	Ui::QtGuiApplication1Class ui;
	bool stereo_processing_run_;
};
