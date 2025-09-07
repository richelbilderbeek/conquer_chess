# Steam

## Sniper

```bash
~/.local/share/Steam/steamapps/common/SteamLinuxRuntime_sniper
```

## Valve Data Format

These are files that setup the relation
between application and depot.

These are to be used with the `steamcmd` tool.

- [Steam documentation](https://partner.steamgames.com/doc/sdk/uploading#BuildingYourGameTutorial)

Parameter|Value  |Script
---------|-------|------
AppId    |3873940|[app_build_3873940.vdf](app_build_3873940.vdf)
DepotID  |3873941|[depot_build_3873941.vdf](depot_build_3873941.vdf)

## Running

Start `steamcmd`:

```bash
steamcmd
```

Login:

```bash
Steam>login richelbilderbeek
Logging in using cached credentials.
Logging in user 'richelbilderbeek' [U:1:174343888] to Steam Public...OK
Waiting for client config...OK
Waiting for user info...OK
```

Build:

```bash
Steam>run_app_build /home/richel/GitHubs/conquer_chess/steam/app_build_3873940.vdf
[2025-08-13 17:15:28]: Starting AppID 3873940 build (flags 0x0).
[2025-08-13 17:15:28]: Building depot 3873941...

Building file mapping...
Scanning content
.... 39.3MB (40%)
...... 89.6MB (93%)
IPC function call IClientUtils::GetAPICallResult took too long: 84 msec
Uploading content...
IPC function call IClientUtils::GetAPICallResult took too long: 56 msec

[2025-08-13 17:15:34]: Successfully finished AppID 3873940 build (BuildID 19585011).
```

## My Conquer Chess Steam folder

```bash
/home/richel/.local/share/Steam/steamapps/common/conquer_chess
```
