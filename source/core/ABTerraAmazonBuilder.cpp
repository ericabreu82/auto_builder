#include <ABTerraAmazonBuilder.h>
#include <ABCommomBuilder.h>

#include<windows.h>

ABTerraAmazonBuilder::ABTerraAmazonBuilder():ABAbstractBuilder()
{
	_buildName	= APP_TERRAAMAZON_NAME;
	_buildDir	= "amazonBuild";
	_releaseDir = "amazonRelease";
	_svnAppPath = "https://201.48.106.39/terraFuncate/trunk/funcate";
}

ABTerraAmazonBuilder::~ABTerraAmazonBuilder()
{
}

bool ABTerraAmazonBuilder::getAppFromRepository()
{
//checkout amazon repository
	std::string outputDir = _buildDir + "/funcate";

	if(!svnCheckoutApp(_svnAppPath, outputDir))
	{
		return false;
	}

//checkout tdkframework repository
	outputDir = _buildDir + "/tdk_framework";

	std::string tdkFrameworkSVN = "https://201.48.106.39/tdk/trunk/tdk_framework";

	if(!svnCheckoutApp(tdkFrameworkSVN, outputDir))
	{
		return false;
	}

//checkout application repository
	outputDir = _buildDir + "/applications";

	std::string applicationSVN = "https://201.48.106.39/tdk_app/trunk/applications";

	if(!svnCheckoutApp(applicationSVN, outputDir))
	{
		return false;
	}

//checkout terralib repository
	outputDir = _buildDir + "/terralib";

	std::string terralibSVN = "https://svn.dpi.inpe.br/terralib/trunk";

	if(!svnCheckoutApp(terralibSVN, outputDir))
	{
		return false;
	}

//checkout terralib src repository
	outputDir = _buildDir + "/src";

	std::string terralib_src_SVN = "https://svn.dpi.inpe.br/terralib/trunk/src";

	if(!svnCheckoutApp(terralib_src_SVN, outputDir))
	{
		return false;
	}

//checkout terralib terralibw repository
	outputDir = _buildDir + "/terralibw";

	std::string terralib_terralibw_SVN = "https://svn.dpi.inpe.br/terralib/trunk/terralibw";

	if(!svnCheckoutApp(terralib_terralibw_SVN, outputDir))
	{
		return false;
	}

	return true;
}

bool ABTerraAmazonBuilder::updateAppFromRepository(const std::string& path)
{
	return false;
}

bool ABTerraAmazonBuilder::buildApp(const std::string& mainDir)
{
	std::string fullBuildDir = mainDir + "/" + _buildDir;
	std::string solutionFile = fullBuildDir + "/funcate/aplicativos/TerraAmazon/windows/terraAmazon.sln";
	std::string buildLogFile = "TerraAmazon_BuildLog.txt";

	return compileApp(solutionFile, buildLogFile, MSVC_2005, MSVC_RELEASE, _msvc2005Path);
}

bool ABTerraAmazonBuilder::copyAppFiles()
{
	std::string buildReleaseDir = _buildDir + "\\Release\\";

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

	return true;
}

bool ABTerraAmazonBuilder::buildPlugins(const std::string& mainDir)
{
	std::string fullBuildDir = mainDir + "/" + _buildDir + "/";
	std::string buildLogFile = "TerraAmazonPlugin_BuildLog.txt";
	std::string solutionFile = "";

//PDI Plugin
	solutionFile = fullBuildDir + "applications/terrabasePlugins/pdiPlugin/mak.vc8/TdkPDIPlugin.sln";

	if(!compileApp(solutionFile, buildLogFile, MSVC_2005, MSVC_RELEASE, _msvc2005Path))
	{
		return false;
	}

//Basic Edition
	solutionFile = fullBuildDir + "applications/terrabasePlugins/TdkBasicEditionPlugin/TdkBasicEditionPlugin-msvc2005.sln";

	if(!compileApp(solutionFile, buildLogFile, MSVC_2005, MSVC_RELEASE, _msvc2005Path))
	{
		return false;
	}

//External Theme
	solutionFile = fullBuildDir + "applications/terrabasePlugins/ExternalTheme/ExternalTheme-msvc2005.sln";

	if(!compileApp(solutionFile, buildLogFile, MSVC_2005, MSVC_RELEASE, _msvc2005Path))
	{
		return false;
	}
	
	return true;
}

bool ABTerraAmazonBuilder::copyPluginFiles()
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

bool ABTerraAmazonBuilder::copyTranslateFiles(const bool& copyPluginTrFiles)
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
//copy tdk plugins plugins translation files
		std::string outDir = _releaseDir + "\\plugins\\tdkplugins";

		//pdi translations files
		copyPDITrFiles(_buildDir, _releaseDir, outDir);

		//basic edition translate files
		std::string basicEditionTrFile = _buildDir + "\\applications\\terrabasePlugins\\TdkBasicEditionPlugin\\";
		copyTrFiles(basicEditionTrFile, true, outDir);
	}

	return true;
}

bool ABTerraAmazonBuilder::copyHelpFiles()
{
//create help directory
	std::string msgCreateDir = "mkdir " + _releaseDir + "\\help";

	if(!executeSystemOperation(msgCreateDir))
	{
		return false;
	}

//copy help files
	std::string sourceDir = _buildDir + "\\applications\\CIGEx\\TerraAmazon\\help";
	std::string destDir = _releaseDir + "\\help";

	std::string msgCopyFiles = "xcopy " + sourceDir + " " + destDir + " /e /EXCLUDE:svnFiles.txt";

	if(!executeSystemOperation(msgCopyFiles))
	{
		return false;
	}

	return true;
}

bool ABTerraAmazonBuilder::sendEmail(const std::string& email)
{
	return sendEmailFunc(email, "Auto Builder", "TerraAmazon Compilation Done.");
}