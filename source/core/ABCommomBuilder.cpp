#include <ABCommomBuilder.h>

#define TERRALIB_SVN_ADDRESS "https://svn.dpi.inpe.br/terralib/trunk"
#define TERRALIB_OGC_SVN_ADDRESS "https://svn.dpi.inpe.br/terraogc/trunk"
#define TDK_FRAMEWORK_SVN_ADDRESS "https://svn.funcate.org.br/tdk/trunk/tdk_framework"
#define TDK_APPLICATION_SVN_ADDRESS "https://svn.funcate.org.br/tdk_app/trunk/applications"

bool svnCheckoutApp(const std::string& svnPath, const std::string& outputDir)
{
	std::string msg = "svn co " + svnPath + " " + outputDir;

	return executeSystemOperation(msg);
}

bool svnUpdateApp(const std::string& svnPath)
{
	std::string msg = "svn up " + svnPath;

	return executeSystemOperation(msg);
}

bool compileApp(const std::string& solutionPath, const std::string& logFileName, const MSVC_VERSION& version, const MSVC_TYPE& type, const std::string& msvcPath)
{
	std::string buildMode = "";

	if(type == MSVC_RELEASE)
	{
		buildMode = "Release";
	}
	else if(type == MSVC_DEBUG)
	{
		buildMode = "Debug";
	}
	else if(type == MSVC_PLUGINDEBUG)
	{
		buildMode = "PluginDebug";
	}
	else if(type == MSVC_PLUGINRELEASE)
	{
		buildMode = "PluginRelease";
	}

	std::string deleteStr = "del " + logFileName;

	executeSystemOperation(deleteStr);

	if(version == MSVC_2003)
	{
		writeToFile("buildApp.bat", "SET PATH=" + msvcPath + ";%PATH%\n", "w");
	}
	else if(version == MSVC_2005)
	{
		writeToFile("buildApp.bat", "SET PATH=" + msvcPath + ";%PATH%\n", "w");
	}

	writeToFile("buildApp.bat", "devenv.com " + solutionPath + " /build " + buildMode + " /out \"" + logFileName + "\"", "a+");

	if(!executeSystemOperation("buildApp.bat"))
	{
		//last chance
		if(!executeSystemOperation("buildApp.bat"))
		{
			std::vector<std::string> vec;
			vec.push_back(logFileName);

			sendEmailFunc("eric.abreu@gmail.com", "Auto Builder", "Build error.", vec);

			return false;
		}
	}

	deleteStr = "del buildApp.bat";

	executeSystemOperation(deleteStr);
	
	return true;
}

bool copyTrFiles(const std::string& directory, const bool& pluginsTrFiles, const std::string releaseDir, std::string outDir)
{
	if(pluginsTrFiles)
	{
		if(outDir.empty())
		{
			outDir = releaseDir + "\\plugins";
		}
	}
	else
	{
		if(outDir.empty())
		{
			outDir = releaseDir;
		}
	}

	std::string msg = "copy " + directory + "*.qm " + outDir;

	return executeSystemOperation(msg);
}

bool copyQtDependencyFiles(const std::string& releaseDir)
{
	std::string qtDir = "%QTDIR%\\lib\\";
	std::string msg = "copy " + qtDir + "*.dll " + releaseDir;

	return executeSystemOperation(msg);
}

