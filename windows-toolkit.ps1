$PROJECT_ROOT = (Resolve-Path .\).Path
$MOZILLA_BUILD = "mozilla-build"
$MOZILLA_TOOLKIT = "MozillaBuildSetup-2.2.0.exe"
$7ZIP = "7z1604-x64.msi"
$NODE = "node-v6.7.0-x64.msi"
$CMAKE = "cmake-3.7.0-rc1-win64-x64.msi"
$SHELL = "start-shell-msvc2015-x64.bat"

Write-Host "Started build in $PROJECT_ROOT"
md -Force $PROJECT_ROOT\$MOZILLA_BUILD | Out-Null

###################################################
################### 7 - Z I P  ####################
###################################################

# Install 7-Zip if it was not yet installed
if(![System.IO.File]::Exists("$PROJECT_ROOT\$MOZILLA_BUILD\7zip-install.log")){
	if(![System.IO.File]::Exists("$PROJECT_ROOT\$MOZILLA_BUILD\$7ZIP")){
		Write-Host "Downloading 7-Zip as $PROJECT_ROOT\$MOZILLA_BUILD\7ZIP"
		Invoke-WebRequest "http://www.7-zip.org/a/$7ZIP" -OutFile "$MOZILLA_BUILD\$7ZIP"
	}
	Write-Host "Installing 7-Zip from $PROJECT_ROOT\$MOZILLA_BUILD\7ZIP"
	msiexec /i $PROJECT_ROOT\$MOZILLA_BUILD\$7ZIP /qn /log $PROJECT_ROOT\$MOZILLA_BUILD\7zip-install.log | Out-Null
}

###################################################
################# N O D E . J S ###################
###################################################

# Install Nodejs if it was not yet installed
if(![System.IO.File]::Exists("$PROJECT_ROOT\$MOZILLA_BUILD\nodejs-install.log")){
	if(![System.IO.File]::Exists("$PROJECT_ROOT\$MOZILLA_BUILD\$NODE")){
		Write-Host "Downloading Node.js as $PROJECT_ROOT\$MOZILLA_BUILD\$NODE"
		Invoke-WebRequest "https://nodejs.org/dist/v6.7.0/$NODE" -OutFile "$MOZILLA_BUILD\$NODE"
	}

	Write-Host "Installing Node.js from $PROJECT_ROOT\$MOZILLA_BUILD\$NODE"
	msiexec /i $PROJECT_ROOT\$MOZILLA_BUILD\$NODE /qn /log $PROJECT_ROOT\$MOZILLA_BUILD\nodejs-install.log | Out-Null
}
$Env:Path += ";C:\Program Files\nodejs"

###################################################
#################### C M A K E ####################
###################################################

# Install CMAKE if it was not yet installed
if(![System.IO.File]::Exists("$PROJECT_ROOT\$MOZILLA_BUILD\cmake-install.log")){
	if(![System.IO.File]::Exists("$PROJECT_ROOT\$MOZILLA_BUILD\$CMAKE")){
		Write-Host "Downloading CMake as $PROJECT_ROOT\$MOZILLA_BUILD\$CMAKE"
		Invoke-WebRequest "https://cmake.org/files/v3.7/$CMAKE" -OutFile "$MOZILLA_BUILD\$CMAKE"
	}

	Write-Host "Installing CMake from $PROJECT_ROOT\$MOZILLA_BUILD\$CMAKE"
	msiexec /i $PROJECT_ROOT\$MOZILLA_BUILD\$CMAKE /qn /log $PROJECT_ROOT\$MOZILLA_BUILD\cmake-install.log | Out-Null
}
$Env:Path += ";C:\Program Files\CMake\bin"

###################################################
################# M O Z I L L A ###################
###################################################

if(![System.IO.File]::Exists("$PROJECT_ROOT\$MOZILLA_BUILD\toolkit-install.log")){
	if(![System.IO.File]::Exists("$PROJECT_ROOT\$MOZILLA_BUILD\$MOZILLA_TOOLKIT")){
		Write-Host "Downloading Mozilla build utils as $PROJECT_ROOT\$MOZILLA_BUILD\$MOZILLA_TOOLKIT"
		Invoke-WebRequest "https://ftp.mozilla.org/pub/mozilla/libraries/win32/$MOZILLA_TOOLKIT" -OutFile "$PROJECT_ROOT\$MOZILLA_BUILD\$MOZILLA_TOOLKIT"
	}

	Write-Host "Extracting Mozilla build utils to $PROJECT_ROOT\$MOZILLA_BUILD"
	Start-Process "C:\Program Files\7-Zip\7z.exe" "x $PROJECT_ROOT\$MOZILLA_BUILD\$MOZILLA_TOOLKIT -o$PROJECT_ROOT\$MOZILLA_BUILD" -NoNewWindow -Wait
	"Success" > $PROJECT_ROOT\$MOZILLA_BUILD\toolkit-install.log
}
$Env:Path += ";$PROJECT_ROOT\$MOZILLA_BUILD\msys\bin"

###################################################
#################### I N I T ######################
###################################################

$Env:STARTUP_DIR = $PROJECT_ROOT
$Env:HOME = $PROJECT_ROOT


write-output @"
if [ -f ~/.bashrc ]; then . ~/.bashrc; fi
"@ | out-file ".bash_profile" -encoding utf8

write-output @"
if [[ \`"$STARTUP_DIR\`" != \`"$PWD\`" ]]; then
    cd $STARTUP_DIR
fi
export PATH=`$PATH:/c/Program\ Files/nodejs
export PATH=`$PATH:/c/Program\ Files/CMake/bin
export PATH=~/$MOZILLA_BUILD/bin:`$PATH
./build.sh
"@ | out-file ".bashrc" -encoding utf8

Start-Process "dos2unix.exe" ".bashrc" -Wait -NoNewWindow
Start-Process "dos2unix.exe" ".bash_profile" -Wait -NoNewWindow

cmd /c $PROJECT_ROOT\$MOZILLA_BUILD\$SHELL