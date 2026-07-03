# GPG Setup Script for Marble ARM64 Windows Fork
# Run this once to enable signed commits with "Verified" badge on GitHub
#
# Usage: .\setup-gpg.ps1

param(
    [string]$Email = "",
    [string]$Name = ""
)

Write-Host "=== GPG Setup for dp1x/marble-arm64-windows ===" -ForegroundColor Cyan
Write-Host ""

# Step 1: Check if GPG is installed
Write-Host "[1/5] Checking GPG installation..." -ForegroundColor Yellow
$gpgPath = Get-Command gpg -ErrorAction SilentlyContinue
if (-not $gpgPath) {
    Write-Host "ERROR: GPG is not installed." -ForegroundColor Red
    Write-Host "Download from: https://gnupg.org/download/index.html" -ForegroundColor White
    Write-Host "Or install via: winget install GnuPG.GnuPG" -ForegroundColor White
    exit 1
}
Write-Host "GPG found: $($gpgPath.Source)" -ForegroundColor Green

# Step 2: Check for existing keys
Write-Host ""
Write-Host "[2/5] Checking for existing GPG keys..." -ForegroundColor Yellow
$existingKeys = gpg --list-secret-keys --keyid-format long 2>&1 | Select-String "sec"
if ($existingKeys) {
    Write-Host "Existing keys found:" -ForegroundColor Green
    $existingKeys | ForEach-Object { Write-Host "  $_" }
    Write-Host ""
    $useExisting = Read-Host "Use existing key? (Y/N)"
    if ($useExisting -ne "Y" -and $useExisting -ne "y") {
        Write-Host "Please select an existing key or generate a new one manually." -ForegroundColor Yellow
        exit 0
    }
    $keyId = ($existingKeys | Select-Object -First 1).ToString().Split('/')[1].Split(' ')[0]
    Write-Host "Using existing key ID: $keyId" -ForegroundColor Green
} else {
    Write-Host "No existing keys found. Generating new key..." -ForegroundColor Yellow
    
    # Step 3: Generate new key
    Write-Host ""
    Write-Host "[3/5] Generating new GPG key (4096-bit RSA, no expiry)..." -ForegroundColor Yellow
    
    if (-not $Email) {
        $Email = Read-Host "Enter your GitHub email address"
    }
    if (-not $Name) {
        $Name = Read-Host "Enter your full name"
    }
    
    # Create batch file for key generation
    $batchContent = @"
%no-protection
Key-Type: RSA
Key-Length: 4096
Subkey-Type: RSA
Subkey-Length: 4096
Name-Real: $Name
Name-Email: $Email
Expire-Date: 0
%commit
"@
    
    $batchPath = "$env:TEMP\gpg-batch.txt"
    $batchContent | Out-File -FilePath $batchPath -Encoding ascii
    
    gpg --batch --generate-key $batchPath
    
    if ($LASTEXITCODE -ne 0) {
        Write-Host "ERROR: Key generation failed." -ForegroundColor Red
        Remove-Item $batchPath -ErrorAction SilentlyContinue
        exit 1
    }
    
    Remove-Item $batchPath -ErrorAction SilentlyContinue
    Write-Host "Key generated successfully!" -ForegroundColor Green
    
    # Get the new key ID
    $keyId = gpg --list-secret-keys --keyid-format long --keyid-format long 2>&1 | 
             Select-String "sec" | 
             Select-Object -First 1 |
             ForEach-Object { ($_ -split '/')[1].Trim() }
    
    Write-Host "New key ID: $keyId" -ForegroundColor Green
}

# Step 4: Export public key
Write-Host ""
Write-Host "[4/5] Exporting public key..." -ForegroundColor Yellow
$pubKey = gpg --armor --export $Email 2>&1 | Out-String
if (-not $pubKey) {
    $pubKey = gpg --armor --export $keyId 2>&1 | Out-String
}

$pubKeyPath = "$env:USERPROFILE\marble-arm64-gpg-public-key.asc"
$pubKey | Out-File -FilePath $pubKeyPath -Encoding ascii
Write-Host "Public key saved to: $pubKeyPath" -ForegroundColor Green
Write-Host ""
Write-Host "=== ACTION REQUIRED ===" -ForegroundColor Cyan
Write-Host "Copy the public key below and add it to GitHub:" -ForegroundColor White
Write-Host "1. Go to: https://github.com/settings/keys" -ForegroundColor White
Write-Host "2. Click 'New GPG key'" -ForegroundColor White
Write-Host "3. Paste the key below" -ForegroundColor White
Write-Host ""
Write-Host $pubKey
Write-Host ""

# Step 5: Configure git
Write-Host "[5/5] Configuring git for GPG signing..." -ForegroundColor Yellow
git config --global user.signingkey $keyId
git config --global commit.gpgsign true
git config --global gpg.program gpg

Write-Host "Git configured:" -ForegroundColor Green
Write-Host "  user.signingkey = $keyId" -ForegroundColor White
Write-Host "  commit.gpgsign = true" -ForegroundColor White
Write-Host "  gpg.program = gpg" -ForegroundColor White

Write-Host ""
Write-Host "=== Setup Complete ===" -ForegroundColor Cyan
Write-Host ""
Write-Host "Next steps:" -ForegroundColor Yellow
Write-Host "1. Paste the public key above into GitHub (github.com/settings/keys)" -ForegroundColor White
Write-Host "2. Wait 2-3 minutes for GitHub to process the key" -ForegroundColor White
Write-Host "3. Make a test commit: git commit --allow-empty -m 'test: GPG signing'" -ForegroundColor White
Write-Host "4. Push to GitHub and verify the 'Verified' badge appears" -ForegroundColor White
Write-Host ""
Write-Host "All future commits will be automatically signed." -ForegroundColor Green
