---
tags:
  - dev
  - developer
  - questions
  - answers
  - guide
---

# Developer guide

## How can I build the game on my own computer?

This will be your folder structure:

```text
.
├── chess-library
├── conquer_chess
├── magic_enum
└── SFGraphing
```

Go to the place where you want these 4 folders to be created,
then do:

```bash
git clone https://github.com/richelbilderbeek/conquer_chess
git clone https://github.com/Neargye/magic_enum --branch v0.9.7
git clone https://github.com/Disservin/chess-library.git
git clone https://github.com/richelbilderbeek/SFGraphing -b patch-1
```

Additionally, install Qt Creator and SFML:

```
sudo apt install libsfml-dev qtcreator
```

Start Qt Creator and open the Qt Creator project `game.pro`.

## How do you upload the Linux executable?

???- question "Prefer a video?"

    Watch the YouTube video
    [Conquer Chess: how to deploy the Linux Steam Runtime 3.0 (Sniper) build](https://youtu.be/cSTRn6L2lo4)

Using a GitHub Actions script that uploads the executable and
all required shared objects.

Then, download all these into the `steam_build/linux64` folder.

Then run `./scripts/run_steamcmd_to_upload.sh` to upload these.

## How do you upload the Windows executable?

???- question "Prefer a video?"

    Watch the YouTube video
    [Conquer Chess: deploying the Windows version](https://youtu.be/N8ZTGgE661c)

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
