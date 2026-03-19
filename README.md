# Conquer Chess: Battle for Liberty 

<!-- markdownlint-disable MD013 --><!-- Badges cannot be split up over lines, hence will break 80 characters per line -->

| Branch    | [![GitHub Actions logo](images/GitHubActions.png)](https://github.com/richelbilderbeek/conquer_chess/actions)                                                                                                         | [![Codecov logo](images/Codecov.png)](https://www.codecov.io)                                                                                                            |
| --------- | --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- | ------------------------------------------------------------------------------------------------------------------------------------------------------------------------ |
| `main`    | [![Check build](https://github.com/richelbilderbeek/conquer_chess/actions/workflows/check_build.yaml/badge.svg?branch=main)](https://github.com/richelbilderbeek/conquer_chess/actions/workflows/check_build.yaml)    | [![codecov](https://codecov.io/gh/richelbilderbeek/conquer_chess/branch/main/graph/badge.svg?token=OWVGV7PST8)](https://codecov.io/gh/richelbilderbeek/conquer_chess)    |
| `develop` | [![Check build](https://github.com/richelbilderbeek/conquer_chess/actions/workflows/check_build.yaml/badge.svg?branch=develop)](https://github.com/richelbilderbeek/conquer_chess/actions/workflows/check_build.yaml) | [![codecov](https://codecov.io/gh/richelbilderbeek/conquer_chess/branch/develop/graph/badge.svg?token=OWVGV7PST8)](https://codecov.io/gh/richelbilderbeek/conquer_chess) |


- Windows build: [![Build status](https://ci.appveyor.com/api/projects/status/rom1qe03qfl84ky1?svg=true)](https://ci.appveyor.com/project/richelbilderbeek/conquer-chess)

<!-- markdownlint-enable MD013 -->

Chess variant.

- [Go to the 'Conquer Chess' site](https://richelbilderbeek.github.io/conquer_chess/)

## Files used by continuous integration scripts

<!-- markdownlint-disable MD013 --><!-- Tables cannot be split up over lines, hence will break 80 characters per line -->

| Filename                                    | Description                                                                                                                                 |
| ------------------------------------------- | ------------------------------------------------------------------------------------------------------------------------------------------- |
| [`.appveyor.yml`](.appveyor.yml)            | AppVeyor script that builds the Windows executable                                                                                          |
| [`.appveyor.yml`](.appveyor.yml)            | AppVeyor script that builds the Windows executable                                                                                          |
| [`.editorconfig`](.editorconfig)            | Editor configuration file, unsure if it is applied anywhere                                                                                 |
| [`.markdownlint.jsonc`](.markdownlint.jsonc)| Configuration of the Markdown linter, use `markdownlint "**/*.md"` to do Markdown linting locally. The name of this file is a default name. |
| [`.markdownlintignore`](.markdownlintignore)| Files ignored by the Markdown linter, use `markdownlint "**/*.md"` to do Markdown linting locally. The name of this file is a default name. |
| [`.wordlist.txt`](.wordlist.txt)            | Whitelisted words for the spell checker, use `pyspelling -c .spellcheck.yml` to do spell check locally                                      |
| [`CMakeLists.txt`](CMakeLists.txt)          | CMake file for the Windows build                                                                                                            |
| [`codecov.yml`](codecov.yml)                | [Codecov](https://codecov.io) settings                                                                                                      |
| [`cpp_code_style`](cpp_code_style)          | C++ code style file, unsure if it is applied anywhere                                                                                       |
| [`Doxyfile`](Doxyfile)                      | Doxygen documentation file                                                                                                                  |
| [`game.pri`](game.pri)                      | Conquer Chess project include file                                                                                                          |
| [`game.pro`](game.pro)                      | Conquer Chess project file                                                                                                                  |
| [`mkdocs.yml`](mkdocs.yml)                  | Conquer Chess MkDocs website settings                                                                                                       |
| [`mlc_config.json`](mlc_config.json)        | Configuration of the link checker, use `markdown-link-check --config mlc_config.json --quiet docs/**/*.md` to do link checking locally      |
| [`qt_quick_style.xml`](qt_quick_style.xml)  | C++ code style file, unsure if it is applied anywhere                                                                                       |
| [`requirements.txt`](requirements.txt)      | Conquer Chess MkDocs website requirements file                                                                                              |
| [`vcpkg.json`](vcpkg.json)                  | Windows build package manager configuration file                                                                                            |

<!-- markdownlint-enable MD013 -->
