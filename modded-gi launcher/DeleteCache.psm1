function DeleteCache {
    param(
        [Parameter(Mandatory = $false)]
        [ValidateSet("CN","Inter")]
        [string]$Server = "CN"
    )
    try{
        $configJson = Get-Content -Path ".\config.json" -Raw | ConvertFrom-Json
        $gameDataDir = $configJson.gameDataDir.$Server.directory
    } catch {
        Write-Error "DeleteCache: 配置未找到：$_"
        return
    }
    foreach($item in $configJson.cacheDir) {
        $cachePath = Join-Path $gameDataDir $item
        try {
            Remove-Item -Path "$cachePath" -Recurse -ErrorAction Stop
        } catch [System.Management.Automation.ItemNotFoundException] {
            # Write-Warning "路径不存在：$cachePath"
        }
    }
    Write-Host "DeleteCache: 删除缓存完成"
    return
}