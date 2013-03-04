#ifndef __QKBDINPUT_H__
#define __QKBDINPUT_H__

#include <QInputContext>

class QKbdFilter : public QObject
{
	Q_OBJECT
public:
	QKbdFilter(QObject *parent = 0);

	bool eventFilter(QObject* obj, QEvent* event);
signals:
	void KeyEvent(QKeyEvent* evt);

public slots:

private:
	bool mDisabled;
};

#endif // __QKBDINPUT_H__
