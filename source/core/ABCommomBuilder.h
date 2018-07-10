/******************************************************************************
 * FUNCATE - GIS development team
 *
 * Auto Build Framework
 */

/** @file ABCommomBuilder.h
  * @brief Defines the builder interface commom functions.
  * @version 1.0.0
  * @author Eric Silva Abreu <eric.abreu@funcate.org.br>
  * @date 2010
  */

#ifndef AB_COMMOM_BUILDER_H
#define AB_COMMOM_BUILDER_H

#include <string>

#include <ABAbstractBuilder.h>

/** @brief Function used to get information from svn
*	param	svnPath		string to define the path
*	param	outputDir	string to define the output directory name
*	return	Return true if thats ok and false in other case.
*/
bool svnCheckoutApp(const std::string& svnPath, const std::string& outputDir);

/** @brief Function used to update information from svn
*	param	svnPath		string to define the path
*	return	Return true if thats ok and false in other case.
*/
bool svnUpdateApp(const std::string& svnPath);

/** @brief Function used to compile the solution file
*	param	solutionPath	string to define the full solution file
*	param	logFileName		string to define the log file name
*	param	version			enum to define if the solution has to be build using 2003 or 2005 msvc
*	param	type			enum to define the compilation mode (release, debug,...)
*	return	Return true if thats ok and false in other case.
*/
bool compileApp(const std::string& solutionPath, const std::string& logFileName, const MSVC_VERSION& version, const MSVC_TYPE& type, const std::string& msvcPath);

/** @brief Function used to copy the translation files
*	param	directory			string to define the location to find the translation files
*	param	pluginsTrFiles		flag to indicate if the tr file was a plugin tr file
*	param	outDir				string to define the output location directory
*	return	Return true if thats ok and false in other case.
*/
bool copyTrFiles(const std::string& directory, const bool& pluginsTrFiles, const std::string releaseDir, std::string outDir = "");

/** @brief Function used to copy the qt file dependencies
*	return	Return true if thats ok and false in other case.
*/
bool copyQtDependencyFiles(const std::string& releaseDir);

/** @brief Function used to execute a operation
*	param	opStr	string that defines the system operation
*	return	Return true if thats ok and false in other case.
*/
bool executeSystemOperation(const std::string& opStr);

/** @brief Function used to write a file
*	param	fileName	string that defines the file name
*	param	text		string to added into file
*	param	mode		string that defines the file access mode (a, w)
*/
void writeToFile(const std::string& fileName, const std::string& text, const std::string& mode);

/** @brief Function used to change the slash to back slash
*	param	str		string that was to be changed
*/
void changeSlash(std::string& str);

/** @brief Function used to send a email
*	param	email				email address
*	param	subject				email subject
*	param	body				email message
*	param	attachedVecList		list of file names to be attached into email
*	return	Return true if thats ok and false in other case.
*/
bool sendEmailFunc(const std::string& email, const std::string& subject, const std::string& body, const std::vector<std::string>& attachedVecList = std::vector<std::string>());

bool getTerraLibWorkspace(const std::string& buildDir);

bool getTerraLibOGCWorkspace(const std::string& buildDir);

bool getTDKWorkspace(const std::string& buildDir);

bool buildPDIPlugin(const std::string& fullDir, const std::string& logFileName, const MSVC_VERSION& version, const MSVC_TYPE& type, const std::string& msvcPath);

bool buildEditionPlugin(const std::string& fullDir, const std::string& logFileName, const MSVC_VERSION& version, const MSVC_TYPE& type, const std::string& msvcPath);

bool copyPDITrFiles(const std::string& buildDir, const std::string releaseDir, std::string outDir = "");

bool copyLayoutDependency(const std::string& buildDir, const std::string& releaseDir, const std::string& destDir);

#endif //AB_COMMOM_BUILDER_H