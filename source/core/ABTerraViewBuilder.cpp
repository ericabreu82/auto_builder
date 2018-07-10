#include <ABTerraViewBuilder.h>
#include <ABCommomBuilder.h>

#include<windows.h>

ABTerraViewBuilder::ABTerraViewBuilder():ABAbstractBuilder()
{
	_buildName	= APP_TERRAVIEW_NAME;
	_buildDir	= "tvBuild";
	_releaseDir = "tvRelease";
	_svnAppPath = "https://svn.dpi.inpe.br/terralib/trunk";
}

ABTerraViewBuilder::~ABTerraViewBuilder()
{
}

bool ABTerraViewBuilder::getAppFromRepository()
{
//checkout terralib repository
	bool res = getTerraLibWorkspace(_buildDir);

//checkout tdk repository
	if(res)
	{
		res = getTDKWorkspace(_buildDir);
	}

	return true;
}

bool ABTerraViewBuilder::updateAppFromRepository(const std::string& path)
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

bool ABTerraViewBuilder::buildApp(const std::string& mainDir)
{
	std::string fullBuildDir = mainDir + "/" + _buildDir;
	std::string solutionFile = fullBuildDir + "/terralib/build/win32-msvc2003/terraView/terraView.sln";
	std::string buildLogFile = "TerrView_BuildLog.txt";

	return compileApp(solutionFile, buildLogFile, MSVC_2003, MSVC_RELEASE, _msvc2003Path);
}

bool ABTerraViewBuilder::copyAppFiles()
{
	std::string buildReleaseDir = _buildDir + "\\terralib\\Release\\";

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

	return true;
}

bool ABTerraViewBuilder::buildPlugins(const std::string& mainDir)
{
	std::string fullBuildDir = mainDir + "/" + _buildDir + "/";
	std::string buildLogFile = "TerrViewPlugin_BuildLog.txt";
	std::string solutionFile = "";

//Copy Database
	solutionFile = fullBuildDir + "terralib/terraViewPlugins/copyDatabase/copyDatabase.sln";

	if(!compileApp(solutionFile, buildLogFile, MSVC_2003, MSVC_RELEASE, _msvc2003Path))
	{
		return false;
	}

//External Theme
	solutionFile = fullBuildDir + "terralib/terraViewPlugins/ExternalTheme/ExternalTheme.sln";

	if(!compileApp(solutionFile, buildLogFile, MSVC_2003, MSVC_RELEASE, _msvc2003Path))
	{
		return false;
	}

//File Theme
	solutionFile = fullBuildDir + "terralib/terraViewPlugins/FileTheme/FileTheme.sln";

	if(!compileApp(solutionFile, buildLogFile, MSVC_2003, MSVC_RELEASE, _msvc2003Path))
	{
		return false;
	}

//Fill Cell
	solutionFile = fullBuildDir + "terralib/terraViewPlugins/fillCell/fillCell.sln";

	if(!compileApp(solutionFile, buildLogFile, MSVC_2003, MSVC_RELEASE, _msvc2003Path))
	{
		return false;
	}

//Flow
	solutionFile = fullBuildDir + "terralib/terraViewPlugins/Flow/Flow.sln";

	if(!compileApp(solutionFile, buildLogFile, MSVC_2003, MSVC_RELEASE, _msvc2003Path))
	{
		return false;
	}

//Oracle Spatial Adapter
	solutionFile = fullBuildDir + "terralib/terraViewPlugins/oracleSpatialAdapter/oracleSpatialAdapter.sln";

	if(!compileApp(solutionFile, buildLogFile, MSVC_2003, MSVC_RELEASE, _msvc2003Path))
	{
		return false;
	}

//Plugin Generator
	solutionFile = fullBuildDir + "terralib/terraViewPlugins/pluginGenerator/pluginGenerator.sln";

	if(!compileApp(solutionFile, buildLogFile, MSVC_2003, MSVC_RELEASE, _msvc2003Path))
	{
		return false;
	}

//Wms

//tdk plugins
	//terra extreme
	solutionFile = fullBuildDir + "applications/terrabase/TerraSIGNewAge/TerraSIGNewAge.sln";

	if(!compileApp(solutionFile, buildLogFile, MSVC_2003, MSVC_RELEASE, _msvc2003Path))
	{
		return false;
	}

	//plugin tdk
	solutionFile = fullBuildDir + "applications/terrabasePlugins/pluginTdk/pluginTdk.sln";

	if(!compileApp(solutionFile, buildLogFile, MSVC_2003, MSVC_PLUGINRELEASE, _msvc2003Path))
	{
		return false;
	}

	//PDI Plugin
	if(!buildPDIPlugin(fullBuildDir, buildLogFile, MSVC_2003, MSVC_RELEASE, _msvc2003Path))
	{
		return false;
	}

	//Basic Edition
	if(!buildEditionPlugin(fullBuildDir, buildLogFile, MSVC_2003, MSVC_RELEASE, _msvc2003Path))
	{
		return false;
	}

	return true;
}

