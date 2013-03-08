#ifndef QMEMMODEL_H
#define QMEMMODEL_H

#include <QAbstractTableModel>
#include <QColor>
#include <map>

class QMemModel : public QAbstractTableModel
{
	Q_OBJECT
public:
	QMemModel(QObject *parent = 0);
	int rowCount(const QModelIndex &/*parent*/) const { return mRows; }
	int columnCount(const QModelIndex &/*parent*/) const { return mColumns; }
	QVariant data(const QModelIndex &index, int role) const;
	void setItem(int row, int col, char data, bool bUpdate = false);
	void update();
	bool setData(const QModelIndex &index, const QVariant &value, int role);
	void setDataPtr(const unsigned char* data);
	QVariant headerData(int section, Qt::Orientation orientation, int role) const;
	void Highlight(const int addr, const int len, const QColor color);
	void ClearHighlights() { mHighlight.clear(); }
signals:

public slots:

private:
	unsigned char* mData;
	std::map<int,QColor> mHighlight;
	static const int mRows = 4096;
	static const int mColumns = 16;

};

#endif // QMEMMODEL_H
