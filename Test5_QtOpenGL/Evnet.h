#pragma once

#include <QEvent>

class Evnet : public QEvent
{
public:
	Evnet();
	~Evnet();

	static const QEvent::Type type_;
};
