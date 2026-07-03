# Upstream Provenance

This repository produces a standalone Windows ARM64 binary of KDE Marble.

| Field | Value |
|-------|-------|
| Upstream repository | https://invent.kde.org/education/marble |
| Upstream version in this fork | v22.07.70 |
| Upstream baseline commit | See `git log --oneline $(git merge-base HEAD origin/master)` once master is present |
| Last sync | v22.07.70 (August 2022) |
| Build system | CMake + vcpkg (arm64-windows triplet) |
| CI runner | windows-11-vs2026-arm |

## Verifying Upstream Identity

Builds are tagged `vYY.MM.MM-arm64` and produced by GitHub Actions.
The workflow commits can be cross-referenced against upstream KDE Marble release tags
(`v22.07.70` etc.) published at https://invent.kde.org/education/marble/-/releases/v22.07.70

## SLSA Provenance

Each release zip has a Sigstore attestation. Verify with:
```
gh attestation verify marble-arm64-win-<version>.zip --repo dp1x/marble-arm64-windows
```

This cryptographically links the artifact to the exact source commit and build environment.

## Checksums

Each release includes SHA256SUMS. Verify with:
```powershell
certutil -hashfile marble-arm64-win-<version>.zip SHA256
```