bool ABTerraViewBuilder::copyPluginFiles()
{
	std::string buildPluginsDir = _buildDir + "\\terralib\\Release\\plugins\\";

	std::string copyDlls = buildPluginsDir + "*.dll";
	
	std::string msgCopyDlls = "copy  " + copyDlls + " " + _releaseDir + "\\plugins";

	if(!executeSystemOperation(msgCopyDlls))
	{
		return false;
	}

//create tdkplugins directory
	std::string msgPluginDir = "mkdir " + _releaseDir + "\\plugins\\tdkplugins";

	if(!executeSystemOperation(msgPluginDir))
	{
		return false;
	}

//copy layout dependency files
	if(!copyLayoutDependencyFiles())
	{
		return false;
	}

//copy dll files
	std::string buildPluginsTDKDir = _buildDir + "\\terralib\\Release\\plugins\\tdkplugins\\";

	copyDlls = buildPluginsTDKDir + "*.dll";
	
	msgCopyDlls = "copy  " + copyDlls + " " + _releaseDir + "\\plugins\\tdkplugins";

	if(!executeSystemOperation(msgCopyDlls))
	{
		return false;
	}

//copy dat files
	copyDlls = buildPluginsTDKDir + "*.dat";
	
	msgCopyDlls = "copy  " + copyDlls + " " + _releaseDir + "\\plugins\\tdkplugins";

	if(!executeSystemOperation(msgCopyDlls))
	{
		return false;
	}

	return true;
}

bool ABTerraViewBuilder::copyTranslateFiles(const bool& copyPluginTrFiles)
{
	std::string tvTranslateDir = _buildDir + "\\terralib\\terraView\\";

	if(!copyTrFiles(tvTranslateDir, false, _releaseDir))
	{
		return false;
	}

	if(copyPluginTrFiles)
	{
		std::string tvTranslateDir = _buildDir + "\\terralib\\Release\\plugins\\";

		if(!copyTrFiles(tvTranslateDir, true, _releaseDir))
		{
			return false;
		}

//copy tdk plugin translation files
		std::string terraBaseTranslateDir = _buildDir + "\\applications\\terrabase\\tdkqt3dialogs\\";

		if(!copyTrFiles(terraBaseTranslateDir, true, _releaseDir))
		{
			return false;
		}

		std::string pluginTDKTranslateDir = _buildDir + "\\applications\\terrabasePlugins\\pluginTdk\\";

		if(!copyTrFiles(terraBaseTranslateDir, true, _releaseDir))
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



//copy tdk plugins plugins translation files
		std::string outDir = _releaseDir + "\\plugins\\tdkplugins";
//pdi translations files
		//pdi translations files
		copyPDITrFiles(_buildDir, _releaseDir, outDir);

//basic edition translate files
		std::string basicEditionTrFile = _buildDir + "\\applications\\terrabasePlugins\\TdkBasicEditionPlugin\\";
		copyTrFiles(basicEditionTrFile, true, outDir);
	}

	return true;
}

bool ABTerraViewBuilder::copyHelpFiles()
{
//create help directory
	std::string msgCreateDir = "mkdir " + _releaseDir + "\\doc";

	if(!executeSystemOperation(msgCreateDir))
	{
		return false;
	}

//copy help files
	std::string sourceDir = _buildDir + "\\terralib\\terraView\\doc";
	std::string destDir = _releaseDir + "\\doc";

	std::string msgCopyFiles = "xcopy " + sourceDir + " " + destDir + " /e /EXCLUDE:svnFiles.txt";

	if(!executeSystemOperation(msgCopyFiles))
	{
		return false;
	}

//rename help directories
	std::string renamePtDir = "rename " + destDir + "\\port" + " " + "pt_BR";

	if(!executeSystemOperation(renamePtDir))
	{
		return false;
	}

	std::string renameEngDir = "rename " + destDir + "\\eng" + " " + "en_US";

	if(!executeSystemOperation(renameEngDir))
	{
		return false;
	}

	if(hasToBuildPlugins())
	{
//create help plugins directory
		std::string msgCreateDir = "mkdir " + _releaseDir + "\\doc\\pt_BR\\helpPlugins";

		if(!executeSystemOperation(msgCreateDir))
		{
			return false;
		}

//copy help plugins files
		sourceDir = _buildDir + "\\applications\\CIGEx\\SigDesktop\\help";
		destDir = _releaseDir + "\\doc\\pt_BR\\helpPlugins";

		std::string msgCopyFiles = "xcopy " + sourceDir + " " + destDir + " /e /EXCLUDE:svnFiles.txt";

		if(!executeSystemOperation(msgCopyFiles))
		{
			return false;
		}
	}

	return true;
}

bool ABTerraViewBuilder::sendEmail(const std::string& email)
{
	return sendEmailFunc(email, "Auto Builder", "TerraView Compilation Done.");
}

bool ABTerraViewBuilder::createGenericTrDir(const bool& createPluginDir)
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
		dir = "mkdir " + _releaseDir  + "\\plugins\\tdkplugins" + "\\messages";
		executeSystemOperation(dir);

		dir = "mkdir " + _releaseDir  + "\\plugins\\tdkplugins" + "\\messages\\pt_BR";
		executeSystemOperation(dir);

		dir = "mkdir " + _releaseDir  + "\\plugins\\tdkplugins" + "\\messages\\pt_BR\\LC_MESSAGES";
		executeSystemOperation(dir);
	}

	return true;
}

bool ABTerraViewBuilder::copyGenericTrFiles(const std::string& directory, const bool& pluginsTrFiles)
{
	std::string outDir = "";

	if(pluginsTrFiles)
	{
		outDir = _releaseDir + "\\plugins\\tdkplugins\\messages\\pt_BR\\LC_MESSAGES";
	}
	else
	{
		outDir = _releaseDir + "\\messages\\pt_BR\\LC_MESSAGES";
	}

	std::string msg = "copy " + directory + "*.mo " + outDir;

	return executeSystemOperation(msg);
}

bool ABTerraViewBuilder::copyLayoutDependencyFiles()
{
	std::string destDir = _releaseDir + "\\plugins\\layout";

	return copyLayoutDependency(_buildDir, _releaseDir, destDir);
}