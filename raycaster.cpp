#include <SDL2/SDL.h>
#include <iostream>
#include <thread>
int myMap[] = {
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 0, 0, 0, 0, 0, 0, 0, 0, 1,
    1, 0, 1, 1, 0, 0, 0, 0, 0, 1,
    1, 0, 1, 0, 0, 0, 0, 0, 0, 1,
    1, 0, 1, 0, 0, 0, 0, 0, 0, 1,
    1, 0, 0, 0, 0, 0, 0, 0, 0, 1,
    1, 0, 0, 0, 0, 0, 0, 0, 0, 1,
    1, 0, 0, 0, 0, 0, 0, 0, 0, 1,
    1, 0, 0, 0, 0, 0, 0, 0, 0, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1

};







void drawRaycast(SDL_Renderer *renderer, double lookX, double lookY, double centerX, double centerY,double radAngle){
    int FOV = 120; // degrees
    if (radAngle < 0){
        radAngle = std::fmod((2 * M_PI) + radAngle, 2 * M_PI);
    }

    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 0);
    
    int screenWidth = 1536;
    int rayCount = screenWidth;
    // start of each ray = centerX, centerY
    double rayAngle = (FOV * (M_PI / 180))/ (rayCount - 1);
    double rayAngleStart = (radAngle - ((FOV * (M_PI / 180))) / 2);

    // let x be the number of rays
    for (int x = 0; x < rayCount; x++){


        

        double rayXH = centerX;
        double rayYH = centerY;
        double rayXV = centerX;
        double rayYV = centerY; 
        bool wallFound = false;
        double changeY;
        bool down = false;
        double directionCheck;
        
        directionCheck = (rayAngleStart + (x * rayAngle));
        
       
        directionCheck = std::fmod(directionCheck, 2 * M_PI);
        if (directionCheck < 0) {
            directionCheck += 2 * M_PI;
        }
      
        double relativeAngle;

        if (directionCheck > (M_PI / 2.000) && (directionCheck < (M_PI * (3.000/2.000)))){
            // up
            changeY = ((std::fmod(centerY, 40)));
            rayYH -= changeY;
   

        } else {
            // down
            down  = true;
            changeY = (39 - (std::fmod(centerY, 40)));
          
            rayYH += changeY;
        }

        // vertical casting with X

        double xDist;
        double vChange;
        double actChange;
        int vertC = centerX / 40;
        double rayXVtemp;
        const double epsilon = 0.0002;
        const double myUpperEpsilon = 1e+10;
        if ( directionCheck > (2 * M_PI)){
            directionCheck = std::fmod(directionCheck, (M_PI * 2));
        }
        double myTheta = std::abs(std::fmod(directionCheck, (M_PI_2)));
        if (myTheta < epsilon) {
            myTheta = 0;
        }


        double tanComp = std::tan(myTheta);

        if (tanComp > myUpperEpsilon){
            tanComp = 0;
        } else if (tanComp < epsilon){
            tanComp =0;
        }

        // vertical step
        double rayYVStep;
        double rayXVStep = 40;

        // horizontal step
        double rayYHStep = 40;
        double rayXHStep;

        if (tanComp != 0 ){
            actChange = changeY * tanComp;

            if(directionCheck > M_PI){
          
                // left
                xDist = std::fmod(centerX, 40); // here to 40
                rayXVtemp = vertC * 40;
                rayXVStep = -1 * rayXVStep;
                if (down){
                    actChange = changeY / tanComp;
                    vChange = (xDist * tanComp); 
                    rayYVStep = (tanComp * 40);
                    rayXHStep = -(40 / tanComp);
                    
                } else {
                    actChange = changeY * tanComp;
                    vChange = -(xDist / tanComp);
                    rayYVStep = -(40 / tanComp);
                    rayXHStep = -(tanComp * 40);
                    rayYHStep = rayYHStep * -1;
                }
            }  else{
                xDist = 40 - (std::fmod(centerX, 40));
                rayXVtemp = ((vertC) * 40) + 39;
                if (down){
                    actChange = changeY * tanComp;
                    vChange = (xDist / tanComp); 
                    rayYVStep = (40 / tanComp);
                    rayXHStep = (40 * tanComp);
                } else {
                    rayYVStep = -(tanComp * 40);
                    actChange = changeY / tanComp;
                    vChange = -(xDist * tanComp); 
                    rayXHStep = (40 / tanComp);
                    rayYHStep = rayYHStep * -1;
                }
            }
            

        } else {

            if (std::abs(directionCheck - M_PI) < epsilon || std::abs(directionCheck) < epsilon || std::abs(directionCheck - (2 * M_PI)) < epsilon){
                actChange = 0;

       
                
                rayXVtemp = centerX;
                if (down){
                    vChange = 500;
                   

                }else{
                    rayYHStep = -40;
                    vChange = -500;
                }
            } else if (std::abs(directionCheck - M_PI_2) < epsilon){
                // 90 degreers
                actChange = 500;
                rayXVtemp = (vertC * 40) + 39;
                rayYH = centerY;
                rayYHStep = 0;


            } 
            else{
                // left
                rayYH = centerY;
                actChange = 500;
                int diff =  centerX / 40;
                rayXVtemp = (vertC * 40);
                rayYHStep = 0;
                rayXVStep = rayXVStep * -1;
            }
            
        }

        rayYV += vChange;


        if(directionCheck > M_PI){
   
            //left
            rayXH -= actChange;          

        }else{
            //right
           
        
            rayXH += actChange;
       
        }

       
        
               
        rayXV = rayXVtemp;

      
        bool verticalStop = false;
        bool horizontalStop = false;

        bool verticalOOB = false;
        bool horizontalOOB = false;

        double hitDistance;
       
    
        while (!wallFound){
        
     
            if (!verticalStop && !verticalOOB){
                int vIndex;
                int vCol;
                int vRow;
                if (directionCheck > M_PI){
                    // right
                    vCol = (rayXV / 40) - 1;
                    
                    // col row
                } else {
                    // left
                    vCol = ((rayXV / 40) + 1);
                }
                vRow = (rayYV / 40);
                vRow = vRow * 10;
                vIndex = vCol + vRow;
                
                
                
                if (vCol > 9 || vCol < 0 || vRow > 90 || vRow < 0){
                    
                    verticalOOB = true;
                    
                } else {
                    if (myMap[vIndex] == 1){
                        verticalStop = true;
                    } 
                }
            }
            if (!horizontalOOB && !horizontalStop){
                int hCol;
                int hRow = (rayYH / 40);
                int hIndex;

                if (directionCheck > (M_PI / 2.000) && (directionCheck < (M_PI * (3.000/2.000)))){
                    // up
                    //rayYHStep = -1 * rayYHStep;
                    hRow  = (hRow - 1) * 10;

                } else {
                    // down

                    hRow = (hRow + 1) * 10;
            
                    
                }

                hCol = (rayXH  / 40);
        
                
                hIndex = hCol + hRow;
            

                if (hCol > 9 || hCol < 0 || hRow > 90 || hRow < 0){
                    horizontalOOB = true;
                } else {
                    if (myMap[hIndex] == 1){
                        horizontalStop = true;
            
                    } 
                }
        
      
            }

            if((verticalStop == true  || verticalOOB ) && (horizontalStop == true || horizontalOOB)){
                
                // calculate smallest
                // then breakstd::cout
                // or if out of bounds break
                
                double horizontalMag = ((rayXH - centerX) * (rayXH - centerX)) + (rayYH - centerY) * (rayYH - centerY);
                double verticalMag = (rayXV - centerX) * (rayXV - centerX) + (rayYV - centerY) * (rayYV - centerY);
                if (verticalStop && horizontalStop && !verticalOOB && !horizontalOOB) {

                


                    if (horizontalMag < verticalMag) {
                        // horizontal
                        SDL_SetRenderDrawColor(renderer, 0, 255, 0,0);
                        if (x == 0){
                            SDL_SetRenderDrawColor(renderer, 0, 0, 255,0);
                        }
                        SDL_RenderDrawLine(renderer, centerX, centerY, rayXH, rayYH);
                        hitDistance = std::sqrt(horizontalMag);
                    } else {
                        // vertical render
                        SDL_SetRenderDrawColor(renderer, 0, 200, 200,0);
                        
                        SDL_RenderDrawLine(renderer, centerX, centerY, rayXV, rayYV);
                        hitDistance = std::sqrt(verticalMag);
                    }
                    wallFound = true;
                    break;
                } else if (verticalStop && horizontalOOB){
                    // favour vertical
                    SDL_SetRenderDrawColor(renderer, 0, 200, 200,0);
                    SDL_RenderDrawLine(renderer, centerX, centerY, rayXV, rayYV);
                    hitDistance = std::sqrt(verticalMag);
                    wallFound = true;
                    break;
                } else if (horizontalStop && verticalOOB){
                    // horzontal
                    SDL_SetRenderDrawColor(renderer, 0, 255, 0,0);
                    SDL_RenderDrawLine(renderer, centerX, centerY, rayXH, rayYH);
                    hitDistance = std::sqrt(horizontalMag);
                    wallFound = true;
                    break;
                }
            } else {
                if (verticalStop || verticalOOB){
                    rayXH += rayXHStep;
                    rayYH += rayYHStep;
                } else if (horizontalStop || horizontalOOB){
                    rayXV += rayXVStep;
                    rayYV += rayYVStep;
                } else if ((!horizontalStop && !horizontalOOB) && (!verticalStop && !verticalOOB)){
                    rayXH += rayXHStep;
                    rayYH += rayYHStep;
                    rayXV += rayXVStep;
                    rayYV += rayYVStep;
                }
                
                
            }
                


        

        }

        //hitDistance /= std::cos(directionCheck - radAngle);

        

        int wallHeight = 7680 / hitDistance;

        int wallTop;
        //int wallColor = 255 - static_cast<int>(hitDistance * 5);
        if (wallHeight < 768){
            wallTop = (768 - wallHeight) / 2;
        } else {
            std::cout << "uh oh" << std::endl;
        }
      
        int wallBottom = wallTop + wallHeight;

        int wallColour = 255 - static_cast<int>(hitDistance);
        // // Draw the wall segment on the screen
        if (wallColour > -1){
            SDL_SetRenderDrawColor(renderer, 0, wallColour, 0, 255);
            if (SDL_RenderDrawLine(renderer, x, wallTop, x, wallBottom) != 0) {
                std::cerr << "SDL_RenderDrawLine error: " << SDL_GetError() << std::endl;
                // You might want to handle the error here, like returning from the function or cleaning up resources
            }
        }
        
    

            
     





        // int wallHeight = 1536 / hitDistance;
        // int wallColor = 255 - static_cast<int>(hitDistance * 5);
        // SDL_SetRenderDrawColor(renderer, wallColor, wallColor, wallColor, 255);
        // int wallTop = (1536 - wallHeight) / 2;
        // int wallBottom = wallTop + wallHeight;
        // SDL_RenderDrawLine(renderer, wallX, wallTop, wallX, wallBottom);
       
        // SDL_SetRenderDrawColor(renderer, 0, 0, 255, 0);
        // if (x ==0){
        //     SDL_SetRenderDrawColor(renderer, 255, 0, 255, 0);
        // }
        // // vertical render
        // SDL_RenderDrawLine(renderer, centerX, centerY, rayXV, rayYV);
        // // horizontal render
        // // my blue
        // SDL_SetRenderDrawColor(renderer, 0, 200, 200,0);
        // if (x == 0) {
        //     SDL_SetRenderDrawColor(renderer, 0, 255, 40, 0);
        // }
        // SDL_RenderDrawLine(renderer, centerX, centerY, rayXH, rayYH);



    }

}









