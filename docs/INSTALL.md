# Installing Project Zomboid Server Tools
To install Project Zomboid Server Tools, you have to create a system account named **pzstadmin**:

	sudo useradd --create-home --shell /bin/bash pzstadmin

Set a password for user **pzstadmin**:
	sudo passwd pzstadmin

Add user **pzstadmin** to group **sudo**:
	sudo usermod -aG sudo pzstadmin

Login as user **pzstadmin**:
	sudo -i -u pzstadmin

Run the following commands to install Project Zomboid Server Tools:

	git clone https://github.com/quarantin/pz-server-tools
	cd pz-server-tools
	./pzst/pzstinstall

# Updating Project Zomboid Server Tools
To update Project Zomboid Server Tools, run the following commands:

	sudo -u pzstadmin pzst update

# Uninstalling Project Zomboid Server Tools
To uninstall Project Zomboid Server Tools, run the following commands:

	sudo -u pzstadmin pzst uninstall
