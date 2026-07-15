#!/usr/bin/env pwsh
<#
.SYNOPSIS
  Bump the ARM64 Windows release by creating a new signed git tag.

.DESCRIPTION
  The release version is single-sourced from the git tag. "Bumping" means
  creating a new tag like v22.08.0-arm64. The CI workflow reads that tag and
  builds/names the artifact accordingly, and passes RELEASE_SERVICE_VERSION to
  CMake so the binary embeds the same version. No CMakeLists edits required.

.PARAMETER Version
  Explicit version to tag (e.g. 22.08.0). If omitted, the patch component of the
  latest v*.*.*-arm64 tag is incremented.

.PARAMETER DryRun
  Print the tag that would be created without actually creating or pushing it.

.EXAMPLE
  .\bump.ps1                 # bumps patch: v22.07.70-arm64 -> v22.07.71-arm64
  .\bump.ps1 22.08.0         # creates v22.08.0-arm64
  .\bump.ps1 -DryRun         # show what would happen
#>
[CmdletBinding()]
param(
  [string]$Version,
  [switch]$DryRun
)

$ErrorActionPreference = 'Stop'

function Get-LatestArm64Tag {
  $all = git tag --list 'v*' 2>$null
  $arm64 = $all | Where-Object { $_ -match '^v(\d+\.\d+\.\d+)-arm64$' }
  if (-not $arm64) { return $null }
  $arm64 | Sort-Object { [version]($_.replace('v','') -replace '-arm64','') } -Descending | Select-Object -First 1
}

function Bump-Patch([string]$v) {
  $m = $v -match '^(\d+)\.(\d+)\.(\d+)$'
  if (-not $m) { throw "Cannot parse version '$v'" }
  $maj = $Matches[1]
  $min = $Matches[2]
  $pat = [int]$Matches[3] + 1
  return "$maj.$min.$pat"
}

$latest = Get-LatestArm64Tag
if ($latest) { Write-Host "Latest arm64 tag: $latest" } else { Write-Host "Latest arm64 tag: <none>" }

if (-not $Version) {
  if (-not $latest) {
    Write-Error "No existing v*.*.*-arm64 tag found and no -Version given. Specify -Version explicitly."
    exit 1
  }
  $base = $latest -replace '^v' -replace '-arm64$'
  $Version = Bump-Patch $base
  Write-Host "Auto-bumped patch -> $Version"
}

if ($Version -notmatch '^\d+\.\d+\.\d+$') {
  Write-Error "Version must be X.Y.Z (got '$Version')."
  exit 1
}

$newTag = "v$Version-arm64"
Write-Host "New tag will be: $newTag"

if ($DryRun) {
  Write-Host "[DryRun] Would run: git tag -s $newTag -m 'Release $newTag'"
  Write-Host "[DryRun] Then: git push origin $newTag"
  Write-Host "[DryRun] Then: gh workflow run ... (auto-triggered by tag push)"
  exit 0
}

# Ensure working tree is clean enough to tag
$status = git status --porcelain
if ($status) {
  Write-Warning "Working tree has uncommitted changes; tagging anyway but you may want to commit first."
}

git tag -s $newTag -m "Release $newTag"
if ($LASTEXITCODE -ne 0) {
  Write-Error "Failed to create signed tag '$newTag'. Check gpg signing config (setup-gpg.ps1)."
  exit 1
}

Write-Host ""
Write-Host "Created signed tag $newTag" -ForegroundColor Green
Write-Host "Next steps:" -ForegroundColor Yellow
Write-Host "  git push origin $newTag"
Write-Host "  (The tag push triggers the CI build automatically.)"
