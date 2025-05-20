struct Player
{
    int x,y;
    int life;
    char name;
};

struct Bullet
{
    int x,y;
    char valid;
    char owner;
    int dx,dy;
    int orix,oriy;
    int tot;
};

struct node
{
    struct node* next;
    struct node* prev;
    struct Bullet* value;
};


void Initial_selection(void);
void Initialization(void);
void Play(void);
void Enemy_move(int loop);
int collision(int x1, int y1, int x2, int y2);
void Remove_bullet(void);
void Enemy_shoot(int loop);
void Bullet_move(int);
void Master_move(int loop);