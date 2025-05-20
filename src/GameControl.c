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

}

void Initial_scene_drawing()
{
  LCD_Clear(BLACK);

  master.x=20;master.y=40;master.life=2100000000;master.name=0;

  enemy[0].x=120;enemy[0].y=10;enemy[0].life=5;enemy[0].name=1;
  enemy[1].x=120;enemy[1].y=35;enemy[1].life=5;enemy[1].name=2;
  enemy[2].x=120;enemy[2].y=60;enemy[2].life=5;enemy[2].name=3;
  
  draw_rect( master.x-1,master.y-2,master.x+1,master.y+2,WHITE);
  draw_rect( enemy[0].x-1,enemy[0].y-2,enemy[0].x+1,enemy[0].y+2,RED);
  draw_rect( enemy[1].x-1,enemy[1].y-2,enemy[1].x+1,enemy[1].y+2,GREEN);
  draw_rect( enemy[2].x-1,enemy[2].y-2,enemy[2].x+1,enemy[2].y+2,BLUE);
}

int collision(int x1, int y1, int x2, int y2)
{
  if(abs(x1-x2)<2 &&abs(y1-y2)<4)
    return 1;
  return 0;
}

void Enemy_move(int loop)
{
  if(loop%20)return;
  for(int i=0;i<3;i++)
  {
    int old_x = enemy[i].x,old_y = enemy[i].y;
    int delta_x = master.x - enemy[i].x,delta_y = master.y - enemy[i].y;
    int new_x=old_x,new_y=old_y;
    
    if (abs(delta_x) > abs(delta_y))
      new_x=enemy[i].x + ((delta_x > 0) ? 1 : -1);
    else
      new_y=enemy[i].y + ((delta_y > 0) ? 1 : -1);

    if(new_x<=2||new_x>=LCD_W-2||new_y<=4||new_y>=LCD_H-4)
    {
      new_x=old_x;new_y=old_y;
      continue;
    }
    
    int collision_flag=0;
    collision_flag|= collision(new_x,new_y,master.x,master.y);
    for(int j=0;j<3;j++)
    {
      if(i!=j)
        collision_flag|=collision(new_x,new_y,enemy[j].x,enemy[j].y);
    }
    if(collision_flag)
    {
      new_x=old_x;
      new_y=old_y;
      continue;
    }
    
    draw_rect(old_x-1, old_y-2, old_x+1, old_y+2, BLACK);
    uint16_t color = (i == 0) ? RED : ((i == 1) ? GREEN : BLUE);
    enemy[i].x=new_x;
    enemy[i].y=new_y;
    draw_rect(new_x-1, new_y-2, new_x+1, new_y+2, color);
  }
}

void Master_move()
{
  if(Get_Button(JOY_LEFT))
  {
    int old_x = master.x,old_y = master.y;
    int new_x=old_x-1,new_y=old_y;
    if(new_x<=2||new_x>=LCD_W-2||new_y<=4||new_y>=LCD_H-4)
      return;
    int collision_flag=0;
    for(int j=0;j<3;j++)collision_flag|=collision(new_x,new_y,enemy[j].x,enemy[j].y);
    if(collision_flag)
      return;

    draw_rect(old_x-1, old_y-2, old_x+1, old_y+2, BLACK);
    master.x=new_x;
    master.y=new_y;
    draw_rect(new_x-1, new_y-2, new_x+1, new_y+2, WHITE);
  }
  if(Get_Button(JOY_RIGHT))
  {
    int old_x = master.x,old_y = master.y;
    int new_x=old_x+1,new_y=old_y;
    if(new_x<=2||new_x>=LCD_W-2||new_y<=4||new_y>=LCD_H-4)
      return;
    int collision_flag=0;
    for(int j=0;j<3;j++)collision_flag|=collision(new_x,new_y,enemy[j].x,enemy[j].y);
    if(collision_flag)
      return;

    draw_rect(old_x-1, old_y-2, old_x+1, old_y+2, BLACK);
    master.x=new_x;
    master.y=new_y;
    draw_rect(new_x-1, new_y-2, new_x+1, new_y+2, WHITE);
  }
  if(Get_Button(JOY_UP))
  {
    int old_x = master.x,old_y = master.y;
    int new_x=old_x,new_y=old_y-1;
    if(new_x<=2||new_x>=LCD_W-2||new_y<=4||new_y>=LCD_H-4)
      return;
    int collision_flag=0;
    for(int j=0;j<3;j++)collision_flag|=collision(new_x,new_y,enemy[j].x,enemy[j].y);
    if(collision_flag)
      return;

    draw_rect(old_x-1, old_y-2, old_x+1, old_y+2, BLACK);
    master.x=new_x;
    master.y=new_y;
    draw_rect(new_x-1, new_y-2, new_x+1, new_y+2, WHITE);
  }
  if(Get_Button(JOY_DOWN))
  {
    int old_x = master.x,old_y = master.y;
    int new_x=old_x,new_y=old_y+1;
    if(new_x<=2||new_x>=LCD_W-2||new_y<=4||new_y>=LCD_H-4)
      return;
    int collision_flag=0;
    for(int j=0;j<3;j++)collision_flag|=collision(new_x,new_y,enemy[j].x,enemy[j].y);
    if(collision_flag)
      return;

    draw_rect(old_x-1, old_y-2, old_x+1, old_y+2, BLACK);
    master.x=new_x;
    master.y=new_y;
    draw_rect(new_x-1, new_y-2, new_x+1, new_y+2, WHITE);
  }
}

void Play()
{
  int loop=0;
  while(loop++<=12000)
  {
    Enemy_move(loop);
    
    Master_move();
    delay_1ms(5);
  }
}



