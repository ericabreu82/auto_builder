/****************************************************************************
** ui.h extension file, included from the uic-generated form implementation.
**
** If you wish to add, delete or rename functions or slots use
** Qt Designer which will update this file, preserving your code. Create an
** init() function in place of a constructor, and a destroy() function in
** place of a destructor.
*****************************************************************************/

#include <qfiledialog.h>
#include <qmessagebox.h>
#include <QApplication.h>

#include <ABAbstractBuilder.h>
#include <ABTerraViewBuilder.h>
#include <ABSigDesktopBuilder.h>
#include <ABTerraAmazonBuilder.h>

#include <ABAbout.h>

void ABMainWindow::init()
{
//clear and insert the applications names that can be build
	appNameComboBox->clear();
	appNameComboBox->insertItem("");

	std::vector<std::string> vec = ABAbstractBuilder::getSupportedApplicationsNames();

	for(unsigned int i = 0; i < vec.size(); ++i)
	{
		appNameComboBox->insertItem(vec[i].c_str());
	}

	msvc2003LineEdit->setText("C:/Arquivos de programas/Microsoft Visual Studio .NET 2003/Common7/IDE/");
	msvc2005LineEdit->setText("C:/Arquivos de programas/Microsoft Visual Studio 8/Common7/IDE/");
	
}


void ABMainWindow::folderPushButton_clicked()
{

}


void ABMainWindow::buildPushButton_clicked()
{
//check input parameters
	if(appNameComboBox->currentText().isEmpty())
	{
		QMessageBox::warning(this, tr("Warning"), tr("Application not defined to build."));
		return;
	}

	if(msvc2003LineEdit->text().isEmpty())
	{
		QMessageBox::warning(this, tr("Warning"), tr(" MSVC 2003 application not defined."));
		return;
	}

	if(msvc2005LineEdit->text().isEmpty())
	{
		QMessageBox::warning(this, tr("Warning"), tr(" MSVC 2005 application not defined."));
		return;
	}

	widgetStack->raiseWidget(2);

	QMessageBox::information(this, tr("Information"), tr("FUNCATE thanks for using our products. Enjoy the coffee!!"));

//build application
	buildApp();
}

void ABMainWindow::msvc2003PushButton_clicked()
{
	QString s = QFileDialog::getExistingDirectory("", this, "open file dialog", "Select Visual Studio 2003 IDE directory");

	if(s.isEmpty())
	{
		msvc2003LineEdit->setText("");
	}
	else
	{
		std::string path = s.latin1();
		path += "/";

		msvc2003LineEdit->setText(path.c_str());
	}
}


void ABMainWindow::msvc2005PushButton_clicked()
{
    QString s = QFileDialog::getExistingDirectory("", this, "open file dialog", "Select Visual Studio 2003 IDE directory");

	if(s.isEmpty())
	{
		msvc2005LineEdit->setText("");
	}
	else
	{
		std::string path = s.latin1();
		path += "/";

		msvc2005LineEdit->setText(path.c_str());
	}
}


