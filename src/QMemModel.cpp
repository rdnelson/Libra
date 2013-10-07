#include "QMemModel.hpp"

QMemModel::QMemModel(QObject *parent) :
	QAbstractTableModel(parent),
	mEditable(false),
	mDirty(false)
{ }

QVariant QMemModel::data(const QModelIndex &index, int role) const {

	//handle out of bounds
	if(index.row() >= mRows)
		return QVariant();
	if(index.column() >= mColumns)
		return QVariant();

	if(role == Qt::DisplayRole) {
		return QString("%1").arg((int)(mData[index.row() * 0x10 + index.column()]) & 0xFF, 2, 16, QChar('0')).toUpper();
	} else if(role == Qt::BackgroundColorRole) {
		if(mHighlight.find(index.row() * 0x10 + index.column()) == mHighlight.end()) {
			return QVariant();
		} else {
			return (*mHighlight.find(index.row() * 0x10 + index.column())).second;
		}
	} else if(role == Qt::UserRole) {
		return mData[index.row() * 0x10 + index.column()] & 0xFF;
	}
	return QVariant();
}

QVariant QMemModel::headerData(int section, Qt::Orientation orientation, int role) const {
	if (role == Qt::DisplayRole) {
		if(orientation == Qt::Horizontal) {
			return QString("%1").arg(section, 0, 16).toUpper();
		} else if(orientation == Qt::Vertical) {
			return QString(QString("%1").arg(section * 0x10, 4, 16, QChar('0')).toUpper());
		}
	}
	return QVariant();
}

bool QMemModel::setData(const QModelIndex &index, const QVariant &value, int role) {
	if(index.row() >= mRows)
		return false;
	if(index.column() >= mColumns)
		return false;

	if(role == Qt::EditRole) {
		if (_validHexText(value.toString(), 2)) {
			mData[index.row() * 0x10 + index.column()] = (unsigned char)(_htoi(value.toString()) & 0xFF);
			mDirty = true;
			emit dataChanged(index, index);
		} else {
			return false;
		}
	}
	return true;
}

void QMemModel::update() {
	emit dataChanged(this->createIndex(0,0), this->createIndex(mRows - 1, mColumns - 1));
}

void QMemModel::setItem(int row, int col, char data, bool bUpdate) {
	if(row >= mRows)
		return;
	if(col >= mColumns)
		return;
	mData[row*0x10 + col] = data;
	if(bUpdate)
		update();
}

void QMemModel::setDataPtr(const unsigned char *data) {
	mData = (unsigned char*)data;
}

void QMemModel::Highlight(const int addr, const int len, const QColor color) {
	for(int i = 0; i < len; i++) {
		mHighlight[(addr + i) % (mRows * 0x10)] = color;
	}
}

Qt::ItemFlags QMemModel::flags(const QModelIndex&) const {
	Qt::ItemFlags retVal = Qt::ItemIsEnabled | Qt::ItemIsSelectable;
	if(mEditable)
		retVal |= Qt::ItemIsEditable;

	return retVal;
}

unsigned int QMemModel::_htoi(const QString& text) {
	unsigned int retVal = 0;

	for (int i = 0; i < text.length(); i++) {
		char lower = text.at(i).toLower().toAscii();
		if(text.at(i).isDigit()) {
			retVal += (text.at(i).toAscii() - '0') << ((text.length() - i - 1) * 4);
		} else if (lower >= 'a' && lower <= 'f') {
			retVal += (lower - 'a' + 10) << ((text.length() - i - 1) * 4);
		}
	}
	return retVal;
}

bool QMemModel::_validHexText(const QString& text, unsigned int maxlen) {
	if ((unsigned int)text.length() > maxlen || text.length() <= 0)  // 5 characters exceeds bounds
		return false;

	for (int i = 0; i < text.length(); i++) {
		if (!text.at(i).isDigit() && !(text.at(i).toLower().toAscii() >= 'a' && text.at(i).toLower().toAscii() <= 'f'))
			return false;
	}
	return true;
}
