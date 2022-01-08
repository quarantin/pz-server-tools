# Install Project Zomboid Dedicated Server
To install a new pzserver, first you have to choose a system username for that server. Each pzserver instance needs a different username. Let's assume you want to use the username **pzserver1**. Run the following command to create the new user, and replace **pzserver1** with whatever username you choose:

	sudo useradd --create-home --shell /bin/bash pzserver1

Set a password for user **pzserver1**:

	sudo passwd pzserver1

Add user **pzserver1** to group **pzst**:

	sudo usermod -aG pzst pzserver1

Login as user **pzserver1**:

	sudo -i -u pzserver1

Type this to get the list of available commands:

	pzserver

Now that you're connected as **pzserver1**, you can run the following command to install your new pzserver:

	pzinstall

Start pzserver:

	pzstart

When run for the first time, **pzstart** will ask you to choose a username and a password for the admin user, and the port of the server.

# Configuring Project Zomboid Dedicated Server

## Configuring Ports
If you will run multiple pzservers on the same machine, you have to configure a different port for each server. Furthermore, each server needs exactly one port, plus one extra port per player configured on the server. So for example if you have a pzserver running on port 16261 and you configure 32 players, your other pzservers can't use a port in the range 16261-16293.
Run the following command to edit pzserver configuration:

	pzconfig

And change the value of the **DefaultPort** option according to your needs:

	DefaultPort=16261

If you need to use RCON, you have to set the option **RCONPassword** and **RCONPort**. You also have to use a different port for each of your pzservers. So for example if using RCON your configuration should look like this:

	RCONPort=27015
	RCONPassword=yourRconPassword

## Adding Mods
If you want to enable mods on your server you have to edit the configuration and add your mods to the **WorkshopItems** option. For each mod you have to add the Workshop ID of the mod, using semicolon ( ; ) as delimiter. The Workshop ID can be found on Steam Workshop when visiting the page for the mod.

For example if you want to install the mod **Just Throw Them Out The Window**, available here:

https://steamcommunity.com/sharedfiles/filedetails/?id=2659216714

you just have to copy-paste the value of the **id** field of your browser address bar, in this case **2659216714**. Let's assume you also want to add the mod **Quick Item Action** available here:

https://steamcommunity.com/sharedfiles/filedetails/?id=2642486124

whose Workshop ID is **2642486124**. Then your configuration should look like this:

	WorkshopItems=2659216714;2642486124

## Sandbox Settings
In order to edit your server sandbox settings run the following command:

	pzsandbox

## Spawn Points and Regions
If you want to change the spawn points where player can land in the game run the following command:

	pzspawnpoints

For spaw regions, run the following command:

	pzspawnregions

# Restarting Project Zomboid Dedicated Server
If you need to restart your pzserver run the following command:

	pzrestart

# Stopping Project Zomboid Dedicated Server
If you want to stop your pzserver run the following command:

	pzquit
