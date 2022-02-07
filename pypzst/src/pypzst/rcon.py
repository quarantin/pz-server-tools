import os
import sys
import socket
import struct
from retry import retry

class Client:

	SERVERDATA_AUTH           = 3
	SERVERDATA_AUTH_RESPONSE  = 2
	SERVERDATA_EXECCOMMAND    = 2
	SERVERDATA_RESPONSE_VALUE = 0

	def __init__(self, host, port, password):
		self.rcon_id = os.urandom(4)
		self.password = password
		self.socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
		self.socket.connect((host, int(port)))
		self.login()

	def send_packet(self, rcon_type, body):
		body = bytes(body, encoding='utf-8')
		payload = struct.pack('I4sI%sscc' % len(body), 10 + len(body), self.rcon_id, rcon_type, body, b'\x00', b'\x00')
		return self.socket.send(payload)

	def serverdata_auth(self):
		return self.send_packet(Client.SERVERDATA_AUTH, self.password)

	def serverdata_auth_response(self):

		response = self.socket.recv(4096)
		# It should be 14 but Project Zomboid RCON implementation is broken
		if len(response) != 28:
			raise Exception('Invalid response from server: Too short')

		size, sess_id, rcon_type, b1, b2 = struct.unpack('IIIcc', response[:14])
		sess_id = sess_id.to_bytes(4, byteorder='little')
		if sess_id != self.rcon_id:
			raise Exception('Authentication failure')

	def serverdata_execcommand(self, command):
		return self.send_packet(Client.SERVERDATA_EXECCOMMAND, command)

	def serverdata_response_value(self):

		response = self.socket.recv(4096)
		if len(response) < 14:
			raise Exception('Invalid response from server: Too short')

		size, sess_id, rcon_type = struct.unpack('III', response[:12])
		sess_id = sess_id.to_bytes(4, byteorder='little')
		if sess_id != self.rcon_id:
			raise Exception('Invalid RCON ID')

		return response[12:-2]

	@retry(tries=10, delay=0.1)
	def login(self):
		self.serverdata_auth()
		self.serverdata_auth_response()

	@retry(tries=10, delay=0.1)
	def run(self, command):
		self.serverdata_execcommand(command)
		response = self.serverdata_response_value()
		if not response:
			raise Exception('Empty response from server')

		return response.decode('utf-8')
