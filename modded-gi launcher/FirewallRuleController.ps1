param(
    [Parameter(Mandatory=$true)]
    [string]$RuleName,
    
    [Parameter(Mandatory=$true)]
    [ValidateSet("Enable", "Disable")]
    [string]$Action
)

# 检查管理员权限
if (-not ([Security.Principal.WindowsPrincipal][Security.Principal.WindowsIdentity]::GetCurrent()).IsInRole([Security.Principal.WindowsBuiltInRole]::Administrator)) {
    Write-Host "[$scriptName] 需要管理员权限运行此脚本" -ForegroundColor Red
    exit 1
}

$scriptFullPath = $MyInvocation.MyCommand.Path
$scriptName = Split-Path -Leaf $scriptFullPath

try {
    # 查找规则是否存在
    $rule = Get-NetFirewallRule -DisplayName $RuleName -ErrorAction Stop
    
    if ($null -eq $rule) {
        Write-Host "[$scriptName] 未找到名称为 '$RuleName' 的防火墙规则" -ForegroundColor Yellow
        exit 1
    }
    
    # 根据操作类型执行相应命令
    switch ($Action) {
        "Enable" {
            Enable-NetFirewallRule -DisplayName $RuleName
            Write-Host "[$scriptName] 启用 防火墙规则 成功: $RuleName" -ForegroundColor Green
        }
        "Disable" {
            Disable-NetFirewallRule -DisplayName $RuleName
            Write-Host "[$scriptName] 禁用 防火墙规则 成功: $RuleName" -ForegroundColor Green
        }
    }
    exit 0
}
catch {
    Write-Host "[$scriptName] 操作防火墙规则时出错: $_" -ForegroundColor Red
    exit 1
}