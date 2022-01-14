#!/usr/bin/env python

import sys
import json
import logging
import requests

import os
from os.path import exists, join

from retry import retry
from pwd import getpwnam, getpwuid

BOT_CACHE = '/var/cache/pzst/bot.json'
BOT_CONFIG = '/etc/pzst/bot.json'
PZST_CONFIG = '/etc/pzst/config.json'
WORKSHOP_URL = 'https://steamcommunity.com/sharedfiles/filedetails/?id='

def get_bot_cache():

	with open(BOT_CACHE) as fd:
		bot_cache = json.loads(fd.read())

	return bot_cache

def get_bot_config():

	with open(BOT_CONFIG) as fd:
		bot_config = json.loads(fd.read())

	return bot_config

def get_mod_url(workshop_id):
	return WORKSHOP_URL + workshop_id

def get_pzst_config():

	with open(PZST_CONFIG) as fd:
		pzst_config = json.loads(fd.read())

	return pzst_config

def get_pzserver_pid(user):

	proc = '/proc'
	uid = getpwnam(user).pw_uid

	for pid in os.listdir(proc):

		if not pid.isnumeric():
			continue

		filepath = join(proc, pid)
		fileinfo = os.stat(filepath)
		if uid != fileinfo.st_uid:
			continue

		filepath = join(proc, pid, 'comm')
		with open(filepath) as fd:
			command = fd.read().strip()

		if command not in [ 'ProjectZomboid3', 'ProjectZomboid6' ]:
			continue

		return pid

	return 'invalid'

@retry(tries=5)
def get_mod_updates(mods):

	steam_url = 'https://api.steampowered.com/ISteamRemoteStorage/GetPublishedFileDetails/v1/'

	if len(mods) == 0:
		return {}

	data = {}
	data['itemcount'] = len(mods)

	for i, workshop_id in enumerate(mods):
		key = 'publishedfileids[%d]' % i
		data[key] = workshop_id

	jsondata = requests.post(steam_url, data=data).json()

	mods = {}

	for workshop_item in jsondata['response']['publishedfiledetails']:

		mod_id = workshop_item['publishedfileid']

		mods[mod_id] = {
			'name':    workshop_item.get('title', mod_id),
			'updated': workshop_item.get('time_updated', 0),
		}

	return mods

def save_bot_data(servers, updated_mods):

	if not updated_mods:
		return

	bot_data = {}
	for mod_id, mod in updated_mods.items():
		for server, server_config in servers.items():
			if not bot_data.get(server):
				bot_data[server] = {}
			if mod_id in server_config.get('WorkshopItems', []):
				bot_data[server][mod_id] = mod

	with open(BOT_CACHE, 'w') as fd:
		fd.write(json.dumps(bot_data))

def init_log(name, logfile, server=''):

	if server:
		server = '.' + server

	loglevel = logging.DEBUG
	logfile = '/var/log/pzst/%s%s.log' % (logfile, server)
	logformat = '%(asctime)s %(name)s %(message)s'
	dateformat = '%Y-%m-%d %H:%M:%S'

	set_permissions = not exists(logfile)

	logging.basicConfig(filename=logfile, format=logformat, datefmt=dateformat, level=loglevel)
	logging.getLogger('retry.api').setLevel(logging.INFO)
	logging.getLogger('urllib3.connectionpool').setLevel(logging.INFO)
	logger = logging.getLogger(name)
	logger.addHandler(logging.StreamHandler(sys.stdout))

	if set_permissions:
		os.chmod(logfile, 0o664)

	return logger

def isfloat(string):
	try:
		float(string)
		return True

	except:
		return False
