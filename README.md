# Multiplayer Stylized Rocket Simulator
This project is a sandbox project for networked 6DoF lunar lander.
With some tweaks to the project it can be used for a space game, Drone simulator etc.   

The plan for this project is to evolve into a multiplayer lunar lander example.

# Over coming gimbal lock in 6DoF
~~In order to enable 6DoF we had to extend the base Character Movement Component for networked physics based movement.~~ 
So I actually found an easier solution which already replicates. If you have tried the third person example in unreal engine you know it replicates well.  

I the addRotation functions eg. ```AddRollInput(float Val)``` works well replicated but gimbal locks,   
this is solved by overriding them in the player controller and using Quaternion rotation.

However there will still be a gimbal lock since the APlayerCameraManager also uses FRotator for rotation.  
Solved by extending the class and overriding the following function with quaternion rotation:   
```virtual void ProcessViewRotation(float DeltaTime, FRotator& OutViewRotation, FRotator& OutDeltaRot) override;```.   

Followed by assigning it in the player controller.

## Controls
Pitch and Roll Axis is controlled by A + D, W + S.   
Thrust is applied with space bar.   

When in game, pause menu can be reached with E key. - Its bound from Player Controller.

## Prequisites
Unreal Engine 5.1  
Steam

## ScreenShots

![MainMenuImage](Documentation/Images/MainMenu.png)
Main Menu.

![CreateServerImage](Documentation/Images/CreateServer.png)
Create Server Widget.

![FindServerWidget](Documentation/Images/FindServer.png)
Find Server Widget.

![InGame](Documentation/Images/InGame.png)
In Game.
