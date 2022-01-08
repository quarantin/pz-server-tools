# Project Zomboid Server Tools
A set of tools to help you run Project Zomboid Dedicated Server (Linux only).

# Features
- Install pzserver easily in a single command
- Backup pzserver saved game automatically
- Restart pzserver at specific times of day
- Update pzserver from Steam automatically
- Restart pzserver when a mod is updated on Steam Workshop
- Manage multiple pzservers running on the same machine
- Run pzserver in the background (no need for screen!)
- Many more...

# Tools
Once Project Zomboid Server Tools have been successfully installed you will have access to the following commands:
- **pzserver**         To manage your Project Zomboid Dedicated Servers
- **pzst**             To manage your Project Zomboid Server Tools
- **javamods**         To configure java mods to run on  your pzserver

To get help about the different commands, just type the name of the command without arguments.

Here's the help section for the **pzserver** command:

	$ pzserver

	Usage: pzserver <command> [args]...

	Available commands:

	  install        Install or update pzserver
	  uninstall      Uninstall pzserver

	  config         Edit pzserver configuration (servertest.ini)
	  sandbox        Edit pzserver sandbox settings
	  spawnpoints    Edit pzserver spawn points
	  spawnregions   Edit pzserver spawn regions
	  checkmods      Check mod updates
	  maxmen         Change pzserver maximum memory allocation

	  adminpasswd    Change pzserver admin password
	  userpasswd     Change pzserver user password
	  status         Check pzserver status
	  console        Show pzserver console output
	  clearbackups   Clear old backups
	  clearzpop      Clear zombie population

	  start          Start pzserver in the background
	  restart        Restart pzserver
	  stop           Stop pzserver
	  kill           Kill pzserver (unclean shutdown)

	  command        Run a server command (additem, chopper, etc)
	  message        Display custom message in-game

Here's the help section for the **pzst** command:

	$ pzst

	Usage: pzst <command>

	Available commands:

	  install        Install pzst
	  update         Update pzst
	  unintsall      Uninstall pzst

	  config         Edit pzst configuration
	  checkconfig    Check pzst configuration for syntax errors

Here's the help section for the **javamods** command:

	$ javamods

	Usage: javamods <commands>

	Available commands:

	  install        Install or update javamods
	  uninstall      Uninstall javamods
	  console        Show javamods console output

# PZ Server Tools Installation
https://github.com/quarantin/pz-server-tools/blob/main/docs/INSTALL.md

# PZ Server Tools Configuration
https://github.com/quarantin/pz-server-tools/blob/main/docs/CONFIG.md

# PZ Server Operations
https://github.com/quarantin/pz-server-tools/blob/main/docs/PZSERVER.md
