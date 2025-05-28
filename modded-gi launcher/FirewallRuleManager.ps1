param(
    [Parameter(Mandatory = $true, Position = 0)]
    [string]$RuleName,

    [Parameter(Mandatory = $true, Position = 1)]
    [ValidateSet("Enable", "Disable","Shift")]
    [string]$Action
)

$rule = Get-NetFirewallRule -DisplayName $RuleName -ErrorAction Continue

if (-not $rule) {
    Write-Debug "未找到名为 '$RuleName' 的防火墙规则" -ForegroundColor Red
    exit 1
}

if ($Action -eq "Enable") {
    Set-NetFirewallRule -DisplayName $RuleName -ErrorAction Continue -Enabled True
}

elseif($Action -eq "Disable") {
    Set-NetFirewallRule -DisplayName $RuleName -ErrorAction Continue -Enabled False
}
else { # $RuleName -eq "Shift"
    $currentStatus = $rule.Enabled
    if($currentStatus -eq $true) {
        Set-NetFirewallRule -DisplayName $RuleName -Enabled False -ErrorAction Continue
    }
    else {
        Set-NetFirewallRule -DisplayName $RuleName -Enabled True -ErrorAction Continue
    }
}