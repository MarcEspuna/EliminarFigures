from nao.nao_controller import RController
from comunication.data_receiver import Client_Socket
import comunication.data_receiver as dt_rcv
from nao.constants import Figures_Names

import sys

def main():
    #init flags
    first_time = True

    #Connection to nao
    ip = raw_input("Introduce the robot IP (default = 192.168.1.140):\n")
    port = raw_input("\nIntroduce the robot Port (default = 9559):\n")
    if ip == "":
        ip = "192.168.1.140"
    if port == "":
        port = 9559
    nao = RController(ip, int(port))

    nao.nao.openCloseHand(True)
    raw_input("Press intro when the pirulu is added")
    nao.nao.openCloseHand(False)

    #Connection to the game
    ip = raw_input("Introduce the game IP (default = 'localhost'):\n")
    port = raw_input("\nIntroduce the game Port (default = 10000):\n")
    if ip == "":
        ip = 'localhost'
    if port == "":
        port = 10000
    sckt_recv = Client_Socket(ip, int(port))

    sckt_recv.get_objects(Figures_Names.NAMES_FIGURES)

    print(Figures_Names.NAMES_FIGURES)

    nao.set_objects(Figures_Names.NAMES_FIGURES)

    robot_movement = 'S'
    while(robot_movement != 'F'):
        try:
            timestamp , robot_movement, person_movement, to_figure, erased_figure, remaining_figures, on_figure = sckt_recv.get_data()

            #If it is the first time, we save the timestamp
            if first_time == True:
                first_time = False
                nao.set_timestamp(timestamp)

            #Check what nao must do
            nao.check_robot_action(sckt_recv, timestamp , robot_movement, person_movement, to_figure, erased_figure, remaining_figures, on_figure)

        except KeyboardInterrupt:
            sckt_recv.close_socket()
            nao.nao.stop()
            sys.exit()

    sckt_recv.close_socket()
    nao.nao.stop()
    nao.nao.say("Fin del juego. Ha sido un placer. Espero volver a verte.")

if __name__ == "__main__":
    main()
