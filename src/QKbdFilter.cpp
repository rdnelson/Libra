#include <QMessageBox>
#include "QKbdFilter.hpp"

QKbdFilter::QKbdFilter(QObject* parent) : QObject(parent), mDisabled(false) {

}

bool QKbdFilter::eventFilter(QObject*, QEvent* event) {
	if(event->type() == QEvent::KeyPress) {
		QKeyEvent* kbdEvent = static_cast<QKeyEvent*>(event);
		if(kbdEvent->modifiers() & Qt::ControlModifier) {
			return false;
		}
		if(kbdEvent->key() < ' ' || kbdEvent->key() > '~')
			return false;
		emit KeyEvent(kbdEvent);
		return true;
	}
	return false;
}
