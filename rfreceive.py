#!/usr/bin/env python
# -*- coding: utf-8 -*-
#
# rfreceive.py
# Description : This code receives RF codes and trigger actions
# 

import sys, os, datetime, time

from time import sleep, clock
from datetime import datetime as dt
from RPi.GPIO import cleanup
from pi_switch import RCSwitchReceiver, RCSwitchSender

class valor:
		sensor_num = 0
		sensor_desc = "zzzz"
		numero = float(0.1)
		momento = time.time()

def procesa_valor(valor_recibido):
	global lista_valores
	grabar = False 
        sensor = valor_recibido / 1000000
        if sensor > len(lista_valores) or sensor < 1:
                print "valor recivido erroneo: %s" % str(valor_recibido)
                return 
	tiempo_transcurrido = time.time() - lista_valores[sensor].momento
	valor = float(valor_recibido - sensor * 1000000) / 100
        print ("sensor %s valor_recibido: %s valor: %s tiempo_transcurido %s") % (str(sensor), str(valor_recibido), str(valor), str(tiempo_transcurrido) )
	if tiempo_transcurrido < 60:
		print "no ha pasado suficiente tiempo desde la ultima lectura de este sensor %s\n"  % tiempo_transcurrido
	elif valor == lista_valores[sensor].numero:
		print "no ha cambiado el valor %s\n" % valor
	else:
		lista_valores[sensor].sensor_num = sensor
		lista_valores[sensor].numero = valor
		lista_valores[sensor].momento = time.time() 
		# ajustes(sensor) 
                imprime_lista()
		grabar_valor(sensor)

def grabar_valor(sensor):
	grabar = "/usr/bin/python /home/pi/actualiz_GS.py " + lista_valores[sensor].sensor_desc + " " + str(lista_valores[sensor].numero)
        print "ejecutando grabar: %s" % grabar
#       res = os.system(grabar)
	print "resultado grabar: %s\n" % res

def imprime_lista():
        for i in range(1,len( lista_valores)): 
            print i, lista_valores[i].sensor_desc, lista_valores[i].numero, str(lista_valores[i].momento) 

if __name__ == '__main__':

    lista_valores = [ valor() for i in range(19)]
		
    lista_valores[1].sensor_desc = 'void'
    lista_valores[2].sensor_desc = 'void'
    lista_valores[3].sensor_desc = 'void'
    lista_valores[4].sensor_desc = 'void'
    lista_valores[5].sensor_desc = 'void'
    lista_valores[6].sensor_desc = ' hum06'
    lista_valores[7].sensor_desc = 'temp07'
    lista_valores[8].sensor_desc = 'temp08'
    lista_valores[9].sensor_desc = ' hum09'
    lista_valores[10].sensor_desc = 'temp10'
    lista_valores[11].sensor_desc = ' hum11'
    lista_valores[12].sensor_desc = ' bat12'
    lista_valores[13].sensor_desc = 'temp13'
    lista_valores[14].sensor_desc = ' hum14'
    lista_valores[15].sensor_desc = 'pres15'
    lista_valores[16].sensor_desc = 'P_Te16'
    lista_valores[17].sensor_desc = 'void'
    lista_valores[18].sensor_desc = 'void'

    print lista_valores[1].sensor_desc

#  inicializamos RCSwitch
    receiver = RCSwitchReceiver()
    receiver.enableReceive(0)
    procesa_valor(14003540)
    imprime_lista()

    while True:
           print dt.now() 
#           procesa_valor(14003540)
           if receiver.available():
               received_value = receiver.getReceivedValue()
               if received_value:
                   print("Value: %s | Bit: %s | Protocol: %s" % (received_value,
                        receiver.getReceivedBitlength(),
                        receiver.getReceivedProtocol()))
		   procesa_valor(received_value)
		   receiver.resetAvailable() 
           sleep(10)
