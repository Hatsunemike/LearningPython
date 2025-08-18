
<#
.SYNOPSIS
  原神启动器防火墙控制脚本
.DESCRIPTION
  0. 删除缓存，修改游戏客户端路径（XXMI配置）
     修改unlockerfps_un配置
  1. 自动启用"原神 禁所有连接"防火墙规则
  2. 启动unlocker_ 
  3. 禁用"原神 禁所有连接"防火墙规则
.NOTES
  需要管理员权限运行
#>

param(
    [ValidateSet("NetOffLaunch", "NetOffLaunch-Imt")]
    [string]$Mode = "NetOffLaunch", 

    # 选择对应服务器的防火墙规则
    [ValidateSet("CN","Inter")]
    [string]$ServerRule = "CN",

    [Parameter(Mandatory = $false)]
    [switch]$ConstOw,

    [Parameter(Mandatory = $false)]
    [bool]$DelCache = $true
)

# 更改工作目录
Set-Location -Path $PSScriptRoot

# 检查管理员权限
if (-not ([Security.Principal.WindowsPrincipal][Security.Principal.WindowsIdentity]::GetCurrent()).IsInRole([Security.Principal.WindowsBuiltInRole]::Administrator)) {
    Write-Host "错误：需要管理员权限运行此脚本" -ForegroundColor Red
    Read-Host "按回车键退出..."
    exit 0
}

# 导入模组
Import-Module .\DeleteCache.psm1, .\Overwrited3dxuser.psm1 -Force

# 导入配置
try {
    $configJson = Get-Content .\config.json -Raw | ConvertFrom-Json
} catch {
    Write-Warning "导入用户配置失败，使用默认配置。ErrorMessage: $_"
    Copy-Item .\config_default.json .\config.json
    $configJson = Get-Content .\config.json -Raw | ConvertFrom-Json
}

# 定义常量
$ControllerPath = Resolve-Path ".\FirewallRuleController.ps1"
$FirewallRuleName = "原神 禁主程序所有连接"
if($ServerRule -eq "Inter") {
    $FirewallRuleName = "原神外服 禁所有连接"
}
$XXMILaucherPath = Join-Path $configJson.XXMI_HOME "Resources\Bin\XXMI Launcher.exe"
$XXMILaucherArgs = "--nogui --xxmi GIMI"

try {

    # 0. 清除缓存+修改游戏客户端路径

    if($DelCache) {
        . ".\AA-DeleteCache.exe"
    }
    . .\ChangeClientPath.exe $ServerRule
    if($LASTEXITCODE -ne 0) {
        Write-Warning "ClientPath更改出错"
    }

    # （可选）覆写d3dx_user.ini
    if ($ConstOw) {
        Overwrited3dxuser("Overwrite")
    }

    # 1. 启动防火墙
    # Write-Host "成功启用 $($rules.Count) 条防火墙规则" -ForegroundColor Green
    & $ControllerPath -RuleName $FirewallRuleName -Action "Enable"
    if ($LASTEXITCODE -ne 0) {
        throw "防火墙启用失败，程序终止"
    }

    # 2. 启动XXMI Launcher
    Write-Host "正在启动 XXMI Launcher..."
    $process = Start-Process -FilePath $XXMILaucherPath -ArgumentList $XXMILaucherArgs -PassThru -ErrorAction Stop
    
    if (-not $process) {
        throw "无法启动 XXMI Launcher"
    }
    
    Write-Host "XXMI Launcher 已启动 (PID: $($process.Id))" -ForegroundColor Green

    # 3. 等待5秒
    Start-Sleep -Seconds 20

    # 4. 禁用防火墙
    # Write-Host "正在禁用防火墙规则: $FirewallRuleName..."
    # $rules | Disable-NetFirewallRule -ErrorAction Stop
    # Write-Host "成功禁用 $($rules.Count) 条防火墙规则" -ForegroundColor Green

    & $ControllerPath -RuleName $FirewallRuleName -Action "Disable"
    if ($LASTEXITCODE -ne 0) {
        Write-Host "警告！防火墙禁用失败，需要手动禁用" -ForegroundColor Yellow
    }

    # (Mode == "NetOffLaunch-Imt") 频繁断网
    if ($Mode -eq "NetOffLaunch-Imt") {
        $NetOffImtSciptPath = ".\IntermittentBlocking.ps1"
        & $NetOffImtSciptPath -RuleName $FirewallRuleName -ControllerPath $ControllerPath -ImtDuration 600
    }

    Write-Host "脚本执行完成" -ForegroundColor Green
}
catch {
    Write-Host "错误：$_" -ForegroundColor Red
    Read-Host "按回车键退出..."
    exit 1
}