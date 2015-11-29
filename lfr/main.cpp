#include <QApplication>
#include "LfWorld.h"
#include "LfWidget.h"
#include <iostream>

using namespace lfr;

int main(int argc, char *argv[])
{
#if 1
	LfParameter param(argc, argv);
	
	if(!param.isValid()) {
		LfParameter::PrintHelp();
		return 1;
	}
	
	LfWorld world(&param);
	
    QApplication app(argc, argv);
    LfWidget widget(&world);
    widget.show();
    return app.exec();
#else
	LfWorld::testLAR();
    return 1;
#endif
}