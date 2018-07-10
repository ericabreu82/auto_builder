#include <qapplication.h>

#include <ABMainWindow.h>


int main(int argc, char ** argv)
{
	QApplication a( argc, argv );
	
	ABMainWindow* mW = new ABMainWindow(NULL, "Auto Builder", true, Qt::WType_TopLevel/* | Qt::WStyle_StaysOnTop*/);
	
	a.setMainWidget(mW);
	a.connect( &a, SIGNAL( lastWindowClosed() ), &a, SLOT( quit() ) );
	
	mW->show();

	mW->init();
	
	return a.exec();

}