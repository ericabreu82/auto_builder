/******************************************************************************
 * FUNCATE - GIS development team
 *
 * Auto Build Framework
 */

/** @file ABSigDesktopBuilder.h
  * @brief Defines the SigDesktop builder interface.
  * @version 1.0.0
  * @author Eric Silva Abreu <eric.abreu@funcate.org.br>
  * @date 2010
  */

#ifndef AB_SIGDESKTOP_BUILDER_H
#define AB_SIGDESKTOP_BUILDER_H

//AB includes
#include <ABAbstractBuilder.h>


 /** @class ABSigDesktopBuilder
  *	 @brief Contains a class that defines the Terra View builder interface.
  *
  * This class extends the ABAbstractBuilder and implements the SigDesktop
  * builder methods.
  *
  * This class can do:
  *		- Build SigDesktop
  *		- Build official plugins
  *		- Copy translation files
  *		- Copy help information
  *
  *	Official Plugins List:
  *		- PDI
  *		- Edition
  *		- Complex Style
  *
  * sa\ ABAbstractBuilder
  */

/**
 * @ingroup ABKernel
 */

class ABSigDesktopBuilder : public ABAbstractBuilder
{
public:

    /** @name Constructor and Destructor Methods
	  *	Methods related to instantiation and destruction.
      */

	//@{

	/** @brief Default Constructor.
	 */
    ABSigDesktopBuilder();

	/** @brief Virtual Destructor.
	*/
    virtual ~ABSigDesktopBuilder();
    //@}

public:

    /** @name Public Methods
	  *	Methods related to handle the application build.
      */
    //@{

	/** @brief Function used to checkout application files from repository
	*	return	Return true if thats ok and false in other case.
	*/
	virtual bool getAppFromRepository();

	/** @brief Function used to update application files from repository
	*	return	Return true if thats ok and false in other case.
	*/
	virtual bool updateAppFromRepository(const std::string& path);

	/** @brief Function used to build application using msvs from command line
	*	param	mainDir		String that defines the application(AutoBuilder) directory
	*	return	Return true if thats ok and false in other case.
	*/
	virtual bool buildApp(const std::string& mainDir);

	/** @brief Function used to copy application files to release directory
	*	return	Return true if thats ok and false in other case.
	*/
	virtual bool copyAppFiles();

	/** @brief Function used to build plugins, if this app support this
	*	return	Return true if thats ok and false in other case.
	*/
	virtual bool buildPlugins(const std::string& mainDir);

	/** @brief Function used to copy plugin files to release directory
	*	return	Return true if thats ok and false in other case.
	*/
	virtual bool copyPluginFiles();

	/** @brief Function used to copy application translation files to release directory
	*	param	copyPluginTrFiles		Define if the plugin translation files also has to copy
	*	return	Return true if thats ok and false in other case.
	*/
	virtual bool copyTranslateFiles(const bool& copyPluginTrFiles);

	/** @brief Function used to copy application help files to release directory
	*	return	Return true if thats ok and false in other case.
	*/
	virtual bool copyHelpFiles();

	/** @brief Function used to send email to a person to notice that compilation is done
	*	param	email		string that defines the email address
	*	return	Return true if thats ok and false in other case.
	*/
	virtual bool sendEmail(const std::string& email);
    //@}

protected:

	/** @brief Function used to copy layout dependencies files
	*	return	Return true if thats ok and false in other case.
	*/
	virtual bool copyLayoutDependencyFiles();
};

/*
** ----------------------------------------------------------------------------
** End:
*/

#endif  //AB_SIGDESKTOP_BUILDER_H
