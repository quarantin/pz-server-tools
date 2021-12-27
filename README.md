# Project Zomboid Server Tools
A set of tools to help you run Project Zomboid Dedicated Server (Linux only).

# Features
- Install Project Zomboid Dedicated Server easily in a single command.
- Restart Project Zomboid Dedicated Server at specific times of day.
- Restart Project Zomboid Dedicated Server when a mod is updated on Steam workshop.
- Manage multiple Project Zomboid Dedicated Server running on the same machine.
- Many more...

# Installing Project Zomboid Server Tools
https://github.com/quarantin/pz-server-tools/blob/main/docs/INSTALL.md

# Configuring Project Zomboid Server Tools
https://github.com/quarantin/pz-server-tools/blob/main/docs/CONFIG.md

# Using Project Zomboid Server Tools
https://github.com/quarantin/pz-server-tools/blob/main/docs/USAGE.md

# Tools
Once Project Zomboid Server Tools have been installed and configured successfully, you will have access to the following commands:
- **pzinstall**: Install Project Zomboid Dedicated Server for the current user
- **pzuninstall**: Uninstall Project Zomboid Dedicated Server for the current user
- **pzupdate**: Update Project Zomboid Dedicated Server
- **pzcheckmods**: Check mod updates
- **pzconfig**: Edit pzserver configuration (servertest.ini)
- **pzsandbox**: Edit pzserver sandbox configuration (servertest_SandboxVars.lua)
- **pzspawnpoints**: Edit pzserver spawn points configuration (servertest_spawnpoints.lua)
- **pzspawnregions**: Edit pzserver spawn regions configuration (servertest_spawnregions.lua)
- **pzserver**: Start pzserver in the background (no need to use screen)
- **pzstart**: An alias for pzserver
- **pzstatus**: Check pzserver status
- **pzlog**: Show pzserver console output
- **pzquit**: Stop pzserver
- **pzstop**: An alias for pzquit
- **pzadminpasswd**: Change pzserver admin password
- **pzuserpasswd**: Change pzserver password of a user
- **pzcmd**: Run a server command (like *chopper*, *additem*, etc)
- **pzmessage**: Display a message on the server visible to all connected players
- **pzrestart**: Restart pzserver
