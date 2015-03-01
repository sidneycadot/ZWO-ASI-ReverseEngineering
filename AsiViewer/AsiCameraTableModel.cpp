
/////////////////////////////
// AsiCameraTableModel.cpp //
/////////////////////////////

#include "AsiCameraTableModel.h"

AsiCameraTableModel::AsiCameraTableModel()
{
}

AsiCameraTableModel::~AsiCameraTableModel()
{
}

int AsiCameraTableModel::rowCount(const QModelIndex & parent) const
{
    return 5;
}

int AsiCameraTableModel::columnCount(const QModelIndex & parent) const
{
    return 12;
}

QVariant AsiCameraTableModel::data(const QModelIndex & index, int role) const
{
    switch (role)
    {
        case Qt::DisplayRole: return QString("data(%1,%2)").arg(index.column()).arg(index.row());
    }

    return QVariant();
}

QVariant AsiCameraTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    switch (orientation)
    {
        case Qt::Horizontal:
        {
            switch (role)
            {
                case Qt::DisplayRole:
                {
                    switch (section)
                    {
                        case  0: return "Name";
                        case  1: return "CameraID";
                        case  2: return "Resolution";
                        case  3: return "Pixel Size [um]";
                        case  4: return "Color";
                        case  5: return "Bayer Pattern";
                        case  6: return "Supported Bins";
                        case  7: return "Supported Video Formats";
                        case  8: return "Mechanical Shutter";
                        case  9: return "ST4 Port";
                        case 10: return "Cooler";
                        case 11: return "USB3";
                    }
                    break;
                }
            }
            break;
        }

        case Qt::Vertical:
        {
            switch (role)
            {
                case Qt::DisplayRole:
                {
                    return QString("%1").arg(section);
                }
                break;
            }
            break;
        }
    }

    return QVariant();
}
