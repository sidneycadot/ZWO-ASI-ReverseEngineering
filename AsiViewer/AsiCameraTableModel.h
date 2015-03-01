
///////////////////////////
// AsiCameraTableModel.h //
///////////////////////////

#ifndef AsiCameraTableModel_h
#define AsiCameraTableModel_h

#include <QAbstractTableModel>

class AsiCameraTableModel : public QAbstractTableModel
{
    public:

        AsiCameraTableModel();
        virtual ~AsiCameraTableModel();

        virtual int rowCount(const QModelIndex & parent) const;
        virtual int columnCount(const QModelIndex & parent) const;
        virtual QVariant data(const QModelIndex & index, int role) const;
        virtual QVariant headerData(int section, Qt::Orientation orientation, int role) const;
};

#endif // AsiCameraTableModel_h
