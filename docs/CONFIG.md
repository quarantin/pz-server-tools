# Project Zomboid Server Tools Configuration

## Text Editor
Project Zomboid Server Tools let you choose which text editor to use to edit the various configuration file of your pzservers. By default it will use **nano**, but you can configure any text editor that suits you. For example if you'd prefer to use **vim**, the configuration should look like this:

	{
		"editor": "vim"
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
		"daily-restarts": [
			"00:00",
			"06:00",
			"12:00",
			"18:00"
		]
	}

## Alternate Start Script
WARNING: This feature is not compatible with javamods! If you enable this, javamods will be disabled.

By default pzst is using the script `start-server.sh` to start the pzserver. In case you need to use an alternate start script, proceed as follow.

Edit the file **/etc/pzst/config.json** as root:

	sudo nano /etc/pzst/config.json

Assuming your alternate start script is named `start-server-other.sh`, your configuration should look like this:

	{
		"pzserverstart": "start-server-other.sh"
	}

In case you want to restore the default start script, simply remove the "pzserverstart" from the file **/etc/pzst/config.json**.
