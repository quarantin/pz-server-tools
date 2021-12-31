# Project Zomboid Server Tools Configuration

## Check Configuration for Syntax Errors
Every time you change Project Zomboid Server Tools configuration you should check for syntax errors. **Project Zomboid Server Tools won't work properly if you have a syntax error in the configuration**.

To check the configuration for syntax errors, run the following command:

	cd "${HOME}"
	cd pz-server-tools
	./bin/checkconfig

## Text Editor
Project Zomboid Server Tools let you choose which text editor to use to edit the various configuration file of your pzservers. By default it will use **nano**, but you can configure any text editor that suits you. For example if you'd prefer to use **vim**, the configuration should look like this:

	{
		"editor": "vim"
	}

## Project Zomboid Dedicated Servers
Project Zomboid Server Tools let you run multiple pzserver instances on the same machine.

Edit the file **/etc/pzst/config.json** as root:

	sudo nano /etc/pzst/config.json

Add each system user that is running a Project Zomboid Dedicated Server to the list under **pzservers**. For example if you have two servers running with users **pzserver1** and **pzserver2**, the configuration should look like this:

	{
		"pzservers": [
			"pzserver1",
			"pzserver2"
		]
	}

## Alerts Before Restart
Project Zomboid Server Tools let you configure how to alert players in-game before restarting pzservers.

Edit the file **/etc/pzst/config.json** as root:

	sudo nano /etc/pzst/config.json

Assuming you want all your pzservers to wait 15 minutes before restart, sending an alert to players every 5 minutes, your configuration should look like this:

	{
		"restart-sequence": [
			15,
			10,
			5,
			2,
			1
		]
	}

This would configure the server to send the first alert 15 minutes before reboot, second alert 10 minutes before, then 5 minutes, 2 minutes, 1 minutes. When the whole sequence is over, the server will reboot.

## Daily Restarts
Project Zomboid Server Tools let you restart your pzserver at specific times of day. The daily restart will take into account the alerts defined in previous section.

Edit the file **/etc/pzst/config.json** as root:

	sudo nano /etc/pzst/config.json

Assuming you want to restart all your pzservers every day at 00:00, 06:00, 12:00, and 18:00, the configuration should look like this:

	{
		"restarts": [
			"00:00",
			"06:00",
			"12:00",
			"18:00"
		]
	}
