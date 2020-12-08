# QSnake
This is Project 1 for SEP Class.
Use QT to complete UI. It is used for teachers and TAs to grade my course.

### How to Start
  Firstly, you will see the welcome page. Press **PLAY** to choose game mode; or press **SETTINGS** to change configurations; or press **X** to exit.
  1. **Last Game**: Continue the last game by loading local saved file "history.snk".
  2. **Single Player**: Start a new game for single player in a basic game map for one snake.
  3. **Two Players**: Start a new game for two players in a basic game map for two snakes.
  4. **Fight AI**: Start a new game for single player to play with AI snake in the same map to "Two Players" 
  5. **<<<**: Back to menu.
  
### In Game
  Press **Esc** on your keyboard to pause, and you will see the Option Page. 
  1. **Restart**: Restart game without saving.
  2. **Save And Exit**: Save the game and back to menu.
  3. **Edit Mode**: 
    Enter edit mode to edit everything on the map now. Click on objects to seleted and examine the parameter. Drag objects to move them. Press **ADD** to add corresponding object to the game at the position where you last clicked. Press **Del** to delete the selected object.
  4. **<<<**: Back to game.
  
### Game Objects
  #### Blocks
  1. **Basic Block**:
    Usually deep red. Cannot automatically move. Snake will be destroyed on hitting it. 
  2. **Snake Body**:
    Usually black. Move with snake. Snake will be destroyed on hitting it. 
  3. **Food Generator**:
    Usually green. Cannot automatically move. Snake will be destroyed on hitting it. 
    Each Food Generator will always provide three Basic Food,and provide one Speed Up Food for every 10s, one Life Up Food every 25s, one Score Bonus Food every 20s.
  #### Snake
  1. **Player Snake**:
    Keep moving. Use certain keys to change direction. Eat food to become longer and stronger.
  2. **AI Snake**:
    Automatically move.
  #### Food
  1. **Basic Food**: 
    Provide 1 or 2 score(s) and 1 or 2 blocks' growth.
  2. **Speed Up Food**:
    Give double speed for 5s.
  3. **Life Up Food**:
    Give one extra life.
  4. **Score Bonus Food**:
    Give 5 to 30 scores.
  #### Other
  1. **Size Controller**:
    Cannot automatically move. Controls size for blocks. Each block has four Size Controller.
  2. **Dying Label**:
    Show some words and are dying in several seconds.
  3. **Partical**:
    Generated when something is destroyed to make the interface more beautiful and natural. Dying in several seconds.
 
