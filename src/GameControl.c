#include "lcd/lcd.h"
#include "utils.h"
#include "assembly/example.h"
#include "GameControl.h"

struct Player master,enemy[3];
struct dir_vec dxy[4] = {
    {-1, 0}, // left
    {1, 0},   // right
    {0, -1}, // up
    {0, 1}  // down    
};

void Initial_selection()
{
  LCD_Clear(BLACK);
    LCD_ShowString(40,25,(u8*)"left:scene1",WHITE);
    LCD_ShowString(40,40,(u8*)"down:scene2",WHITE);
    LCD_ShowString(40,55,(u8*)"right:scene3",WHITE);  
  while(1)
  {
    if(Get_Button(JOY_LEFT)) {
      LCD_Clear(BLACK);
      LCD_ShowString(60,15,(u8*)"scene1",WHITE);
      break;
    }
    if(Get_Button(JOY_DOWN)) {
      LCD_Clear(BLACK);
      LCD_ShowString(60,15,(u8*)"scene2",WHITE);
      break;
    }
    if(Get_Button(JOY_RIGHT)) {
      LCD_Clear(BLACK);
      LCD_ShowString(60,15,(u8*)"scene3",WHITE);
      break;
    }
    // Small delay to prevent CPU hogging
    delay_1ms(5);
  }
  LCD_ShowString(60, 35, (u8*)"5", WHITE);
  delay_1ms(1000);
  LCD_ShowString(60, 35, (u8*)"4", WHITE);
  delay_1ms(1000);
  LCD_ShowString(60, 35, (u8*)"3", WHITE);
  delay_1ms(1000);
  LCD_ShowString(60, 35, (u8*)"2", WHITE);
  delay_1ms(1000);
  LCD_ShowString(60, 35, (u8*)"1", WHITE);
  delay_1ms(1000);
  LCD_ShowString(60, 35, (u8*)"0", WHITE);
  LCD_Clear(BLACK);

  draw_rect(60,35,70,40,WHITE);
  draw_rect(72,35,82,40,WHITE);
}

void Initial_scene_drawing()
{
  LCD_Clear(BLACK);

  master.x=20;master.y=40;master.life=2100000000;master.name=0;master.dir=0;

  enemy[0].x=120;enemy[0].y=10;enemy[0].life=5;enemy[0].name=1;enemy[0].dir=0;
  enemy[1].x=120;enemy[1].y=35;enemy[1].life=5;enemy[1].name=2;enemy[1].dir=0;
  enemy[2].x=120;enemy[2].y=60;enemy[2].life=5;enemy[2].name=3;enemy[2].dir=0;
  
  draw_rect( master.x-1,master.y-2,master.x+1,master.y+2,WHITE);
  draw_rect( enemy[0].x-1,enemy[0].y-2,enemy[0].x+1,enemy[0].y+2,RED);
  draw_rect( enemy[1].x-1,enemy[1].y-2,enemy[1].x+1,enemy[1].y+2,GREEN);
  draw_rect( enemy[2].x-1,enemy[2].y-2,enemy[2].x+1,enemy[2].y+2,BLUE);
}

