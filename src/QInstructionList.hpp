#ifndef __QINSTRUCTIONLIST_HPP__
#define __QINSTRUCTIONLIST_HPP__

#include <QTableWidget>

class QInstructionList : public QTableWidget {
	Q_OBJECT

	public:
		explicit QInstructionList(QWidget* parent = 0);
		void mousePressEvent(QMouseEvent*);
		void mouseMoveEvent(QMouseEvent*);
};
#endif