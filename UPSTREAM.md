# Upstream Provenance

This repository produces a standalone Windows ARM64 binary of KDE Marble.

| Field | Value |
|-------|-------|
| Upstream repository | https://invent.kde.org/education/marble |
| Upstream release version | v22.07.70 |
| Baseline source | KDE Marble v22.07.70 source tree |
| Upstream remote | `upstream` → https://invent.kde.org/education/marble.git |
| Last known upstream commit on master | `ddab2bd2a` (v26.07.70) |
| Build system | CMake + vcpkg (arm64-windows triplet) |
| CI runner | windows-11-vs2026-arm |

## Source Provenance

This fork was initialized from the KDE Marble v22.07.70 source tree.

**Upstream repository (anyone can verify):**
```
git clone https://invent.kde.org/education/marble.git
cd marble
git log --oneline -1
# ddab2bd2a GIT_SILENT Update Appstream for new release
```

**Version in this source:**
- `CMakeLists.txt` line 7-10: `RELEASE_SERVICE_VERSION = 22.07.70`
- This matches KDE's official v22.07.70 release

**Upstream release page (human verification):**
https://invent.kde.org/education/marble/-/releases/v22.07.70

## Git Ancestry Verification

Because this fork was initialized from a source snapshot (not a `git clone`),
there is no shared git history with upstream. This is the same method used by
Linux distributions (Debian, Fedora, Arch) to package software.

To mechanically verify the source matches upstream:
1. Clone upstream: `git clone https://invent.kde.org/education/marble.git`
2. Check upstream version: `git show HEAD:CMakeLists.txt | grep RELEASE_SERVICE_VERSION`
3. Compare file hashes between upstream v22.07.70 source and this fork

## SLSA Provenance

Each release zip has a Sigstore attestation. Verify with:
```
gh attestation verify marble-arm64-win-<version>.zip --repo dp1x/marble-arm64-windows
```

This cryptographically links the artifact to:
- The exact source commit in this repo
- The exact build environment (windows-11-vs2026-arm)
- The exact time of build

## Checksums

Each release includes SHA256SUMS. Verify with:
```powershell
certutil -hashfile marble-arm64-win-<version>.zip SHA256
```

Compare the output hash against the value in `SHA256SUMS`.

## GPG Signature Verification

Release tags are signed with the maintainer's GPG key. Verify with:
```powershell
git tag -v v22.07.70-arm64
```

Expected output: `Good signature from <MAINTAINER_NAME>`

## Complete Verification Chain

A third party can verify this fork is a legitimate ARM64 port of KDE Marble v22.07.70:

1. **Source identity**: Check `UPSTREAM.md` → points to https://invent.kde.org/education/marble
2. **Version match**: Compare `CMakeLists.txt` RELEASE_SERVICE_VERSION = 22.07.70 with upstream v22.07.70 release
3. **Binary provenance**: `gh attestation verify` proves the zip was built from commit `e2f20cac3` in this repo
4. **Integrity**: `certutil -hashfile` + `SHA256SUMS` proves the zip was not modified
5. **Maintainer identity**: GPG signature on the release tag proves it was signed by the repo owner
