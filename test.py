import pygame

# pygame setup
pygame.init()
screen = pygame.display.set_mode((1280, 720))
clock = pygame.time.Clock()
running = True

class Triangle:
	def __init__(self, points) -> None:
		self.points = points

	def draw(self):

		x, y = self.translate_coor(points[0], points[1])
		pygame.draw.line(screen, (255, 255, 255), (x, y), )


while running:
	# poll for events
	# pygame.QUIT event means the user clicked X to close your window
	for event in pygame.event.get():
		if event.type == pygame.QUIT:
			running = False

	# fill the screen with a color to wipe away anything from last frame
	screen.fill("black")



	# RENDER YOUR GAME HERE

	# flip() the display to put your work on screen
	pygame.display.flip()

	clock.tick(60)	# limits FPS to 60

pygame.quit()