bool executeSystemOperation(const std::string& opStr)
{
	int res = system(opStr.c_str());

	if(res == 0)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void writeToFile(const std::string& fileName, const std::string& text, const std::string& mode)
{
	FILE *f;

	f = fopen(fileName.c_str(), mode.c_str());

	if(f != NULL)
	{
		fprintf(f, "%s", text.c_str());

		fclose(f);
	}
}

void changeSlash(std::string& str)
{
	std::string fileNameCopy = str;

	for(unsigned int  i = 0; i < fileNameCopy.size(); ++i)
	{
		if(fileNameCopy[i] == '/')
		{
			fileNameCopy[i] = '\\';
		}
	}

	str = fileNameCopy;
}
bool sendEmailFunc(const std::string& email, const std::string& subject, const std::string& body, const std::vector<std::string>& attachedVecList)
{
	std::string msg = "sendEmail";
				msg+= " -f eric.abreu@funcate.org.br";
				msg+= " -t " + email;
				msg+= " -u " + subject;
				msg+= " -m " + body;

				for(unsigned int i = 0; i < attachedVecList.size(); ++i)
				{
					msg += " -a " + attachedVecList[i];
				}

				msg+= " -s mail.funcate.org.br -xu eric.abreu -xp mwtbdltr";
	
 	return executeSystemOperation(msg);
}

bool getTerraLibWorkspace(const std::string& buildDir)
{
//checkout terralib repository
	std::string outputDir = buildDir + "/terralib";

	if(!svnCheckoutApp(TERRALIB_SVN_ADDRESS, outputDir))
	{
		return false;
	}

	return true;
}

bool getTerraLibOGCWorkspace(const std::string& buildDir)
{
	//checkout terralib repository
	std::string outputDir = buildDir + "/terraogc";

	if(!svnCheckoutApp(TERRALIB_OGC_SVN_ADDRESS, outputDir))
	{
		return false;
	}

	return true;
}

bool getTDKWorkspace(const std::string& buildDir)
{
	//checkout tdkframework repository
	std::string outputDir = buildDir + "/tdk_framework";

	if(!svnCheckoutApp(TDK_FRAMEWORK_SVN_ADDRESS, outputDir))
	{
		return false;
	}

//checkout application repository
	outputDir = buildDir + "/applications";

	if(!svnCheckoutApp(TDK_APPLICATION_SVN_ADDRESS, outputDir))
	{
		return false;
	}

	return true;
}

bool buildPDIPlugin(const std::string& fullDir, const std::string& logFileName, const MSVC_VERSION& version, const MSVC_TYPE& type, const std::string& msvcPath)
{
	if(version == MSVC_2005)
	{
		std::string solutionFile = fullDir + "applications/terrabasePlugins/pdiPlugin/mak.vc8/TdkPDIPlugin.sln";

		if(!compileApp(solutionFile, logFileName, MSVC_2005, type, msvcPath))
		{
			return false;
		}
	}
	else if(version == MSVC_2003)
	{
		std::string solutionFile = fullDir + "applications/terrabasePlugins/pdiPlugin/mak.vc71/TdkPDIPlugin.sln";

		if(!compileApp(solutionFile, logFileName, MSVC_2003, type, msvcPath))
		{
			return false;
		}
	}
	else
	{
		return false;
	}

	return true;
}

bool buildEditionPlugin(const std::string& fullDir, const std::string& logFileName, const MSVC_VERSION& version, const MSVC_TYPE& type, const std::string& msvcPath)
{
	if(version == MSVC_2005)
	{
		std::string solutionFile = fullDir + "applications/terrabasePlugins/TdkBasicEditionPlugin/TdkBasicEditionPlugin-msvc2005.sln";

		if(!compileApp(solutionFile, logFileName, MSVC_2005, type, msvcPath))
		{
			return false;
		}
	}
	else if(version == MSVC_2003)
	{
		std::string solutionFile = fullDir + "applications/terrabasePlugins/TdkBasicEditionPlugin/TdkBasicEditionPlugin.sln";

		if(!compileApp(solutionFile, logFileName, MSVC_2003, type, msvcPath))
		{
			return false;
		}
	}
	else
	{
		return false;
	}

	return true;
}

bool copyPDITrFiles(const std::string& buildDir, const std::string releaseDir, std::string outDir)
{
	std::string pdiPlugin = buildDir + "\\applications\\terrabasePlugins\\pdiPlugin\\TdkPDIPlugin\\";

//pdi main translate file
	copyTrFiles(pdiPlugin, true, releaseDir, outDir);

	//pdi pdiAntennaPattern translate file
	std::string pdiAntennaPattern = pdiPlugin + "pdiAntennaPattern\\";
	copyTrFiles(pdiAntennaPattern, true, releaseDir, outDir);

	//pdi pdiArithmeticOp translate file
	std::string pdiArithmeticOp = pdiPlugin + "pdiArithmeticOp\\";
	copyTrFiles(pdiArithmeticOp, true, releaseDir, outDir);

	//pdi pdiClassification translate file
	std::string pdiClassification = pdiPlugin + "pdiClassification\\";
	copyTrFiles(pdiClassification, true, releaseDir, outDir);

	//pdi pdiColorTransform translate file
	std::string pdiColorTransform = pdiPlugin + "pdiColorTransform\\";
	copyTrFiles(pdiColorTransform, true, releaseDir, outDir);

	//pdi pdiCommon translate file
	std::string pdiCommon = pdiPlugin + "pdiCommon\\";
	copyTrFiles(pdiCommon, true, releaseDir, outDir);

	//pdi pdiContrast translate file
	std::string pdiContrast = pdiPlugin + "pdiContrast\\";
	copyTrFiles(pdiContrast, true, releaseDir, outDir);

	//pdi pdiConvertSlantGround translate file
	std::string pdiConvertSlantGround = pdiPlugin + "pdiConvertSlantGround\\";
	copyTrFiles(pdiConvertSlantGround, true, releaseDir, outDir);

	//pdi pdiFusion translate file
	std::string pdiFusion = pdiPlugin + "pdiFusion\\";
	copyTrFiles(pdiFusion, true, releaseDir, outDir);

	//pdi pdiMixModel translate file
	std::string pdiMixModel = pdiPlugin + "pdiMixModel\\";
	copyTrFiles(pdiMixModel, true, releaseDir, outDir);

	//pdi pdiMorphFilter translate file
	std::string pdiMorphFilter = pdiPlugin + "pdiMorphFilter\\";
	copyTrFiles(pdiMorphFilter, true, releaseDir, outDir);

	//pdi pdiMosaic translate file
	std::string pdiMosaic = pdiPlugin + "pdiMosaic\\";
	copyTrFiles(pdiMosaic, true, releaseDir, outDir);

	//pdi pdiMultiRes translate file
	std::string pdiMultiRes = pdiPlugin + "pdiMultiRes\\";
	copyTrFiles(pdiMultiRes, true, releaseDir, outDir);

	//pdi pdiRasterCompose translate file
	std::string pdiRasterCompose = pdiPlugin + "pdiRasterCompose\\";
	copyTrFiles(pdiRasterCompose, true, releaseDir, outDir);

	//pdi pdiRasterCut translate file
	std::string pdiRasterCut = pdiPlugin + "pdiRasterCut\\";
	copyTrFiles(pdiRasterCut, true, releaseDir, outDir);

	//pdi pdiRasterViewer translate file
	std::string pdiRasterViewer = pdiPlugin + "pdiRasterViewer\\";
	copyTrFiles(pdiRasterViewer, true, releaseDir, outDir);

	//pdi pdiReg translate file
	std::string pdiReg = pdiPlugin + "pdiReg\\";
	copyTrFiles(pdiReg, true, releaseDir, outDir);

	//pdi pdiRestoration translate file
	std::string pdiRestoration = pdiPlugin + "pdiRestoration\\";
	copyTrFiles(pdiRestoration, true, releaseDir, outDir);

	//pdi pdiSaveRaster translate file
	std::string pdiSaveRaster = pdiPlugin + "pdiSaveRaster\\";
	copyTrFiles(pdiSaveRaster, true, releaseDir, outDir);

	//pdi pdiSegmentation translate file
	std::string pdiSegmentation = pdiPlugin + "pdiSegmentation\\";
	copyTrFiles(pdiSegmentation, true, releaseDir, outDir);

	//pdi pdiSupClassification translate file
	std::string pdiSupClassification = pdiPlugin + "pdiSupClassification\\";
	copyTrFiles(pdiSupClassification, true, releaseDir, outDir);

	//pdi pdiVector2Raster translate file
	std::string pdiVector2Raster = pdiPlugin + "pdiVector2Raster\\";
	copyTrFiles(pdiVector2Raster, true, releaseDir, outDir);

	//pdi pdiVector2Raster translate file
	std::string pdiRaster2Vector = pdiPlugin + "pdiRaster2Vector\\";
	copyTrFiles(pdiRaster2Vector, true, releaseDir, outDir);

	return true;
}

bool copyLayoutDependency(const std::string& buildDir, const std::string& releaseDir, const std::string& destDir)
{
	//create layout directory
	std::string msgCreateDir = "mkdir " + releaseDir + "\\layout";

	if(!executeSystemOperation(msgCreateDir))
	{
		return false;
	}

//copy dependency files
	std::string sourceDir = buildDir + "\\tdk_framework\\tdklayout\\dependencies\\";

	std::string copyPNGFiles = sourceDir + "*.png";

	std::string msgCopyPNGFiles = "copy  " + copyPNGFiles + " " + destDir;

	if(!executeSystemOperation(msgCopyPNGFiles))
	{
		return false;
	}

	std::string copyTTFFiles = sourceDir + "*.TTF";

	std::string msgCopyTTFFiles = "copy  " + copyTTFFiles + " " + destDir;

	if(!executeSystemOperation(msgCopyTTFFiles))
	{
		return false;
	}

	return true;
}