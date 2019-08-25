#include "QtGuiApplication1.h"

QtGuiApplication1::QtGuiApplication1(QWidget *parent)
	: QMainWindow(parent)
{
	stereo_processing_run_ = false;
	ui.setupUi(this);
	QPixmap newImage(DEFAULT_REF_IMAGE_PATH);
	ui.targetImage->setPixmap(newImage);
	connect(ui.pushButton_2, &QPushButton::clicked, this, &QtGuiApplication1::processButtonPress);
}