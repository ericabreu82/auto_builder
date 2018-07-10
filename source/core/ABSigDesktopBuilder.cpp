#include <ABSigDesktopBuilder.h>
#include <ABCommomBuilder.h>

#include<windows.h>

ABSigDesktopBuilder::ABSigDesktopBuilder():ABAbstractBuilder()
{
	_buildName	= APP_SIGDESKTOP_NAME;
	_buildDir	= "sigBuild";
	_releaseDir = "sigRelease";
}

ABSigDesktopBuilder::~ABSigDesktopBuilder()
{
}

bool ABSigDesktopBuilder::getAppFromRepository()
{
//checkout terralib repository
	bool res = getTerraLibWorkspace(_buildDir);

//checkout tdk repository
	if(res)
	{
		res = getTDKWorkspace(_buildDir);
	}

	return res;
}

bool ABSigDesktopBuilder::updateAppFromRepository(const std::string& path)
{
	std::string outputDir = path + "/terralib";
	
	if(!svnUpdateApp(outputDir))
	{
		return false;
	}

	if(hasToBuildPlugins())
	{
//update tdkframework repository
		outputDir = path + "/tdk_framework";

		if(!svnUpdateApp(outputDir))
		{
			return false;
		}

//update application repository
		outputDir = path + "/applications";

		if(!svnUpdateApp(outputDir))
		{
			return false;
		}
	}

	return true;
}

bool ABSigDesktopBuilder::buildApp(const std::string& mainDir)
{
	std::string fullBuildDir = mainDir + "/" + _buildDir;
	std::string solutionFile = fullBuildDir + "/applications/CIGEx/SigDesktop-msvc2005.sln";
	std::string buildLogFile = "SigDesktop_BuildLog.txt";

	return compileApp(solutionFile, buildLogFile, MSVC_2005, MSVC_RELEASE, _msvc2005Path);
}

bool ABSigDesktopBuilder::copyAppFiles()
{
	std::string buildReleaseDir = _buildDir + "\\applications\\CIGEx\\SigDesktop\\Release\\mt.vc8\\bin\\";

	std::string copyDlls = buildReleaseDir + "*.dll";
	std::string copyExe = buildReleaseDir + "*.exe";

	std::string msgCopyDlls = "copy  " + copyDlls + " " + _releaseDir;

	std::string msgCopyExe = "copy  " + copyExe + " " + _releaseDir;

	if(!executeSystemOperation(msgCopyDlls))
	{
		return false;
	}

	if(!executeSystemOperation(msgCopyExe))
	{
		return false;
	}

	if(!copyQtDependencyFiles(_releaseDir))
	{
		return false;
	}

	if(!copyLayoutDependencyFiles())
	{
		return false;
	}

	return true;
}

bool ABSigDesktopBuilder::buildPlugins(const std::string& mainDir)
{
	std::string fullBuildDir = mainDir + "/" + _buildDir + "/";
	std::string buildLogFile = "SigDesktopPlugin_BuildLog.txt";
	std::string solutionFile = "";

//PDI Plugin
	if(!buildPDIPlugin(fullBuildDir, buildLogFile, MSVC_2005, MSVC_RELEASE, _msvc2005Path))
	{
		return false;
	}

//Basic Edition
	if(!buildEditionPlugin(fullBuildDir, buildLogFile, MSVC_2005, MSVC_RELEASE, _msvc2005Path))
	{
		return false;
	}

//External Theme
	solutionFile = fullBuildDir + "applications/terrabasePlugins/ExternalTheme/ExternalTheme-msvc2005.sln";

	if(!compileApp(solutionFile, buildLogFile, MSVC_2005, MSVC_RELEASE, _msvc2005Path))
	{
		return false;
	}

//File Theme
	solutionFile = fullBuildDir + "applications/terrabasePlugins/fileTheme/FileTheme-msvc2005.sln";

	if(!compileApp(solutionFile, buildLogFile, MSVC_2005, MSVC_RELEASE, _msvc2005Path))
	{
		return false;
	}
	
	return true;
}

