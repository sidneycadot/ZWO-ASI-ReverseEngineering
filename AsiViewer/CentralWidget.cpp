
///////////////////////
// CentralWidget.cpp //
///////////////////////

#include <QDebug>
#include <QHBoxLayout>
#include <QLabel>
#include <QTableView>
#include <QGroupBox>

#include "CentralWidget.h"
#include "AsiCameraTableModel.h"

CentralWidget::CentralWidget()
{
    qDebug() << __PRETTY_FUNCTION__;

    label = new QLabel("placeholder");

    //QLabel * description = new QLabel("This table shows the information of the ASI cameras connected to the system.");

    //QTableView * tableview = new QTableView();
    //QVBoxLayout * groupbox_layout = new QVBoxLayout();
    //groupbox_layout->addWidget(description);
    //groupbox_layout->addWidget(tableview);

    //QGroupBox * groupbox = new QGroupBox();
    //groupbox->setLayout(groupbox_layout);

    QHBoxLayout * layout = new QHBoxLayout();
    layout->addWidget(label);
    setLayout(layout);

    //AsiCameraTableModel * model = new AsiCameraTableModel();
    //tableview->setModel(model);

    asiThread = new AsiThread(this);

    bool connectOk = QObject::connect(asiThread, SIGNAL(receivedImage(const QByteArray &)), this, SLOT(updateImage(const QByteArray &)));
    Q_ASSERT(connectOk);

    asiThread->start();
}

CentralWidget::~CentralWidget()
{
    qDebug() << __PRETTY_FUNCTION__;
}

void CentralWidget::updateImage(const QByteArray & imageBytes)
{
    qDebug() << __PRETTY_FUNCTION__ << imageBytes.size();

    QImage image(1280, 960, QImage::Format_RGB888);

    const unsigned char * data = reinterpret_cast<const unsigned char *>(imageBytes.constData());

    int min_value = 65535;
    int max_value = 0;

    for (int r = 0; r < 960; ++r)
    {
        for (int c = 0; c < 1280; ++c)
        {
            int hi = data[(r * 1280 + c) * 2 + 1];
            int lo = data[(r * 1280 + c) * 2 + 0];

            int value = 256 * hi + lo;

            min_value = std::min(value, min_value);
            max_value = std::max(value, max_value);
        }
    }

    qDebug() << min_value << max_value;

    for (int r = 0; r < 960; ++r)
    {
        unsigned char * scanline = image.scanLine(r);

        for (int c = 0; c < 1280; ++c)
        {
            int hi = data[(r * 1280 + c) * 2 + 1];
            int lo = data[(r * 1280 + c) * 2 + 0];

            int value = 256 * hi + lo;

            double v = double(value - min_value) / (max_value - min_value);

            int bvalue = round(v * 255.0);

            scanline[3 *c + 0] = bvalue;
            scanline[3 *c + 1] = bvalue;
            scanline[3 *c + 2] = bvalue;
        }
    }

    label->setPixmap(QPixmap::fromImage(image));
}
