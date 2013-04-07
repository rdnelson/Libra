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
		QTableWidgetItem* tmp = 0;

		//select the clicked index
		selectionModel()->select(index, QItemSelectionModel::ClearAndSelect | QItemSelectionModel::Rows);
		selectionModel()->setCurrentIndex(index, QItemSelectionModel::NoUpdate);

		//if the item exists, check above and below it for any lines that have the same address
		if(item) {
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
		QTableWidgetItem* tmp = 0;

		//select the clicked index
		selectionModel()->select(index, QItemSelectionModel::ClearAndSelect | QItemSelectionModel::Rows);
		selectionModel()->setCurrentIndex(index, QItemSelectionModel::NoUpdate);

		//if the item exists, check above and below it for any lines that have the same address
		if(item) {
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
	} else {
		clearSelection();
		selectionModel()->setCurrentIndex(QModelIndex(), QItemSelectionModel::Clear);
	}
}