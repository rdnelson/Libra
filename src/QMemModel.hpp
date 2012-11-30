#ifndef QMEMMODEL_H
#define QMEMMODEL_H

#include <QAbstractTableModel>

class QMemModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    QMemModel(QObject *parent = 0);
    int rowCount(const QModelIndex &/*parent*/) const { return 4096; }
    int columnCount(const QModelIndex &/*parent*/) const { return 16; }
    QVariant data(const QModelIndex &index, int role) const;
    void setItem(int row, int col, char data, bool bUpdate = false);
    void update();
    bool setData(const QModelIndex &index, const QVariant &value, int role);
    void copyData(const unsigned char* data);
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
signals:
    
public slots:
    
private:
    char mData[0x10000];
    static const int mRows = 4096;
    static const int mColumns = 16;
    
};

#endif // QMEMMODEL_H
