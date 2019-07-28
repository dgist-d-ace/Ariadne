
#include "DataContainer.h"
#include <iostream>

class Driving : public QObject
{
	Q_OBJECT
protected:

public:
	DataContainer *dataContainer;
	Driving();

public slots:
	void Basic();

	void Mission1();

};

