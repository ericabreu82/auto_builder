/******************************************************************************
 * FUNCATE - GIS development team
 *
 * Auto Build Framework
 */

/** @file ABTerraAmazonBuilder.h
  * @brief Defines the TerraAmazon builder interface.
  * @version 1.0.0
  * @author Eric Silva Abreu <eric.abreu@funcate.org.br>
  * @date 2010
  */

#ifndef AB_TERRAAMAZON_BUILDER_H
#define AB_TERRAAMAZON_BUILDER_H

//AB includes
#include <ABAbstractBuilder.h>


 /** @class ABTerraAmazonBuilder
  *	 @brief Contains a class that defines the Terra Amazon builder interface.
  *
  * This class extends the ABAbstractBuilder and implements the TerraAmazon
  * builder methods.
  *
  * This class can do:
  *		- Build TerraAmazon
  *		- Build official plugins
  *		- Copy translation files
  *		- Copy help information
  *
  *	Official Plugins List:
  *		- PDI
  *		- Cloud Detection
  *
  * sa\ ABAbstractBuilder
  */

/**
 * @ingroup ABKernel
 */

class ABTerraAmazonBuilder : public ABAbstractBuilder
{
public:

    /** @name Constructor and Destructor Methods
	  *	Methods related to instantiation and destruction.
      */

	//@{

	/** @brief Default Constructor.
	 */
    ABTerraAmazonBuilder();

	/** @brief Virtual Destructor.
	*/
    virtual ~ABTerraAmazonBuilder();
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
};

/*
** ----------------------------------------------------------------------------
** End:
*/

#endif  //AB_TERRAAMAZON_BUILDER_H
