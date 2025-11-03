## Discontinued for now
This project will no longer be updated, but it still works for versions prior to 1.21.114.

## McPatch

**McPatch** is a utility tool designed to apply byte-level patches to the **Minecraft: Bedrock Edition** executable. This project is a community-maintained version of the original patcher by *VastraKai*, which has since been removed from GitHub.

---

### Current Features

> Each patch is based on a unique byte signature and is applied directly to the game executable.

| Patch                  | Description                                                                                                |
|------------------------|------------------------------------------------------------------------------------------------------------|
| **GuiScale**           | Forces a smaller GUI scale for better visibility on low-resolution screens.                                |
| **TeleportRotation**   | Disables the camera snap/rotation that occurs during teleportation.                                        |
| **ItemUseDelay**       | Removes or reduces delay between item uses.                                                                |
| **MinimalViewBobbing** | Disables camera bobbing when walking.                                                                      |
| **NoHurtCam**          | Removes the screen shake when taking damage.                                                               |
| **NoJumpDelay**        | Eliminates delay between consecutive jumps.                                                                |
| **PlayScreenFix**      | Replaces the new PlayScreen with the old one.  (Thanks [@alteik](https://github.com/alteik/PlayScreenFix)) |

---

### Download

 [**Download the latest release here**](https://github.com/Zwuiix-cmd/McPatch/releases/download/latest/McPatch.exe)

Compatible with the latest versions of **Minecraft Bedrock** (requires `.exe` executable).

---

### 🔧 Example Patch Signatures

Patch signatures are hexadecimal byte sequences used to locate the relevant instructions in the executable:

```json
{
  "GuiScale": "00 00 ? ? 00 00 A0 40 00 00 C0 40",
  "TeleportRotation": "E8 ? ? ? ? 48 8B 03 48 8D 94 24 ? ? ? ? 48 8B 4B",
  "ItemUseDelay": "FF 15 ? ? ? ? 48 8B ? 48 8B ? 48 8B ? ? ? ? ? FF 15 ? ? ? ? 32 DB",
  "MinimalViewBobbing": "FF 15 ? ? ? ? 80 7C 24 60 ? 0F 84 ? ? ? ? 48 89",
  "NoHurtCam": "76 0E F3 0F 5C C7",
  "NoJumpDelay": "C7 47 ? ? ? ? ? 48 8B 9C 24 ? ? ? ? 0F 28 74 24 ? 48 81 C4",
  "PlayScreenFix": "6D 63 2D 61 62 2D 6E 65 77 2D 70 6C 61 79 2D 73 63 72 65 65 6E 2D"
}
```

Wildcards (`?`) allow for flexible matching across different Minecraft builds.

---

#### How It Works

1. The patcher scans the game executable for the defined byte signature.
2. It replaces bytes at the found location with the custom patch.
3. Unmatched patches are skipped with error messages in the log.

---

### Contributing

Want to help improve the tool?

* Fork the repository
* Make your changes
* Submit a pull request

All contributions are welcome!

---

### Disclaimer

Use at your own risk.
This tool modifies the game binary directly and could trigger anti-cheat systems in multiplayer environments.

---

### License

This project is open-source under the [MIT License](https://opensource.org/licenses/MIT).

---

Would you like me to output this as a downloadable `README.md` file for your repository?
