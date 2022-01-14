#!/etc/pzst/env/bin/python

import sys
import discord

from pypzst import get_bot_cache, get_bot_config, get_workshop_url


def start_bot(config):

	client = discord.Client()

	def bold(text):
		return '**' + text + '**'

	def link(text, href):
		return '[' + text + '](' + href + ')'

	@client.event
	async def on_ready():
		print('We have logged in as {0.user}'.format(client))

	@client.event
	async def on_message(message):
		if message.author == client.user:
			return

		if message.content.startswith('!mods'):

			description = ''

			for server, updates in get_bot_cache().items():
				for mod_id, mod in updates.items():
					status = mod['status'] == 'new' and '(New mod)' or '(Mod updated)'
					description += '[ ' + link(mod_id, get_workshop_url(mod_id)) + ' ] ' + bold(mod['name']) + ' ' + status + '\n'

			embed = discord.Embed(title='Updated Mods', description=description)
			await message.channel.send(embed=embed)

	client.run(config['discord_token'])


if __name__ == '__main__':

	config = get_bot_config()

	if len(sys.argv) > 1:
		invite_url = 'https://discordapp.com/oauth2/authorize?client_id=%s&scope=bot&permissions=0' % config['app_id']
		print(invite_url)

	else:
		start_bot(config)
