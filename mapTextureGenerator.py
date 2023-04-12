import PIL
from PIL import Image

print("Pillow Version: " + PIL.__version__)

image = Image.open('textureBrick1.png')

print(image.size)

output = [ [0]*image.size[0] for i in range(image.size[1])]


for x in range(0, image.size[0]):
    for y in range(0, image.size[1]):
        pixel = image.getpixel((x, y))

        # print a c++ format of the array of each red green and blue value
        output[x][y] = "(Color){" + str(pixel[0]) + ", " + str(pixel[1]) + ", " + str(pixel[2]) + "," + "255" + "}"

# Printing to a c++ format of a 2d array
for i in range(0, len(output)):
    print("{ ", end="")
    for j in range(0, len(output[i])):
        if j == len(output[i]) - 1:
            print(output[i][j], end="")
        else:
            print(output[i][j] + ", ", end="")

    print("},")

image.show()