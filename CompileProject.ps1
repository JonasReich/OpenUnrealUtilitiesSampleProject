# Powershell setting: Stop on first error
$ErrorActionPreference = "Stop"

# Find UAT (Unreal Automation Tool) and UBT (Unreal Build Tool)
$ScriptDirectory = Split-Path $MyInvocation.MyCommand.Path -Parent
$UProjectPath = Resolve-Path "$ScriptDirectory/*.uproject"
if ($UProjectPath -match "(?<ProjectName>[a-zA-Z]+).uproject") {
    $ProjectName = $Matches["ProjectName"]
} else {
    Write-Error "Project name not found in uproject file path '$UProjectPath'"
}

$UProjectJson = Get-Content -raw $UProjectPath
$UProject = ConvertFrom-Json -InputObject $UProjectJson
$EngineKey = $UProject.EngineAssociation
$EngineInstallRoot = (Get-ItemProperty "Registry::HKEY_LOCAL_MACHINE\SOFTWARE\EpicGames\Unreal Engine\$EngineKey" -Name "InstalledDirectory").InstalledDirectory

# Programs
$UAT = Resolve-Path "$EngineInstallRoot\Engine\Build\BatchFiles\RunUAT.bat"
$UBT = Resolve-Path "$EngineInstallRoot\Engine\Build\BatchFiles\Build.bat"
$UE4Editor = Resolve-Path "$EngineInstallRoot\Engine\Binaries\Win64\UE4Editor.exe"
$UE4EditorCmd = Resolve-Path "$EngineInstallRoot\Engine\Binaries\Win64\UE4Editor-Cmd.exe"

# Regenerate project files
#&$UBT -projectfiles -project="$UProjectPath" -game -rocket -progress

# Compile editor binaries with BuildGraph -> doesn't work ATM
&$UAT BuildGraph -script="$ScriptDirectory\Graph.xml" -target="Compile Game Editor" -set:ProjectName=$ProjectName -Set:ProjectDir="$ScriptDirectory" -Set:BuildConfig=Development

# Compile editor binaries
$EditorTargetName = $ProjectName+"Editor"
#&$UBT "$EditorTargetName" "Development" "Win64" -project="$UProjectPath" -NoHotReloadFromIDE -editorrecompile -progress -noubtmakefiles -utf8output

#&$UBT "$ProjectName" "Development" "Win64" -project="$UProjectPath" -NoHotReloadFromIDE -editorrecompile -progress -noubtmakefiles -utf8output


# Launch editor standalone
#&$UE4EditorCmd "$UProjectPath" -game

# Launch editor with tests
$EditorTestArgs = @("-editor", "-editortest")
$TestArgs = @("-unattended", "-buildmachine", "-stdout", "-nullrhi", "-nopause", "-nosplash")

$ExtraTests = "OpenUnrealUtilities" # Fill from cmdline?
$TestFilter = "$ProjectName+Project.Functional+$ExtraTests"
$RunTestCmdArg = "-ExecCmds=`"Automation RunTests Now $TestFilter;Quit`""
#&$UE4EditorCmd "$UProjectPath" $EditorTestArgs $RunTestCmdArg $TestArgs
