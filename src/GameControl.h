struct Player
{
    int x,y;
    int life;
    int name;
};

struct dir_vec
{
    int dx,dy;
};

void Initial_selection(void);
void Initial_scene_drawing(void);
void Play(void);
void Enemy_move(int loop);
int collision(int x1, int y1, int x2, int y2);