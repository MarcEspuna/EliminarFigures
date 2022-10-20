
import socket
import sys
import struct

MESSAGE_LENGTH = 14

class Client_Socket():
    def __init__(self, ip, port):
        self.game_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

        datos_servidor = (ip, port)
        self.game_socket.connect(datos_servidor)

    def get_objects(self, objs):
        try:
            header = 'c'
            while header != 'f':
                header = self.game_socket.recv(1)
                header = struct.unpack('c', header)[0]
                length = self.game_socket.recv(1)
                length = struct.unpack('c', length)[0]
                length = length.encode("hex")
                length = int(length,16)
                if header == 'c':
                    print(header)
                    print(length)
                    name = self.game_socket.recv(length)
                    print(name)
                    name=name[:-1]
                    objs.append(name)
        except:
            raise KeyboardInterrupt

    def get_data(self):
        try:

            received_data = 0
            data=str()
            while received_data < MESSAGE_LENGTH:
                data += self.game_socket.recv(MESSAGE_LENGTH)
                received_data += len(data)
                print >>sys.stderr, 'recibido "%s"' % data
                if len(data) == 0:
                    raise KeyboardInterrupt
            timestamp = struct.unpack('d', data[0:8])[0]
            robot_movement = struct.unpack('c', data[8])[0]
            person_movement = struct.unpack('c', data[9])[0]
            to_figure = struct.unpack('c', data[10])[0]
            erased_figure = struct.unpack('c', data[11])[0]
            remaining_figures = struct.unpack('c', data[12])[0]
            on_figure = struct.unpack('c', data[13])[0] == 'Y'
            print(timestamp , robot_movement, person_movement, to_figure, erased_figure, remaining_figures, on_figure)
            return timestamp , robot_movement, person_movement, to_figure, erased_figure, remaining_figures, on_figure
        except:
            raise KeyboardInterrupt

    def close_socket(self):
        print >>sys.stderr, 'cerrando socket'
        self.game_socket.close()
