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
            timestamp=struct.pack('d',time.time())
            #timestamp = bytes(time.time())
            robot_movement = struct.pack('c','L')
            person_movement = struct.pack('c','S')
            to_figure = struct.pack('c',str(2))
            erased_figure = struct.pack('c',str(0))
            remaining_figures = struct.pack('c',str(3))
            on_figure = struct.pack('c','Y')
            data = timestamp+robot_movement+person_movement+to_figure+erased_figure+remaining_figures+on_figure
            print >>sys.stderr, 'enviando mensaje al cliente'
            print(data)
            connection.sendall(data)
            time.sleep(2)
    finally:
         connection.close()
