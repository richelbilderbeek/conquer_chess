---
tags:
  - dev
  - developer
  - questions
  - answers
  - guide
---

# Developer guide

## How do you upload the Linux executable?

Using a GitHub Actions script that uploads the executable and
all required shared objects.

Then, download all these into the `steam_build/linux64` folder.

Then run `./scripts/run_steamcmd_to_upload.sh` to upload these.

## How do you upload the Windows executable?

Using an AppVeyor script that uploads the executable and
all required DLLs.

Then, download all these into the `windows_binary` folder.

Then run `./scripts/run_steamcmd_to_upload.sh` to upload these.

## How did you set the AppVeyor script to SFML 2.6.1?

- Search for `vcpkg SFML`
- Find [`https://vcpkg.link/ports/sfml`](https://vcpkg.link/ports/sfml)
- Click on the 30+ versions tab

- Click on SFML 2.6.1

??? question "How does that look like?"

  ![Click on SFML 2.6.1](vcpkg_select_sfml_2_6_1.png)

- Click on 'Subtree'

??? question "How does that look like?"

  ![Click on 'Subtree'](vcpkg_select_sfml_2_6_1_subtree.png)

- Extract the full commit number from the GitHub URL,
  in this case `163b97d4625073093ae73585695cc4cfe4480998`

??? question "How does that look like?"

  ![Extract the full commit number from the GitHub URL](vcpkg_select_sfml_2_6_1_subtree_copy_commit.png)

- In the root folder, the file `vcpkg.json` must use that commit number
  as a `builtin-baseline`.

??? question "How does that look like?"

    Here is an example (and complete) `vcpkg.json`:

    ```bash
    {
      "name": "conquer-chess",
      "version": "0.13.0",
      "dependencies": [
        {
          "name": "sfml",
          "features": [
            "audio",
            "graphics",
            "network",
            "window"
          ]
        }
      ],
      "builtin-baseline": "163b97d4625073093ae73585695cc4cfe4480998"
    }
    ```

Due to this, `vcpkg` will look for SFML 2.6.1 and all its dependencies
at that point in its commit history.

## How did you generate the FEN strings

I often used <https://www.365chess.com/analysis_board.php>.

## How is the user input handled?

See [architecture](architecture/README.md)

## Where is the code documentation?

See [the Doxygen generated documentation](../docs/index.html).
