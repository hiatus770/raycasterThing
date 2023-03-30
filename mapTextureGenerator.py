import PIL
from PIL import Image

print("Pillow Version: " + PIL.__version__)

image = Image.open('testMap.png')

print(image.size)

output = [ [0]*image.size[0] for i in range(image.size[1])]


for x in range(0, image.size[0]):
    for y in range(0, image.size[1]):
        pixel = image.getpixel((x, y))
        # check if its black if so then set the right value in the output array to that color 
        if pixel == (255, 255, 255):
            output[y][x] = 1 

# Printing to a c++ format of a 2d array
for i in range(0, len(output)):
    print("{ ", end="")
    for j in range(0, len(output[i])):
        if output[i][j] == 1:
            print("0, ", end="")
        else :
            print("1, ", end="") 
    print("},")

image.show()