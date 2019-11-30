$version = (cmake . -Bbuild/production -DCMAKE_BUILD_TYPE=Release -G "NMake Makefiles" | Select-String '-- inform version: ')
$version = $version.ToString().Split(' ')[3]

$prefix = "inform-$version"
$target = "dist/$prefix"
$archive = "$($target)_win-amd64.zip"

Copy-Item LICENSE $target
Copy-Item README.adoc $target
Copy-Item CHANGELOG.md $target
Copy-Item -Recurse include $target
New-Item -Type Directory $target/lib
Copy-Item inform.dll $target/lib
Copy-Item inform.lib $target/lib
Copy-Item inform_static.lib $target/lib

Remove-Item -force $archive
Add-Type -Assembly System.IO.Compression.FileSystem
$compressionLevel = [System.IO.Compression.CompressionLevel]::Optimal
[System.IO.Compression.ZipFile]::CreateFromDirectory($target, $archive, $compressionLevel, $true)

$osname = (Get-WmiObject Win32_OperatingSystem).Name
$osarch = (Get-WmiObject Win32_OperatingSystem).OSArchitecture
$machname = (Get-WmiObject Win32_OperatingSystem).CSName
