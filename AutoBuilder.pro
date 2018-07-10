TEMPLATE	= vcapp
LANGUAGE	= C++

CONFIG	+= qt warn_on rtti exceptions debug

INCLUDEPATH	+= . source source/core

HEADERS	+= source/core/ABAbstractBuilder.h \
	source/core/ABCommomBuilder.h \
	source/core/ABTerraViewBuilder.h \
	source/core/ABSigDesktopBuilder.h \
	source/core/ABTerraAmazonBuilder.h

SOURCES	+= source/main.cpp \
	source/core/ABAbstractBuilder.cpp \
	source/core/ABCommomBuilder.cpp \
	source/core/ABTerraViewBuilder.cpp \
	source/core/ABSigDesktopBuilder.cpp \
	source/core/ABTerraAmazonBuilder.cpp

FORMS	= ui/ABMainWindow.ui \
	ui/ABAbout.ui

IMAGES	= images/Apply.png \
	images/AutoBuilder.png \
	images/Build.png \
	images/Error.png \
	images/Exit.png \
	images/Help.png \
	images/Item.png \
	images/Clock.png \
	images/Run.png \
	images/Remove.png \
	images/ABLogo.png \
	images/Monkey.png \
	images/Funcate.png \
	images/Sendmail-logo.png \
	images/Subversion-logo.png \
	images/VisualStudio-logo.png

TARGET = AutoBuilder

win32 {
		CONFIG += windows
        DEFINES += UNICODE
}

UI_DIR = source/.ui
MOC_DIR = source/.moc

CONFIG	+= qt warn_on rtti exceptions debug
