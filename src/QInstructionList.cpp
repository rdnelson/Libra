#include "QInstructionList.hpp"
#include <QMouseEvent>

QInstructionList::QInstructionList(QWidget* parent) : QTableWidget(parent)
{

}

void QInstructionList::mousePressEvent(QMouseEvent* evt) {
	//Get the clicked index
	QModelIndex index = indexAt(evt->pos());

	//if nothing was clicked, just clear the selection
	if(index.isValid()) {
		//get the clicked item
		QTableWidgetItem* item = this->itemAt(evt->pos());

		//select the clicked index
		selectionModel()->select(index, QItemSelectionModel::ClearAndSelect | QItemSelectionModel::Rows);
		selectionModel()->setCurrentIndex(index, QItemSelectionModel::NoUpdate);

        _highlightInstruction(item);
	} else {
		clearSelection();
		selectionModel()->setCurrentIndex(QModelIndex(), QItemSelectionModel::Clear);
	}
}

void QInstructionList::mouseMoveEvent(QMouseEvent* evt) {
	//Get the clicked index
	QModelIndex index = indexAt(evt->pos());

	//if nothing was clicked, just clear the selection
	if(index.isValid()) {
		//get the clicked item
		QTableWidgetItem* item = this->itemAt(evt->pos());

		//select the clicked index
		selectionModel()->select(index, QItemSelectionModel::ClearAndSelect | QItemSelectionModel::Rows);
		selectionModel()->setCurrentIndex(index, QItemSelectionModel::NoUpdate);

        _highlightInstruction(item);
	} else {
		clearSelection();
		selectionModel()->setCurrentIndex(QModelIndex(), QItemSelectionModel::Clear);
	}
}

void QInstructionList::selectInstruction(unsigned int idx) {
    _highlightInstruction(this->item(idx, 0));
}

void QInstructionList::_highlightInstruction(QTableWidgetItem* item) {
		//if the item exists, check above and below it for any lines that have the same address
		QTableWidgetItem* tmp = 0;
		if(item) {
            if (item->backgroundColor() == Qt::yellow || item->backgroundColor() == Qt::green) {
                this->setStyleSheet("QTableWidget { selection-background-color: #00FF00; }");
            } else if (item->backgroundColor() == Qt::red || item->backgroundColor() == QColor::fromRgb(0x88, 0x44, 0xff)) {
                this->setStyleSheet("QTableWidget { selection-background-color: #8844FF; }");
            } else {
                this->setStyleSheet("QTableWidget { selection-background-color: #0080FF; }");
            }

			for(int i = row(item) - 1; i >= 0; i--) {
				if((tmp = this->item(i, 0)) && tmp->data(32).toInt() == item->data(32).toInt())  {
					selectionModel()->select(indexFromItem(tmp), QItemSelectionModel::Select | QItemSelectionModel::Rows);
				} else {
					break;
				}
			}
			for(int i = row(item) + 1; i < this->rowCount(); i++) {
				if((tmp = this->item(i,0)) && tmp->data(32).toInt() == item->data(32).toInt()) {
					selectionModel()->select(indexFromItem(tmp), QItemSelectionModel::Select | QItemSelectionModel::Rows);
				} else {
					break;
				}
			}
		}
}