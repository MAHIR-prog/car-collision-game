#define _CRT_SECURE_NO_WARNINGS 
#include <raylib.h>
#include <stdbool.h>
#include <stdio.h>
#include "car.h"
#include <stdlib.h>
void loadHighScore(int highScores[3]) {
    highScores[0] = 0;
    highScores[1] = 0;
    highScores[2] = 0;
    FILE* file;
    file = fopen("highScore.txt","r");
    if (file != NULL) {
        fscanf(file,"%d %d %d",&highScores[0],&highScores[1],&highScores[2]);
        fclose(file);
    }
}
void savescore(int highScores[3]){
    FILE* file;
    file = fopen("highScore.txt","w");
    if (file != NULL) {
        fprintf(file,"%d\n%d\n%d\n",highScores[0],highScores[1],highScores[2]);
        fclose(file);
    }
}
int main(){
    int level = 0;
    int highScores[3];
    loadHighScore(highScores);
    int score = 0;
    car enemies[15]; // car is datatype define in car.h // yeh hamare enemy kai car ko store karta jayega // hum enemy car banate jayegai or yeh store karta jayega
    int totalenemies = 5;
    int enemySpeed =3;
    int flag = -1;
    int lanes = 800/3;
    InitWindow(800,600,"Car Collision Game"); // InitWindow is func of raylib which is use to form a screen of 800px width and 600px height 
    SetTargetFPS(60); // SetTargetFPS is another func in raylib yeh decide karta hai kai hamara game kitna smooth hoga means yeh decide karta hai kai hamare screen par aik second mai kitne display(images) show ya change hongai in 60 fps aik second mai 60 time images change hogi 

    car main_player =CarCreate((Vector2){370,250},(Vector2){100,130},RED);// car is our define datatype joh car.h mai define howa hai
    // Now CarCreate is func define in car.c yeh hamare car ko create karaha hai iska nfirst argument position of car lehraha hai iska dousara argument car ka size lehraha hai or third argument car ka colour lehraha hai toh first argument position ka yeh kheraha hai kai car ko 370px (in x-axis) par or 250px (in y-axis) par spawn karo then second argument of size yeh kheraha hai kai car ka size 100px width or 130px height hoga.

    Texture2D player_image = LoadTexture("player.png"); // Now Texture2D is datatype define in raylib which is use to store any image of anything like in this it is storing image of player in player_texture variable ismai aik function bhi istemal howa named LoadTexture(define in raylib) yeh image ko store karwaraha hai in player_texture matlab main kaam LoadTexture karaha hai
    Texture2D enemy_image = LoadTexture("enemy.png");
    main_player.image = player_image;// Now car.h mai joh car datatype a structure present hai iusmai aik image naam ka bhi datatype present hai toh we know that main_player of car datatype so main_player is object of that car datatype so humnai main_player ko hi istemal karke image ko access kiya or iusmai hi player.png ko store karwaya
    // player_image mai hi player ki image present hai toh humnai main_player.image ko player_image hi pass kardiya
    GameState state = MENU;
    while(WindowShouldClose() == false){ // WindowShouldClose() is raylib func agar user nai windows mai X button dabaya toh yeh true return karaiga while loop stops agar nhi kiya toh yeh false return karaiga or game chalta rahega
        if(state == PLAYING){
            CarUpdate(&main_player); // Carupdate puri game ko chala raha hai kai jaise jaise user up down key press karaiga toh kaie kaisa car move karaigi or kaise kaise position change hogi toh Carupdate Puri game ko chala raha hai.
            for(int i=0;i < totalenemies;i++){
                EnemyUpdate(&enemies[i]); // So yeh EnemyUpdate enemy ki position ko update karaha hai kai iuski x or y direction ko velocity kai zariye update karaha hai
                if(CheckCollisionRecs(CarGetRect(&main_player),CarGetRect(&enemies[i])) == true) { // CheckCollisionRecs is raylib func joh two cars kai b/w collision check karaiga.
                    // CarGetRect(&player) player ka rectangle return karaiga means player ki har movement ki waja sai joh position change hoga iusai car ka rectangle bhi change hoga or wohi pass hoga for both player and enemies then CheckCollisionRecs Wohi collision ko check karaiga agar collision howa hoga toh it returns true warna false 
                    // agar false
                    state = END; // Agar Collision howa hoga toh gamestate directly END state par chala jayega
                    if(score > highScores[level]){
                        highScores[level]=score;
                        savescore(highScores);
                    }
                    break;
                }
                if(enemies[i].position.y > 600){ // If enemy ki joh car hai agar woh screen sai bahir chali gayi toh score+=1 means player suceesfully dodge one car toh score increase hojayega or phir enemyRespawn hoga same car wapsi respwan hogi
                    score ++;
                    EnemyRespawn(&enemies[i],enemySpeed,enemies,totalenemies);
                }
            }
        }else if(state == END){
            if(IsKeyPressed(KEY_ENTER)){ // If game end hochuki but user nai wapsi enter press kiya toh iuski harcheez reset hojayegi iuska score wapsi zero hojayega player wapsi iuski initial position par bhej diya jayega or state menu hojayegi 
                score = 0;
                main_player.position =(Vector2){ 370,250 };
                state = MENU;
            }
        }else if(state == MENU){
            // Now flag yeh show karaiga kai user nai koi difficulty select ki hai ya nhi agar user nai difficulty choose kiya to flag =1 if not flag =0 
            flag = 0;
            // IsKeyPressed raylib ka function hai or KEY_ONE(user press 1 key), KEY_TWO(user press2 key) , KEY_THREE(user press 3 key) yeh sab raylib kai predefine constant hai 
            if(IsKeyPressed(KEY_ONE)){
                flag = 1;
                totalenemies = 5;
                enemySpeed = 3;
                level = 0;
                // Now if user choose easy so total enemy ki amount chage hojayegi and in this level variable is use to form file handeling easy takai har level par alag alag score store hoga agar level 1 hai toh file mai easy mode ka score store hoga same for medium(level 2) and hard(level 3)
                // if easy choose kiya toh easy ka highscore store hoga
            }else if(IsKeyPressed(KEY_TWO)){
                flag = 1;
                totalenemies = 8;
                enemySpeed = 4;
                level = 1;
            }else if(IsKeyPressed(KEY_THREE)){
                flag = 1;
                totalenemies = 12;
                enemySpeed = 5;
                level = 2;
            }
            if(flag == 1){
                for (int i=0;i < totalenemies;i++) {
                    enemies[i] = CarCreate((Vector2){370,250}, (Vector2){100,130}, BLUE);
                    enemies[i].image = enemy_image;
                    EnemyRespawn(&enemies[i], enemySpeed, enemies, i);
                }
                state = PLAYING;
            }
        }
        BeginDrawing();
        ClearBackground(GRAY);
        DrawRectangle(lanes - 5,0,4,600,BLACK);
        DrawRectangle((lanes - 5) *2,0,4,600,BLACK);
        if(state == PLAYING){
            CarDraw(&main_player);
            for(int i=0; i < totalenemies;i++){
                CarDraw(&enemies[i]);
            }
            DrawText(TextFormat("Score: %d", score),15,15,30,BLACK);
        }else if(state == END){
            DrawText("GAME OVER", 280,230,50,RED);
            DrawText(TextFormat("Final Score: %d", score),290,300,30,BLACK);
            DrawText(TextFormat("High Score: %d", highScores[level]),290,350,30,BLACK);
            DrawText("Press ENTER to restart",260,400,25,BLACK);
        }else if(state == MENU){
            DrawText("CAR COLLISION GAME",150,180,50,BLACK);
            DrawText("Press 1 for Easy",280,280,25,GREEN);
            DrawText("Press 2 for Medium",280,330,25,ORANGE);
            DrawText("Press 3 for Hard",280,380,25,RED);

        }
        EndDrawing();
    }
    UnloadTexture(player_image);
    UnloadTexture(enemy_image);
    CloseWindow();
    return 0;
}
