import os;
import json;
import urllib;
import multiprocessing


from PyUtils import *
from PyUtils import ProJucerUtils

njobs = multiprocessing.cpu_count()

# configuration  = "Release"
configuration  = "Debug"
bumpVersion = False
sendToOwncloud = True
specificVersion = ""
cleanFirst = False;
localExportPath2 = [
# "/Volumes/Thor/OO\ Projets/OwnCloud/Tools/LGML/App-Dev/OSX/"
# ,"/Volumes/Pguillerme/Documents/LGML/"
];
architecture = "i386"



localExportPath = "../Builds/MacOSX/build/"
localExportPath = os.path.abspath(localExportPath)+"/"

xcodeProjPath = "../Builds/MacOSX/" 
executable_name = "LGML"+("" if configuration=="Release" else "_"+configuration)
gitPath = "../../../"
appPath = xcodeProjPath+"build/"+configuration+"/"+executable_name+".app"



def generateProductBaseName():
	return executable_name+ "_v"+str(ProJucerUtils.getVersion())



def buildApp(xcodeProjPath,configuration,appPathm,njobs,clean = False):
	if len(appPath)>10:
		sh("rm -rf "+appPath)

	if clean:
		sh("cd "+xcodeProjPath+ " && "\
		+" xcodebuild -project LGML.xcodeproj" \
		+" -configuration "+configuration
		+" clean")

	sh("cd "+xcodeProjPath+ " && "\
		+" xcodebuild -project LGML.xcodeproj" \
		+" -configuration "+configuration
		+" -arch "+architecture
		+" -jobs "+str(njobs))



def createDmg(exportFileBaseName,appPath):
	if sh("which appdmg")!="":
		jsonPath = "dmgExport.json"

		def createAppdmgJSON(appPath ,destPath):
			jdata =  {
		  	"title": "Le Grand Mechant Loop",
		  	"icon": "",
		  	"background": "../Resources/grandlouloup.png",
		  	"icon-size": 80,
			"contents": [
			{ "x": 448, "y": 304, "type": "link", "path": "/Applications" },
			{ "x": 192, "y": 304, "type": "file", "path": appPath}]
			}


			with open(destPath,'w') as f:
				json.dump(jdata,f)

		createAppdmgJSON(appPath,jsonPath)
		dmgPath = exportFileBaseName+".dmg"
		sh("rm -f \""+dmgPath+"\"")
		sh("appdmg "+jsonPath+" \""+dmgPath+"\"")
		sh("rm "+jsonPath)
		return dmgPath

	else:
		print "no appdmg exporter : using zip"
		sh("zip -rv9 \""+exportFileBaseName+".zip\" \""+appPath+"\"")


def sendToOwnCloud(originPath,destPath):
	credPath = os.path.dirname(os.path.abspath(__file__));
	credPath = os.path.join(credPath,"owncloud.password")

	with open(credPath) as json_data:
		credentials = json.loads(json_data.read())

	sh("curl -X PUT \"https://163.172.42.66/owncloud/remote.php/webdav/"+destPath+"\" --data-binary @\""+originPath+"\" -u "+credentials["pass"]+" -k")

# print executeCmd(proJucerPath+ " --status "+ projectPath)

# formatCode("../Source");
if __name__ == "__main__":
	ProJucerUtils.updateVersion(bumpVersion,specificVersion);
	ProJucerUtils.buildJUCE();
	buildApp(xcodeProjPath,configuration,appPath,njobs,cleanFirst);

	localPath = localExportPath+generateProductBaseName();
	dmgPath = createDmg(localPath,appPath);
	for p in localExportPath2:
		sh("cp "+dmgPath+" "+p+generateProductBaseName()+".dmg")
	if sendToOwncloud:
		ownCloudPath = "Tools/LGML/App-Dev/OSX/1.x-2.0/"+generateProductBaseName()+".dmg"
		sendToOwnCloud(localPath+".dmg",urllib.pathname2url(ownCloudPath))
	# gitCommit()

