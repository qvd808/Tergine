import pygame
import math
import numpy as np

# pygame setup
pygame.init()
SCREEN_WIDTH = 1280
SCREEN_HEIGHT = 720
screen = pygame.display.set_mode((SCREEN_WIDTH,SCREEN_HEIGHT))
clock = pygame.time.Clock()
running = True

def crossProduct(triTranslated):
	temp1 = np.zeros(3)
	temp1[0] = triTranslated.points[1][0] - triTranslated.points[0][0]
	temp1[1] = triTranslated.points[1][1] - triTranslated.points[0][1]
	temp1[2] = triTranslated.points[1][2] - triTranslated.points[0][2]

	temp2 = np.zeros(3)
	temp2[0] = triTranslated.points[2][0] - triTranslated.points[0][0]
	temp2[1] = triTranslated.points[2][1] - triTranslated.points[0][1]
	temp2[2] = triTranslated.points[2][2] - triTranslated.points[0][2]

	output = np.cross(temp1, temp2)
	l = math.sqrt(output[0] * output[0]) + (output[1] * output[1]) + (output[2] * output[2])
	if l!= 0:
		output[0] /= l
		output[1] /= l
		output[2] /= l
	
	return output

class Point:
	def __init__(self,x , y) -> None:
		self.x = x
		self.y = y

class Triangle:
	def __init__(self, points) -> None:
		self.points = []
		for point in points:
			if len(point) == 3:
				vec = np.zeros(4)
				vec[:3] = point
				self.points.append(vec)
			else:
				self.points.append(np.array(point))

	def draw(self):

		new_point_x = self.translate_coor(self.points[0])
		new_point_y = self.translate_coor(self.points[1])
		new_point_z = self.translate_coor(self.points[2])
		# print(new_point_x, new_point_y, new_point_z)
		pygame.draw.line(screen, (255, 255, 255), new_point_x, new_point_y, 1)
		pygame.draw.line(screen, (255, 255, 255), new_point_x, new_point_z, 1)
		pygame.draw.line(screen, (255, 255, 255), new_point_y, new_point_z, 1)
	
	def translate_coor(self,point):
		x = point[0]
		y = point[1]
		x_new = (SCREEN_WIDTH * (x + 1)) / 2
		y_new = (SCREEN_HEIGHT * (y + 1)) / 2
		return (x_new, y_new)

#SOUTH
tri1 = Triangle([[0.0, 0.0, 0.0], [0.0, 1.0, 0.0], [1.0, 1.0, 0.0]])
tri2 = Triangle([[0.0, 0.0, 0.0], [1.0, 1.0, 0.0], [1.0, 0.0, 0.0]])

#EAST
tri3 = Triangle( [[1.0, 0.0, 0.0], [1.0, 1.0, 0.0], [1.0, 1.0, 1.0]]);
tri4 = Triangle( [[1.0, 0.0, 0.0], [1.0, 1.0, 0.0], [1.0, 0.0, 1.0]]);

#NORTH
tri5 = Triangle( [[1.0, 0.0, 0.0], [1.0, 1.0, 1.0], [0.0, 1.0, 1.0]]);
tri6 = Triangle( [[1.0, 0.0, 1.0], [0.0, 1.0, 1.0], [0.0, 0.0, 1.0]]);

#WEST
tri7 = Triangle( [[0.0, 0.0, 1.0], [0.0, 1.0, 1.0], [1.0, 1.0, 1.0]]);
tri8 = Triangle( [[0.0, 0.0, 1.0], [0.0, 1.0, 0.0], [0.0, 0.0, 0.0]]);

#TOP
tri9 = Triangle( [[0.0, 1.0, 0.0], [0.0, 1.0, 1.0], [1.0, 1.0, 1.0]]);
tri10 = Triangle([[0.0, 1.0, 0.0], [1.0, 1.0, 1.0], [1.0, 1.0, 0.0]])

#BOTTOM
tri11 = Triangle( [[1.0, 0.0, 1.0], [0.0, 0.0, 1.0], [0.0, 0.0, 0.0]]);
tri12 = Triangle( [[1.0, 0.0, 1.0], [0.0, 0.0, 0.0], [1.0, 0.0, 0.0]]);


list_of_triangle = [tri1, tri2, tri3, tri4, tri5, tri6, tri7, tri8, tri9, tri10, tri11, tri12]

