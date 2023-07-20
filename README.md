# Raycaster Thing! 

This is my 2D raycaster project created using Raylib, this uses some more advanced techniques for ray marching and was programmed in good old C++. 
I used stepping techniques for my program which improved its efficiency by huge amounts. 

![image](https://github.com/hiatus770/raycasterThing/assets/77402029/a0687c88-d37e-466d-878e-16d6073fdc1b)

I used the above algorithm from a youtube video and implemented it into my program in order to make it run at mucher higher frame rates. 


Heres an example from the program running with a custom texture loaded onto the tiles! 

https://github.com/hiatus770/raycasterThing/assets/77402029/0f451326-3fd3-4855-b99b-133d5ea14db0


## Usage 

If you already have Raylib installed then you are good to go, otherwise install raylib and run the makefile in the editor of your choice. I

You can upload custom textures to the program by uploading a texture, and then running textureMapGenerator.py which will create a huge 2D array that has to be copy pasted into maps.h 
Once inside of maps.h, you have to change the brickWidth constant to reflect whatever the size of your custom png texture. 
