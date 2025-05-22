#include "lcd/lcd.h"
#include "utils.h"
#include "assembly/example.h"
#include "GameControl.h"
#include <math.h>
#include <time.h>
//#include "../include/gd32vf103_libopt.h"
#include "gd32vf103.h"

uint64_t get_time_ms() {
  return get_timer_value() / (SystemCoreClock / 4000);
}
struct Player master,enemy[3];
struct node chain_start,chain_end;
short bullet_count=0,FPS;
int last_button1,last_button2;
u8 *scene[] = {(u8*)"left:scene1", (u8*)"down:scene2", (u8*)"right:scene3"};
int sceney[]={10,25,40};
int init_string_success=0;

void Initial_selection()
{
  LCD_Clear(BLACK);

  show_initial_string(40,0,NULL,WHITE);
  if(init_string_success)LCD_ShowString(40,60,(u8*)"Select scene!",BRED);

  while(1)
  {
    int flag=choose_button(Get_Button(JOY_LEFT),Get_Button(JOY_DOWN),Get_Button(JOY_RIGHT));
    if(flag==1) {
      LCD_Clear(BLACK);
      LCD_ShowString(60,15,(u8*)"scene1",WHITE);
      break;
    }
    if(flag==2) {
      LCD_Clear(BLACK);
      LCD_ShowString(60,15,(u8*)"scene2",WHITE);
      break;
    }
    if(flag==3) {
      LCD_Clear(BLACK);
      LCD_ShowString(60,15,(u8*)"scene3",WHITE);
      break;
    }
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
  LCD_ShowString(60, 35, (u8*)"0", WHITE);/**/
  LCD_Clear(BLACK);
  draw_rect(0,0,LCD_W-1,LCD_H-1,GBLUE);

}

void Initialization()
{
  master.x=20;master.y=40;master.life=2100000000;master.name=0;

  enemy[0].x=120;enemy[0].y=10;enemy[0].life=10;enemy[0].name=1;
  enemy[1].x=120;enemy[1].y=35;enemy[1].life=10;enemy[1].name=2;
  enemy[2].x=120;enemy[2].y=60;enemy[2].life=10;enemy[2].name=3;
  
  draw_rect( master.x-1,master.y-2,master.x+1,master.y+2,WHITE);
  draw_rect( enemy[0].x-1,enemy[0].y-2,enemy[0].x+1,enemy[0].y+2,RED);
  draw_rect( enemy[1].x-1,enemy[1].y-2,enemy[1].x+1,enemy[1].y+2,GREEN);
  draw_rect( enemy[2].x-1,enemy[2].y-2,enemy[2].x+1,enemy[2].y+2,BLUE);

  chain_start.next = &chain_end;chain_start.prev = NULL;chain_start.value = NULL;
  chain_end.prev = &chain_start;chain_end.next = NULL;chain_end.value = NULL;
}

int collision(int x1, int y1, int x2, int y2)
{
  if(abs(x1-x2)<2 &&abs(y1-y2)<4)
    return 1;
  return 0;
}

void Enemy_move(int loop)
{
  if(loop%10)return;
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

void Enemy_update()
{
  for(int i=0;i<3;i++)
  {
    if(enemy[i].life<=0)
    {
      draw_rect(enemy[i].x-1, enemy[i].y-2, enemy[i].x+1, enemy[i].y+2, BLACK);
      enemy[i].x=120;
      if(i==0)enemy[i].y=10;
      if(i==1)enemy[i].y=35;
      if(i==2)enemy[i].y=60;
      enemy[i].life=10;

      while(collision(enemy[i].x,enemy[i].y,master.x,master.y))enemy[i].x-=2;
      for(int j=0;j<3;j++)
      {
        if(i!=j)
          while(collision(enemy[i].x,enemy[i].y,enemy[j].x,enemy[j].y))enemy[i].x-=2;
      }

      draw_rect(enemy[i].x-1, enemy[i].y-2, enemy[i].x+1, enemy[i].y+2, (i == 0) ? RED : ((i == 1) ? GREEN : BLUE));
    }
  }
}

void Master_move(int loop)
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

void Remove_bullet()
{
  struct node* node = chain_start.next;
  while(node!=&chain_end)
  {
    struct Bullet* bullet = node->value;
    if(bullet->valid==0)
    {
      if(bullet->owner==1)draw_circ(bullet->x,bullet->y,1,BLACK);
      if(bullet->owner==2)draw_rect(bullet->x,bullet->y,bullet->x,bullet->y,BLACK);
      if(bullet->owner==3)draw_pointbig(bullet->x,bullet->y,BLACK);
      if(bullet->owner==4)draw_point(bullet->x,bullet->y,BLACK);
      if(bullet->owner==0)draw_point(bullet->x,bullet->y,BLACK);
      node->prev->next=node->next;
      node->next->prev=node->prev;
      struct node* temp = node;
      node=node->next;
      free(temp->value);
      free(temp);
      bullet_count--;
    }
    else
      node=node->next;
  }
}

void Generate_bullet(int x,int y,int dx,int dy,char owner,char target)
{
  if(x<=2||x>=LCD_W-2||y<=4||y>=LCD_H-4)
    return;

  struct Bullet* bullet = (struct Bullet*)malloc(sizeof(struct Bullet));
  struct node* node = (struct node*)malloc(sizeof(struct node));
  node->value = bullet;
  node->next = &chain_end;node->prev = chain_end.prev;chain_end.prev->next = node;chain_end.prev = node;

  bullet->dx=dx;bullet->dy=dy;bullet->valid=1;bullet->owner=owner;//bullet->x=x+bullet->dx;bullet->y=y+bullet->dy;
  bullet->x=x;bullet->y=y;bullet->orix=x;bullet->oriy=y;
  bullet->tot=0;bullet->target=target;

  if(owner==1)
    draw_circ(bullet->x,bullet->y,1,RED);
  if(owner==2)
    draw_rect(bullet->x,bullet->y,bullet->x,bullet->y,GREEN);
  if(owner==3)
    draw_pointbig(bullet->x,bullet->y,BLUE);
  if(owner==4||owner==0)
    draw_point(bullet->x,bullet->y,WHITE);

  bullet_count++;
}

void Enemy_shoot(int loop)
{
  // Enemy1 shoot
  if(enemy[0].life>0&&(loop%10==0))
  {
    Generate_bullet(enemy[0].x,enemy[0].y,0,1,1,0);
    Generate_bullet(enemy[0].x,enemy[0].y,0,-1,1,0);
    Generate_bullet(enemy[0].x,enemy[0].y,1,0,1,0);
    Generate_bullet(enemy[0].x,enemy[0].y,-1,0,1,0);
  }
  // Enemy2 shoot
  if(enemy[1].life&&loop%10==0)
  {
    int bias=-10;
    for(;bias<=10;bias+=4)
    {
      int dx,dy,bulx,buly;
      int delta_x=master.x-enemy[1].x,delta_y=master.y-enemy[1].y;
      if (abs(delta_x) > abs(delta_y)){dx=((delta_x > 0) ? 1 : -1);dy=0;}
      else {dx=0;dy=((delta_y > 0) ? 1 : -1);}
      if (abs(delta_x) > abs(delta_y))
        {bulx=enemy[1].x+dx;buly=enemy[1].y+dy+bias;}
      else
        {bulx=enemy[1].x+dx+bias;buly=enemy[1].y+dy;}

      Generate_bullet(bulx,buly,dx,dy,2,0);
    }
  }
  // Enemy3 shoot
  if(enemy[2].life&&loop%40==0)
  {
    Generate_bullet(enemy[2].x,enemy[2].y,1,0,3,0);
    Generate_bullet(enemy[2].x,enemy[2].y,0,1,3,0);
    Generate_bullet(enemy[2].x,enemy[2].y,-1,0,3,0);
    Generate_bullet(enemy[2].x,enemy[2].y,0,-1,3,0);
    Generate_bullet(enemy[2].x,enemy[2].y,7,12,3,0);
    Generate_bullet(enemy[2].x,enemy[2].y,12,7,3,0);
    Generate_bullet(enemy[2].x,enemy[2].y,-7,12,3,0);
    Generate_bullet(enemy[2].x,enemy[2].y,12,-7,3,0);
    Generate_bullet(enemy[2].x,enemy[2].y,-12,7,3,0);
    Generate_bullet(enemy[2].x,enemy[2].y,-12,-7,3,0);
    Generate_bullet(enemy[2].x,enemy[2].y,-7,-12,3,0);
    Generate_bullet(enemy[2].x,enemy[2].y,7,-12,3,0);
  }
}

void Master_shoot(int loop)
{
  int ms=get_time_ms();
  if(Get_Button(BUTTON_1)&&(ms-last_button1)>=300)
  {
    last_button1=ms;
    Generate_bullet(master.x,master.y,0,-1,4,0);
    Generate_bullet(master.x,master.y,1,0,4,0);
    Generate_bullet(master.x,master.y,0,1,4,0);
    Generate_bullet(master.x,master.y,-1,0,4,0);
    Generate_bullet(master.x,master.y,7,12,4,0);
    Generate_bullet(master.x,master.y,12,7,4,0);
    Generate_bullet(master.x,master.y,-7,12,4,0);
    Generate_bullet(master.x,master.y,12,-7,4,0);
    Generate_bullet(master.x,master.y,-12,7,4,0);
    Generate_bullet(master.x,master.y,-12,-7,4,0);
    Generate_bullet(master.x,master.y,-7,-12,4,0);
    Generate_bullet(master.x,master.y,7,-12,4,0);
  }
  if(Get_Button(BUTTON_2)&&(ms-last_button2)>=300)
  {
    last_button2=ms;
    int deltax,deltay,target;
    target=loop%3;
    int tempdx[]={0,3,-3,0,0},tempdy[]={0,0,0,3,-3};
    for(int i=0;i<5;i++)
    {
      deltax=enemy[target].x-master.x;
      deltay=enemy[target].y-master.y;
      Generate_bullet(master.x+tempdx[i],master.y+tempdy[i],deltax,deltay,0,target+1);
      target++;target%=3;
    }
  }
}

void Bullet_move(int loop)
{
  if(loop%2)return;
  struct node* node=chain_start.next;
  while(node!=&chain_end)
  {
    struct Bullet* bullet=node->value;
    int old_x=bullet->x,old_y=bullet->y;
    if(bullet->owner==1)draw_circ(old_x,old_y,1,BLACK);
    if(bullet->owner==2)draw_rect(old_x,old_y,old_x,old_y,BLACK);
    if(bullet->owner==3)draw_pointbig(old_x,old_y,BLACK);
    if(bullet->owner==4||bullet->owner==0)draw_point(old_x,old_y,BLACK);

    if((bullet->owner==4||bullet->owner==0)&&collision(master.x,master.y,bullet->x,bullet->y))draw_rect(master.x-1,master.y-2,master.x+1,master.y+2,WHITE);
    if((bullet->owner==1)&&collision(enemy[0].x,enemy[0].y,bullet->x,bullet->y))draw_rect(enemy[0].x-1,enemy[0].y-2,enemy[0].x+1,enemy[0].y+2,RED);
    if((bullet->owner==2)&&collision(enemy[1].x,enemy[1].y,bullet->x,bullet->y))draw_rect(enemy[1].x-1,enemy[1].y-2,enemy[1].x+1,enemy[1].y+2,GREEN);
    if((bullet->owner==3)&&collision(enemy[2].x,enemy[2].y,bullet->x,bullet->y))draw_rect(enemy[2].x-1,enemy[2].y-2,enemy[2].x+1,enemy[2].y+2,RED);

    if(bullet->owner==1)
    {
      bullet->tot++;
      if(bullet->tot==60)bullet->valid=0;
      if(bullet->tot==30)
      {
        if(bullet->dx==1){bullet->dx=0;bullet->dy=1;}
        else if(bullet->dy==1){bullet->dx=-1;bullet->dy=0;}
        else if(bullet->dx==-1){bullet->dx=0;bullet->dy=-1;}
        else{bullet->dx=1;bullet->dy=0;}
      }
      if(bullet->valid)
      {
        bullet->x+=bullet->dx;
        bullet->y+=bullet->dy;        
      }
    }
    if(bullet->owner==2)
    {
      bullet->tot++;
      bullet->x+=bullet->dx;
      bullet->y+=bullet->dy;
      //if(bullet->tot>=60)bullet->valid=0;
    }
    if(bullet->owner==3||bullet->owner==4)
    {
      bullet->tot++;
      double tempx,tempy;
      tempx=(double)bullet->orix+(bullet->tot*bullet->dx)/sqrt(bullet->dx*bullet->dx+bullet->dy*bullet->dy)*((bullet->x)/abs(bullet->x));
      tempy=(double)bullet->oriy+(bullet->tot*bullet->dy)/sqrt(bullet->dx*bullet->dx+bullet->dy*bullet->dy)*((bullet->y)/abs(bullet->y));
      bullet->x=tempx;
      bullet->y=tempy;
    }
    if(bullet->owner==0)
    {
      bullet->tot++;
      if(bullet->tot==7)bullet->tot=0;
      double tempx,tempy;
      tempx=(double)bullet->orix+(bullet->tot*bullet->dx)/sqrt(bullet->dx*bullet->dx+bullet->dy*bullet->dy)*((bullet->x)/abs(bullet->x));
      tempy=(double)bullet->oriy+(bullet->tot*bullet->dy)/sqrt(bullet->dx*bullet->dx+bullet->dy*bullet->dy)*((bullet->y)/abs(bullet->y));
      bullet->x=tempx;
      bullet->y=tempy;
      if(bullet->tot%7==6)
      {
        double deltax,deltay,odx=bullet->dx,ody=bullet->dy;
        deltax=enemy[bullet->target-1].x-bullet->x;
        deltay=enemy[bullet->target-1].y-bullet->y;
        double cos;
        cos=(deltax*odx+deltay*ody)/sqrt(deltax*deltax+deltay*deltay)/sqrt(odx*odx+ody*ody);
        if(cos<0.8)
        {
          double ndx1,ndx2,ndy1,ndy2;
          ndx1=0.8*odx-0.6*ody;ndy1=0.8*ody+0.6*odx;
          ndx2=0.8*odx+0.6*ody;ndy2=0.8*ody-0.6*odx;
          double cos1,cos2;
          cos1=(deltax*ndx1+deltay*ndy1)/sqrt(deltax*deltax+deltay*deltay)/sqrt(ndx1*ndx1+ndy1*ndy1);
          cos2=(deltax*ndx2+deltay*ndy2)/sqrt(deltax*deltax+deltay*deltay)/sqrt(ndx2*ndx2+ndy2*ndy2);
          if(cos1>cos2){bullet->dx=ndx1;bullet->dy=ndy1;}
          else {bullet->dx=ndx2;bullet->dy=ndy2;}
        }
        else{
          bullet->dx=deltax;bullet->dy=deltay;
        }
        bullet->orix=bullet->x;
        bullet->oriy=bullet->y;
      }
    }

    if(bullet->owner==1||bullet->owner==2||bullet->owner==3)
      if(collision(bullet->x,bullet->y,master.x,master.y))
        bullet->valid=0;

    if(bullet->owner==0||bullet->owner==4)
      for(int i=0;i<3;i++)
        if(collision(bullet->x,bullet->y,enemy[i].x,enemy[i].y))
        {
          enemy[i].life--;
          bullet->valid=0;
        }

    if(bullet->x<=2||bullet->x>=LCD_W-3||bullet->y<=4||bullet->y>=LCD_H-4)bullet->valid=0;

    if(bullet->valid&&bullet->owner==1)draw_circ(bullet->x,bullet->y,1,RED);
    if(bullet->valid&&bullet->owner==2)draw_rect(bullet->x,bullet->y,bullet->x,bullet->y,GREEN);
    if(bullet->valid&&bullet->owner==3)draw_pointbig(bullet->x,bullet->y,BLUE);
    if(bullet->valid&&(bullet->owner==4||bullet->owner==0))draw_point(bullet->x,bullet->y,WHITE);
    
    node=node->next;
  }
}

void Play()
{
  int loop=0,last_loop=0;
  int last_ms=0;
  while(loop<=998244353)
  {
    int ms=get_time_ms();
    if(ms-last_ms>150)
    {
      FPS=(loop-last_loop)*1000/(ms-last_ms);
      last_ms=ms;last_loop=loop;
    }

    Enemy_update();
    Enemy_move(loop);
    Master_move(loop);

    Enemy_shoot(loop);
    Master_shoot(loop);

    Bullet_move(loop);
    Remove_bullet();

    LCD_ShowNum(5,60,bullet_count,3,YELLOW);
    LCD_ShowNum(120,60,FPS,3,YELLOW);
    LCD_ShowNum(5,8,ms/1000,3,YELLOW);
    //LCD_ShowNum(110,8,loop,4,YELLOW);

    loop++;
  }
}
