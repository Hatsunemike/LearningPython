param(
    [Parameter(Mandatory = $true)]
    [string]$RuleName,

    [int]$ImtDuration = 300,

    [string]$ControllerPath = ".\FirewallRuleController.ps1"
)

$ImtEnableTime = 1
$ImtDisableTime = 5

Write-Host "=== 开始间歇性断网控制（持续 $ImtDuration 秒）==="
$stopwatch = [System.Diagnostics.Stopwatch]::StartNew()

while ($stopwatch.Elapsed.TotalSeconds -lt $ImtDuration) {
    # 启用规则5秒
    & $ControllerPath -RuleName $RuleName -Action "Enable"
    Start-Sleep -Seconds $ImtEnableTime
    
    # 检查是否超时
    if ($stopwatch.Elapsed.TotalSeconds -ge $ImtDuration) { break }
    
    # 禁用规则1秒
    & $ControllerPath -RuleName $RuleName -Action "Disable"
    Start-Sleep -Seconds $ImtDisableTime
}

# 最终确保规则是禁用的
& $ControllerPath -RuleName $RuleName -Action "Disable"
Write-Host "=== 间歇性断网控制结束 ===" -ForegroundColor Yellow