bool ABSigDesktopBuilder::copyPluginFiles()
{
	std::string buildPluginsDir = _buildDir + "\\tdk_framework\\Release\\mt.vc8\\plugins\\";

	std::string copyDlls = buildPluginsDir + "*.dll";

	std::string msgCopyDlls = "copy  " + copyDlls + " " + _releaseDir + "\\plugins";

	if(!executeSystemOperation(msgCopyDlls))
	{
		return false;
	}

	std::string copyDatFiles = buildPluginsDir + "*.dat";

	std::string msgCopyDatFiles = "copy  " + copyDatFiles + " " + _releaseDir + "\\plugins";

	if(!executeSystemOperation(msgCopyDatFiles))
	{
		return false;
	}

	return true;
}

bool ABSigDesktopBuilder::copyTranslateFiles(const bool& copyPluginTrFiles)
{
	std::string sigDesktopTranslateDir = _buildDir + "\\applications\\CIGEx\\tdkqt3dialogs\\mak.x\\";

	if(!copyTrFiles(sigDesktopTranslateDir, false, _releaseDir))
	{
		return false;
	}

	std::string terraBaseTranslateDir = _buildDir + "\\applications\\terrabase\\tdkqt3dialogs\\";

	if(!copyTrFiles(terraBaseTranslateDir, false, _releaseDir))
	{
		return false;
	}

//copy generic translate
	if(createGenericTrDir(copyPluginTrFiles))
	{
		std::string tdkFrameworkDir = _buildDir + "\\tdk_framework\\messages\\pt_BR\\LC_MESSAGES\\";
		copyGenericTrFiles(tdkFrameworkDir, false);

		std::string applicationDir = _buildDir + "\\applications\\terrabase\\messages\\pt_BR\\LC_MESSAGES\\";
		copyGenericTrFiles(applicationDir, false);

		if(copyPluginTrFiles)
		{
//copy generic translate plugin files
			//pdi
			std::string pdiDir = _buildDir + "\\applications\\terrabasePlugins\\pdiPlugin\\messages\\pt_BR\\LC_MESSAGES\\";
			copyGenericTrFiles(pdiDir, true);

			//basic edition
			std::string basicEditionDir = _buildDir + "\\applications\\terrabasePlugins\\TdkBasicEditionPlugin\\";
			copyGenericTrFiles(basicEditionDir, true);
		}
	}

	if(copyPluginTrFiles)
	{
//pdi translations files
		copyPDITrFiles(_buildDir, _releaseDir);

//basic edition translate files
		std::string basicEditionTrFile = _buildDir + "\\applications\\terrabasePlugins\\TdkBasicEditionPlugin\\";
		copyTrFiles(basicEditionTrFile, true, _releaseDir);

//external theme translate files
		std::string externalThemeTrFile = _buildDir + "\\applications\\terrabasePlugins\\ExternalTheme\\q3Pro\\";
		copyTrFiles(externalThemeTrFile, true, _releaseDir);

//file theme translate files
		std::string fileThemeTrFile = _buildDir + "\\applications\\terrabasePlugins\\fileTheme\\";
		copyTrFiles(fileThemeTrFile, true, _releaseDir);
	}

	return true;
}

bool ABSigDesktopBuilder::copyHelpFiles()
{
//create help directory
	std::string msgCreateDir = "mkdir " + _releaseDir + "\\help";

	if(!executeSystemOperation(msgCreateDir))
	{
		return false;
	}

//copy help files
	std::string sourceDir = _buildDir + "\\applications\\CIGEx\\SigDesktop\\help";
	std::string destDir = _releaseDir + "\\help";

	std::string msgCopyFiles = "xcopy " + sourceDir + " " + destDir + " /e /EXCLUDE:svnFiles.txt";

	if(!executeSystemOperation(msgCopyFiles))
	{
		return false;
	}

	return true;
}

bool ABSigDesktopBuilder::sendEmail(const std::string& email)
{
	return sendEmailFunc(email, "Auto Builder", "SigDesktop Compilation Done.");
}

bool ABSigDesktopBuilder::copyLayoutDependencyFiles()
{
	std::string destDir = _releaseDir + "\\layout";

	return copyLayoutDependency(_buildDir, _releaseDir, destDir);
}