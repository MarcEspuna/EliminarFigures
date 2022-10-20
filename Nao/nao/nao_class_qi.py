#! /usr/bin/env python
# -*- encoding: UTF-8 -*-

import qi
import motion
import time
from math import degrees, radians
from random import uniform as randInterval
from random import random


NAO_IP = "192.168.1.141"
NAO_PORT = 9559

class Robot():
    VEL_SPEACH = 100
    #COLORS
    WHITE   = 0x00FFFFFF
    YELLOW  = 0x00FFFF00
    GREEN   = 0x0000FF00
    BLUE    = 0x000000FF
    RED     = 0x00FF0000
    CYAN    = 0x0000FFFF
    MAGENTA = 0x00FF00FF
    #FACES
    HAPPY1  = 'happy1'
    HAPPY2  = 'happy2'
    HAPPY3  = 'happy3'
    NEUTRAL = 'neutral'
    ANGRY   = 'angry'
    SAD     = 'sad'
    #HEAD MOVEMENT
    RIGHT   = 'right'
    LEFT    = 'left'
    UP      = 'up'
    DOWN    = 'down'
    STOP    = 'stop'

    def __init__(self, ip = NAO_IP, port = NAO_PORT):
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
        self.face_service = self.session.service("ALFaceDetection")

        self.configure()


    def configure(self):
        #self.motion_service.wakeUp()
        self.posture_service.goToPosture("Crouch", 1.0)
        self.motion_service.rest()
        self.tts_service.setLanguage("Spanish")
        self.face_service.subscribe("Test_Face", 500, 0.0)

    def stop(self):
        self.motion_service.rest()

    def stop_talking(self):
        self.tts_service.stopAll()

    def say(self, utterance, anim=False):
        if anim:
            configuration = {"bodyLanguageMode":"contextual"}
            self.anim_tts_service.say(utterance, configuration, _async=True)
        else:
            self.tts_service.setParameter("speed", self.VEL_SPEACH)
            self.tts_service.say(utterance, _async=True)

    def moveArm(self):
        self.posture_service.goToPosture("Stand", 1.0)
        #self.motion_service.setStiffnesses("LShoulderPitch", 1.0) #Angles màxims: 119.5 (endarrera) i -119.5 (endavant)
        #self.motion_service.setStiffnesses("RHipYawPitch", 1.0)
        #self.motion_service.setStiffnesses("LHipYawPitch", 1.0)
        #self.motion_service.setStiffnesses("LShoulderRoll", 1.0)
        #self.motion_service.setStiffnesses("LHipPitch", 1.0)
        #self.motion_service.setStiffnesses("LHipRoll", 1.0)
        #self.motion_service.setStiffnesses("RHipPitch", 1.0)
        #self.motion_service.setStiffnesses("RHipRoll", 1.0)
        #self.motion_service.setStiffnesses("LWristYaw", 1.0)

        pitch = 0 * 3.14 / 180.0
        self.motion_service.setAngles(["LShoulderPitch"],[pitch], 0.1)
        time.sleep(4.5)
        pitch = 100.0 * 3.14/180.0
        self.motion_service.setAngles(["LShoulderPitch"],[pitch], 0.5)
        time.sleep(1.5)
        self.posture_service.goToPosture("Crouch", 1.0)

    def colouredEyes(self):
        self.leds_service.rasta(2)

    def setNaoFace(self, emotion):
        if emotion == self.HAPPY1:
            self.leds_service.rotateEyes(self.YELLOW, 1, 2, _async=True)
        elif emotion == self.HAPPY2:
            self.leds_service.rotateEyes(self.YELLOW, 0.5, 2, _async=True)
        elif emotion == self.HAPPY3:
            self.leds_service.rotateEyes(self.YELLOW, 0.25, 2, _async=True)
        elif emotion == self.NEUTRAL:
            self.leds_service.rotateEyes(self.WHITE, 4, 2, _async=True)
        elif emotion == self.ANGRY:
            self.leds_service.rotateEyes(self.RED, 4, 20, _async=True)
        elif emotion == self.SAD:
            self.leds_service.rotateEyes(self.CYAN, 0.5, 2, _async=True)


    def headBackground(self):
        minYaw = radians(-10)
        maxYaw = radians(10)
        minPitch = radians(-5)
        maxPitch = radians(20)

        headYaw = randInterval(minYaw, maxYaw)
        headPitch = randInterval(minPitch, maxPitch)
        speed = 0.1
        self.motion_service.setAngles(["HeadYaw", "HeadPitch"],[headYaw, headPitch], speed, _async=True)

    def setNaoHead(self, movement):
        self.motion_service.setStiffnesses("Head", 1.0)
        if movement == self.UP or movement == self.DOWN:
            if movement == self.UP:
                pitch1=0.0
                pitch2=20.0/180.0*3.14
            elif movement == self.DOWN:
                pitch1=20.0/180.0*3.14
                pitch2=0.0
            yaw=0.0
            speed = 0.5
            self.motion_service.setAngles(["HeadYaw", "HeadPitch"],[yaw, pitch1], speed, _async=True)
            speed=0.1
            self.motion_service.setAngles(["HeadYaw", "HeadPitch"],[yaw, pitch2], speed, _async=True)
        elif movement == self.RIGHT or movement == self.LEFT:
            if movement == self.RIGHT:
                yaw1=60.0/180.0*3.14
                yaw2=-60.0/180.0*3.14
            elif movement == self.LEFT:
                yaw1=-60.0/180.0*3.14
                yaw2=60.0/180.0*3.14
            pitch=0.0
            speed = 0.5
            self.motion_service.setAngles(["HeadYaw", "HeadPitch"],[yaw1, pitch], speed, _async=True)
            speed=0.1
            self.motion_service.setAngles(["HeadYaw", "HeadPitch"],[yaw2, pitch], speed, _async=True)
        else:
            self.motion_service.setAngles(["HeadYaw", "HeadPitch"],[0, 0], 0.1, _async=True)

    def setPosture(self, posture):
        if posture == "stand":
            self.posture_service.goToPosture("Stand", 1.0)
        elif posture == "sit":
            self.posture_service.goToPosture("Sit", 1.0)
        elif posture == "crouch":
            self.posture_service.goToPosture("Crouch", 1.0)
        else:
            print("Invalid posture: %s" % posture)

    def playAnimation(self):
        self.animation_service.run("animations/Stand/Gestures/Hey_1")

    def openCloseHand(self, open_close):
        if open_close:
            self.motion_service.openHand("LHand")
        else:
            self.motion_service.closeHand("LHand")

    def getFaces(self):
        # ALMemory variable where the ALFaceDetection module
        # outputs its results.
        memValue = "FaceDetected"

        # A simple loop that reads the memValue and checks whether faces are detected.
        for i in range(0, 20):
            time.sleep(0.5)
            val = self.memory_service.getData(memValue, 0)
            print ""
            print "\*****"
            print ""

        # Check whether we got a valid output: a list with two fields.
        if(val and isinstance(val, list) and len(val) == 2):
            # We detected faces !
            # For each face, we can read its shape info and ID.
            # First Field = TimeStamp.
            timeStamp = val[0]
            # Second Field = array of face_Info's.
            faceInfoArray = val[1]

            try:
                # Browse the faceInfoArray to get info on each detected face.
                for faceInfo in faceInfoArray:
                    # First Field = Shape info.
                    faceShapeInfo = faceInfo[0]
                    # Second Field = Extra info (empty for now).
                    faceExtraInfo = faceInfo[1]
                    print "  alpha %.3f - beta %.3f" % (faceShapeInfo[1], faceShapeInfo[2])
                    print "  width %.3f - height %.3f" % (faceShapeInfo[3], faceShapeInfo[4])
            except Exception, e:
                print "faces detected, but it seems getData is invalid. ALValue ="
                print val
                print "Error msg %s" % (str(e))
        else:
            print "Error with getData. ALValue = %s" % (str(val))
            # Unsubscribe the module.

        self.face_service.unsubscribe("Test_Face")
        print "Test terminated successfully."
