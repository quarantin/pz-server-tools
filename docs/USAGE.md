# Usage

## Install Project Zomboid Dedicated Server
To install a new pzserver, first you have to choose a system username for that server. Each pzserver instance needs a different username. Let's assume you want to use the username **pzserver1**. Run the following commands to create the new user, and replace **pzserver1** with whatever username you choose:

	sudo useradd -m -k /etc/skel -s /bin/bash pzserver1
	sudo passwd pzserver1
	sudo usermod -aG sudo pzserver1

Then login as that user:

	sudo -i -u pzserver1

Now that you're connected as **pzserver1**, you can run the following command to install your new pzserver:

	pzinstall

## Starting Project Zomboid Dedicated Server
Once your new pzserver is installed you can start it using the following command:

	pzserver

## Restarting Project Zomboid Dedicated Server
If you need to restart your pzserver run the following command:

	pzrestart

## Stopping Project Zomboid Dedicated Server
If you want to stop your pzserver run the following command:

	pzquit
