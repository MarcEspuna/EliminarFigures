#! /usr/bin/env python
# -*- encoding: UTF-8 -*-

import qi
import time
from math import degrees, radians
from random import uniform as randInterval
from random import random


NAO_IP = "192.168.1.100"
NAO_PORT = 9559

class Robot():

    def __init__(self, ip, port):
        self.session = qi.Session()
        try:
            self.session.connect("tcp://" + ip + ":" + str(port))
        except RuntimeError:
            print("Can't connect to Nao at IP " + ip + " on port " + str(port) + ".\n")
            raise RuntimeError

        self.motion_service = self.session.service("ALMotion")
        self.tts_service = self.session.service("ALTextToSpeech")
        self.anim_tts_service = self.session.service("ALAnimatedSpeech")
        self.leds_service = self.session.service("ALLeds")
        self.animation_service = self.session.service("ALAnimationPlayer")
        self.posture_service = self.session.service("ALRobotPosture")
        self.memory_service = self.session.service("ALMemory")

        self.configure()


    def configure(self):
        self.motion_service.wakeUp()
        self.tts_service.setLanguage("English")

    def stop(self):
        self.motion_service.rest()

    def say(self, utterance, anim=False):
        if anim:
            configuration = {"bodyLanguageMode":"contextual"}
            self.anim_tts_service.say(utterance, configuration)
        else:
            self.tts_service.say(utterance)

    def colouredEyes(self):
        self.leds_service.rasta(2)

    def headBackground(self):
        minYaw = radians(-10)
        maxYaw = radians(10)
        minPitch = radians(-5)
        maxPitch = radians(20)

        headYaw = randInterval(minYaw, maxYaw)
        headPitch = randInterval(minPitch, maxPitch)
        speed = 0.1
        self.motion_service.setAngles(["HeadYaw", "HeadPitch"],[headYaw, headPitch], speed)


    def setPosture(self, posture):
        if posture == "stand":
            self.posture_service.goToPosture("Stand", 1.0)
        elif posture == "sit":
            self.posture_service.goToPosture("Sit", 1.0)
        elif posture == "crouch":
            self.posture_service.goToPosture("Crouch", 1.0)
        else:
            print("Invalid posture: ", posture)

    def playAnimation(self):
        self.animation_service.run("animations/Stand/Gestures/Hey_1")

if __name__ == "__main__":
    robot = Robot(NAO_IP, NAO_PORT)

#    initTime = time.time()
#    while time.time() - initTime < 5:
#        robot.headBackground()
#        time.sleep(1)

#    robot.colouredEyes()
#    robot.say("See you later")

    robot.setPosture("stand")
    robot.playAnimation()
    robot.stop()
