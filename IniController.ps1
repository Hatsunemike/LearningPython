param(
    [ValidateSet("1","0")]
    [string]$Open = "1",

    [string]$Directory = "."
)

function Enable-IniFiles {
    param([string]$Path)
    
    Get-ChildItem -Path $Path -Recurse -File -Filter "*.ini.disabled" | ForEach-Object {
        $newName = $_.Name -replace '\.ini\.disabled$', '.ini'
        
        try {
            Rename-Item -Path $_.FullName -NewName $newName -ErrorAction Stop
            # Write-Host "已启用: $($_.FullName) -> $newName" -ForegroundColor Green
        }
        catch {
            Write-Host "错误: 无法重命名 $($_.FullName) - $($_.Exception.Message)" -ForegroundColor Red
        }
    }
}

function Disable-IniFiles {
    param([string]$Path)
    
    Get-ChildItem -Path $Path -Recurse -File -Filter "*.ini" | ForEach-Object {
        # 跳过已经是 .disabled 的文件
        if ($_.Name -notmatch '\.disabled$') {
            $newName = $_.Name + ".disabled"
            $newPath = Join-Path $_.Directory.FullName $newName
            
            # 检查目标文件是否已存在
            if (Test-Path $newPath) {
                Write-Host "跳过: $newName 已存在" -ForegroundColor Yellow
                return
            }
            
            try {
                Rename-Item -Path $_.FullName -NewName $newName -ErrorAction Stop
                Write-Host "已禁用: $($_.FullName) -> $newName" -ForegroundColor Yellow
            }
            catch {
                Write-Host "错误: 无法重命名 $($_.FullName) - $($_.Exception.Message)" -ForegroundColor Red
            }
        }
    }
}

# 验证目录存在
if (-not (Test-Path $Directory)) {
    Write-Host "错误: 目录 '$Directory' 不存在" -ForegroundColor Red
    exit 1
}

# 验证参数
if ($Open -ne "0" -and $Open -ne "1") {
    Write-Host "用法: .\ScriptName.ps1 -Open <0|1> [-Directory <路径>]" -ForegroundColor Cyan
    Write-Host "参数说明:" -ForegroundColor Cyan
    Write-Host "  -Open 0        : 禁用所有 .ini 文件（添加 .disabled 后缀）" -ForegroundColor Cyan
    Write-Host "  -Open 1        : 启用所有 .ini.disabled 文件（移除 .disabled 后缀）" -ForegroundColor Cyan
    Write-Host "  -Directory     : 指定目录（默认: 当前目录）" -ForegroundColor Cyan
    exit 1
}

# 执行相应操作
Write-Host "正在处理目录: $((Get-Item $Directory).FullName)" -ForegroundColor Magenta

if ($Open -eq "1") {
    # Write-Host "正在启用 .ini.disabled 文件..." -ForegroundColor Green
    Enable-IniFiles -Path $Directory
}
else {
    # Write-Host "正在禁用 .ini 文件..." -ForegroundColor Yellow
    Disable-IniFiles -Path $Directory
}

# Write-Host "操作完成!" -ForegroundColor Magenta