void drawPlayer(SDL_Renderer *renderer, double playerx, double playery, int playerRotation, int playerSize)
{
    // check if player is in wall tile

    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_Rect playerRect = {
        playerx, playery, playerSize, playerSize
    };
    SDL_RenderFillRect(renderer, &playerRect);
    double centerX = (playerx + (playerSize / 2.0));
    double centerY = (playery + (playerSize / 2.0));

    double radAngle =  (playerRotation % 360) * M_PI / 180;
    double lookX  = centerX + 30 * std::sin(radAngle);
    double lookY = centerY + 30 * std::cos(radAngle);

    SDL_RenderDrawLine(renderer, centerX, centerY, lookX, lookY);

    drawRaycast(renderer, lookX, lookY, centerX, centerY, radAngle);
  


}





void drawMap(SDL_Renderer *renderer)
{
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_Rect squareRect = {0,
                           0,
                           400,
                           400};
    SDL_RenderFillRect(renderer, &squareRect);

    
    for(int i = 0; i < sizeof(myMap) / sizeof(myMap[0]); i++){
        if(myMap[i] == 1){
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            int column = (i / 10) ;
            int row  = (i % 10) ;
            SDL_Rect wallTile= {(row * 40), (column * 40), 40, 40};
       
            SDL_RenderFillRect(renderer, &wallTile);
         
        } 
    }

}
int main()
{
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        std::cerr << "SDL init failed" << SDL_GetError() << std::endl;
        return 1;
    }
    SDL_Window *window = SDL_CreateWindow(
        "Chainsawman",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        1536, 768,
        SDL_WINDOW_SHOWN);

    if (!window)
    {
        std::cerr << "Window creation failed" << SDL_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    
    if (!renderer)
    {
        std::cerr << "SDL renderer not correct" << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    double playerx = 200;
    double playery = 200;
    int playerSize = 10; // pixels
    int playerRotation = 0; // in  degrees
    int lookSensitivity = 1;
    const int movementSpeed = 1; 


    const int targetFrameRate = 60;
    const Uint32 frameDelay = 1000 / targetFrameRate;

    Uint32 frameStart;
    int frameTime;


    
    std::unordered_map<SDL_Keycode, bool>keysPressed;
    // main game loop
    bool quit = false;
    while (!quit)
    {
        frameStart = SDL_GetTicks();
        // clear the screen
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        // render game graphics here
        drawMap(renderer);

        // check if player is not colliding
 

        // check sides

        drawPlayer(renderer, playerx, playery, playerRotation, playerSize);

        //check for front collisions


        bool front_collision = false;
        int playerColumn = (playery - 1) / 40;
        int playerRow = playerx / 40;
        int playerIndex = playerColumn * 10 + playerRow;
        if (myMap[playerIndex] ==  1){
            front_collision = true;
        }
        playerRow = (playerx + playerSize - 1) /  40;
        playerIndex = playerColumn * 10 + playerRow;
        if(myMap[playerIndex] == 1 ){
            front_collision = true;
        }
        
        // check for back collisions
        bool back_collision = false;
        playerColumn = (playery + playerSize) / 40;
        playerIndex  = playerColumn * 10 + playerRow;
        if (myMap[playerIndex] ==  1){
            back_collision = true;
        }
        playerRow = (playerx) /  40;
        playerIndex = playerColumn * 10 + playerRow;
        if(myMap[playerIndex] == 1 ){
            back_collision = true;
        }

        // left collisioin

        bool left_collision = false;
        playerColumn = (playery / 40);
        playerRow = (playerx - 1) / 40;
        playerIndex = (playerColumn * 10) + playerRow;
        if(myMap[playerIndex] == 1){
            left_collision = true;
        }
        playerColumn = (playery + playerSize - 1) / 40;
        playerIndex = (playerColumn * 10) + playerRow;
        if(myMap[playerIndex] == 1){
            left_collision = true;
        }

        //right

        bool right_collision = false;
        playerRow = (playerx + playerSize)  / 40;
        playerIndex = playerColumn * 10 + playerRow;
        if(myMap[playerIndex] ==  1){
            right_collision = true;
        }
        playerColumn = (playery) / 40;
        playerIndex = playerColumn * 10 + playerRow;
        if(myMap[playerIndex] == 1){
            right_collision = true;
        }

        double centerX = (playerx + (playerSize / 2.0));
        double centerY = (playery + (playerSize / 2.0));

        double radAngle =  (playerRotation % 360) * M_PI / 180;
        if (radAngle < 0){
            radAngle = 2 * M_PI + radAngle;
        }
        double lookX  = centerX + 30 * std::sin(radAngle);
        double lookY = centerY + 30 * std::cos(radAngle);

        // for (int ticker; ticker < (sizeof(hitboxMap) / sizeof(hitboxMap[0])); ticker++){
        //     int playerColumn = playery / 40;
        //     int playerRow = playerx / 40;
        //     int playerIndex = playerColumn * 10 + playerRow;
        // }
        // int playerColumn = playery / 40;
        // int playerRow = playerx / 40;
        // int playerIndex = playerColumn * 10 + playerRow;
        // bool frame_collision = true;
        
        // drawPlayer(renderer, playerx, playery);
        // // update render
        SDL_RenderPresent(renderer);
        
        double holdery = 0.0;
        double holderx = 0.0;

        SDL_Event event;
        while (SDL_PollEvent(&event))
        {   
            if (event.type == SDL_QUIT)
            {
                quit = true;
            } else if (event.type == SDL_KEYDOWN){
                keysPressed[event.key.keysym.sym] = true;
              
            } else if (event.type == SDL_KEYUP){
                keysPressed[event.key.keysym.sym] = false;
            }
        }
        if (keysPressed[SDLK_w]){

            holdery += std::cos(radAngle) * movementSpeed;
            holderx += std::sin(radAngle) * movementSpeed;
                 
        }
        if (keysPressed[SDLK_s]){
            holdery -= std::cos(radAngle) * movementSpeed;
            holderx -= std::sin(radAngle) * movementSpeed;
       
         
        }
        if (keysPressed[SDLK_d]){
            holdery += std::cos(radAngle + (M_PI / 2)) * movementSpeed;
            holderx += std::sin(radAngle+ (M_PI / 2)) * movementSpeed;
        }
        if (keysPressed[SDLK_a]){
            holdery -= std::cos(radAngle + (M_PI / 2)) * movementSpeed;
            holderx -= std::sin(radAngle+ (M_PI / 2)) * movementSpeed;
        }

        if (front_collision && (!back_collision)){
            if (holdery > 0){
                playery += holdery;
            }
        }
        else if (back_collision && (!front_collision)){
            if (holdery < 0){
                playery += holdery;
            }
        }
        else {
            playery += holdery;
        }

        if (left_collision && (!right_collision)){
            if (holderx > 0){
                playerx += holderx;
            }
        }
        else if (right_collision && (!left_collision)){
            if (holderx < 0){
                playerx += holderx;
            }
        }
        else {
            playerx += holderx;
        }

        
        if(keysPressed[SDLK_RIGHT]){
            playerRotation += lookSensitivity;
        }
        if(keysPressed[SDLK_LEFT]){
            playerRotation -= lookSensitivity;
        }
        
        frameTime = SDL_GetTicks() - frameStart;
        if (frameDelay > frameTime){
            SDL_Delay(frameDelay - frameTime);
        }
        

    }
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
