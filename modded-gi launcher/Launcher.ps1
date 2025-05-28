param(
    [Parameter(Mandatory = $false)]
    [ValidateSet("CN","Inter")]
    [string]$Server = "CN",

    [Parameter(Mandatory = $false)]
    [switch]$ConstOw
)

$FirewallRuleManager = Resolve-Path "./FirewallRuleManager.ps1"

$FirewallRule = "原神 禁所有连接"
if($Server -eq "CN") {
    $FirewallRule = "原神外服 禁所有连接"
}

$gimiHome = "D:\XXMI\GIMI"
$xxmiLauncher = "D:\XXMI\Resources\Bin\XXMI Launcher.exe"

function OverwriteModConstant {
    if(-not (Test-Path ".\d3dx_user.ini")) {
        Write-Host "[Warning] OverwriteModConstant: 未找到用于覆写的d3dx_user.ini，覆写中止。"
        return
    }
    Copy-Item ".\d3dx_user.ini" "$gimiHome\d3dx_user.ini" -Force
    Write-Host "d3dx_user.ini have been overwrited" -ForegroundColor White
}

try {
    if($ConstOw) {
        OverwriteModConstant
    }
    exit 0
    & $FirewallRuleManager -RuleName $FirewallRule -Action "Enable"
    Start-Process -FilePath $xxmiLauncher -ArgumentList "--nogui","--xxmi","GIMI"
    Start-Sleep -Seconds 10
    & $FirewallRuleManager -RuleName $FirewallRule -Action "Disable"
    Write-Host "脚本执行完成" -ForegroundColor Green
    exit 0
} catch {
    Write-Host "脚本执行失败" -ForegroundColor Red
    exit 1
}