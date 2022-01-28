#!/usr/bin/env python

import sys
import logging
import requests

import os
from os.path import exists, isfile, join

from retry import retry
from pwd import getpwnam, getpwuid


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

	return None

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

def parse_server_config(server):

	server_config = {}

	server_ini = '/home/%s/Zomboid/Server/servertest.ini' % server
	if not isfile(server_ini):
		return server_config

	with open(server_ini) as fd:
		lines = fd.readlines()

	for line in lines:

		line = line.strip()
		if not line:
			continue

		tokens = line.split('=', 1)

		key = tokens[0].strip()
		value = tokens[1].strip()

		values = set()
		for value in value.split(';'):
			value = value.strip()
			if value:
				values.add(value)

		server_config[key] = values

		if not values:
			server_config[key] = ''

		elif len(values) == 1:
			server_config[key] = next(iter(values))

	return server_config