fNear = 0.1
fFar = 1000.0
fFov = 90.0
fAspectRatio = SCREEN_HEIGHT/SCREEN_WIDTH
fFovRad = 1.0 /math.tan(fFov * 0.5 / 180.0 * 3.14159);

matProj = np.zeros((4, 4))
matProj[0][0] = fAspectRatio * fFovRad;
matProj[1][1] = fFovRad;
matProj[2][2] = fFar / (fFar - fNear);
matProj[3][2] = (-fFar * fNear) / (fFar - fNear);
matProj[2][3] = 1.0;
matProj[3][3] = 0.0;

matRotZ = np.zeros((4, 4))
matRotX = np.zeros((4, 4))

fTheta = 0;
fElapsedTime = 0.05;
# CAMERA = {0};

while running:
	# poll for events
	# pygame.QUIT event means the user clicked X to close your window
	for event in pygame.event.get():
		if event.type == pygame.QUIT:
			running = False

	# fill the screen with a color to wipe away anything from last frame
	screen.fill("black")

	fTheta += 1.0 * fElapsedTime

	matRotZ[0][0] = math.cos(fTheta);
	matRotZ[0][1] = math.sin(fTheta);
	matRotZ[1][0] = -math.sin(fTheta);
	matRotZ[1][1] = math.cos(fTheta);
	matRotZ[2][2] = 1;
	matRotZ[3][3] = 1;


	matRotX[0][0] = 1;
	matRotX[1][1] = math.cos(fTheta * 0.5);
	matRotX[1][2] = math.sin(fTheta * 0.5);
	matRotX[2][1] = -math.sin(fTheta * 0.5);
	matRotX[2][2] = math.cos(fTheta * 0.5);
	matRotX[3][3] = 1;

	translateZ = 100

	for tri in list_of_triangle:

		pZ0 = matRotZ.dot(tri.points[0])
		if (pZ0[3] != 0):
			pZ0[0] /= pZ0[3]
			pZ0[1] /= pZ0[3]
			pZ0[2] /= pZ0[3]

		pZ1 = matRotZ.dot(tri.points[1])
		if (pZ1[3] != 0):
			pZ1[0] /= pZ1[3]
			pZ1[1] /= pZ1[3]
			pZ1[2] /= pZ1[3]

		pZ2 = matRotZ.dot(tri.points[2])
		if (pZ2[3] != 0):
			pZ2[0] /= pZ2[3]
			pZ2[1] /= pZ2[3]
			pZ2[2] /= pZ2[3]
		
		triRotatedZ = Triangle([pZ0, pZ1, pZ2])

		pZX0 = matRotX.dot(triRotatedZ.points[0])

		if (pZX0[3] != 0):
			pZX0[0] /= pZX0[3]
			pZX0[1] /= pZX0[3]
			pZX0[2] /= pZX0[3]

		pZX1 = matRotX.dot(triRotatedZ.points[1])
		if (pZX1[3] != 0):
			pZX1[0] /= pZX1[3]
			pZX1[1] /= pZX1[3]
			pZX1[2] /= pZX1[3]

		pZX2 = matRotX.dot(triRotatedZ.points[2])
		if (pZX2[3] != 0):
			pZX2[0] /= pZX2[3]
			pZX2[1] /= pZX2[3]
			pZX2[2] /= pZX2[3]
		
		triRotatedZX = Triangle([pZX0, pZX1, pZX2])

		triTranslated = Triangle(triRotatedZX.points)
		# print(triRotatedZX.points)
		triTranslated.points[0][2] = triRotatedZX.points[0][2] + translateZ
		triTranslated.points[1][2] = triRotatedZX.points[1][2] + translateZ
		triTranslated.points[2][2] = triRotatedZX.points[2][2] + translateZ

		normal = crossProduct(triTranslated)
		if ((normal[0] * triTranslated.points[0][0] +  normal[1] * triTranslated.points[0][1] + normal[2] * triTranslated.points[0][2]) > 0):

			projectedP0 = matProj.dot(triTranslated.points[0])
			projectedP1 = matProj.dot(triTranslated.points[1])
			projectedP2 = matProj.dot(triTranslated.points[2])
			triProjected = Triangle([projectedP0, projectedP1, projectedP2])
			
			triProjected.draw()

	# RENDER YOUR GAME HERE

	# flip() the display to put your work on screen
	pygame.display.flip()

	# fElapsedTime += 0.0001
	clock.tick(30)	# limits FPS to 60

pygame.quit()
