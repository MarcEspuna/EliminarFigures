import socket
import sys
import time
import struct

objetoSocket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
datos_servidor = ('localhost', 10000)
print >>sys.stderr, 'empezando a levantar %s puerto %s' % datos_servidor
objetoSocket.bind(datos_servidor)

objetoSocket.listen(1)
while True:
    print >>sys.stderr, 'Esperando para realizar conexion'
    connection, datos_cliente = objetoSocket.accept()

    try:
        print >>sys.stderr, 'conexion desde', datos_cliente
        while True:
            print(connection.recv(1))
    finally:
         connection.close()
