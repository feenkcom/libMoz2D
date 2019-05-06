$VerbosePreference = "Continue"
Write-Host "Started build in"

$PROJECT_ROOT = (Resolve-Path .\).Path
$MOZILLA_BUILD = "mozilla-build"
$MOZILLA_TOOLKIT = "MozillaBuildSetup-2.2.0.exe"
$7ZIP = "7z1604-x64.msi"
$NODE = "node-v6.7.0-x64.msi"
$CMAKE = "cmake-3.7.0-rc1-win64-x64.msi"
$SHELL = "start-shell-msvc2015-x64.bat"

$VerbosePreference = "Continue"
md -Force $PROJECT_ROOT\$MOZILLA_BUILD | Out-Null

###################################################
################### 7 - Z I P  ####################
###################################################

# Install 7-Zip if it was not yet installed
if(![System.IO.File]::Exists("$PROJECT_ROOT\$MOZILLA_BUILD\7zip-install.log")){
	if(![System.IO.File]::Exists("$PROJECT_ROOT\$MOZILLA_BUILD\$7ZIP")){
		Invoke-WebRequest "http://www.7-zip.org/a/$7ZIP" -OutFile "$MOZILLA_BUILD\$7ZIP"
	}
	msiexec /i $PROJECT_ROOT\$MOZILLA_BUILD\$7ZIP /qn /log $PROJECT_ROOT\$MOZILLA_BUILD\7zip-install.log | Out-Null
}