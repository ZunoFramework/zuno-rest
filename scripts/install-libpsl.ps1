$LibpslDir = "$PSScriptRoot\deps\libpsl"
$BuildDir = "$LibpslDir\build"

if (-not (Test-Path $LibpslDir)) {
    git clone https://github.com/rockdaboot/libpsl.git $LibpslDir
}

if (-not (Test-Path $BuildDir)) {
    New-Item -ItemType Directory -Path $BuildDir | Out-Null
}

cmake -S $LibpslDir -B $BuildDir `
    -DCMAKE_BUILD_TYPE=Release `
    -DCMAKE_INSTALL_PREFIX="$LibpslDir\install" `
    -DBUILD_SHARED_LIBS=ON `
    -DPSL_BUILD_TESTS=OFF

cmake --build $BuildDir --target install --config Release
