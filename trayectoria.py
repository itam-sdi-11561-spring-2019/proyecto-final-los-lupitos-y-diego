import rospy
import math
import time 
import serial
from geometry_msgs.msg import Pose2D
from graphical_client.msg import Pose2D_Array

port = "/dev/ttyUSB0"
arduino = serial.Serial(port, 57600)


def init_pose():
	pose = Pose2D()
	pose.x = 0
	pose.y = 0
	pose.theta = 0
	return pose

miCoord = init_pose()
coorObjetivo = init_pose()

def mPos(data):
	global miCoord
	miCoord.x = data.x
	miCoord.y = data.y
	miCoord.theta = data.theta
	go_to_goal()


def objetivo():
	global coorObjetivo
	coorObjetivo.x = data.x
	coorObjetivo.y = data.y
	coorObjetivo.theta = data.theta

def go_to_goal():
	global miCoord
	global coorObjetivo

	xInicial = miCoord.x
	yInicial = miCoord.y
	tInicial = miCoord.theta

	xFinal = miCoord.x
	yFinal = miCoord.y

	k1 = 0.15
	Distancia = abs(math.sqrt(((xFinal - xInicial)**2) + ((yFinal - yInicial)**2)))
	vel_lineal = k1 *Distancia

	k2 = 1
	theta_Final = math.atan2(yFinal - yInicial, xFinal - xInicial)
	vel_angular = ka * (theta_Final - tInicial)

	velIzq = (vel_lineal - ((110*vel_angular)/2))/21
	velDer = (vel_lineal + ((110*vel_angular)/2))/21

	cadena_vel = str(velIzq) +'_'+str(velDer)
	print(cadena_vel)
	arduino.write(cadena_vel)
	time.sleep(2)

def talker():
	global Pose2D
	rospy.init_node('talker', anonymous = True)
	pub = rospy.Publisher('/trayectoria1', Pose2D_Array, queu_size = 6)
	rospy.Subscriber("/y_r0", Pose2D, miPos)
	rospy.Subscriber("/ball", Pose2D, miPos)
	while not rospy.is_shutdown():
		arr = Pose2D_Array()
	for i in range(6):
		pose = init_pose()
		pose.x = 100 * (i + 1)
		pose.y = 100 * (i + 1) * aux
		pose.theta += 0.785 *i 
		aux *= -1
	print "the array is: ", arr
	pub.publish(arr)
	rate.sleep()

if __name__ = '__main__':
	try: 
		talker()
	except rospy.ROSInterruptException
		pass








