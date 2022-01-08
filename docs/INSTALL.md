# Installing Project Zomboid Server Tools
To install Project Zomboid Server Tools, you have to create a system account named **pzadmin**:

	sudo useradd --create-home --shell /bin/bash pzadmin

Set a password for user **pzadmin**:
	sudo passwd pzadmin

Add user **pzadmin** to group **sudo**:
	sudo usermod -aG sudo pzadmin

Login as user **pzadmin**:
	sudo -i -u pzadmin

Run the following commands to install Project Zomboid Server Tools:

	git clone https://github.com/quarantin/pz-server-tools pzst
	cd pzst
	./bin/install

# Updating Project Zomboid Server Tools
To update Project Zomboid Server Tools, run the following commands:

	sudo -i -u pzadmin
	cd pzst
	./bin/update

# Uninstalling Project Zomboid Server Tools
To uninstall Project Zomboid Server Tools, run the following commands:

	sudo -i -u pzadmin
	cd pzst
	./bin/uninstall
