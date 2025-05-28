function Overwrited3dxuser{
    param(
        [Parameter(Mandatory = $false)]
        [ValidateSet("Overwrite","Storage")]
        [String]$Mode = "Overwrite"
    )
    if($Mode -eq "Overwrite") {
        Write-Host "Overwrited3dxuser 模式：覆写"
    }
    elseif ($Mode -eq "Storage") {
        Write-Host "Overwrited3dxuser 模式：存储"
    }
    try {
        $configJson = Get-Content .\config.json -Raw | ConvertFrom-Json
        $xxmiConfigPath = Join-Path $configJson.XXMI_HOME "XXMI Launcher Config.json"
        $xxmiConfigJson = Get-Content $xxmiConfigPath -Raw | ConvertFrom-Json
        $gimiPath = Join-Path $configJson.XXMI_HOME $xxmiConfigJson.Importers.GIMI.Importer.importer_folder

        $d3dxPath = Join-Path $gimiPath "d3dx_user.ini"
        if ($Mode -eq "Overwrite") { # 使用.\d3dx_user.ini 覆写 $gimiPath\d3dx_user.ini
            Copy-Item ".\d3dx_user.ini" $d3dxPath -Force
        }
        elseif($Mode -eq "Storage") { # 将$gimiPath\d3dx_user.ini 存储到 .\d3dx_user.ini
            Copy-Item $d3dxPath ".\d3dx_user.ini" -Force
        }
    } catch {
        Write-Warning "覆写d3dx_user.ini失败, Errormessage: $_"
        return
    }
    Write-Host "覆写d3dx_user.ini成功"
    return
}