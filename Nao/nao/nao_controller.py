"""***************************************************************************************************************
nao_controller.py
-----------------
This class sends commands to the robot depending on the input recieved
***************************************************************************************************************"""
#! /usr/bin/env python
# -*- encoding: UTF-8 -*-
"""++++++++++++++++++++++++++++++
            Imports
++++++++++++++++++++++++++++++"""

####################
# External
####################
import random
import time

####################
# Internal
####################
from nao_class_qi import Robot
from constants import *
#import interface

"""++++++++++++++++++++++++++++++
           Constants
++++++++++++++++++++++++++++++"""

MAX_TIME_BETWEEN_ERASED_FIGURES = 60
MAX_TIME_MOVED_CURSOR = 60
MAX_TIME_NO_SELECTED_FIGURE = 20 #5
MAX_ADVISE_NO_SELECTED_FIGURE = 3
MIN_TIME_BET_SPEAK = 0


####################
# Verbal comunication
####################
HELLO = "Hola, selecciona la figura a eliminar."
PRESS_BUTTON = "Ya hemos llegado a la figura! Presionemos a la vez en tres, dos, uno. Ya!"
LESS_FIGURES_ENCOURAGE = "Ya nos queda menos, vamos a terminar pronto si jugamos juntos."
VERY_GOOD_ENCOURAGE = "Lo estas haciendo muy bien, sigue jugando asi."
TRY_ERASE_ENCOURAGE = "Vamos a intentar eliminar el objeto %s."
PRESS_BUTTON_ENCOURAGE = "Recuerda que cuando lleguemos a la figura tienes que pulsar el boton eliminar."
ONE_LESS_FIGURE1 = "Una figura menos, que bien!"
ONE_LESS_FIGURE2 = "Bien! Otra figura eliminada!"
ONE_LESS_FIGURE3 = "Quedan %s figuras para eliminar, bien!" #TODO
ONE_LESS_FIGURE3_1 = "Solo queda una figura!"
ONE_LESS_FIGURE4 = "Ya queda menos!"
ONE_LESS_FIGURE5 = "Hemos eliminado el objeto %s, buen trabajo en equipo!"
ONE_LESS_FIGURE6 = "Que bien trabajamos juntos! A por la siguiente."
ONE_LESS_FIGURE7 = "Buen trabajo en equipo! Intentemos eliminar ahora el objeto %s."
ROBOT_RIGHT = "Voy hacia la derecha."
ROBOT_LEFT = "Voy hacia la izquierda."
ROBOT_UP = "Voy hacia arriba."
ROBOT_DOWN = "Voy hacia abajo."
ROBOT_STOP = "Ya he llegado."
REMEMBER_SELECT_FIGURE1 = "Debes seleccionar la siguiente figura. Ponte encima y presiona el boton seleccionar."
REMEMBER_SELECT_FIGURE2 = "Recuerda seleccionar la siguiente figura. Ponte encima y presiona el boton seleccionar."
REMEMBER_SELECT_FIGURE3 = "Por favor, selecciona la siguiente figura. Ponte encima y presiona el boton seleccionar."
FIGURE_SELECTED = "Vamos a eliminar el objeto %s"
END_GAME = "Dejamos el juego por hoy. Espero volver a verte pronto."
"""++++++++++++++++++++++++++++++
           CLASS
++++++++++++++++++++++++++++++"""

