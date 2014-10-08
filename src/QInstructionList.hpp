#ifndef __QINSTRUCTIONLIST_HPP__
#define __QINSTRUCTIONLIST_HPP__

#include <QTableWidget>

class QInstructionList : public QTableWidget {
	Q_OBJECT

	public:
		explicit QInstructionList(QWidget* parent = 0);
		void mousePressEvent(QMouseEvent*);
		void mouseMoveEvent(QMouseEvent*);

        void selectInstruction(unsigned int idx);

    private:
        void _highlightInstruction(QTableWidgetItem* item);
};
#endif