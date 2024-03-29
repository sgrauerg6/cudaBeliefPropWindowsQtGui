#include "QtGuiApplication1.h"

QtGuiApplication1::QtGuiApplication1(QWidget *parent)
	: QMainWindow(parent), stereo_processing_run_(false)
{
	ui.setupUi(this);
	QPixmap newImage(DEFAULT_REF_IMAGE_PATH);
	ui.targetImage->setPixmap(newImage);
	connect(ui.pushButton_2, &QPushButton::clicked, this, &QtGuiApplication1::processButtonPress);
}

//funtion to process button press in GUI
void QtGuiApplication1::processButtonPress()
{
	QObject* button = QObject::sender();
	if (button == ui.pushButton_2)
	{
		std::string updatedImageFilePath;
		std::string updatedLabelText;
		std::string updatedButtonText;

		if (!stereo_processing_run_)
		{
			setCursor(Qt::WaitCursor);

			//check which radio button is checked to determine implementation to run
			bpImplementation implementationToRun = bpImplementation::NAIVE_CPU;
			if (ui.radioButton->isChecked())
			{
				implementationToRun = bpImplementation::NAIVE_CPU;
			}
			else if (ui.radioButton_2->isChecked())
			{
				implementationToRun = bpImplementation::OPTIMIZED_CPU;
			}
			else
			{
				implementationToRun = bpImplementation::OPTIMIZED_CUDA;
			}

			//process stereo set; function returns std::pair with runtime first and file path of output disparity map second
			auto bpRuntimeAndOutputDispMap = GuiProcessStereoSet::processStereoSet(implementationToRun);
			setCursor(Qt::ArrowCursor);

			updatedImageFilePath = bpRuntimeAndOutputDispMap.second;
			updatedButtonText = "Reset";
			updatedLabelText = "Computed Disparity Map (Runtime: " + std::to_string(bpRuntimeAndOutputDispMap.first) + " )";
		}
		else
		{
			updatedImageFilePath = DEFAULT_REF_IMAGE_PATH;
			updatedButtonText = "Run Stereo Processing";
			updatedLabelText = "Reference Image of Stereo Set";
		}

		//switch to opposite of current setting for whether or not stereo processing run
		stereo_processing_run_ = !stereo_processing_run_;

		//update GUI reflecting changes from button press
		QPixmap newImage(updatedImageFilePath.c_str());
		ui.targetImage->setPixmap(newImage);
		ui.pushButton_2->setText(updatedButtonText.c_str());
		ui.label_2->setText(updatedLabelText.c_str());

		repaint();
	}
}