class RController():

    """++++++++++++++++++++++++++++++
               Functions
    ++++++++++++++++++++++++++++++"""

    '''*********************************************************************
    *Name: __init__
    *Funcition: init of the class
    *Parameters: -
    *Return: -
    *********************************************************************'''
    def __init__(self, ip, port):
        #interface.create_interface(nao = Robot())

        self.nao = Robot(ip, port)

        self.last_time_fig_erased = time.time()
        self.last_time_user_moved = time.time()
        self.no_figure_selected = time.time()
        self.old_timestamp = time.time()
        self.last_time_spoke = time.time()
        self.previous_rob_movement = Movement.STOP
        self.count_advise_no_figure_selec = 0
        self.advise_select_figure = True
        self.message_on_figure_said = False

    '''*********************************************************************
    *Name: set_objects
    *Funcition: inits the objects names
    *Parameters: names_figures: names of the objects/figures
    *Return: -
    *********************************************************************'''
    def set_objects(self, names_figures):
        self.name_figures = names_figures

    '''*********************************************************************
    *Name: set_timestamp
    *Funcition: inits the timestamp
    *Parameters: timestamp: timestamp of the first  message
    *Return: -
    *********************************************************************'''
    def set_timestamp(self, timestamp):
        self.nao.stop_talking()
        self.nao.say(HELLO)
        self.last_time_fig_erased = timestamp
        self.last_time_user_moved = timestamp
        self.no_figure_selected = timestamp
        self.old_timestamp = timestamp
        self.last_time_spoke = timestamp

    '''*********************************************************************
    *Name: check_figure_name
    *Funcition: checks the name of the figure
    *Parameters: id_figure: ID of the figure
    *Return: The name of the fiture
    *********************************************************************'''
    def check_figure_name(self, id_figure):
        id_figure = int(id_figure)
        return self.name_figures[id_figure]

    '''*********************************************************************
    *Name: random_encourage
    *Funcition: Randomly encourage the user
    *Parameters: next_figure: which is the figure to be erased next
    *Return: -
    *********************************************************************'''
    def random_encourage(self, next_figure):
        self.nao.setNaoFace(self.nao.NEUTRAL)
        rand = random.randint(0, 3)

        if rand == 0:
            self.nao.stop_talking()
            self.nao.say(LESS_FIGURES_ENCOURAGE)
        elif rand == 1:
            self.nao.stop_talking()
            self.nao.say(VERY_GOOD_ENCOURAGE)
        elif rand == 2:
            figure_name = self.check_figure_name(next_figure)
            self.nao.stop_talking()
            self.nao.say(TRY_ERASE_ENCOURAGE % figure_name)
        else:
            self.nao.stop_talking()
            self.nao.say(PRESS_BUTTON_ENCOURAGE)

    '''*********************************************************************
    *Name: check_child
    *Funcition: Checks if the child is present (face recongition)
    *Parameters: next_figure: figure to erase
    *Return: -
    *********************************************************************'''
    def check_child(self, next_figure):
        #TODO
        no_child= False
        if no_child:
            self.random_encourage(next_figure)
            return True
        return False

    '''*********************************************************************
    *Name: check_on_figure
    *Funcition: Checks if they are on the selected figure
    *Parameters: -
    *Return: -
    *********************************************************************'''
    def check_on_figure(self, sckt, timestamp, on_figure, to_figure):
        if on_figure and not self.message_on_figure_said:
            self.message_on_figure_said = True
            first_time = True
            need_low_arm = False
            erased_figure = Figures_IDs.NO_FIGURE
            init_timestamp = timestamp
            while on_figure and erased_figure == Figures_IDs.NO_FIGURE:
                timestamp , robot_movement, person_movement, to_figure, erased_figure, remaining_figures, on_figure = sckt.get_data()
                if first_time:
                    self.nao.stop_talking()
                    self.nao.say(PRESS_BUTTON)
                    self.nao.moveArm()
                    first_time = False
                else:
                    #TODO provar amb el marc
                    if need_low_arm == False and timestamp - init_timestamp > 10:
                        self.nao.posture_service.goToPosture("Stand", 1.0)
                        self.nao.say("Vaya, no has pulsado. Pulsa cuando quieras.")
                    #    pitch = 0 * 3.14 / 180.0
                    #    self.nao.motion_service.setAngles(["LShoulderPitch"],[pitch], 0.1)
                        need_low_arm = True
            if need_low_arm:
                self.nao.posture_service.goToPosture("Crouch", 1.0)

            #self.nao.stop_talking()
            #self.nao.say(PRESS_BUTTON)
            #self.nao.moveArm()
            return True
        return False

    '''*********************************************************************
    *Name: check_child_movement
    *Funcition: Checks the child's game movement
    *Parameters: -
    *Return: -
    *********************************************************************'''
    def check_child_movement(self, timestamp, mov, on_figure, next_figure):
        if mov == Movement.STOP and timestamp - self.last_time_user_moved > MAX_TIME_MOVED_CURSOR:
            #if on_figure:
            #    self.nao.stop_talking()
            #    self.nao.say(PRESS_BUTTON)
            #    self.nao.moveArm()
            #    return True
            #else:
            self.random_encourage(next_figure)
            return True
        else:
            self.last_time_user_moved = timestamp
            return False

    '''*********************************************************************
    *Name: check_time_erased_figuremain
    *Funcition: Checks the last time a figure was erased
    *Parameters: timestamp: time sended the message
    *            next_figure: which is the next figure to be erased
    *Return: Ture if MAX_TIME_BETWEEN_ERASED_FIGURES has passed. False
    *        otherwise
    *********************************************************************'''
    def check_time_erased_figure(self, timestamp, next_figure):
        if timestamp - self.last_time_fig_erased > MAX_TIME_BETWEEN_ERASED_FIGURES:
            self.random_encourage(next_figure)
            return True
        return False

    '''*********************************************************************
    *Name: check_figure_erased
    *Funcition: Checks what figure has been erased
    *Parameters: timestamp: time sended the message
    *            fig_erased: id figure erased
    *            num_figures: number of figures remaining
    *            next_figure: which is the next figure to be erased
    *Return: True if executed. False otherwise
    *********************************************************************'''
    def check_figure_erased(self, timestamp, fig_erased, num_figures, next_figure):
        if fig_erased != Figures_IDs.NO_FIGURE:
            #save time
            self.last_time_fig_erased = timestamp
            self.nao.setNaoFace(self.nao.HAPPY3)
            rand = random.randint(0, 5)
            if rand == 0:
                self.nao.stop_talking()
                self.nao.say(ONE_LESS_FIGURE1)
            elif rand == 1:
                self.nao.stop_talking()
                self.nao.say(ONE_LESS_FIGURE2)
            elif rand == 2:
                if num_figures > '1':
                    self.nao.stop_talking()
                    self.nao.say(ONE_LESS_FIGURE3 % str(num_figures))
                else:
                    self.nao.stop_talking()
                    self.nao.say(ONE_LESS_FIGURE3_1)
            elif rand == 3:
                self.nao.stop_talking()
                self.nao.say(ONE_LESS_FIGURE4)
            elif rand == 4:
                name_figure = self.check_figure_name(fig_erased)
                self.nao.stop_talking()
                self.nao.say(ONE_LESS_FIGURE5 % name_figure)
            elif rand == 5:
                self.nao.stop_talking()
                self.nao.say(ONE_LESS_FIGURE6)
            return True
        return False
    '''*********************************************************************
    *Name: check_robot_movement
    *Funcition: Checks the game movment of the robot
    *Parameters: -
    *Return: True if the action has been performed. False otherwise.
    *********************************************************************'''
    def check_robot_movement(self, robot_movement):
        self.nao.setNaoFace(self.nao.HAPPY2)
        #rand = random.randint(0, 1)
        #if rand == 0:
        #if self.previous_rob_movement != robot_movement:
        if robot_movement == Movement.RIGHT:
            #TODO treure?
            #self.nao.stop_talking()
            self.nao.say(ROBOT_RIGHT)
            self.nao.setNaoHead(self.nao.RIGHT)
        elif robot_movement == Movement.LEFT:
            #self.nao.stop_talking()
            self.nao.say(ROBOT_LEFT)
            self.nao.setNaoHead(self.nao.LEFT)
        elif robot_movement == Movement.UP:
            #self.nao.stop_talking()
            self.nao.say(ROBOT_UP)
            self.nao.setNaoHead(self.nao.UP)
        elif robot_movement == Movement.DOWN:
            #self.nao.stop_talking()
            self.nao.say(ROBOT_DOWN)
            self.nao.setNaoHead(self.nao.DOWN)
        elif robot_movement == Movement.STOP:
            #self.nao.stop_talking()
            self.nao.say(ROBOT_STOP)
            self.nao.setNaoHead(self.nao.STOP)
        #else:
        #    return False
        #else:
        #    return False
        return True

    '''*********************************************************************
    *Name: check_robot_action
    *Funcition: Checks the action to perform by the robot
    *Parameters: -
    *Return: -
    *********************************************************************'''
    def check_robot_action(self, sckt, timestamp , robot_movement, person_movement, to_figure, erased_figure, remaining_figures, on_figure):
        #Check if there is a figure selected
        #If not
        if to_figure == Figures_IDs.NO_FIGURE:
            self.check_figure_erased(timestamp, erased_figure, remaining_figures, to_figure)
            self.message_on_figure_said = False
            self.advise_select_figure = True
            if timestamp - self.no_figure_selected >= MAX_TIME_NO_SELECTED_FIGURE:
                self.count_advise_no_figure_selec += 1
                #resetegem timestamp
                self.no_figure_selected = timestamp
                if self.count_advise_no_figure_selec <= MAX_ADVISE_NO_SELECTED_FIGURE:
                    self.nao.setNaoFace(self.nao.SAD)
                    if self.count_advise_no_figure_selec == 1:
                        self.nao.stop_talking()
                        self.nao.say(REMEMBER_SELECT_FIGURE1)
                    elif self.count_advise_no_figure_selec == 2:
                        self.nao.stop_talking()
                        self.nao.say(REMEMBER_SELECT_FIGURE2)
                    else:
                        self.nao.stop_talking()
                        self.nao.say(REMEMBER_SELECT_FIGURE3)
        #If so
        else:
            #Advise the figure
            if self.advise_select_figure:
                self.nao.setNaoFace(self.nao.HAPPY1)
                #self.nao.stop_talking()
                self.nao.say(FIGURE_SELECTED % self.check_figure_name(to_figure))
                self.advise_select_figure = False
                self.last_time_spoke = timestamp
                #Check the robot movement
                self.check_robot_movement(robot_movement)

            #Check if the timestamp has changed
            if self.old_timestamp != timestamp and timestamp - self.last_time_spoke >= MIN_TIME_BET_SPEAK:
                if robot_movement == Movement.FINISH:
                    self.nao.say(END_GAME)
                elif self.check_on_figure(sckt, timestamp, on_figure, to_figure):
                    pass
                #Check if there is the child
                elif self.check_child(to_figure):
                    pass
                #Check if a figure has been erased
                #elif self.check_figure_erased(timestamp, erased_figure, remaining_figures, to_figure):
                #    pass
                #Check the movement of the child
                elif self.check_child_movement(timestamp, person_movement, on_figure, to_figure):
                    pass
                #Check the last time a figure has been erased
                elif self.check_time_erased_figure(timestamp, to_figure):
                    pass
                #Check the robot movement
                #elif self.check_robot_movement(robot_movement):
                #    pass

            #Save last time user moved
            if person_movement != Movement.STOP:
                self.last_time_user_moved = timestamp

            #Save new values
            self.old_timestamp = timestamp
            self.previous_rob_movement = robot_movement

            #Reset flags and counter
            self.count_advise_no_figure_selec = 0
            self.no_figure_selected = timestamp
            self.last_time_spoke = timestamp
