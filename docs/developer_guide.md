---
tags:
  - dev
  - developer
  - questions
  - answers
  - guide
---

# Developer guide

## How do you upload the non-standalone Linux executable?

In the Qt Creator project settings, do not use a shadow build
and instead let `conquer_chess` be generated in the root folder.

## How do you upload the Windows executable?

Using an AppVeyor script that uploads the executable and
all required DLLs.

Then, download all these into the `windows_binary` folder.

Then run `./scripts/run_steamcmd_to_upload.sh` to upload these.

## How did you generate the FEN strings

I often used <https://www.365chess.com/analysis_board.php>.

## How is the user input handled?

See [architecture](architecture/README.md)

## Where is the code documentation?

See [the Doxygen generated documentation](docs/index.html).