void ABMainWindow::buildApp()
{

//checkout or update
	if(updateRadioButton->isChecked())
	{
		if(dirLineEdit->text().isEmpty())
		{
			QMessageBox::warning(this, tr("Warning"), tr("Workspace directory not defined."));
			return;
		}
	}

//set interface icons
	task1PixmapLabel->setPixmap(QPixmap::fromMimeSource("Clock.png"));
	task2PixmapLabel->setPixmap(QPixmap::fromMimeSource("Clock.png"));
	task3PixmapLabel->setPixmap(QPixmap::fromMimeSource("Clock.png"));
	task4PixmapLabel->setPixmap(QPixmap::fromMimeSource("Clock.png"));
	task5PixmapLabel->setPixmap(QPixmap::fromMimeSource("Clock.png"));
	task6PixmapLabel->setPixmap(QPixmap::fromMimeSource("Clock.png"));
	task7PixmapLabel->setPixmap(QPixmap::fromMimeSource("Clock.png"));

	if(!buildPluginsCheckBox->isChecked())
	{
		task4PixmapLabel->setPixmap(QPixmap::fromMimeSource("Remove.png"));
	}

	if(!copyTrFilesCheckBox->isChecked())
	{
		task6PixmapLabel->setPixmap(QPixmap::fromMimeSource("Remove.png"));
	}

	if(!copyHelpFilesCheckBox->isChecked())
	{
		task7PixmapLabel->setPixmap(QPixmap::fromMimeSource("Remove.png"));
	}

	task1PixmapLabel->repaint(true);
	task2PixmapLabel->repaint(true);
	task3PixmapLabel->repaint(true);
	task4PixmapLabel->repaint(true);
	task5PixmapLabel->repaint(true);
	task6PixmapLabel->repaint(true);
	task7PixmapLabel->repaint(true);

//get application builder type
	std::string appName = appNameComboBox->currentText().latin1();

	ABAbstractBuilder* builder = NULL;

	if(appName == std::string(APP_TERRAVIEW_NAME))
	{
		builder = new ABTerraViewBuilder();
	}
	else if(appName == std::string(APP_SIGDESKTOP_NAME))
	{
		builder = new ABSigDesktopBuilder();
	}
	else if(appName == std::string(APP_TERRAAMAZON_NAME))
	{
		builder = new ABTerraAmazonBuilder();
	}

//set msvc paths
	builder->setMSVC2003Path(msvc2003LineEdit->text().latin1());
	builder->setMSVC2005Path(msvc2005LineEdit->text().latin1());

//set build plugins state
	builder->setBuildPluginsState(buildPluginsCheckBox->isChecked());

	if(!builder)
	{
		QMessageBox::critical(this, tr("Error"), tr("Unexpected Error."));
		return;
	}

	std::string msg  = "Sorry, something went wrong. A team of highly trained monkeys has been dispatched to deal with this situation. \n";
				msg += "If you see them, show them this information: ";


	if(checkoutRadioButton->isChecked())
	{
		//create build directory
		task1PixmapLabel->setPixmap(QPixmap::fromMimeSource("Run.png"));
		task1PixmapLabel->repaint(true);
		if(builder->createBuildDirectory())
		{
			task1PixmapLabel->setPixmap(QPixmap::fromMimeSource("Apply.png"));
			task1PixmapLabel->repaint(true);
		}
		else
		{
			task1PixmapLabel->setPixmap(QPixmap::fromMimeSource("Error.png"));

			msg += "Error creating build directory.";

			QMessageBox::warning(this, tr("Warning"), msg.c_str());
			delete builder;

			widgetStack->raiseWidget(0);

			return;
		}

		//get application from svn
		task2PixmapLabel->setPixmap(QPixmap::fromMimeSource("Run.png"));
		task2PixmapLabel->repaint(true);
		if(builder->getAppFromRepository())
		{
			task2PixmapLabel->setPixmap(QPixmap::fromMimeSource("Apply.png"));
			task2PixmapLabel->repaint(true);
		}
		else
		{
			task2PixmapLabel->setPixmap(QPixmap::fromMimeSource("Error.png"));

			msg += "Error getting application from svn.";

			QMessageBox::warning(this, tr("Warning"), msg.c_str());

			delete builder;

			widgetStack->raiseWidget(0);

			return;
		}
	}
	else if(updateRadioButton->isChecked())
	{
		task1PixmapLabel->setPixmap(QPixmap::fromMimeSource("Apply.png"));
		task1PixmapLabel->repaint(true);

		//update application from svn
		task2PixmapLabel->setPixmap(QPixmap::fromMimeSource("Run.png"));
		task2PixmapLabel->repaint(true);

		std::string path = dirLineEdit->text().latin1();

		if(builder->updateAppFromRepository(path))
		{
			task2PixmapLabel->setPixmap(QPixmap::fromMimeSource("Apply.png"));
			task2PixmapLabel->repaint(true);
		}
		else
		{
			task2PixmapLabel->setPixmap(QPixmap::fromMimeSource("Error.png"));

			msg += "Error updating application from svn.";

			QMessageBox::warning(this, tr("Warning"), msg.c_str());

			delete builder;

			widgetStack->raiseWidget(0);

			return;
		}
	}
	else
	{
		QMessageBox::warning(this, tr("Warning"), tr("SVN option not defined."));

		delete builder;

		widgetStack->raiseWidget(0);

		return;
	}

//build application
	task3PixmapLabel->setPixmap(QPixmap::fromMimeSource("Run.png"));
	task3PixmapLabel->repaint(true);
	if(builder->buildApp(qApp->applicationDirPath().latin1()))
	{
		task3PixmapLabel->setPixmap(QPixmap::fromMimeSource("Apply.png"));
		task3PixmapLabel->repaint(true);
	}
	else
	{
		task3PixmapLabel->setPixmap(QPixmap::fromMimeSource("Error.png"));

		msg += "Error building application from svn.";

		QMessageBox::warning(this, tr("Warning"), msg.c_str());

		delete builder;

		widgetStack->raiseWidget(0);

		return;
	}

//build plugins
	if(buildPluginsCheckBox->isChecked())
	{
		task4PixmapLabel->setPixmap(QPixmap::fromMimeSource("Run.png"));
		task4PixmapLabel->repaint(true);

		if(builder->buildPlugins(qApp->applicationDirPath().latin1()))
		{
			task4PixmapLabel->setPixmap(QPixmap::fromMimeSource("Apply.png"));
			task4PixmapLabel->repaint(true);
		}
		else
		{
			task4PixmapLabel->setPixmap(QPixmap::fromMimeSource("Error.png"));

			msg += "Error building plugins.";
			
			QMessageBox::warning(this, tr("Warning"), msg.c_str());

			delete builder;

			widgetStack->raiseWidget(0);

			return;
		}
	}

//create output directory
	task5PixmapLabel->setPixmap(QPixmap::fromMimeSource("Run.png"));
	task5PixmapLabel->repaint(true);

	if(builder->createReleaseDirectory(buildPluginsCheckBox->isChecked()))
	{
//copy app to release directory
		if(builder->copyAppFiles())
		{
			if(buildPluginsCheckBox->isChecked())
			{
//copy plugins to release directory
				if(!builder->copyPluginFiles())
				{
					task5PixmapLabel->setPixmap(QPixmap::fromMimeSource("Error.png"));

					msg += "Error copying plugins files.";
					
					QMessageBox::warning(this, tr("Warning"), msg.c_str());

					delete builder;

					widgetStack->raiseWidget(0);

					return;
				}
			}

			task5PixmapLabel->setPixmap(QPixmap::fromMimeSource("Apply.png"));
			task5PixmapLabel->repaint(true);
		}
		else
		{
			task5PixmapLabel->setPixmap(QPixmap::fromMimeSource("Error.png"));

			msg += "Error copying application files.";
			
			QMessageBox::warning(this, tr("Warning"), msg.c_str());

			delete builder;

			widgetStack->raiseWidget(0);

			return;
		}
	}
	else
	{
		task5PixmapLabel->setPixmap(QPixmap::fromMimeSource("Error.png"));

		msg += "Error creating output directory.";
			
		QMessageBox::warning(this, tr("Warning"), msg.c_str());

		delete builder;

		widgetStack->raiseWidget(0);

		return;
	}

//copy tr files
	if(copyTrFilesCheckBox->isChecked())
	{
		task6PixmapLabel->setPixmap(QPixmap::fromMimeSource("Run.png"));
		task6PixmapLabel->repaint(true);

		if(builder->copyTranslateFiles(buildPluginsCheckBox->isChecked()))
		{
			task6PixmapLabel->setPixmap(QPixmap::fromMimeSource("Apply.png"));
			task6PixmapLabel->repaint(true);
		}
		else
		{
			task6PixmapLabel->setPixmap(QPixmap::fromMimeSource("Error.png"));

			msg += "Error copying translation files.";
			
			QMessageBox::warning(this, tr("Warning"), msg.c_str());

			delete builder;

			widgetStack->raiseWidget(0);

			return;
		}
	}

//copy help files
	if(copyHelpFilesCheckBox->isChecked())
	{
		task7PixmapLabel->setPixmap(QPixmap::fromMimeSource("Run.png"));
		task7PixmapLabel->repaint(true);

		if(builder->copyHelpFiles())
		{
			task7PixmapLabel->setPixmap(QPixmap::fromMimeSource("Apply.png"));
			task7PixmapLabel->repaint(true);
		}
		else
		{
			task7PixmapLabel->setPixmap(QPixmap::fromMimeSource("Error.png"));

			msg += "Error copying help files.";
			
			QMessageBox::warning(this, tr("Warning"), msg.c_str());

			delete builder;

			widgetStack->raiseWidget(0);

			return;
		}
	}

//send email
	if(!emailLineEdit->text().isEmpty())
	{
		std::string email = emailLineEdit->text().latin1();
		
		builder->sendEmail(email);
	}

	delete builder;

	QMessageBox::information(this, tr("Information"), tr("Application built successfully!"));

	widgetStack->raiseWidget(0);
}

void ABMainWindow::optionsPushButton_clicked()
{
	widgetStack->raiseWidget(1);
}


void ABMainWindow::backPushButton_clicked()
{
	widgetStack->raiseWidget(0);
}


void ABMainWindow::helpPushButton_clicked()
{
	ABAbout about(this, "About Window", true);

	about.exec();
}


void ABMainWindow::dirPushButton_clicked()
{
	QString s = QFileDialog::getExistingDirectory("", this, "open file dialog", "Select Workspace directory");

	if(s.isEmpty())
	{
		dirLineEdit->setText("");
	}
	else
	{
		std::string path = s.latin1();
		path += "/";

		dirLineEdit->setText(path.c_str());
	}
}
