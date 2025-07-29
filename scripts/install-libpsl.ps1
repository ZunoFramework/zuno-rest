# Variables
$LibpslRepo = "https://github.com/rockdaboot/libpsl.git"
$LibpslDir = "$env:USERPROFILE/libpsl"
$BuildMode = "release"

# Requisitos básicos
choco install make git python -y

# Clonar el repositorio
if (-Not (Test-Path $LibpslDir)) {
  git clone $LibpslRepo $LibpslDir
}

# Cambiar a carpeta msvc
Set-Location "$LibpslDir\msvc"

# Compilar usando nmake (Developer Command Prompt requerido)
nmake /f Makefile.vc CFG=$BuildMode

# Rutas de salida típicas
$LibDir = "$LibpslDir\msvc\$BuildMode"
Write-Host "Libpsl compilado en: $LibDir"

# Opcional: exportar variables para CMake
echo "LIBPSL_INCLUDE_DIR=$LibpslDir\include" >> $env:GITHUB_ENV
echo "LIBPSL_LIBRARY=$LibDir\psl.lib" >> $env:GITHUB_ENV