/******************************************************************************
 * FUNCATE - GIS development team
 *
 * Auto Build Framework
 */

/** @file ABTerraViewBuilder.h
  * @brief Defines the TerraView builder interface.
  * @version 1.0.0
  * @author Eric Silva Abreu <eric.abreu@funcate.org.br>
  * @date 2010
  */

#ifndef AB_TERRAVIEW_BUILDER_H
#define AB_TERRAVIEW_BUILDER_H

//AB includes
#include <ABAbstractBuilder.h>


 /** @class ABTerraViewBuilder
  *	 @brief Contains a class that defines the Terra View builder interface.
  *
  * This class extends the ABAbstractBuilder and implements the TerraView
  * builder methods.
  *
  * This class can do:
  *		- Build TerraView
  *		- Build official plugins
  *		- Copy translation files
  *		- Copy help information
  *
  *	Official Plugins List:
  *		- Copy Database
  *		- External Theme
  *		- File Theme
  *		- Fill Cell
  *		- Flow
  *		- Oracle Spatial Adapter
  *		- Plugin Generator
  *		- Wms Client
  *
  * sa\ ABAbstractBuilder
  */

/**
 * @ingroup ABKernel
 */

class ABTerraViewBuilder : public ABAbstractBuilder
{
public:

    /** @name Constructor and Destructor Methods
	  *	Methods related to instantiation and destruction.
      */

	//@{

	/** @brief Default Constructor.
	 */
    ABTerraViewBuilder();

	/** @brief Virtual Destructor.
	*/
    virtual ~ABTerraViewBuilder();
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

	/** @brief Function used to create the generic translate directory
	*	param	createPluginDir		Flag to indicate if also has to create the generic tr plugin directory
	*	return	Return true if thats ok and false in other case.
	*/
	virtual bool createGenericTrDir(const bool& createPluginDir);

	/** @brief Function used to copy the generic translate files
	*	param	directory		directory path
	*	param	pluginsTrFiles	Flag to indicate if also has to copy the generic tr plugin files
	*	return	Return true if thats ok and false in other case.
	*/
	virtual bool copyGenericTrFiles(const std::string& directory, const bool& pluginsTrFiles);
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

#endif  //AB_TERRAVIEW_BUILDER_H
