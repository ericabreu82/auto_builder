/******************************************************************************
 * FUNCATE - GIS development team
 *
 * Auto Build Framework
 */

/** @file ABAbstractBuilder.h
  * @brief Defines the builder interface.
  * @version 1.0.0
  * @author Eric Silva Abreu <eric.abreu@funcate.org.br>
  * @date 2010
  */

#ifndef AB_ABSTRACT_BUILDER_H
#define AB_ABSTRACT_BUILDER_H

//std includes
#include <vector>
#include <string>

// APPs names defines
#define APP_TERRAVIEW_NAME		"TerraView 2003"
#define APP_TERRAAMAZON_NAME	"TerraAmazon 2005"
#define APP_TERRASGT_NAME		"TerraSGT 2005"
#define APP_TERRAEXTREME_NAME	"TerraExtreme 2005"
#define APP_SIGDESKTOP_NAME		"SigDesktop 2005"

// Visual Studio Version
enum MSVC_VERSION
{
	MSVC_2003, MSVC_2005
};

enum MSVC_TYPE
{
	MSVC_DEBUG, MSVC_RELEASE, MSVC_PLUGINDEBUG, MSVC_PLUGINRELEASE
};

 /** @class ABAbstractBuilder
  *	 @brief Contains a class that defines the builder interface.
  *
  * This class represents an abstract interface that defines the virtual methods
  * to automatic build applications. This methods used MS_DOS calls to run in console
  * mode functions to get apps from svn and build.
  *
  * At first time the list of applications that can be build automatic are:
  *		- Terra View
  *		- Terra Amazon
  *		- Sig Desktop
  *
  */

/**
 * @ingroup ABKernel
 */

class ABAbstractBuilder
{
public:

    /** @name Constructor and Destructor Methods
	  *	Methods related to instantiation and destruction.
      */

	//@{

	/** @brief Default Constructor.
	 */
    ABAbstractBuilder();

	/** @brief Virtual Destructor.
	*/
    virtual ~ABAbstractBuilder();
    //@}

public:

    /** @name Public Methods
	  *	Methods related to handle the application build.
      */
	//@{

	/** @brief Function used to create the builder directory
	*	return	Return true if thats ok and false in other case.
	*/
	virtual bool createBuildDirectory();

	/** @brief Function used to checkout application files from repository
	*	return	Return true if thats ok and false in other case.
	*/
	virtual bool getAppFromRepository() = 0;

	/** @brief Function used to update application files from repository
	*	return	Return true if thats ok and false in other case.
	*/
	virtual bool updateAppFromRepository(const std::string& path) = 0;

	/** @brief Function used to build application using msvs from command line
	*	param	mainDir		String that defines the application(AutoBuilder) directory
	*	return	Return true if thats ok and false in other case.
	*/
	virtual bool buildApp(const std::string& mainDir) = 0;

	/** @brief Function used to create the release directory
	*	param	createPluginsDir		Define if the plugin directory has to be created
	*	return	Return true if thats ok and false in other case.
	*/
	virtual bool createReleaseDirectory(const bool& createPluginsDir);

	/** @brief Function used to copy application files to release directory
	*	return	Return true if thats ok and false in other case.
	*/
	virtual bool copyAppFiles() = 0;

	/** @brief Function used to build plugins, if this app support this
	*	param	mainDir		String that defines the application(AutoBuilder) directory
	*	return	Return true if thats ok and false in other case.
	*/
	virtual bool buildPlugins(const std::string& mainDir) = 0;

	/** @brief Function used to copy plugin files to release directory
	*	return	Return true if thats ok and false in other case.
	*/
	virtual bool copyPluginFiles() = 0;

	/** @brief Function used to copy application translation files to release directory
	*	param	copyPluginTrFiles		Define if the plugin translation files also has to copy
	*	return	Return true if thats ok and false in other case.
	*/
	virtual bool copyTranslateFiles(const bool& copyPluginTrFiles) = 0;

	/** @brief Function used to copy application help files to release directory
	*	return	Return true if thats ok and false in other case.
	*/
	virtual bool copyHelpFiles() = 0;

	/** @brief Function used to send email to a person to notice that compilation is done
	*	param	email		string that defines the email address
	*	return	Return true if thats ok and false in other case.
	*/
	virtual bool sendEmail(const std::string& email) = 0;

	/** @brief Function used to remove a directory
	*	param	dirName		string that defines the directory name
	*	return	Return true if thats ok and false in other case.
	*/
	virtual bool removeDirectory(const std::string& dirName);

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


public:

	/** @name Public Methods
	 *	Methods publics not extensibles.
     */
	//@{

	/** @brief Function used to get the list of applications that can be build.
	*	return	List of applications names
	*/
	static std::vector<std::string> getSupportedApplicationsNames();

	/** @brief Function used to define the msvc2003 path.
	*	params	path	Full path that locale the file devenv.com
	*/
	void setMSVC2003Path(const std::string& path);

	/** @brief Function used to define the msvc2005 path.
	*	params	path	Full path that locale the file devenv.com
	*/
	void setMSVC2005Path(const std::string& path);

	/** @brief Function used to define if plugins has to be builded.
	*	params	hasToBuild	Flag to indicate if has to build plugins
	*/
	void setBuildPluginsState(const bool& hasToBuild);

	/** @brief Function used to know if plugins has to be builded.
	*	return	Flag to indicate if has to build plugins
	*/
	bool hasToBuildPlugins();
	//@}


protected:

	std::string _buildName;			//!< Attribute used to define builder application name
	std::string _buildDir;			//!< Attribute used to define the directory used to build application
	std::string _releaseDir;		//!< Attribute used to define the release directory
	std::string _svnAppPath;		//!< Attribute used to define the repository path
	std::string _msvc2003Path;		//!< Attribute used to define the msvc2003 location
	std::string _msvc2005Path;		//!< Attribute used to define the msvc2005 location
	bool		_buildPlugins;		//!< Attribute used to define if plugins has to been builded

};

/*
** ----------------------------------------------------------------------------
** End:
*/

#endif  //AB_ABSTRACT_BUILDER_H
