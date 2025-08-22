param(
    [Parameter(Mandatory = $true)]
    [string]$NetAdapterName,

    [int]$ImtDuration = 3600
)

function Disable-NetAdapter-Log {
    param(
        [Parameter(Mandatory=$true)]
        [int]$Duration
    )
    Disable-NetAdapter -Name $NetAdapterName -Confirm:$false
    Write-Host "断网，持续 $Duration 秒"
    Start-Sleep -Seconds $Duration
}

function Enable-NetAdapter-Log {
    param(
        [Parameter(Mandatory=$true)]
        [int]$Duration
    )
    Enable-NetAdapter -Name $NetAdapterName -Confirm:$false
    Write-Host "联网，持续 $Duration 秒"
    Start-Sleep -Seconds $Duration
}

$ImtEnableTime = 8
$ImtDisableTime = 0

Write-Host "=== 开始间歇性断网控制（持续 $ImtDuration 秒）==="
$stopwatch = [System.Diagnostics.Stopwatch]::StartNew()

while ($stopwatch.Elapsed.TotalSeconds -lt $ImtDuration) {
    # 断网
    Disable-NetAdapter-Log($ImtDisableTime)
    # 联网
    Enable-NetAdapter-Log($ImtEnableTime)
}

# 最终确保联网
if ((Get-NetAdapter -Name $NetAdapterName).Status -ne "Up") {
    Enable-NetAdapter -Name $NetAdapterName -Confirm:$false
}