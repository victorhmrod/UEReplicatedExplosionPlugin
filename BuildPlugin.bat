@echo off

set EngineVersion=5.3

for /f "skip=2 tokens=2*" %%a in ('reg query "HKEY_LOCAL_MACHINE\SOFTWARE\EpicGames\Unreal Engine\%EngineVersion%" /v "InstalledDirectory"') do set "EngineDirectory=%%b"

set AutomationToolPath="C:\Program Files\EpicGames\UE_5.3\Engine\Build\BatchFiles\RunUAT.bat"
set PluginPath="E:\Unreal Projects\Sandbox\Plugins\ReplicatedExplosionPlugin\ReplicatedExplosionPlugin.uplugin"
set OutputPath="C:\Users\Henrique\Downloads\REP\Build\5.3"

title Build Plugin
echo Automation Tool Path: %AutomationToolPath%
echo:

call %AutomationToolPath% BuildPlugin -Plugin=%PluginPath% -Package=%OutputPath% -Rocket -TargetPlatforms=Win64
echo:
pause
exit 0
