#include "QKbdFilter.hpp"

QKbdFilter::QKbdFilter(QObject* parent) : QObject(parent), mDisabled(false) {

}

bool QKbdFilter::eventFilter(QObject*, QEvent* event) {
	if(event->type() == QEvent::KeyPress) {
		QKeyEvent* kbdEvent = static_cast<QKeyEvent*>(event);
		if(kbdEvent->modifiers() & Qt::ControlModifier) {
			return false;
		}
		char keyPress =	kbdEvent->key();
		if((keyPress < ' ' || keyPress > '~') && (keyPress != 0x04)) //Allow printable charachters, and enter
			return false;
		emit KeyEvent(kbdEvent);
		return true;
	}
	return false;
}
