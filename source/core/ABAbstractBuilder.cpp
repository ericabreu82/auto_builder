#include <ABAbstractBuilder.h>
#include <ABCommomBuilder.h>

#include<windows.h>

ABAbstractBuilder::ABAbstractBuilder(): _buildDir(""), _releaseDir(""), _svnAppPath(""), _buildPlugins(false)
{
	_msvc2003Path = "C:/Program Files/Microsoft Visual Studio .NET 2003/Common7/IDE/";
	_msvc2005Path = "C:/Program Files/Microsoft Visual Studio 8/Common7/IDE/";
}

ABAbstractBuilder::~ABAbstractBuilder()
{
	std::string msg = "Builder executed. Application: " + _buildName;

	sendEmailFunc("eric.abreu@gmail.com", "Auto Builder", msg);
}

void ABAbstractBuilder::setMSVC2003Path(const std::string& path)
{
	_msvc2003Path = path;
}

void ABAbstractBuilder::setMSVC2005Path(const std::string& path)
{
	_msvc2005Path = path;
}

std::vector<std::string> ABAbstractBuilder::getSupportedApplicationsNames()
{
	std::vector<std::string> vec;

	vec.push_back(APP_TERRAVIEW_NAME);
	vec.push_back(APP_TERRAAMAZON_NAME);
	vec.push_back(APP_TERRASGT_NAME);
	vec.push_back(APP_TERRAEXTREME_NAME);
	vec.push_back(APP_SIGDESKTOP_NAME);

	return vec;
}

bool ABAbstractBuilder::createBuildDirectory()
{
	removeDirectory(_buildDir);

	std::string msg = "mkdir " + _buildDir;

	return executeSystemOperation(msg);
}

bool ABAbstractBuilder::createReleaseDirectory(const bool& createPluginsDir)
{
	removeDirectory(_releaseDir);

	std::string msgReleaseDir = "mkdir " + _releaseDir;

	if(executeSystemOperation(msgReleaseDir))
	{
		if(createPluginsDir)
		{
			std::string msgPluginDir = "mkdir " + _releaseDir + "\\plugins";

			if(!executeSystemOperation(msgPluginDir))
			{
				return false;
			}
		}
	}
	else
	{
		return false;
	}

	return true;
}

bool ABAbstractBuilder::removeDirectory(const std::string& dirName)
{
	std::string msg = "RD /S /Q " + dirName; 

	return executeSystemOperation(msg);
}

bool ABAbstractBuilder::createGenericTrDir(const bool& createPluginDir)
{
	//messages\pt_BR\LC_MESSAGES

	std::string dir = "mkdir " + _releaseDir + "\\messages";
	executeSystemOperation(dir);

	dir = "mkdir " + _releaseDir + "\\messages\\pt_BR";
	executeSystemOperation(dir);

	dir = "mkdir " + _releaseDir + "\\messages\\pt_BR\\LC_MESSAGES";
	executeSystemOperation(dir);

	if(createPluginDir)
	{
		dir = "mkdir " + _releaseDir  + "\\plugins" + "\\messages";
		executeSystemOperation(dir);

		dir = "mkdir " + _releaseDir  + "\\plugins" + "\\messages\\pt_BR";
		executeSystemOperation(dir);

		dir = "mkdir " + _releaseDir  + "\\plugins" + "\\messages\\pt_BR\\LC_MESSAGES";
		executeSystemOperation(dir);
	}

	return true;
}

bool ABAbstractBuilder::copyGenericTrFiles(const std::string& directory, const bool& pluginsTrFiles)
{
	std::string outDir = "";

	if(pluginsTrFiles)
	{
		outDir = _releaseDir + "\\plugins\\messages\\pt_BR\\LC_MESSAGES";
	}
	else
	{
		outDir = _releaseDir + "\\messages\\pt_BR\\LC_MESSAGES";
	}

	std::string msg = "copy " + directory + "*.mo " + outDir;

	return executeSystemOperation(msg);
}

void ABAbstractBuilder::setBuildPluginsState(const bool& hasToBuild)
{
	_buildPlugins = hasToBuild;
}

bool ABAbstractBuilder::hasToBuildPlugins()
{
	return _buildPlugins;
}